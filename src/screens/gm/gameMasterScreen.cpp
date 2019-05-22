#include "main.h"
#include "gameGlobalInfo.h"
#include "GMActions.h"
#include "gameMasterScreen.h"
#include "objectCreationView.h"
#include "globalMessageEntryView.h"
#include "tweak.h"
#include "chatDialog.h"
#include "spaceObjects/cpuShip.h"
#include "spaceObjects/spaceStation.h"
#include "spaceObjects/explosionEffect.h"
#include "spaceObjects/zone.h"

#include "screenComponents/radarView.h"

#include "gui/gui2_togglebutton.h"
#include "gui/gui2_selector.h"
#include "gui/gui2_autolayout.h"
#include "gui/gui2_listbox.h"
#include "gui/gui2_label.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_textentry.h"

GameMasterScreen::GameMasterScreen()
: click_and_drag_state(CD_None)
{
    main_radar = new GuiRadarView(this, "MAIN_RADAR", 50000.0f, &targets, my_spaceship); // my_spaceship === nullptr
    main_radar->setStyle(GuiRadarView::Rectangular)->longRange()->gameMaster()->enableTargetProjections(nullptr)->setAutoCentering(false);
    main_radar->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    main_radar->setCallbacks(
        [this](sf::Vector2f position) { this->onMouseDown(position); },
        [this](sf::Vector2f position) { this->onMouseDrag(position); },
        [this](sf::Vector2f position) { this->onMouseUp(position); }
    );
    box_selection_overlay = new GuiOverlay(main_radar, "BOX_SELECTION", sf::Color(255, 255, 255, 32));
    box_selection_overlay->hide();

    pause_button = new GuiToggleButton(this, "PAUSE_BUTTON", "Pause", [this](bool value) {
        if (!value)
            gameMasterActions->commandSetGameSpeed(1.0f);
        else
            gameMasterActions->commandSetGameSpeed(0.0f);
    });
    pause_button->setValue(engine->getGameSpeed() == 0.0f)->setPosition(20, 20, ATopLeft)->setSize(250, 50);

    if (gameGlobalInfo->intercept_all_comms_to_gm < CGI_Always){
        intercept_comms_button = new GuiToggleButton(this, "INTERCEPT_COMMS_BUTTON", "Intercept all comms", [this](bool value) {
            gameMasterActions->commandInterceptAllCommsToGm(value);
        });
        intercept_comms_button->setValue((int)gameGlobalInfo->intercept_all_comms_to_gm)->setTextSize(20)->setPosition(300, 20, ATopLeft)->setSize(200, 25);
    }

    faction_selector = new GuiSelector(this, "FACTION_SELECTOR", [this](int index, string value) {
        gameMasterActions->commandSetFactionId(index, targets.getTargets());
    });
    for(P<FactionInfo> info : factionInfo)
        faction_selector->addEntry(info->getName(), info->getName());
    faction_selector->setPosition(20, 70, ATopLeft)->setSize(250, 50);

    personality_selector = new GuiSelector(this, "FACTION_SELECTOR", [this](int index, string value) {
        for(P<SpaceObject> obj : targets.getTargets())
        {
            obj->setPersonalityId(index);
        }
    });
    personality_selector->addEntry("Normal","Normal");
    personality_selector->addEntry("Pacifique","Pacifique");
    personality_selector->addEntry("Hostile","Hostile");
    personality_selector->addEntry("Solo","Solo");
    personality_selector->setPosition(20, 120, ATopLeft)->setSize(250, 50);

    global_message_button = new GuiButton(this, "GLOBAL_MESSAGE_BUTTON", "Message global", [this]() {
        global_message_entry->show();
    });
    global_message_button->setPosition(20, -20, ABottomLeft)->setSize(250, 50);

    player_ship_selector = new GuiSelector(this, "PLAYER_SHIP_SELECTOR", [this](int index, string value) {
        P<SpaceObject> ship = gameGlobalInfo->getPlayerShip(value.toInt());
        if (ship)
            target = ship;
        main_radar->setViewPosition(ship->getPosition());
        if(!position_text_custom)
            position_text->setText(getStringFromPosition(ship->getPosition()));
        targets.set(ship);
    });
    player_ship_selector->setPosition(270, -20, ABottomLeft)->setSize(250, 50);

    position_text_custom = false;
    position_text = new GuiTextEntry(this, "SECTOR_NAME_TEXT", "");
    position_text->setPosition(620, -20, ABottomLeft)->setSize(250, 50);
    position_text->callback([this](string text){
        position_text_custom = true;
    });
    position_text->validator(isValidPositionString);
    position_text->enterCallback([this](string text){
        position_text_custom = false;
        if (position_text->isValid())
        {
            sf::Vector2f pos = getPositionFromSring(text);
            main_radar->setViewPosition(pos);
        }
    });
    position_text->setText(getStringFromPosition(main_radar->getViewPosition()));

    CPU_ship_selector = new GuiSelector(this, "CPU_SHIP_SELECTOR", [this](int index, string value) {
        P<SpaceShip> ship = space_object_list[value.toInt()];
        if (ship)
            target = ship;
        main_radar->setViewPosition(ship->getPosition());
        targets.set(ship);
    });
    CPU_ship_selector->setPosition(270, -70, ABottomLeft)->setSize(250, 50);

    space_station_selector = new GuiSelector(this, "SPACE_STATION_SELECTOR", [this](int index, string value) {
        P<SpaceObject> station = space_object_list[value.toInt()];
        if (station)
            target = station;
        main_radar->setViewPosition(station->getPosition());
        targets.set(station);
    });
    space_station_selector->setPosition(270, -120, ABottomLeft)->setSize(250, 50);

    //create_button = new GuiButton(this, "CREATE_OBJECT_BUTTON", "Creer...", [this]() {
    //    object_creation_screen->show();
    //});
    //create_button->setPosition(20, -70, ABottomLeft)->setSize(250, 50);

    copy_scenario_button = new GuiButton(this, "COPY_SCENARIO_BUTTON", "Copier scenario", [this]() {
        Clipboard::setClipboard(getScriptExport(false));
    });
    copy_scenario_button->setTextSize(20)->setPosition(-20, -20, ABottomRight)->setSize(125, 25);

    copy_selected_button = new GuiButton(this, "COPY_SELECTED_BUTTON", "Copier selection", [this]() {
        Clipboard::setClipboard(getScriptExport(true));
    });
    copy_selected_button->setTextSize(20)->setPosition(-20, -45, ABottomRight)->setSize(125, 25);

    create_button = new GuiButton(this, "CREATE_OBJECT_BUTTON", "Create...", [this]() {
        object_creation_view->show();
    });
    create_button->setPosition(20, -70, ABottomLeft)->setSize(250, 50);

    cancel_create_button = new GuiButton(this, "CANCEL_CREATE_BUTTON", "Annuler", [this]() {
        create_button->show();
        cancel_create_button->hide();
    });
    cancel_create_button->setPosition(20, -70, ABottomLeft)->setSize(250, 50)->hide();

    tweak_button = new GuiButton(this, "TWEAK_OBJECT", "Tweak", [this]() {
        for(P<SpaceObject> obj : targets.getTargets())
        {
            if (P<PlayerSpaceship>(obj))
            {
                player_tweak_dialog->open(obj);
                break;
            }
            else if (P<SpaceShip>(obj))
            {
                ship_tweak_dialog->open(obj);
                break;
            }
			else if (P<ShipTemplateBasedObject>(obj))
			{
                template_tweak_dialog->open(obj);
                break;
            }
            else
            {
                object_tweak_dialog->open(obj);
                break;
            }
        }
    });
    // tweaks only work on the server
    tweak_button->setPosition(20, -120, ABottomLeft)->setSize(250, 50)->setEnable(bool(game_server))->hide();

    player_comms_hail = new GuiButton(this, "HAIL_PLAYER", "Contacter vaisseau", [this]() {
        for(P<SpaceObject> obj : targets.getTargets())
        {
            if (P<PlayerSpaceship>(obj))
            {
                int idx = gameGlobalInfo->findPlayerShip(obj);
                chat_dialog_per_ship[idx]->show()->setPosition(main_radar->worldToScreen(obj->getPosition()))->setSize(300, 300);
            }
        }
    });
    player_comms_hail->setPosition(20, -170, ABottomLeft)->setSize(250, 50)->hide();

    info_layout = new GuiAutoLayout(this, "INFO_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    info_layout->setPosition(-20, 20, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    gm_script_options = new GuiListbox(this, "GM_SCRIPT_OPTIONS", [this](int index, string value)
    {
        gm_script_options->setSelectionIndex(-1);
        gameMasterActions->commandCallGmScript(index, getSelection());
    });
    gm_script_options->setPosition(20, 200, ATopLeft)->setSize(250, 500);

    order_layout = new GuiAutoLayout(this, "ORDER_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    order_layout->setPosition(-20, 350, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    (new GuiLabel(order_layout, "ORDERS_LABEL", "Ordres:", 20))->addBackground()->setSize(GuiElement::GuiSizeMax, 30);
    (new GuiButton(order_layout, "ORDER_IDLE", "Repos", [this]() {
        gameMasterActions->commandOrderShip(SO_Idle, getSelection());
    }))->setTextSize(20)->setSize(GuiElement::GuiSizeMax, 30);
    (new GuiButton(order_layout, "ORDER_ROAMING", "Errant", [this]() {
        gameMasterActions->commandOrderShip(SO_Roaming, getSelection());
    }))->setTextSize(20)->setSize(GuiElement::GuiSizeMax, 30);
    (new GuiButton(order_layout, "ORDER_STAND_GROUND", "En place", [this]() {
        gameMasterActions->commandOrderShip(SO_StandGround, getSelection());
    }))->setTextSize(20)->setSize(GuiElement::GuiSizeMax, 30);
    (new GuiButton(order_layout, "ORDER_DEFEND_LOCATION", "Defendre localisation", [this]() {
        gameMasterActions->commandOrderShip(SO_DefendLocation, getSelection());
    }))->setTextSize(20)->setSize(GuiElement::GuiSizeMax, 30);

    chat_layer = new GuiElement(this, "");
    chat_layer->setPosition(0, 0)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    for(int n=0; n<GameGlobalInfo::max_player_ships; n++)
    {
        chat_dialog_per_ship.push_back(new GameMasterChatDialog(chat_layer, main_radar, n));
        chat_dialog_per_ship[n]->hide();
    }

    player_tweak_dialog = new GuiObjectTweak(this, TW_Player);
    player_tweak_dialog->hide();
	template_tweak_dialog = new GuiObjectTweak(this, TW_Template);
    template_tweak_dialog->hide();
    ship_tweak_dialog = new GuiObjectTweak(this, TW_Ship);
    ship_tweak_dialog->hide();
    object_tweak_dialog = new GuiObjectTweak(this, TW_Object);
    object_tweak_dialog->hide();

    global_message_entry = new GuiGlobalMessageEntryView(this);
    global_message_entry->hide();
    object_creation_view = new GuiObjectCreationView(this, [this](){
        create_button->hide();
        cancel_create_button->show();
        object_creation_view->hide();
    });
    object_creation_view->hide();
}

void GameMasterScreen::update(float delta)
{
    float mouse_wheel_delta = InputHandler::getMouseWheelDelta();
    if (mouse_wheel_delta != 0.0)
    {
        float view_distance = main_radar->getDistance() * (1.0 - (mouse_wheel_delta * 0.1f));
        if (view_distance > max_distance)
            view_distance = max_distance;
        if (view_distance < min_distance)
            view_distance = min_distance;
        main_radar->setDistance(view_distance);
        if (view_distance < 10000)
            main_radar->shortRange();
        else
            main_radar->longRange();
    }

    bool has_object = false;
    bool has_cpu_ship = false;
    bool has_player_ship = false;

    // Add and remove entries from the player ship list.
    for(int n=0; n<GameGlobalInfo::max_player_ships; n++)
    {
        P<PlayerSpaceship> ship = gameGlobalInfo->getPlayerShip(n);
        if (ship)
        {
            if (player_ship_selector->indexByValue(string(n)) == -1)
                player_ship_selector->addEntry(ship->getTypeName() + " " + ship->getCallSign(), string(n));

            if (ship->isCommsBeingHailedByGM() || ship->isCommsChatOpenToGM())
            {
                if (!chat_dialog_per_ship[n]->isVisible())
                {
                    chat_dialog_per_ship[n]->show()->setPosition(main_radar->worldToScreen(ship->getPosition()))->setSize(300, 300);
                }
            }
        }else{
            if (player_ship_selector->indexByValue(string(n)) != -1)
                player_ship_selector->removeEntry(player_ship_selector->indexByValue(string(n)));
        }
    }

    // Add and remove entries from the CPU ship and space station list.
    int n = 0;
    foreach(SpaceObject, obj, space_object_list)
    {
        P<SpaceShip> ship = obj;
        P<SpaceStation> station = obj;
        if (ship)
        {
            if (CPU_ship_selector->indexByValue(string(n)) == -1)
                CPU_ship_selector->addEntry(ship->getTypeName() + " " + ship->getCallSign(), string(n));
        }else{
            if (CPU_ship_selector->indexByValue(string(n)) != -1)
                CPU_ship_selector->removeEntry(CPU_ship_selector->indexByValue(string(n)));
        }
        if (station)
        {
            if (space_station_selector->indexByValue(string(n)) == -1)
                space_station_selector->addEntry(station->getTypeName() + " " + station->getCallSign(), string(n));
        }else{
            if (space_station_selector->indexByValue(string(n)) != -1)
                space_station_selector->removeEntry(space_station_selector->indexByValue(string(n)));
        }
    n += 1;
    }

    // Record object type.
    for(P<SpaceObject> obj : targets.getTargets())
    {
        has_object = true;
        if (P<CpuShip>(obj))
            has_cpu_ship = true;
        else if (P<PlayerSpaceship>(obj))
            has_player_ship = true;
    }

    // Show player ship selector only if there are player ships.
    player_ship_selector->setVisible(player_ship_selector->entryCount() > 0);

    // Show tweak button.
    tweak_button->setVisible(has_object);

    order_layout->setVisible(has_cpu_ship);
//    gm_script_options->setVisible(!has_cpu_ship);
    player_comms_hail->setVisible(has_player_ship);

    std::unordered_map<string, string> selection_info;

    // For each selected object, determine and report their type.
    for(P<SpaceObject> obj : targets.getTargets())
    {
        std::unordered_map<string, string> info = obj->getGMInfo();
        for(std::unordered_map<string, string>::iterator i = info.begin(); i != info.end(); i++)
        {
            if (selection_info.find(i->first) == selection_info.end())
            {
                selection_info[i->first] = i->second;
            }
            else if (selection_info[i->first] != i->second)
            {
                selection_info[i->first] = "*mixed*";
            }
        }
    }

    if (targets.getTargets().size() == 1)
    {
        P<SpaceObject> target = targets.getTargets()[0];
        selection_info["Position"] = string(target->getPosition().x, 0) + "," + string(target->getPosition().y, 0);

        for(int n=0; n<GameGlobalInfo::max_player_ships; n++)
        {
            P<PlayerSpaceship> ship = gameGlobalInfo->getPlayerShip(n);
            if (ship)
            {
                float distance = sf::length(targets.getTargets()[0]->getPosition() - ship->getPosition());
                selection_info["Distance PJ " + string(n+1)] = string(distance / 1000.0f,0) + " U";
            }
        }
    }

    unsigned int cnt = 0;
    for(std::unordered_map<string, string>::iterator i = selection_info.begin(); i != selection_info.end(); i++)
    {
        if (cnt == info_items.size())
        {
            info_items.push_back(new GuiKeyValueDisplay(info_layout, "INFO_" + string(cnt), 0.5, i->first, i->second));
            info_items[cnt]->setSize(GuiElement::GuiSizeMax, 30);
        }else{
            info_items[cnt]->show();
            info_items[cnt]->setKey(i->first)->setValue(i->second);
        }
        cnt++;
    }
    while(cnt < info_items.size())
    {
        info_items[cnt]->hide();
        cnt++;
    }

    bool gm_functions_changed = gm_script_options->entryCount() != int(gameGlobalInfo->gm_callback_names.size());
    auto it = gameGlobalInfo->gm_callback_names.begin();
    for(int n=0; !gm_functions_changed && n<gm_script_options->entryCount(); n++)
    {
        if (gm_script_options->getEntryName(n) != *it)
            gm_functions_changed = true;
        it++;
    }
    if (gm_functions_changed)
    {
        gm_script_options->setOptions({});
        for(const string& callbackName : gameGlobalInfo->gm_callback_names)
        {
            gm_script_options->addEntry(callbackName, callbackName);
        }
    }
    pause_button->setValue(engine->getGameSpeed() == 0.0f);
    if (gameGlobalInfo->intercept_all_comms_to_gm < CGI_Always)
        intercept_comms_button->setValue(gameGlobalInfo->intercept_all_comms_to_gm);
}

void GameMasterScreen::onMouseDown(sf::Vector2f position)
{
    if (click_and_drag_state != CD_None)
        return;
    if (InputHandler::mouseIsDown(sf::Mouse::Right))
    {
        click_and_drag_state = CD_DragViewOrOrder;
    }
    else
    {
        if (cancel_create_button->isVisible())
        {
            object_creation_view->createObject(position);
        }else{
            click_and_drag_state = CD_BoxSelect;

            float min_drag_distance = main_radar->getDistance() / 450 * 10;

            for(P<SpaceObject> obj : targets.getTargets())
            {
                if ((obj->getPosition() - position) < std::max(min_drag_distance, obj->getRadius()))
                    click_and_drag_state = CD_DragObjects;
            }
        }
    }
    drag_start_position = position;
    drag_previous_position = position;
}

void GameMasterScreen::onMouseDrag(sf::Vector2f position)
{
    switch(click_and_drag_state)
    {
    case CD_DragViewOrOrder:
    case CD_DragView:
        click_and_drag_state = CD_DragView;
        main_radar->setViewPosition(main_radar->getViewPosition() - (position - drag_previous_position));
        if(!position_text_custom)
            position_text->setText(getStringFromPosition(main_radar->getViewPosition()));
        position -= (position - drag_previous_position);
        break;
    case CD_DragObjects:
        gameMasterActions->commandMoveObjects(position - drag_previous_position, targets.getTargets());
        break;
    case CD_BoxSelect:
        box_selection_overlay->show();
        box_selection_overlay->setPosition(main_radar->worldToScreen(drag_start_position), ATopLeft);
        box_selection_overlay->setSize(main_radar->worldToScreen(position) - main_radar->worldToScreen(drag_start_position));
        break;
    default:
        break;
    }
    drag_previous_position = position;
}

void GameMasterScreen::onMouseUp(sf::Vector2f position)
{
    switch(click_and_drag_state)
    {
    case CD_DragViewOrOrder:
        {
            //Right click
            bool shift_down = InputHandler::keyboardIsDown(sf::Keyboard::LShift) || InputHandler::keyboardIsDown(sf::Keyboard::RShift);
            gameMasterActions->commandContextualGoTo(position, shift_down, targets.getTargets());
        }
        break;
    case CD_BoxSelect:
        {
            PVector<Collisionable> objects = CollisionManager::queryArea(drag_start_position, position);
            PVector<SpaceObject> space_objects;
            foreach(Collisionable, c, objects)
            {
                if (!P<Zone>(c))
                    space_objects.push_back(c);
            }
            targets.set(space_objects);
            if (space_objects.size() > 0)
            {
                faction_selector->setSelectionIndex(space_objects[0]->getFactionId());
                personality_selector->setSelectionIndex(space_objects[0]->getPersonalityId());
            }
        }
        break;
    default:
        break;
    }
    click_and_drag_state = CD_None;
    box_selection_overlay->hide();
}

void GameMasterScreen::onKey(sf::Event::KeyEvent key, int unicode)
{
    switch(key.code)
    {
    case sf::Keyboard::Delete:
        gameMasterActions->commandDestroy(targets.getTargets());
        break;
    case sf::Keyboard::F5:
        Clipboard::setClipboard(getScriptExport(false));
        break;

    case sf::Keyboard::Escape:
    case sf::Keyboard::Home:
        destroy();
        returnToShipSelection();
        break;
    case sf::Keyboard::P:
        if (engine->getGameSpeed() == 0.0f)
            gameMasterActions->commandSetGameSpeed(1.0f);
        else
            gameMasterActions->commandSetGameSpeed(0.0f);
        break;
    default:
        break;
    }
}

PVector<SpaceObject> GameMasterScreen::getSelection()
{
    return targets.getTargets();
}

string GameMasterScreen::getScriptExport(bool selected_only)
{
    string output;
    PVector<SpaceObject> objs;
    if (selected_only)
    {
        objs = targets.getTargets();
    }else{
        objs = space_object_list;
    }

    foreach(SpaceObject, obj, objs)
    {
        string line = obj->getExportLine();
        if (line == "")
            continue;
        output += "    " + line + "\n";
    }
    return output;
}
