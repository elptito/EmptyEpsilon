#include "objectCreationView.h"
#include "GMActions.h"
#include "factionInfo.h"
#include "shipTemplate.h"
#include "gui/gui2_panel.h"
#include "gui/gui2_selector.h"
#include "gui/gui2_listbox.h"
#include "gui/gui2_textentry.h"
#include <regex>
//#include "screenComponents/rotatingModelView.h"

GuiObjectCreationView::GuiObjectCreationView(GuiContainer* owner, func_t enterCreateMode)
: GuiOverlay(owner, "OBJECT_CREATE_SCREEN", sf::Color(0, 0, 0, 128)), enterCreateMode(enterCreateMode)
{
    GuiPanel* box = new GuiPanel(this, "FRAME");
    box->setPosition(0, 0, ACenter)->setSize(1000, 500);

    faction_selector = new GuiSelector(box, "FACTION_SELECTOR", nullptr);
    for(P<FactionInfo> info : factionInfo)
        faction_selector->addEntry(info->getName(), info->getName());
    faction_selector->setSelectionIndex(0);
    faction_selector->setPosition(20, 20, ATopLeft)->setSize(300, 50);

    (new GuiButton(box, "CREATE OBJECT", "Creer", [this]() {
        setCreateScript(script + ":setFactionId(" + string(faction_selector->getSelectionIndex()) + ")");
    }))->setPosition(20, 70, ATopLeft)->setSize(300, 50);

//    model_view = new GuiRotatingModelView(box, "MODEL_VIEW", nullptr);
//    model_view->setPosition(20, 120, ATopLeft)->setSize(300, 300);

    std::vector<string> template_names = ShipTemplate::getTemplateNameList(ShipTemplate::Station);
    std::sort(template_names.begin(), template_names.end());
    listbox_station = new GuiListbox(box, "CREATE_STATIONS", [this](int index, string value)
    {
        script = "SpaceStation():setRotation(random(0, 360)):setTemplate(\"" + value + "\")";
//        model_view->setModel(ShipTemplate::getTemplate(value)->model_data);
        listbox_other->setSelectionIndex(-1);
        listbox_ship->setSelectionIndex(-1);
    });
    listbox_station->setTextSize(20)->setButtonHeight(30)->setPosition(-350, 20, ATopRight)->setSize(300, 250);
    for(string template_name : template_names)
        listbox_station->addEntry(template_name, template_name);

    listbox_other = new GuiListbox(box, "CREATE_OTHERS", [this](int index, string value)
    {
        script = value + "()";
//        model_view->setModel(nullptr);
        listbox_station->setSelectionIndex(-1);
        listbox_ship->setSelectionIndex(-1);
    });
    listbox_other->setTextSize(20)->setButtonHeight(30)->setPosition(350, -20, ABottomLeft)->setSize(300, 250);
    listbox_other->addEntry("Warp Jammer", "WarpJammer");
    listbox_other->addEntry("Mine", "Mine");
    listbox_other->addEntry("Asteroid", "Asteroid");
    listbox_other->addEntry("BlackHole", "BlackHole");
    listbox_other->addEntry("Nebula", "Nebula");
    listbox_other->addEntry("WormHole", "WormHole");
    listbox_other->addEntry("Planet", "Planet");

    std::vector<string> class_names = ShipTemplate::getTemplateClassList(ShipTemplate::Ship);
    std::sort(class_names.begin(), class_names.end());
    ship_class_selector = new GuiSelector(box, "CLASS_SELECTOR", [this](int index, string value){
        listShipTemplate(value, regex_ship->getText());
    });

    for(string class_name : class_names)
        ship_class_selector->addEntry(class_name, class_name);
    ship_class_selector->setSelectionIndex(0);
    ship_class_selector->setPosition(-20, 20, ATopRight)->setSize(300, 50);

    regex_ship = new GuiTextEntry(box, "SECTOR_NAME_TEXT", "");
    regex_ship->setPosition(-20, 80, ATopRight)->setSize(300, 50);
    regex_ship->enterCallback([this](string text){
        listbox_ship->setSelectionIndex(-1);
        listbox_station->setSelectionIndex(-1);
        listbox_other->setSelectionIndex(-1);
        listShipTemplate(ship_class_selector->getSelectionValue(), text);
    });

    listbox_ship = new GuiListbox(box, "CREATE_SHIPS", [this](int index, string value)
    {
        script = "CpuShip():setRotation(random(0, 360)):setTemplate(\"" + value + "\"):orderRoaming()";
//        model_view->setModel(ShipTemplate::getTemplate(value)->model_data);
        listbox_station->setSelectionIndex(-1);
        listbox_other->setSelectionIndex(-1);
    });
    listbox_ship->setTextSize(20)->setButtonHeight(30)->setPosition(-20, 140, ATopRight)->setSize(300, 340);

    (new GuiButton(box, "CLOSE_BUTTON", "Cancel", [this]() {
        create_script = "";
        this->hide();
    }))->setPosition(20, -20, ABottomLeft)->setSize(300, 50);
}

bool GuiObjectCreationView::onMouseDown(sf::Vector2f position)
{
    //Catch clicks.
    return true;
}

void GuiObjectCreationView::setCreateScript(string script)
{
    create_script = script;
    enterCreateMode();
}

void GuiObjectCreationView::createObject(sf::Vector2f position)
{
    if (create_script == "")
        return;
    gameMasterActions->commandRunScript(create_script + ":setPosition("+string(position.x)+","+string(position.y)+")");
}

void GuiObjectCreationView::listShipTemplate(string class_name, string reg)
{
    std::vector<string> template_names = ShipTemplate::getTemplateNameList(ShipTemplate::Ship);
    std::sort(template_names.begin(), template_names.end());

    std::regex name_rgx("(.*)("+reg+")(.*)", std::regex_constants::icase);

    listbox_ship->setOptions({});
    P<ShipTemplate> ship_template;
    for(string template_name : template_names)
    {
        ship_template = ShipTemplate::getTemplate(template_name);
        string test_name = ship_template->getClass();
        if (test_name != class_name)
            continue;
        if (reg == "" || std::regex_match(template_name,name_rgx))
            listbox_ship->addEntry(template_name, template_name);
    }
}
