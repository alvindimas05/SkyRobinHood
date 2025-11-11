#include "candle_run_view.hpp"

void CandleRunView::Draw()
{
    if (!model.isTSMLoaded)
    {
        Text::Primary("Please load TSM to run the Candle Run feature.");
        return;
    }

    Text::Primary("Don't forget to turn on Auto Burn Candles");
    Text::Primary("and Auto Burn Plants from TSM!");

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

    if (!model.candleRunMessage.empty())
    {
        Text::Primary(model.candleRunMessage.c_str());
    }

    ImGui::Separator();

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