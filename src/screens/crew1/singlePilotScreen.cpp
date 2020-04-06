#include "main.h"
#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "singlePilotScreen.h"
#include "singlePilotView.h"
#include "gui/gui2_element.h"
#include "gui/gui2_togglebutton.h"
#include "preferenceManager.h"

#include "screenComponents/viewport3d.h"

#include "screenComponents/alertOverlay.h"

SinglePilotScreen::SinglePilotScreen(GuiContainer* owner, P<PlayerSpaceship> targetSpaceship)
: GuiOverlay(owner, "SINGLEPILOT_SCREEN", colorConfig.background)
{
    init(owner, targetSpaceship);
}

SinglePilotScreen::SinglePilotScreen(GuiContainer* owner)
: GuiOverlay(owner, "SINGLEPILOT_SCREEN", colorConfig.background)
{
    init(owner, my_spaceship);
}

void SinglePilotScreen::init(GuiContainer* owner, P<PlayerSpaceship> targetSpaceship)
{
    current_spaceship = targetSpaceship;

    // single pilot
    single_pilot_view = new SinglePilotView(this, targetSpaceship);
    single_pilot_view->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));
    
}

void SinglePilotScreen::onDraw(sf::RenderTarget& window)
{
    GuiOverlay::onDraw(window);
}
 
void SinglePilotScreen::setTargetSpaceship(P<PlayerSpaceship> targetSpaceship)
{
    single_pilot_view->setTargetSpaceship(targetSpaceship);
    current_spaceship = targetSpaceship;
}
