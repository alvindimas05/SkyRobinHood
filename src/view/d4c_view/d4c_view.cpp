#include "d4c_view.hpp"

void D4CView::Draw()
{
    if(!model.errorMessage.empty()) {
        Text::Primary(model.errorMessage.c_str());
        return;
    }

    Text::Primary("Desperate for Candles (D4C)");
    Text::Primary("Convert Seasonal Candles to White Candles");

    if(!model.convertingMessage.empty()) {
        ImGui::Separator();
        Text::Primary(model.convertingMessage.c_str());
    }
    ImGui::Separator();

    Button::PushStylePrimary();
    if (ImGui::Button("Start Converting"))
    {
        controller.d4cController->Start();
    }
    Button::PopStylePrimary();
}