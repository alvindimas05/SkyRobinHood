#include "view.hpp"
#include "view/window/window.hpp"
#include "view/button/button.hpp"
#include "view/checkbox/checkbox.hpp"
#include "view/text/text.hpp"
#include "controller/candle_run_controller/candle_run_controller.hpp"

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
    Window::Start();
    Window::PushStyle();

    if (ImGui::Begin("Robin Hood", nullptr, ImGuiWindowFlags_NoCollapse))
    {
        // Start Candle Run Section
        float buttonWidth = ImGui::GetContentRegionAvail().x;
        float buttonHeight = 60.0f;

        Button::PushStylePrimary();
        if (model.isRunning)
        {
            Button::PushStyleDanger();
            if (ImGui::Button("Stop Candle Run", ImVec2(buttonWidth, buttonHeight)))
            {
                controller.candleRunController->Stop();
            }
            Button::PopStyleDanger();
        }
        else
        {
            if (ImGui::Button("Start Candle Run", ImVec2(buttonWidth, buttonHeight)))
            {
                controller.candleRunController->Start();
            }
        }
        Button::PopStylePrimary();

        ImGui::Separator();

        // End Candle Run Section

        // Start Map Selection Section
        Checkbox::PushStyle();

        bool allSelected = (model.selectedMaps.size() == model.maps.size());

        if (ImGui::Checkbox("Select All Maps", &allSelected))
        {
            if (allSelected)
                model.selectedMaps = model.maps;
            else
                model.selectedMaps.clear();
        }

        Text::Primary("Select Maps:");
        ImGui::Separator();

        for (const auto &map : model.maps)
        {
            bool selected = std::any_of(model.selectedMaps.begin(), model.selectedMaps.end(),
                                        [&](const Map &m)
                                        { return m.name == map.name; });

            if (ImGui::Checkbox(map.name.c_str(), &selected))
            {
                if (selected)
                {
                    if (std::none_of(model.selectedMaps.begin(), model.selectedMaps.end(),
                                     [&](const Map &m)
                                     { return m.name == map.name; }))
                    {
                        model.selectedMaps.push_back(map);
                    }
                }
                else
                {
                    model.selectedMaps.erase(std::remove_if(model.selectedMaps.begin(), model.selectedMaps.end(),
                                                            [&](const Map &m)
                                                            { return m.name == map.name; }),
                                             model.selectedMaps.end());
                }
            }
        }

        Checkbox::PopStyle();
        // End Map Selection Section
    }

    ImGui::End();
    Window::PopStyle();
}

void RHView::Init()
{
}