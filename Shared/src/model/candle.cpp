#include "candle.hpp"
#include <fstream>

// TODO: Platform-specific - Android uses #embed, Windows needs different approach
#ifdef __ANDROID__
static constexpr unsigned char candlesJson[] = {
#embed "../../Candles.json"
};
#endif

static const std::unordered_set<std::string> validCandleNames = {
    "Candle",
    "Plant",
    "Season Candle",
    "Bonus Cake",
    "Rainbow Cake",
    "Candle & Winged Light",
    "Trial of Earth Statue",
    "Trial of Earth End"};

Candle::Candle(const nlohmann::json &j)
{
    name = j["name"].get<std::string>();
    map = j["map"].get<std::string>();
    x = j["x"].get<double>();
    y = j["y"].get<double>();
    z = j["z"].get<double>();
}

std::vector<Candle> Candle::GetAll()
{
#ifdef __ANDROID__
    nlohmann::json j = nlohmann::json::parse(
        reinterpret_cast<const char *>(candlesJson),
        reinterpret_cast<const char *>(candlesJson + sizeof(candlesJson)));
#else
    // Load from file system for Windows
    nlohmann::json j;
    std::ifstream file("../../Shared/Candles.json");
    if (file.is_open())
    {
        file >> j;
        file.close();
    }
    else
    {
        j = nlohmann::json::array();
    }
#endif

    std::vector<Candle> candles;
    candles.reserve(j.size());

    for (const auto &item : j)
    {
        std::string name = item["name"].get<std::string>();
        if (validCandleNames.count(name))
        {
            candles.emplace_back(item);
        }
    }

    return candles;
}
