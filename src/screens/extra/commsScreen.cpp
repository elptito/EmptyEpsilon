#include "commsScreen.h"
#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "scriptInterface.h"

#include "screenComponents/commsOverlayHelios.h"
#include "screenComponents/customShipFunctions.h"

CommsScreen::CommsScreen(GuiContainer* owner)
: GuiOverlay(owner, "COMMS_SCREEN", colorConfig.background)
{
	(new GuiCommsOverlayHelios(this))->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
}

void CommsScreen::onDraw(sf::RenderTarget& window)
{

}
