#include "model/model.hpp"
#include "controller/controller.hpp"
#include "imgui/imgui.h"

struct RHView
{
    RHModel &model;
    RHController &controller;
    RHView(RHModel &m, RHController &c) : model(m), controller(c) {};

    // const char *GetStatusIcon(MapCollectionStatus status);
    void Draw();
    void Init();
};