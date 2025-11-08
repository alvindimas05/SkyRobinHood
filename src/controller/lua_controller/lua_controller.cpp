#include "lua_controller.hpp"
#include "controller/controller.hpp"
#include "utils/pattern_scanner/pattern_scanner.hpp"
#include "dobby/dobby.h"
#include <queue>
#include <mutex>

static uint64_t luaState = 0;
static Game::LuaDebugDoString originalLuaDebugDoString = nullptr;
static Game::Update originalGameUpdate = nullptr;
static std::queue<std::string> scriptQueue;
static std::mutex queueMutex;

static uint64_t hookedUpdate(uint64_t a1, uint64_t a2, uint64_t a3, unsigned int a4) {
    if (!luaState) {
        luaState = *(uint64_t*)(a2 + 32);
    }

    std::lock_guard<std::mutex> lock(queueMutex);
    while (!scriptQueue.empty() && luaState) {
        std::string script = scriptQueue.front();
        scriptQueue.pop();
        originalLuaDebugDoString(luaState, const_cast<char*>(script.c_str()));
    }

    return originalGameUpdate(a1, a2, a3, a4);
}

void LuaController::Init() {
    auto& game = controller.model.game;
    
    uintptr_t updateAddr = PatternScanner::FindPattern(game.updateBytes, game.updateMask);
    if (updateAddr) {
        originalGameUpdate = (Game::Update)updateAddr;
        DobbyHook((void*)updateAddr, (void*)hookedUpdate, (void**)&originalGameUpdate);
    }

    uintptr_t luaDebugDoStringAddr = PatternScanner::FindPattern(game.luaDebugDoStringBytes, game.luaDebugDoStringMask);
    if (luaDebugDoStringAddr) {
        originalLuaDebugDoString = (Game::LuaDebugDoString)luaDebugDoStringAddr;
    }
}

void LuaController::ExecuteString(const char* luaCode) {
    std::lock_guard<std::mutex> lock(queueMutex);
    scriptQueue.push(std::string(luaCode));
}