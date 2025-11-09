#include "main.h"
#include "Cipher/Cipher.h"
#include "imgui/imgui.h"
#include "utils/log/log.hpp"

#include "utils/pattern_scanner/pattern_scanner.hpp"
#include "model/model.hpp"
#include "view/view.hpp"
#include "controller/controller.hpp"

RHModel model;
RHController controller(model);
RHView view(model, controller);

void Menu(bool *_pOpen)
{
    view.Draw();
}

void InitLate()
{
}

void Init()
{
    Log::info("Initializing...");
    model.Init();
    view.Init();
    controller.Init();
}