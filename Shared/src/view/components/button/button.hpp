#pragma once

// TODO: Platform-specific ImGui includes
#ifdef __ANDROID__
#include "imgui/imgui.h"
#elif _WIN32
#include "../../../../Windows/src/dependencies/imgui/imgui.h"
#else
#include "imgui.h"
#endif

struct Button
{
    static void PushStylePrimary();
    static void PopStylePrimary();
    static void PushStyleDanger();
    static void PopStyleDanger();
};
