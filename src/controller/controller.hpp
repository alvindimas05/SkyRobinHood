#include "model/model.hpp"

struct LuaController;
struct CandleRunController;

#pragma once
struct RHController
{
    RHModel &model;
    LuaController *luaController;
    CandleRunController *candleRunController;

    RHController(RHModel &m);
    void Init();
    void StartCandleRun();
    void StopCandleRun();
    bool CheckForTSM();
};