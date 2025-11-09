#include "view.hpp"
#include "view/window/window.hpp"
#include "view/button/button.hpp"
#include "view/checkbox/checkbox.hpp"
#include "view/text/text.hpp"

// const char *RHView::GetStatusIcon(MapCollectionStatus status)
// {
//     switch (status)
//     {
//     case MapCollectionStatus::NOT_COLLECTED:
//         return "[x]";
//     case MapCollectionStatus::PROCESSING:
//         return "[~]";
//     case MapCollectionStatus::COMPLETED:
//         return "[✔]";
//     default:
//         return "[x]";
//     }
// }

void RHView::Draw()
{
    Window::Init();
    Window::PushStyle();

    if (ImGui::Begin("Robin Hood", nullptr, ImGuiWindowFlags_NoCollapse))
    {
        Button::PushStylePrimary();

        float buttonWidth = ImGui::GetContentRegionAvail().x;
        float buttonHeight = 60.0f;

        if (model.isRunning)
        {
            Button::PushStyleDanger();
            if (ImGui::Button("Stop Candle Run", ImVec2(buttonWidth, buttonHeight)))
            {
                controller.StopCandleRun();
            }
            Button::PopStyleDanger();
        }
        else
        {
            if (ImGui::Button("Start Candle Run", ImVec2(buttonWidth, buttonHeight)))
            {
                controller.StartCandleRun();
            }
        }

        Button::PopStylePrimary();
        ImGui::Separator();

        Text::Primary("Select Maps:");
    
        ImGui::Separator();
        

        Checkbox::PushStyle();

        static std::unordered_map<std::string, bool> mapSelections;

        for (const auto& map : model.maps)
        {
            bool selected = mapSelections[map.name];
            if (ImGui::Checkbox(map.name.c_str(), &selected))
            {
                mapSelections[map.name] = selected;
            }
        }

        Checkbox::PopStyle();
    }

    ImGui::End();

    Window::PopStyle();
}
void RHView::Init()
{
}