#ifndef SINGLE_PILOT_SCREEN_H
#define SINGLE_PILOT_SCREEN_H

#include "gui/gui2_overlay.h"
#include "gui/gui2_element.h"
#include "singlePilotView.h"
#include "screenComponents/targetsContainer.h"
#include "spaceObjects/playerSpaceship.h"

class GuiViewport3D;
class GuiToggleButton;

class SinglePilotScreen : public GuiOverlay
{
private:
    bool first_person;

    GuiOverlay* background_crosses;
    SinglePilotView* single_pilot_view;

    GuiViewport3D* viewport;
    GuiElement* left_panel;
    GuiToggleButton* show_3d_button;

    P<PlayerSpaceship> current_spaceship;

    void init(GuiContainer* owner, P<PlayerSpaceship> targetSpaceship);

public:
    SinglePilotScreen(GuiContainer* owner);
    SinglePilotScreen(GuiContainer* owner, P<PlayerSpaceship> targetSpaceship);

    void setTargetSpaceship(P<PlayerSpaceship> targetSpaceship);
    virtual void onDraw(sf::RenderTarget& window) override;
};

#endif//SINGLE_PILOT_SCREEN_H
