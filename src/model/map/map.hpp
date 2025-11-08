#include <string>
#include <vector>

enum class MapCollectionStatus
{
    NOT_COLLECTED,
    PROCESSING,
    COMPLETED,
};

class Map
{
    public:
        std::string name;
        MapCollectionStatus status = MapCollectionStatus::NOT_COLLECTED;
        
        Map(const std::string& name) : name(name) {}
        static std::vector<Map> GetAll();
};