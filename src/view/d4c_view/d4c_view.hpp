#pragma once
#include "model/model.hpp"
#include "controller/controller.hpp"
#include "controller/candle_run_controller/candle_run_controller.hpp"
#include "controller/d4c_controller/d4c_controller.hpp"
#include "view/components/button/button.hpp"
#include "view/components/checkbox/checkbox.hpp"
#include "view/components/text/text.hpp"

struct D4CView
{
    RHController &controller;
    RHModel &model;
    D4CView(RHController &controller, RHModel &model) : controller(controller), model(model) {}

    void Draw();
};