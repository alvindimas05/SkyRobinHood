#include "checkbox.hpp"

void Checkbox::PushStyle()
{
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.08f, 0.22f, 0.13f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.10f, 0.30f, 0.18f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.15f, 0.40f, 0.22f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.40f, 0.90f, 0.50f, 1.0f));
}

void Checkbox::PopStyle()
{
    ImGui::PopStyleColor(4);
}