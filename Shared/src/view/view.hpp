#pragma once
#include "model/model.hpp"
#include "controller/controller.hpp"
#include "candle_run_view/candle_run_view.hpp"
#include "d4c_view/d4c_view.hpp"

/**
 * RHView - Shared View for RobinHood application
 * Handles UI rendering using ImGui (platform-independent)
 * Platform-specific rendering context is handled by platform code
 */
struct RHView
{
    RHModel &model;
    RHController &controller;
    CandleRunView *candleRunView;
    D4CView *d4cView;

    RHView(RHModel &m, RHController &c) : model(m), controller(c) {}

    void Init();
    void Draw();
};
