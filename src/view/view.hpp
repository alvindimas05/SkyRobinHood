#pragma once
#include "model/model.hpp"
#include "controller/controller.hpp"
#include "imgui/imgui.h"
#include "view/components/window/window.hpp"
#include "candle_run_view/candle_run_view.hpp"
#include "d4c_view/d4c_view.hpp"

struct RHView
{
    RHModel &model;
    RHController &controller;
    CandleRunView *candleRunView;
    D4CView *d4cView;

    RHView(RHModel &m, RHController &c) : model(m), controller(c) {};

    // const char *GetStatusIcon(MapCollectionStatus status);
    void Draw();
    void Init();
};