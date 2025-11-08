#include "model/model.hpp"
#include "utils/pattern_scanner/pattern_scanner.hpp"

#pragma once
struct RHController
{
    RHModel &model;
    Game::Update Update;
    Game::LuaDebugDoString LuaDebugDoString;

    RHController(RHModel &m) : model(m) {}
    void Init();
};