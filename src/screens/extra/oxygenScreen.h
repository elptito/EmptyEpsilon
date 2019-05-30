#ifndef OXYGENSCREEN_H
#define OXYGENSCREEN_H

#include "gui/gui2_overlay.h"
#include "shipTemplate.h"

class GuiKeyValueDisplay;
class GuiAutoLayout;
class GuiProgressbar;
class GuiLabel;

class OxygenScreen : public GuiOverlay
{
private:
    constexpr static int max_oxygen_zones = 10;
    GuiAutoLayout* oxygen_view;
    GuiAutoLayout* oxygen_intern[max_oxygen_zones];
    GuiAutoLayout* oxygen_extern[max_oxygen_zones];
    GuiKeyValueDisplay* oxygen_value_intern[max_oxygen_zones];
    GuiKeyValueDisplay* oxygen_value_extern[max_oxygen_zones];
    GuiProgressbar* oxygen_bar_intern[max_oxygen_zones];
    GuiProgressbar* oxygen_bar_extern[max_oxygen_zones];
    GuiLabel* oxygen_label;

    float time_left;

public:
    OxygenScreen(GuiContainer* owner);

    void onDraw(sf::RenderTarget& window) override;
    string timeCount(float numSec);
};

#endif//OXYGENSCREEN_H
