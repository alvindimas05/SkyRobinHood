#include <thread>
#include <format>
#include "d4c_controller.hpp"
#include "controller/controller.hpp"
#include "controller/lua_controller/lua_controller.hpp"
#include "controller/game_mod_controller/game_mod_controller.hpp"
#include "model/model.hpp"
#include "utils/log/log.hpp"
#include "model/timeline.hpp"

static std::thread workerThread;
static std::vector<Timeline> timelines = {
    Timeline("Ap11_Intro_Ending", 25),
    // Timeline("Ap10_Intro_Ending", 25),
    Timeline("APEnd_season_04", 60)};

void D4CController::Start()
{
    if (controller.model.isConverting)
    {
        Log::warn("D4C conversion already running!");
        return;
    }

    controller.model.isConverting = true;

    workerThread = std::thread([this]()
                               {
        Log::info("Starting D4C conversion...");

        controller.model.convertingMessage = "Loading map CandleSpace...";
        controller.luaController->LoadLevel("CandleSpace");
        std::this_thread::sleep_for(std::chrono::seconds(10));

        for (const auto& timeline : timelines) {
            controller.luaController->PlayTimeline(timeline.name.c_str());
            
            if(timeline.name == "APEnd_season_04") {
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }

            int waitDuration = timeline.duration;
            if(controller.gameModController->SetGameSpeedToMax()) {
                waitDuration = (waitDuration * TIMELINE_SPEED_MULTIPLIER) + TIMELINE_EXTRA_DELAY;
            }
            controller.model.convertingMessage = std::format("Playing timeline {} for {} seconds", timeline.name, waitDuration);

            std::this_thread::sleep_for(std::chrono::seconds(waitDuration));
            controller.gameModController->ResetGameSpeed();
        }

        controller.luaController->LoadLevel("CandleSpace");

        Log::info("D4C conversion completed.");
        controller.model.isConverting = false;
        workerThread.detach(); });
}
