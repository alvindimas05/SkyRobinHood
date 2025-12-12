#pragma once
struct RHController;

struct GameModController
{
    RHController &controller;

    GameModController(RHController &_controller) : controller(_controller) {}
    bool SetGameSpeed(float speed);
    bool SetGameSpeedToMax();
    bool ResetGameSpeed();
};
