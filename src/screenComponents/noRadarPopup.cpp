#include "noRadarPopup.h"
#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"

#include "gui/gui2_overlay.h"
#include "gui/gui2_container.h"
#include "gui/gui2_label.h"
#include "gui/gui2_button.h"
#include "gui/gui2_panel.h"

GuiNoRadarPopup::GuiNoRadarPopup(GuiContainer* owner)
: GuiElement(owner, "NO_RADAR_POPUP"), owner(owner)
{
    setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    no_radar_overlay = new GuiOverlay(this, "NO_RADAR", sf::Color(0, 0, 0, 128));
    (new GuiPanel(no_radar_overlay, "NO_RADAR_FRAME"))->setPosition(0, 0, ACenter)->setSize(500, 100);
    (new GuiLabel(no_radar_overlay, "NO_RADAR_TEXT", "PAS D'AUSPEX", 70))->setPosition(0, 0, ACenter)->setSize(500, 100);

}

void GuiNoRadarPopup::onDraw(sf::RenderTarget& window)
{
    if (my_spaceship && my_spaceship->getLongRangeRadarRange() <=0)
    {
        no_radar_overlay->show();
    }
    else
    {
        no_radar_overlay->hide();
    }
}
