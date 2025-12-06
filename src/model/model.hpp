#pragma once
#include <string>
#include <vector>
#include "map/map.hpp"
#include "candle/candle.hpp"
#include "game/game.hpp"

struct RHModel
{
    bool isRunning = false;
    bool isConverting = false;
    Game game;

    std::vector<Map> maps;
    std::vector<Candle> candles;
    std::vector<Map> selectedMaps;

    std::string errorMessage;

    std::string candleRunMessage;
    std::string candleRunErrorMessage;

    std::string convertingMessage;

    void Init();
    std::vector<Candle> GetCandlesInMap(Map map);
};
