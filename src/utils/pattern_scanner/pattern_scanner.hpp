#include <cstdint>
#include <cstring>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#elif defined(__ANDROID__)
#include <link.h>
#include <elf.h>
#include <sys/mman.h>
#include <unistd.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#endif

#pragma once
class PatternScanner
{
public:
    struct MemoryRegion
    {
        uintptr_t base;
        size_t size;
        uint8_t *data;
    };
    static uintptr_t FindPattern(const uint8_t *pattern, const char *mask, uintptr_t start = 0, size_t search_size = 0);
    static uintptr_t FindPattern(const std::vector<uint8_t> &pattern, const std::string &mask);

private:
    static uintptr_t SearchInRegion(const uint8_t *data, size_t size, const uint8_t *pattern, const char *mask);
    static std::vector<MemoryRegion> GetExecutableRegions();
};