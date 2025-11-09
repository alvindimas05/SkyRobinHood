#include <thread>
#include <chrono>
#include "controller.hpp"
#include "lua_controller/lua_controller.hpp"
#include "candle_run_controller/candle_run_controller.hpp"
#include "utils/log/log.hpp"

RHController::RHController(RHModel &m)
    : model(m)
{
    luaController = new LuaController(*this);
    candleRunController = new CandleRunController(*this);
}

void RHController::Init()
{
    luaController->Init();
}