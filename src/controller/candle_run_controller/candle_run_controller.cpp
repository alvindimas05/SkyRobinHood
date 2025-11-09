#include <thread>
#include <atomic>
#include "candle_run_controller.hpp"
#include "controller/controller.hpp"
#include "controller/lua_controller/lua_controller.hpp"
#include "model/model.hpp"
#include "utils/log/log.hpp"

static std::thread workerThread;

void CandleRunController::Start()
{
    if (controller.model.isRunning)
    {
        Log::warn("Candle run already running!");
        return;
    }

    controller.model.isRunning = true;

    workerThread = std::thread([this]()
                               {
        for (const auto& map : controller.model.selectedMaps)
        {
            if (!controller.model.isRunning)
                break;

            Log::info("Loading map: %s", map.name.c_str());
            controller.luaController->LoadLevel(map.name.c_str());
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));

            for (const auto& candle : controller.model.GetCandlesInMap(map))
            {
                if (!controller.model.isRunning)
                    break;

                Log::info("Teleporting to at (%.2f, %.2f, %.2f)", candle.x, candle.y, candle.z);
                controller.luaController->TeleportToCoords(candle.x, candle.y, candle.z);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }

        controller.model.isRunning = false;
        Log::info("Candle run completed."); });
}

void CandleRunController::Stop()
{
    if (!controller.model.isRunning)
        return;

    controller.model.isRunning = false;

    if (workerThread.joinable())
    {
        workerThread.join();
        Log::info("Candle run stopped and thread joined.");
    }
}
