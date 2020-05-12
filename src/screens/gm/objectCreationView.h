#ifndef OBJECT_CREATION_VIEW_H
#define OBJECT_CREATION_VIEW_H

#include "gui/gui2_overlay.h"
#include "shipTemplate.h"
#include "gui/gui2_listbox.h"

class GuiSelector;
class GuiContainer;
class GuiListbox;
class GuiTextEntry;
//class GuiRotatingModelView;

class GuiObjectCreationView : public GuiOverlay
{
    typedef std::function<void()> func_t;
private:
    string script;
    string create_script;
//    GuiRotatingModelView* model_view;
    GuiSelector* faction_selector;
    GuiSelector* ship_class_selector;
    GuiTextEntry* regex_ship;
    GuiListbox* listbox_other;
    GuiListbox* listbox_station;
    GuiListbox* listbox_ship;
    GuiSelector* player_cpu_selector;
    func_t enterCreateMode;
public:
    GuiListbox* cpu_ship_listbox;
    GuiListbox* player_ship_listbox;
    GuiObjectCreationView(GuiContainer* owner, func_t enterCreateMode);

    virtual bool onMouseDown(sf::Vector2f position);

    void setCreateScript(string script);

    void createObject(sf::Vector2f position);
    void listShipTemplate(string class_name, string regex_ship = "");
};

#endif//OBJECT_CREATION_VIEW_H
