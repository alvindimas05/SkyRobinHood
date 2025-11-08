#include "controller.hpp"

void RHController::Init()
{
    uintptr_t updateAddr = PatternScanner::FindPattern(model.game.updateBytes, model.game.updateMask);
    if (updateAddr)
    {
        Update = (Game::Update)updateAddr;
    }

    uintptr_t luaDebugDoStringAddr = PatternScanner::FindPattern(model.game.luaDebugDoStringBytes, model.game.luaDebugDoStringMask);
    if (luaDebugDoStringAddr)
    {
        LuaDebugDoString = (Game::LuaDebugDoString)luaDebugDoStringAddr;
    }
}