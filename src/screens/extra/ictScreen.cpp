#include "ictScreen.h"
#include "screenComponents/customShipFunctions.h"

#include "gui/gui2_listbox.h"
#include "gui/gui2_autolayout.h"
#include "gui/gui2_label.h"

IctScreen::IctScreen(GuiContainer* owner)
: GuiOverlay(owner, "ICT_SCREEN", colorConfig.background)
{

    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(400, GuiElement::GuiSizeMax);
    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    (new GuiLabel(left_col, "", "List of Stations", 30))->setSize(400, 50);

    positions_list = new GuiListbox(left_col, "POSITIONS_LIST", [this](int index, string value) {
        return;
    });
    positions_list->setSize(400, GuiElement::GuiSizeMax);

    for(int n = 0; n < max_crew_positions; n++)
    {
        string position_name = getCrewPositionName(ECrewPosition(n));
        if (my_spaceship && my_spaceship->hasPlayerAtPosition(ECrewPosition(n)))
            positions_list->addEntry(position_name, position_name);
    }
    
    (new GuiLabel(right_col, "", "List of Systems", 30))->setSize(400, 50);

    systems_list = new GuiListbox(right_col, "SYSTEMS_LIST", [this](int index, string value) {
        return;
    });
    systems_list->setSize(400, GuiElement::GuiSizeMax);
    
    for(int n=0; n<SYS_COUNT; n++)
    {
        string system_name = getLocaleSystemName(ESystem(n));
        if (my_spaceship && my_spaceship->hasSystem(ESystem(n)))
            systems_list->addEntry(system_name, system_name);   
    }

    (new GuiCustomShipFunctions(this, ictScreen, "", my_spaceship))->setPosition(-20, 120, ATopRight)->setSize(250, GuiElement::GuiSizeMax);
}


void IctScreen::onDraw(sf::RenderTarget& window)
{

}
