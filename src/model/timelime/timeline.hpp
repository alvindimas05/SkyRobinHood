#include <string>

static const int TIMELINE_FAST_DURATION = 10;
struct Timeline
{
    std::string name;
    int duration;

    Timeline(const std::string &name, int duration)
        : name(name), duration(duration) {}
};