#ifndef OBJECT_CREATION_VIEW_H
#define OBJECT_CREATION_VIEW_H

#include "gui/gui2_overlay.h"
#include "shipTemplate.h"

class GuiSelector;
class GuiContainer;
class GuiListbox;
class GuiRotatingModelView;


class GuiObjectCreationView : public GuiOverlay
{
    typedef std::function<void()> func_t;
private:
    string script;
    string create_script;
    P<ModelData> model_data;
    GuiRotatingModelView* model_view;
    GuiSelector* faction_selector;
    GuiListbox* listbox_other;
    GuiListbox* listbox_station;
    GuiListbox* listbox_ship;
    func_t enterCreateMode;
public:
    GuiObjectCreationView(GuiContainer* owner, func_t enterCreateMode);

    virtual bool onMouseDown(sf::Vector2f position);

    void setCreateScript(string script);

    void createObject(sf::Vector2f position);
};

#endif//OBJECT_CREATION_VIEW_H
