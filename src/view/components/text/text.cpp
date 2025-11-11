#include "text.hpp"

void Text::Primary(const char* text)
{
    ImGui::TextColored(ImVec4(0.85f, 0.95f, 0.88f, 1.0f), "%s", text);
}