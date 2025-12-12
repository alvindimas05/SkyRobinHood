#pragma once
#include "model/model.hpp"

struct LuaController;
struct CandleRunController;
struct D4CController;
struct GameModController;

/**
 * RHController - Shared Controller for RobinHood application
 * Handles business logic and coordinates between model and view
 * Platform-independent implementation
 */
struct RHController
{
    RHModel &model;
    LuaController *luaController;
    CandleRunController *candleRunController;
    D4CController *d4cController;
    GameModController *gameModController;

    RHController(RHModel &m);
    void Init();
    void StartCandleRun();
    void StopCandleRun();
    bool CheckForTSM();
};
