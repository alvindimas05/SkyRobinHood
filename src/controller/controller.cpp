#include "controller.hpp"
#include "lua_controller/lua_controller.hpp"

RHController::RHController(RHModel &m) 
    : model(m)
{
    luaController = new LuaController(*this);
}

void RHController::StartCandleRun()
{
    if (model.isRunning)
        return;

    model.isRunning = true;
}

void RHController::Init()
{
    luaController->Init();
}