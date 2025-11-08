#include "candle.hpp"

static constexpr unsigned char candlesJson[] = {
#embed "../../../assets/Candles.json"
};

Candle::Candle(const nlohmann::json& j) {
    name = j["name"].get<std::string>();
    map = j["map"].get<std::string>();
    x = j["x"].get<double>();
    y = j["y"].get<double>();
    z = j["z"].get<double>();
}

std::vector<Candle> Candle::GetAll() {
    nlohmann::json j = nlohmann::json::parse(
        reinterpret_cast<const char*>(candlesJson), 
        reinterpret_cast<const char*>(candlesJson + sizeof(candlesJson))
    );
    std::vector<Candle> candles;
    for (const auto& item : j) {
        std::string name = item["name"].get<std::string>();
        if (name == "Candle" || "Plant" || name == "Season Candle" || 
            name == "Bonus Cake" || name == "Rainbow Cake" || name == "Candle & Winged Light") {
            candles.emplace_back(item);
        }
    }
    return candles;
}