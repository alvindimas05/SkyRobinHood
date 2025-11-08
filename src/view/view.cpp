#include "view.hpp"

const char *RHView::GetStatusIcon(MapCollectionStatus status)
{
    switch (status)
    {
    case MapCollectionStatus::NOT_COLLECTED:
        return "[x]";
    case MapCollectionStatus::PROCESSING:
        return "[~]";
    case MapCollectionStatus::COMPLETED:
        return "[✔]";
    default:
        return "[x]";
    }
}

void RHView::Draw()
{
    ImGui::SetNextWindowSize(ImVec2(320, 480), ImGuiCond_Once);
    ImGui::SetNextWindowSizeConstraints(ImVec2(280, 400), ImVec2(FLT_MAX, FLT_MAX));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.12f, 0.08f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.10f, 0.28f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.07f, 0.22f, 0.13f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.12f, 0.36f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.05f, 0.14f, 0.09f, 0.9f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.95f, 0.88f, 1.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));

    if (ImGui::Begin("Robin Hood", nullptr, flags))
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.35f, 0.20f, 0.95f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15f, 0.45f, 0.25f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.10f, 0.30f, 0.18f, 1.0f));

        float buttonWidth = ImGui::GetContentRegionAvail().x;

        if (model.isRunning)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.93f, 0.27f, 0.27f, 0.9f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.98f, 0.31f, 0.31f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.82f, 0.23f, 0.23f, 1.0f));

            if (ImGui::Button("Stop Candle Run", ImVec2(buttonWidth, 40)))
            {
                controller.StopCandleRun();
            }

            ImGui::PopStyleColor(3);
        }
        else
        {
            if (ImGui::Button("Start Candle Run", ImVec2(buttonWidth, 40)))
            {
                controller.StartCandleRun();
            }
        }

        ImGui::PopStyleColor(3);
    }
    ImGui::End();

    ImGui::PopStyleVar(5);
    ImGui::PopStyleColor(6);
}
void RHView::Init()
{
}