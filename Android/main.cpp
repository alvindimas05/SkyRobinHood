#include "main.h"
#include "Cipher/Cipher.h"
#include "imgui/imgui.h"

// Use shared code instead of local
#include "../Shared/src/utils/log/log.hpp"
#include "../Shared/src/utils/pattern_scanner/pattern_scanner.hpp"
#include "../Shared/src/model/model.hpp"
#include "../Shared/src/view/view.hpp"
#include "../Shared/src/controller/controller.hpp"

RHModel model;
RHController controller(model);
RHView view(model, controller);

void Menu(bool *_pOpen)
{
    view.Draw();
}

void InitLate()
{
    Log::info("Initializing late...");
    model.InitLate();
}

void Init()
{
    Log::info("Initializing...");
    model.Init();
    controller.Init();
    view.Init();
}