#include "model.hpp"

void RHModel::Init()
{
    game.Init();
    maps = Map::GetAll();
    candles = Candle::GetAll();
}

std::vector<Candle> RHModel::GetCandlesInMap(Map map)
{
    std::vector<Candle> result;
    for (const auto &candle : candles)
    {
        if (candle.map == map.name)
        {
            result.push_back(candle);
        }
    }
    return result;
}