#include "imgui/imgui.h"

struct Button {
    static void PushStylePrimary();
    static void PopStylePrimary();
    static void PushStyleDanger();
    static void PopStyleDanger();
};