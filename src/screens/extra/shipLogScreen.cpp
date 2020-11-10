#include "shipLogScreen.h"
#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"

#include "gui/gui2_advancedscrolltext.h"
#include "screenComponents/customShipFunctions.h"

ShipLogScreen::ShipLogScreen(GuiContainer* owner, string station)
: GuiOverlay(owner, "SHIP_LOG_SCREEN", colorConfig.background), station(station)
{
    GuiAutoLayout* shiplog_layout = new GuiAutoLayout(this, "SHIPLOG_LAYOUT", GuiAutoLayout::LayoutHorizontalRightToLeft);
    shiplog_layout->setPosition(50, 80)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    custom_function_sidebar= new GuiCustomShipFunctions(shiplog_layout, shipLog, "", my_spaceship);
    custom_function_sidebar->setSize(270, GuiElement::GuiSizeMax);
    (new GuiOverlay(this, "", sf::Color::White))->setTextureTiled("gui/BackgroundCrosses");
    log_text = new GuiAdvancedScrollText(shiplog_layout, "SHIP_LOG");
    log_text->enableAutoScrollDown();
    log_text->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
}

void ShipLogScreen::onDraw(sf::RenderTarget& window)
{
    GuiOverlay::onDraw(window);

    if (my_spaceship)
    {
        const std::vector<PlayerSpaceship::ShipLogEntry>& logs = my_spaceship->getShipsLog(station);
        if (custom_function_sidebar->hasEntries())
            custom_function_sidebar->show();
        else
            custom_function_sidebar->hide();

        if (log_text->getEntryCount() > 0 && logs.size() == 0)
            log_text->clearEntries();

        while(log_text->getEntryCount() > logs.size())
        {
            log_text->removeEntry(0);
        }

        if (log_text->getEntryCount() > 0 && logs.size() > 0 && log_text->getEntryText(0) != logs[0].text)
        {
            bool updated = false;
            for(unsigned int n=1; n<log_text->getEntryCount(); n++)
            {
                if (log_text->getEntryText(n) == logs[0].text)
                {
                    for(unsigned int m=0; m<n; m++)
                        log_text->removeEntry(0);
                    updated = true;
                    break;
                }
            }
            if (!updated)
                log_text->clearEntries();
        }

        while(log_text->getEntryCount() < logs.size())
        {
            int n = log_text->getEntryCount();
            log_text->addEntry(logs[n].prefix, logs[n].text, logs[n].color);
        }
    }
}
