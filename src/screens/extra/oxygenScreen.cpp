#include "oxygenScreen.h"

#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "screenComponents/customShipFunctions.h"

#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_autolayout.h"
#include "gui/gui2_label.h"
#include "gameGlobalInfo.h"

OxygenScreen::OxygenScreen(GuiContainer *owner)
    : GuiOverlay(owner, "OXYGEN_SCREEN", colorConfig.background)
{
    GuiOverlay *background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    oxygen_view = new GuiAutoLayout(this, "OXYGEN_LAYOUT", GuiAutoLayout::LayoutHorizontalRows);
    oxygen_view->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax)->setPosition(0, 0, ACenter);

    (new GuiLabel(oxygen_view, "OXYGEN_LABEL", "Niveau actuel de l'oxygene", 100))->setPosition(0, 200, ACenter);

    for(int n=0; n<max_oxygen_zones; n++)
    {
        oxygen_value[n] = new GuiKeyValueDisplay(oxygen_view, "OXY_VALUE" + string(n), 0.8, "Zone " + string(n+1), "0%");
        oxygen_value[n]->setSize(GuiElement::GuiSizeMax, 40)->setPosition(0, 400, ACenter);
    }
}

void OxygenScreen::onDraw(sf::RenderTarget &window)
{
    GuiOverlay::onDraw(window);

    if (my_spaceship)
    {
        for(int n=0; n<max_oxygen_zones; n++)
        {
            oxygen_value[n]->setValue(string(int(100 * my_spaceship->getOxygenRatio(n))) + "%");
            oxygen_value[n]->setVisible(my_spaceship->getOxygenMax(n)>0);
        }
    }
}
