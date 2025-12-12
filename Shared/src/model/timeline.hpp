#pragma once
#include <string>

static const float TIMELINE_SPEED_MULTIPLIER = 0.1;
static const int TIMELINE_EXTRA_DELAY = 2;

struct Timeline
{
    std::string name;
    int duration;

    Timeline(const std::string &name, int duration)
        : name(name), duration(duration) {}
};
