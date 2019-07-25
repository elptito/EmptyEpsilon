#ifndef NOISE_OVERLAY_H
#define NOISE_OVERLAY_H

#include "gui/gui2_element.h"

class GuiNoiseOverlay : public GuiElement
{
private:
    sf::Color color;
    bool onShipDestroyed;
public:
    GuiNoiseOverlay(GuiContainer* owner, string id = "NOISE_OVERLAY", bool onShipDestroyed = true);
    virtual void onDraw(sf::RenderTarget& window);
    void setAlpha(int alpha);
};

#endif//NOISE_OVERLAY_H
