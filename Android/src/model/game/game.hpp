#include <cstdint>
#include <cstddef>
#include "Cipher/Cipher.h"
#include "utils/log/log.hpp"

struct Game
{
    uintptr_t baseAddr;
    uintptr_t gameSpeedAddr;

    typedef uint64_t (*Update)(uint64_t a1, uint64_t a2, uint64_t a3, unsigned int a4);
    static const uint8_t updateBytes[44];
    const char *updateMask = "xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxx";

    typedef uint64_t (*LuaDebugDoString)(uint64_t state, char *str);
    static const uint8_t luaDebugDoStringBytes[40];
    const char *luaDebugDoStringMask = "xxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxx";
    void Init();
    void InitLate();
    uintptr_t GetGameSpeedAddress();
};