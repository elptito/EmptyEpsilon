#include "shipLogScreen.h"
#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "gui/gui2_textentry.h"

#include "gui/gui2_advancedscrolltext.h"

ShipLogScreen::ShipLogScreen(GuiContainer* owner, string station)
: GuiOverlay(owner, "SHIP_LOG_SCREEN", colorConfig.background), station(station)
{
    (new GuiOverlay(this, "", sf::Color::White))->setTextureTiled("gui/BackgroundCrosses");

    log_text = new GuiAdvancedScrollText(this, "SHIP_LOG");
    log_text->enableAutoScrollDown();
    log_text->setPosition(50, -50, EGuiAlign::ABottomLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    new_line = new GuiTextEntry(this, "", "");
    new_line->setPosition(0, 0, EGuiAlign::ABottomLeft)->setSize(GuiElement::GuiSizeMax, 50);
    new_line->enterCallback([this](string text){
        // TODO extract command
        my_spaceship->commandAddLogLine(text, this->station);
        new_line->setText("");
    });
}

void ShipLogScreen::onDraw(sf::RenderTarget& window)
{
    GuiOverlay::onDraw(window);
    new_line->setFocus();

    if (my_spaceship)
    {
        const std::vector<PlayerSpaceship::ShipLogEntry>& logs = my_spaceship->getShipsLog(station);
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
