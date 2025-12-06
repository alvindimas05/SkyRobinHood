#include <thread>
#include <format>
#include "d4c_controller.hpp"
#include "controller/controller.hpp"
#include "controller/lua_controller/lua_controller.hpp"
#include "model/model.hpp"
#include "utils/log/log.hpp"

static std::thread workerThread;
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

        controller.model.convertingMessage = std::format("Playing AP11 Intro Ending Timeline for {} seconds", 25);
        controller.luaController->PlayTimeline("Ap11_Intro_Ending");
        std::this_thread::sleep_for(std::chrono::seconds(25));

        controller.model.convertingMessage = std::format("Playing AP10 Intro Ending Timeline for {} seconds", 25);
        controller.luaController->PlayTimeline("Ap10_Intro_Ending");
        std::this_thread::sleep_for(std::chrono::seconds(25));

        controller.model.convertingMessage = std::format("Playing AP End Season 4 Timeline for {} seconds", 60);
        controller.luaController->PlayTimeline("APEnd_season_04");
        std::this_thread::sleep_for(std::chrono::seconds(60));

        controller.luaController->LoadLevel("CandleSpace");

        Log::info("D4C conversion completed.");
        controller.model.isConverting = false;
        workerThread.detach();
    });
}