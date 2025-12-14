#include <Windows.h>

#include <fstream>

#include "console.hpp"

#include "../hooks/hooks.hpp"

void Console::Alloc( ) {
    AllocConsole( );

    freopen_s(reinterpret_cast<FILE**>(stdin), "conin$", "r", stdin);
    freopen_s(reinterpret_cast<FILE**>(stdout), "conout$", "w", stdout);

    ::ShowWindow(GetConsoleWindow( ), SW_SHOW);
}

void Console::Free( ) {
    fclose(stdin);
    fclose(stdout);

    if (H::bShuttingDown) {
        ::ShowWindow(GetConsoleWindow( ), SW_HIDE);
    } else {
        FreeConsole( );
    }
}
