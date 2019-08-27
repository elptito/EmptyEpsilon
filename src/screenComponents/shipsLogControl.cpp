#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "shipsLogControl.h"

#include "gui/gui2_panel.h"
#include "gui/gui2_advancedscrolltext.h"

ShipsLog::ShipsLog(GuiContainer* owner, string station)
: GuiElement(owner, ""), station(station)
{
    setPosition(0, 0, ABottomCenter);
    setSize(GuiElement::GuiSizeMax, 50);
    setMargins(20, 0);

    open = false;

    log_text = new GuiAdvancedScrollText(this, "");
    log_text->enableAutoScrollDown();
    log_text->setMargins(15, 15, 15, 0)->setPosition(0, 0)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
}

void ShipsLog::onDraw(sf::RenderTarget& window)
{
    if (!my_spaceship)
        return;

//    const std::vector<PlayerSpaceship::ShipLogEntry>& logs = my_spaceship->getShipsLog(station);

    if (open)
    {
        log_text->setMargins(15, 15, 15, 0);
        drawStretchedHV(window, sf::FloatRect(rect.left, rect.top, rect.width, rect.height + 100), 25.0f, "gui/PanelBackground");
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
    }else{
        log_text->setMargins(15, 12, 15, 0);

        string background = "gui/ButtonBackground.disabled";
        if (my_spaceship &&
            (station == "intern" && my_spaceship->timer_log_intern > 0 && int(my_spaceship->timer_log_intern) % 2 == 0)
            || (station == "extern" && my_spaceship->timer_log_extern > 0 && int(my_spaceship->timer_log_extern) % 2 == 0)
            || (station == "docks" && my_spaceship->timer_log_docks > 0 && int(my_spaceship->timer_log_docks) % 2 == 0)
            || (station == "science" && my_spaceship->timer_log_science > 0 && int(my_spaceship->timer_log_science) % 2 == 0)
        )
            background = "gui/ButtonBackground.hover";

        drawStretchedHV(window, sf::FloatRect(rect.left, rect.top, rect.width, rect.height), 50.0f, background);
        const std::vector<PlayerSpaceship::ShipLogEntry>& logs = my_spaceship->getShipsLog(station);
        if (log_text->getEntryCount() > 0 && logs.size() == 0)
            log_text->clearEntries();
        if (log_text->getEntryCount() > 0 && logs.size() > 0)
        {
            if (log_text->getEntryText(0) != logs.back().text)
                log_text->clearEntries();
        }
        if (log_text->getEntryCount() == 0 && logs.size() > 0)
            log_text->addEntry("LOG : ", logs.back().text, logs.back().color);
//            log_text->addEntry(logs.back().prefix, logs.back().text, logs.back().color);
    }
}

bool ShipsLog::onMouseDown(sf::Vector2f position)
{
    open = !open;
    if (open)
        setSize(getSize().x, 800);
    else
        setSize(getSize().x, 50);
    return true;
}

void ShipsLog::onHotkey(const HotkeyResult& key)
{
    if ((key.category == "RELAY" || key.category == "ENGINEERING") && my_spaceship)
    {
        if (key.hotkey == "OPEN_LOG")
        {
			open = !open;
		    if (open)
				setSize(getSize().x, 800);
			else
				setSize(getSize().x, 50);
		}
	}
}
