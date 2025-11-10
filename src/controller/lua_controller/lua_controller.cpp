#include <queue>
#include <mutex>
#include "lua_controller.hpp"
#include "controller/controller.hpp"
#include "model/model.hpp"
#include "utils/pattern_scanner/pattern_scanner.hpp"
#include "dobby/dobby.h"
#include "utils/log/log.hpp"

static uint64_t luaState = 0;
static Game::LuaDebugDoString originalLuaDebugDoString = nullptr;
static Game::Update originalGameUpdate = nullptr;
static std::queue<std::string> scriptQueue;
static std::mutex queueMutex;

static uint64_t hookedUpdate(uint64_t a1, uint64_t a2, uint64_t a3, unsigned int a4)
{
    if (!luaState)
    {
        Log::info("Storing Lua state from Game Update");
        luaState = *(uint64_t *)(a2 + 32);
    }

    std::lock_guard<std::mutex> lock(queueMutex);
    while (!scriptQueue.empty() && luaState)
    {
        std::string script = scriptQueue.front();
        scriptQueue.pop();
        originalLuaDebugDoString(luaState, const_cast<char *>(script.c_str()));
    }

    return originalGameUpdate(a1, a2, a3, a4);
}

void LuaController::Init()
{
    auto &game = controller.model.game;

    uintptr_t updateAddr = PatternScanner::FindPattern(game.updateBytes, game.updateMask);
    if (updateAddr)
    {
        originalGameUpdate = (Game::Update)updateAddr;
        DobbyHook((void *)updateAddr, (void *)hookedUpdate, (void **)&originalGameUpdate);
        Log::info("Hooked Game Update function at address: 0x%lx", updateAddr);
    }
    else
    {
        LOGE("Failed to find Game Update function pattern.");
    }

    uintptr_t luaDebugDoStringAddr = PatternScanner::FindPattern(game.luaDebugDoStringBytes, game.luaDebugDoStringMask);
    if (luaDebugDoStringAddr)
    {
        originalLuaDebugDoString = (Game::LuaDebugDoString)luaDebugDoStringAddr;
        Log::info("Found LuaDebugDoString function at address: 0x%lx", luaDebugDoStringAddr);
    }
    else
    {
        LOGE("Failed to find LuaDebugDoString function pattern.");
    }
}
void LuaController::ExecuteString(const char *luaCode)
{
    std::lock_guard<std::mutex> lock(queueMutex);
    scriptQueue.push(std::string(luaCode));
}

void LuaController::LoadLevel(const char *levelName)
{
    char buffer[512];

    std::snprintf(buffer, sizeof(buffer), R"(
        local e = game:eventBarn():AddEventByMetaName("ChangeLevelWithFade")
        e:levelName("%s")

        e:fadeOutDuration(0)
        e:fadeInDuration(0)
        e:fadeHoldDuration(0)

        e:isLevelComplete(false)
        e:Start()
    )",
                  levelName);

    ExecuteString(buffer);
}

void LuaController::TeleportToCoords(float x, float y, float z)
{
    char buffer[512];

    std::snprintf(buffer, sizeof(buffer), R"(
        local e = game:eventBarn():AddEventByMetaName("AvatarSetPos")
        local m = game:markerBarn():CreateMarker(0)
        m:pos({%f, %f, %f})

        e:marker(m)
        e:onGround(false)
        e:breakHandHold(false)
        e:killMomentum(false)
        e:setLevelStartPosition(false)
        e:Start()

        game:markerBarn():ReleaseMarker(m)
    )",
                  x, y, z);

    ExecuteString(buffer);
}

void LuaController::PlayTimeline(const char *timelineName)
{
    char buffer[64];

    std::snprintf(buffer, sizeof(buffer), "PlayTimeline(game, '%s')", timelineName);

    ExecuteString(buffer);
}