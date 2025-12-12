#pragma once
struct RHController;

struct D4CController
{
    RHController &controller;

    D4CController(RHController &_controller) : controller(_controller) {}
    void Start();
};
