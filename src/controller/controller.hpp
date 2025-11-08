#include "model/model.hpp"
#include "utils/pattern_scanner/pattern_scanner.hpp"

struct LuaController;

#pragma once
struct RHController
{
    RHModel &model;
    LuaController *luaController;

    RHController(RHModel &m);
    void Init();
    void StartCandleRun();
    void StopCandleRun();
};