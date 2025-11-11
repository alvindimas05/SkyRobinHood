#include <thread>
#include <atomic>
#include <format>
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

            std::vector<Candle> candles = controller.model.GetCandlesInMap(map);
            if(candles.empty()){
                Log::warn("Empty candles at map: %s", map.name.c_str());
                continue;
            }

            Log::info("Loading map: %s", map.name.c_str());
            controller.model.candleRunMessage = std::format("Loading {}", map.name);
            controller.luaController->LoadLevel(map.name.c_str());
            std::this_thread::sleep_for(std::chrono::milliseconds(10000));

            controller.model.candleRunMessage = std::format("Processing candles on {}", map.name);

            const auto& timeline = *map.timeline;
            bool isTimelineRun = false;
            for (const auto& candle : candles)
            {
                if (!controller.model.isRunning)
                    break;
                controller.luaController->TeleportToCoords(candle.x, candle.y, candle.z);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));

                // If timeline candle name is empty, just run it after first candle.
                // Either not, look for matching name and run the timeline after going to that candle.
                // Make sure to set isTimelineRun to prevent running multiple times
                if (map.timeline && !isTimelineRun && (timeline.candleName.empty() || candle.name == timeline.candleName)) {
                    isTimelineRun = true;

                    Log::info("Playing timeline for %i seconds: %s", timeline.duration, timeline.name.c_str());
                    controller.model.candleRunMessage = std::format("Playing timeline for {} seconds on {}", timeline.duration, timeline.name);
                    controller.luaController->PlayTimeline(timeline.name.c_str());
                    std::this_thread::sleep_for(std::chrono::seconds(timeline.duration));
                }
            }
        }

        controller.model.isRunning = false;
        Log::info("Candle run completed.");
        controller.model.candleRunMessage = ""; });
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
