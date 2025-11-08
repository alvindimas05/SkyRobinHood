#include "controller.hpp"
#include "lua_controller/lua_controller.hpp"
#include <thread>
#include <chrono>

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

    std::thread([this]() {
        Map map("CandleSpace");
        luaController->LoadLevel(map.name.c_str());
        std::vector<Candle> candles = model.GetCandlesInMap(map);
        
        for (const auto& candle : candles)
        {
            if (!model.isRunning)
                break;
                
            luaController->TeleportToCoords(candle.x, candle.y, candle.z);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        
        model.isRunning = false;
    }).detach();
}

void RHController::StopCandleRun()
{
    if (!model.isRunning)
        return;

    model.isRunning = false;
}

void RHController::Init()
{
    luaController->Init();
}