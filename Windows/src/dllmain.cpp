#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <TlHelp32.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "console/console.hpp"
#include "dependencies/minhook/MinHook.h"
#include "hooks/hooks.hpp"
#include "utils/log/log.hpp"

// powrprof.dll proxy variables
HMODULE dllHandle = nullptr;
BOOLEAN (*o_GetPwrCapabilities)(PSYSTEM_POWER_CAPABILITIES);
LONG (*o_CallNtPowerInformation)(POWER_INFORMATION_LEVEL, PVOID, ULONG, PVOID, ULONG);
POWER_PLATFORM_ROLE (*o_PowerDeterminePlatformRole)( );

// Global path for config
std::string g_ModPath;

// Export powrprof.dll functions
extern "C" __declspec(dllexport) BOOLEAN __stdcall GetPwrCapabilities(PSYSTEM_POWER_CAPABILITIES lpspc) {
    return o_GetPwrCapabilities(lpspc);
}

extern "C" __declspec(dllexport) LONG __stdcall CallNtPowerInformation(
    POWER_INFORMATION_LEVEL InformationLevel,
    PVOID InputBuffer,
    ULONG InputBufferLength,
    PVOID OutputBuffer,
    ULONG OutputBufferLength) {
    return o_CallNtPowerInformation(InformationLevel, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength);
}

extern "C" __declspec(dllexport) POWER_PLATFORM_ROLE PowerDeterminePlatformRole( ) {
    return o_PowerDeterminePlatformRole( );
}

// Helper function to get registry key path
#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((LONG)0x00000000L)
#endif

#ifndef STATUS_BUFFER_TOO_SMALL
#define STATUS_BUFFER_TOO_SMALL ((LONG)0xC0000023L)
#endif

std::wstring GetKeyPathFromKKEY(HKEY key) {
    std::wstring keyPath;
    if (key != NULL) {
        HMODULE dll = LoadLibrary("ntdll.dll");
        if (dll != NULL) {
            typedef DWORD(__stdcall * NtQueryKeyType)(
                HANDLE KeyHandle,
                int KeyInformationClass,
                PVOID KeyInformation,
                ULONG Length,
                PULONG ResultLength);

            NtQueryKeyType func = reinterpret_cast<NtQueryKeyType>(::GetProcAddress(dll, "NtQueryKey"));

            if (func != NULL) {
                DWORD size = 0;
                DWORD result = 0;
                result = func(key, 3, 0, 0, &size);
                if (result == STATUS_BUFFER_TOO_SMALL) {
                    size = size + 2;
                    wchar_t* buffer = new (std::nothrow) wchar_t[size / sizeof(wchar_t)];
                    if (buffer != NULL) {
                        result = func(key, 3, buffer, size, &size);
                        if (result == STATUS_SUCCESS) {
                            buffer[size / sizeof(wchar_t)] = L'\0';
                            keyPath = std::wstring(buffer + 2);
                        }
                        delete[] buffer;
                    }
                }
            }
            FreeLibrary(dll);
        }
    }
    return keyPath;
}

#undef STATUS_BUFFER_TOO_SMALL
#undef STATUS_SUCCESS

// Registry hook for Vulkan layer injection
typedef LSTATUS(__stdcall* PFN_RegEnumValueA)(
    HKEY hKey,
    DWORD dwIndex,
    LPSTR lpValueName,
    LPDWORD lpcchValueName,
    LPDWORD lpReserved,
    LPDWORD lpType,
    LPBYTE lpData,
    LPDWORD lpcbData);

PFN_RegEnumValueA oRegEnumValueA = nullptr;

LSTATUS hkRegEnumValueA(
    HKEY hKey,
    DWORD dwIndex,
    LPSTR lpValueName,
    LPDWORD lpcchValueName,
    LPDWORD lpReserved,
    LPDWORD lpType,
    LPBYTE lpData,
    LPDWORD lpcbData) {

    std::wstring path = GetKeyPathFromKKEY(hKey);

    // Create config file path
    std::string configPath = g_ModPath + "\\mod_config.json";

    // Ensure config file exists
    std::ifstream checkFile(configPath);
    if (!checkFile.is_open( )) {
        std::ofstream createFile(configPath);
        if (createFile.is_open( )) {
            createFile << "{\n  \"file_format_version\": \"1.0.0\",\n  \"layer\": {\n    \"name\": \"ModLayer\",\n    \"type\": \"GLOBAL\",\n    \"library_path\": \"" << g_ModPath << "\\\\powrprof.dll\",\n    \"api_version\": \"1.1.0\",\n    \"implementation_version\": \"1\",\n    \"description\": \"Mod Loader Layer\"\n  }\n}\n";
            createFile.close( );
        }
    } else {
        checkFile.close( );
    }

    LSTATUS result = oRegEnumValueA(hKey, dwIndex, lpValueName, lpcchValueName, lpReserved, lpType, lpData, lpcbData);

    // Inject our config as a Vulkan implicit layer
    if (wcscmp(path.c_str( ), L"\\REGISTRY\\MACHINE\\SOFTWARE\\Khronos\\Vulkan\\ImplicitLayers") == 0 && dwIndex == 0) {
        for (size_t i = 0; i < configPath.size( ); i++) {
            lpValueName[i] = configPath[i];
        }
        lpValueName[configPath.size( )] = '\0';

        *lpcchValueName = 2048;
        lpData = nullptr;
        *lpcbData = 4;
    }

    return result;
}

