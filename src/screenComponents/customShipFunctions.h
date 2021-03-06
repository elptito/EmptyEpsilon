#ifndef CUSTOM_SHIP_FUNCTIONS_H
#define CUSTOM_SHIP_FUNCTIONS_H

#include "playerInfo.h"
#include "gui/gui2_autolayout.h"
#include "spaceObjects/playerSpaceship.h"

class GuiCustomShipFunctions : public GuiAutoLayout
{
private:
    P<PlayerSpaceship> target_spaceship;

    class Entry
    {
    public:
        string name;
        GuiElement* element;
    };

    ECrewPosition position;
    std::vector<Entry> entries;
public:
    GuiCustomShipFunctions(GuiContainer* owner, ECrewPosition position, string id, P<PlayerSpaceship> targetSpaceship);

    virtual void onDraw(sf::RenderTarget& window) override;

    bool hasEntries();
    void checkEntries();
    void createEntries();
    void setTargetSpaceship(P<PlayerSpaceship> targetSpaceship);
};

#endif//CUSTOM_SHIP_FUNCTIONS_H
