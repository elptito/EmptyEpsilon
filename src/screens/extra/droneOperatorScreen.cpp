#include "droneOperatorScreen.h"

#include "playerInfo.h"
#include "gameGlobalInfo.h"

#include "screenComponents/alertOverlay.h"
#include "screenComponents/noiseOverlay.h"

#include "gui/gui2_overlay.h"
#include "gui/gui2_autolayout.h"
#include "gui/gui2_panel.h"
#include "gui/gui2_label.h"
#include "gui/gui2_listbox.h"
#include "screenComponents/powerDamageIndicator.h"

const ECrewPosition crewPosition = ECrewPosition::singlePilot;
DroneOperatorScreen::DroneOperatorScreen(GuiContainer *owner)
    : GuiOverlay(owner, "DRONE_PILOT_SCREEN", colorConfig.background), mode(DroneSelection)
{
    background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));

    // Draw a container for drone selection UI
    droneSelection = new GuiPanel(this, "");
    droneSelection->setPosition(0, 0, ACenter)->setSize(500, 500);

    // Drone list
    drone_list = new GuiListbox(droneSelection, "PLAYER_SHIP_LIST", [this](int _, string value) {connect(value);});
    drone_list->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    // single pilot UI
    single_pilot_view = new SinglePilotView(this, selected_drone);
    single_pilot_view->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    noise_overlay = new GuiNoiseOverlay(this, "CONNECTION_NOISE", false);
    
    connection_label = new GuiLabel(this, "CONNECTION_LABEL", "0%", 30);
    connection_label->setPosition(0, -50, ABottomCenter)->setSize(460, 50);

    disconnect_button = new GuiButton(this, "DISCONNECT_BUTTON", "Disconnect", [this]() {disconnect();});
    disconnect_button->setPosition(0, 0, ABottomCenter)->setSize(400, 50);
    disconnect_button->moveToFront();
    // label for when there are no drones
    no_drones_label = new GuiLabel(this, "SHIP_SELECTION_NO_SHIPS_LABEL", "No active drones in range", 30);
    no_drones_label->setPosition(0, 100, ATopCenter)->setSize(460, 50);
    // Prep the alert overlay.
    (new GuiPowerDamageIndicator(this, "DRONES_PDI", SYS_Drones, ATopCenter, my_spaceship))->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
}

void DroneOperatorScreen::connect(string value){
    if (value != ""){
        P<PlayerSpaceship> ship = getObjectById(value.toInt());
        // If the selected item is a ship ...
        if (ship)
        // TODO :  check if occupied
        {
            mode = Piloting;
            selected_drone = ship;
            single_pilot_view->setTargetSpaceship(selected_drone);
        }
    }
}

void DroneOperatorScreen::disconnect()
{
    mode = drone_list->entryCount() == 0 ? NoDrones : DroneSelection;
    selected_drone = NULL;
    single_pilot_view->setTargetSpaceship(selected_drone);
}

bool DroneOperatorScreen::isConnectable(P<PlayerSpaceship> ship)
{
    return ship 
    && ship->ship_template 
    && ship->ship_template->getType() == ShipTemplate::TemplateType::Drone 
    && ship->getFactionId() == my_spaceship->getFactionId()
    && getConnectionQuality(ship) >= 0.01f; 
}
float DroneOperatorScreen::getConnectionQuality(P<PlayerSpaceship> ship)
{
    float rangeFactor = 1 - std::min(1.0f, (length(ship->getPosition() - my_spaceship->getPosition()) / my_spaceship->getDronesControlRange()));
    float droneStateFactor = std::min(1.0f, ship->getSystemEffectiveness(SYS_Drones));
    return rangeFactor * droneStateFactor;
}
void DroneOperatorScreen::onDraw(sf::RenderTarget &window)
{
    if (my_spaceship)
    {
        // Update the player ship list with all player ships.
        std::vector<string> options;
        std::vector<string> values;
        for (int n = 0; n < GameGlobalInfo::max_player_ships; n++)
        {
            P<PlayerSpaceship> ship = gameGlobalInfo->getPlayerShip(n);
            if (isConnectable(ship)) {
                options.push_back(ship->getTypeName() + " " + ship->getCallSign() + "(" + string(int(getConnectionQuality(ship) * 100)) + "%)");
                values.push_back(ship->getMultiplayerId());
            }
        }
        drone_list->setOptions(options, values);
        // automatically change mode if needed
        if (!selected_drone || !isConnectable(selected_drone) || selected_drone->isDestroyed())
        {
           disconnect();
        }
        // update display according to mode
        switch (mode)
        {
        case DroneSelection:
            noise_overlay->hide();
            no_drones_label->hide();
            droneSelection->show();
            single_pilot_view->hide();
            disconnect_button->hide();
            connection_label->hide();
            break;
        case Piloting:
            noise_overlay->show();
            noise_overlay->setAlpha(std::max(0, int(Tween<float>::linear(
                getConnectionQuality(selected_drone),
                0, 0.5, 220, 0
            ))));
            no_drones_label->hide();
            droneSelection->hide();
            single_pilot_view->show();
            disconnect_button->setText("Disconnect " + selected_drone->callsign);
            disconnect_button->show();
            connection_label->setText(string(int(getConnectionQuality(selected_drone) * 100)) + "%");
            connection_label->show();
            break;
        case NoDrones:
            noise_overlay->hide();
            no_drones_label->show();
            droneSelection->hide();
            single_pilot_view->hide();
            disconnect_button->hide();
            connection_label->hide();
            break;
        }
    }
}

void DroneOperatorScreen::onHotkey(const HotkeyResult& key)
{
    if (my_spaceship){
        if (key.category == "DRONE_OPERATOR")
        {
            if (key.hotkey == "DISCONNECT" && mode == Piloting) {
                disconnect();
                drone_list->setSelectionIndex(-1);
            } else if (key.hotkey == "CONNECT" && mode == DroneSelection) {
                connect(drone_list->getSelectionValue());
            } else if (key.hotkey == "PREV_DRONE") {
                int idx = drone_list->getSelectionIndex() - 1;
                if (idx < -1) {
                    idx = drone_list->entryCount() - 1;
                }
                drone_list->setSelectionIndex(idx);
            } else if (key.hotkey == "NEXT_DRONE") {
                int idx = drone_list->getSelectionIndex() + 1;
                if (idx >= drone_list->entryCount()) {
                    idx = -1;
                }
                drone_list->setSelectionIndex(idx);
            }
        }
    }
}