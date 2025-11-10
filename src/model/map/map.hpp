#include <string>
#include <vector>
#include <optional>

enum class MapCollectionStatus
{
    NOT_COLLECTED,
    PROCESSING,
    COMPLETED,
};

struct MapTimeline
{
    std::string name;
    std::string candleName;
    int duration;

    MapTimeline(const std::string &name, const std::string &candleName, int duration)
        : name(name), candleName(candleName), duration(duration) {}
};

class Map
{
public:
    std::string name;
    MapCollectionStatus status = MapCollectionStatus::NOT_COLLECTED;
    std::optional<MapTimeline> timeline;

    Map(const std::string &name) : name(name) {}
    static std::vector<Map> GetAll();
};