#pragma once
#include <string>
#include <vector>
#include "map.hpp"
#include "candle.hpp"
#include "game.hpp"

/**
 * RHModel - Shared Model for RobinHood application
 * Contains the core application state and data structures
 * Platform-independent implementation
 */
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
    void InitLate();
    std::vector<Candle> GetCandlesInMap(Map map);
};
