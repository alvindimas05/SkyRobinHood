#include "view.hpp"

void RHView::Init()
{
    candleRunView = new CandleRunView(controller, model);
}

void RHView::Draw()
{
    Window::Start();
    Window::PushStyle();

    if (ImGui::Begin("Robin Hood", nullptr, ImGuiWindowFlags_None))
    {
        candleRunView->Draw();
    }

    ImGui::End();
    Window::PopStyle();
}