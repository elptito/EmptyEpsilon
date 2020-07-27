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
    GuiAutoLayout* oxygen_view;
    GuiProgressbar* oxygen_bar;

public:
    OxygenScreen(GuiContainer* owner);

    void onDraw(sf::RenderTarget& window) override;
};

#endif//OXYGENSCREEN_H
