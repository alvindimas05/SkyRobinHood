#include "map.hpp"

static const std::vector<std::string> maps = {
    "CandleSpace", "MainStreet",
    "Dawn", "DawnCave", "Dawn_TrialsWater", "Dawn_TrialsEarth", "Dawn_TrialsAir", "Dawn_TrialsFire",
    "Prairie_ButterflyFields", "Prairie_Cave", "Prairie_NestAndKeeper", "Prairie_Island", "Prairie_Village", "Prairie_WildLifePark", "DayHubCave", "DayEnd"
                                                                                                                                                   "Rain",
    "RainForest", "RainMid", "RainShelter", "Rain_Cave", "Rain_BaseCamp", "RainEnd", "Skyway",
    "Sunset", "SunsetRace", "Sunset_Citadel", "Sunset_FlyRace", "SunsetColosseum", "SunsetEnd2", "SunsetVillage", "Sunset_YetiPark",
    "Dusk_Start", "Dusk", "Dusk_Triangle", "DuskOasis", "DuskGraveyard", "Dusk_CrabField", "DuskMid", "DuskEnd",
    "Night", "Night2", "NightEnd", "NightArchive", "Night_Shelter", "Night_IPHallway",
    "NightDesert", "NightDesert_Beach", "Night_JarCave", "Night_PaintedWorld",
    "Event_DaysOfMischief", "TGCOffice"};

std::vector<Map> Map::GetAll()
{
    std::vector<Map> result;
    for (const auto &mapName : maps)
    {
        Map map(mapName);

        if (mapName == "Dawn_TrialsEarth")
        {
            map.timeline = MapTimeline("Revelation", "Trial of Earth Statue", 45);
        }
        else if (mapName == "DayHubCave")
        {
            map.timeline = MapTimeline("PlatformUp", "", 5);
        }

        result.push_back(std::move(map));
    }
    return result;
}