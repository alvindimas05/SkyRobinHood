#include "candle.hpp"

static constexpr unsigned char candlesJson[] = {
#embed "../../../assets/Candles.json"
};

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
    nlohmann::json j = nlohmann::json::parse(
        reinterpret_cast<const char *>(candlesJson),
        reinterpret_cast<const char *>(candlesJson + sizeof(candlesJson)));

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
