#pragma once
#include <string>
#include <vector>

struct Game
{
    typedef uint64_t (*Update)(uint64_t a1, uint64_t a2, uint64_t a3, unsigned int a4);
    static const uint8_t updateBytes[32];
    const char *updateMask = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

    typedef uint64_t (*LuaDebugDoString)(uint64_t state, char *str);
    static const uint8_t luaDebugDoStringBytes[32];
    const char *luaDebugDoStringMask = "xxxxxxxxxxxxxxxxxxxxxxxxxx????xx";
};

enum class CollectionStatus
{
    NOT_COLLECTED,
    PROCESSING,
    COMPLETED,
};

struct Map
{
    std::string name;
    CollectionStatus status;
};

struct Realm
{
    std::string name;
    std::vector<Map> maps;
    bool expanded = false;

    CollectionStatus getStatus() const;
};

struct RHModel
{
    bool isRunning = false;
    std::vector<Realm> realms;
    Game game;

    void Init();
};
