#include "oxygenScreen.h"

#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "screenComponents/customShipFunctions.h"

#include "gui/gui2_element.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_progressbar.h"
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

    (new GuiLabel(oxygen_view, "OXYGEN_LABEL", "Oxygen Monitor", 100))->setPosition(0, 200, ACenter);

    oxygen_bar = new GuiProgressbar(oxygen_view, "ENERGY_BAR_INTERN", 0.0, 1.0, 0.0);
    oxygen_bar->setColor(sf::Color(192, 192, 32, 128))->setText("Oxygen")->setDrawBackground(true)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax)->setMargins(10, 0, 10, 0)->setPosition(0, 200, ACenter);
}

void OxygenScreen::onDraw(sf::RenderTarget &window)
{
    GuiOverlay::onDraw(window);

    if (my_spaceship)
    {
        oxygen_bar->setValue(my_spaceship->getOxygen() / my_spaceship->getMaxOxygen());
        string oxygen = string(my_spaceship->getOxygen() / my_spaceship->getMaxOxygen() * 100, 0) + "%";
        oxygen_bar->setText(oxygen);
    }
}
