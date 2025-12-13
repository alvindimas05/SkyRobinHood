#include "view.hpp"
#include "components/window/window.hpp"

#ifdef __ANDROID__
#include "imgui/imgui.h"
#elif _WIN32
#include "../dependencies/imgui/imgui.h"
#else
#include "imgui.h"
#endif

void RHView::Init()
{
    candleRunView = new CandleRunView(controller, model);
    d4cView = new D4CView(controller, model);
}

void RHView::Draw()
{
    Window::Start();
    Window::PushStyle();

    if (ImGui::Begin("Robin Hood", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::BeginTabBar("MainTabBar", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Candle Run"))
            {
                candleRunView->Draw();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("D4C"))
            {
                d4cView->Draw();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    ImGui::End();
    Window::PopStyle();
}
