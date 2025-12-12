#include "button.hpp"

void Button::PushStylePrimary()
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.35f, 0.20f, 0.95f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15f, 0.45f, 0.25f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.10f, 0.30f, 0.18f, 1.0f));
}

void Button::PopStylePrimary()
{
    ImGui::PopStyleColor(3);
}

void Button::PushStyleDanger()
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.93f, 0.27f, 0.27f, 0.9f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.98f, 0.31f, 0.31f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.82f, 0.23f, 0.23f, 1.0f));
}

void Button::PopStyleDanger()
{
    ImGui::PopStyleColor(3);
}