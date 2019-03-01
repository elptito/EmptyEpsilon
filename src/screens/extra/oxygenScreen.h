#ifndef OXYGENSCREEN_H
#define OXYGENSCREEN_H

#include "gui/gui2_overlay.h"
#include "shipTemplate.h"

class GuiKeyValueDisplay;
class GuiAutoLayout;
class GuiSelector;

class OxygenScreen : public GuiOverlay
{
private:
    constexpr static int max_oxygen_zones = 10;
    GuiAutoLayout* oxygen_view;
    GuiKeyValueDisplay* oxygen_value[max_oxygen_zones];

public:
    OxygenScreen(GuiContainer* owner);

    void onDraw(sf::RenderTarget& window) override;
};

#endif//OXYGENSCREEN_H
