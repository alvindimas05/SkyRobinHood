#include "model/model.hpp"
#include "controller/controller.hpp"
#include "imgui/imgui.h"

struct RHView
{
    RHModel &model;
    RHController &controller;
    RHView(RHModel &m, RHController &c) : model(m), controller(c) {};

    const char *getStatusIcon(CollectionStatus status);
    void _Draw();
    void Draw();
    void Init();
};