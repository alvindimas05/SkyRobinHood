#include <thread>
#include <chrono>

#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <regex>

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

bool RHController::CheckForTSM()
{
    try
    {
        std::ifstream in("/proc/self/maps");
        if (!in)
        {
            Log::error("Cannot open /proc/self/maps");
            return false;
        }
        std::string line;
        std::set<std::string> so_paths;
        std::regex so_re(R"((/\S+\.so(\.\S*)?))");
        while (std::getline(in, line))
        {
            std::smatch m;
            if (std::regex_search(line, m, so_re))
                so_paths.insert(m[1]);
        }
        for (auto &p : so_paths)
        {
            if (p.contains("TSM"))
            {
                Log::info("TSM is loaded!");
                return true;
            }
        }
    }
    catch (...)
    {
        Log::error("Exception when trying to check for TSM");
    }
    return false;
}

void RHController::Init()
{
    luaController->Init();
    std::thread([this]()
                {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        auto result = CheckForTSM();
        if(!result){
            model.candleRunErrorMessage = "Please load TSM to run the Candle Run feature";
        } })
        .detach();
}