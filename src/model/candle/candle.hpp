#include <iostream>
#include <vector>
#include <string>
#include <span>
#include "nlohmann/json.hpp"

class Candle {
    public:
        std::string name;
        std::string map;
        double x;
        double y;
        double z;

        Candle(const nlohmann::json& j);
        static std::vector<Candle> GetAll();
};