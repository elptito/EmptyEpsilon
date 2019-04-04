#ifndef WATER_OVERLAY_H
#define WATER_OVERLAY_H

#include "gui/gui2_element.h"

class GuiWaterOverlay : public GuiElement, public Updatable
{
private:
    float x = 0;
    float y = 0;
    float angle_x = 1;
    float angle_y = 1;

public:
    GuiWaterOverlay(GuiContainer* owner);

    virtual void onDraw(sf::RenderTarget& window);
    virtual void update(float delta);
};

#endif//WATER_OVERLAY_H

