#pragma once
#include "model/model.hpp"

struct LuaController;
struct CandleRunController;
struct D4CController;

#pragma once
struct RHController
{
    RHModel &model;
    LuaController *luaController;
    CandleRunController *candleRunController;
    D4CController *d4cController;

    RHController(RHModel &m);
    void Init();
    void StartCandleRun();
    void StopCandleRun();
    bool CheckForTSM();
};