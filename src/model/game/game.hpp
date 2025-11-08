#include <cstdint>

struct Game
{
    typedef uint64_t (*Update)(uint64_t a1, uint64_t a2, uint64_t a3, unsigned int a4);
    static const uint8_t updateBytes[32];
    const char *updateMask = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

    typedef uint64_t (*LuaDebugDoString)(uint64_t state, char *str);
    static const uint8_t luaDebugDoStringBytes[32];
    const char *luaDebugDoStringMask = "xxxxxxxxxxxxxxxxxxxxxxxxxx????xx";
};