#include "window.hpp"

void Window::Init()
{
    ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_Once);
    ImGui::SetNextWindowSizeConstraints(ImVec2(300, 300), ImVec2(FLT_MAX, FLT_MAX));
}

void Window::PushStyle()
{
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
}

void Window::PopStyle()
{
    ImGui::PopStyleVar(5);
    ImGui::PopStyleColor(6);
}