void TerminateCrashHandler( ) {
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32FirstW(snapshot, &entry) == TRUE) {
        while (Process32NextW(snapshot, &entry) == TRUE) {
            if (lstrcmpW(entry.szExeFile, L"crashpad_handler.exe") == 0 ||
                lstrcmpW(entry.szExeFile, L"crash_reporter.exe") == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, entry.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                    Log::info("Terminated crash handler process\n");
                }
            }
        }
    }
    CloseHandle(snapshot);
}

// Load the real powrprof.dll
void LoadPowrProfWrapper( ) {
    dllHandle = LoadLibraryW(L"C:\\Windows\\System32\\powrprof.dll");
    if (dllHandle == NULL) {
        dllHandle = LoadLibraryW(L"C:\\Windows\\System32\\POWRPROF.dll");
    }

    Log::info("Loading powrprof.dll symbols...\n");

    if (dllHandle != NULL) {
        o_GetPwrCapabilities = (BOOLEAN (*)(PSYSTEM_POWER_CAPABILITIES))GetProcAddress(dllHandle, "GetPwrCapabilities");
        o_CallNtPowerInformation = (LONG (*)(POWER_INFORMATION_LEVEL, PVOID, ULONG, PVOID, ULONG))GetProcAddress(dllHandle, "CallNtPowerInformation");
        o_PowerDeterminePlatformRole = (POWER_PLATFORM_ROLE (*)( ))GetProcAddress(dllHandle, "PowerDeterminePlatformRole");

        if (o_GetPwrCapabilities == nullptr || o_CallNtPowerInformation == nullptr || o_PowerDeterminePlatformRole == nullptr) {
            Log::error("Could not locate symbols in powrprof.dll\n");
        } else {
            Log::info("powrprof.dll loaded successfully\n");
        }
    } else {
        Log::error("Failed to load powrprof.dll\n");
    }
}

DWORD WINAPI OnProcessAttach(LPVOID lpParam);
DWORD WINAPI OnProcessDetach(LPVOID lpParam);

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);

        // Load the real powrprof.dll first
        LoadPowrProfWrapper( );

        // Get mod directory path
        WCHAR path[MAX_PATH];
        GetModuleFileNameW(NULL, path, MAX_PATH);
        std::wstring ws(path);
        std::string _path(ws.begin( ), ws.end( ));
        g_ModPath = _path.substr(0, _path.find_last_of("\\/"));

        HANDLE hHandle = CreateThread(NULL, 0, OnProcessAttach, hinstDLL, 0, NULL);
        if (hHandle != NULL) {
            CloseHandle(hHandle);
        }
    } else if (fdwReason == DLL_PROCESS_DETACH && !lpReserved) {
        OnProcessDetach(NULL);

        // Cleanup
        if (dllHandle != NULL) {
            FreeLibrary(dllHandle);
        }
    }

    return TRUE;
}

DWORD WINAPI OnProcessAttach(LPVOID lpParam) {
    Console::Alloc( );

    // Hook RegEnumValueA for Vulkan layer injection
    HMODULE advapi32 = LoadLibraryW(L"advapi32.dll");
    if (advapi32 != NULL) {
        FARPROC fnRegEnumValue = GetProcAddress(advapi32, "RegEnumValueA");
        if (fnRegEnumValue != NULL) {
            MH_Initialize( );

            if (MH_CreateHook(reinterpret_cast<LPVOID>(fnRegEnumValue), reinterpret_cast<LPVOID>(&hkRegEnumValueA), reinterpret_cast<LPVOID*>(&oRegEnumValueA)) == MH_OK) {
                if (MH_EnableHook(reinterpret_cast<LPVOID>(fnRegEnumValue)) == MH_OK) {
                    Log::info("Successfully hooked RegEnumValueA for Vulkan layer injection");

                    // Terminate crash handlers - Disabled for basic demo
                    TerminateCrashHandler( );
                } else {
                    Log::error("Failed to enable RegEnumValueA hook\n");
                }
            } else {
                Log::error("Failed to create RegEnumValueA hook\n");
            }
        } else {
            Log::error("Failed to get RegEnumValueA address\n");
        }
    } else {
        Log::error("Failed to load advapi32.dll\n");
    }

    // Initialize your hooks
    H::Init( );

    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}

DWORD WINAPI OnProcessDetach(LPVOID lpParam) {
    H::Free( );
    MH_Uninitialize( );
    Console::Free( );

    return 0;
}
