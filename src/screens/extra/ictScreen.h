#ifndef ICT_SCREEN_H
#define ICT_SCREEN_H

#include "gui/gui2_overlay.h"
#include "shipTemplate.h"

class GuiLabel;
class GuiListbox;

class IctScreen : public GuiOverlay
{
private:
    GuiListbox* positions_list;
    GuiListbox* systems_list;
public:
    IctScreen(GuiContainer* owner);
    void onDraw(sf::RenderTarget& window) override;
};

#endif//ICT_SCREEN_H
