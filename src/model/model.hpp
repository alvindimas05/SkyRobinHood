#pragma once
#include <string>
#include <vector>
#include "map/map.hpp"
#include "candle/candle.hpp"
#include "game/game.hpp"

struct RHModel
{
    bool isRunning = false;
    Game game;

    std::vector<Map> maps;
    std::vector<Candle> candles;
    std::vector<Map> selectedMaps;

    void Init();
    std::vector<Candle> GetCandlesInMap(Map map);
};
