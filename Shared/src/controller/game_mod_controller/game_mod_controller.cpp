#include "game_mod_controller.hpp"
#include "controller/controller.hpp"
#include "utils/log/log.hpp"

bool GameModController::SetGameSpeed(float speed)
{
    if (!controller.model.game.gameSpeedAddr)
    {
        Log::warn("Game speed address not initialized.");
        return false;
    }
    *(float *)(controller.model.game.gameSpeedAddr) = speed;
    return true;
}

bool GameModController::SetGameSpeedToMax()
{
    return SetGameSpeed(100.0f);
}

bool GameModController::ResetGameSpeed()
{
    return SetGameSpeed(1.0f);
}
