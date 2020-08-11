#ifndef DRONE_OPERATOR_SCREEN_H
#define DRONE_OPERATOR_SCREEN_H

#include "playerInfo.h"
#include "screens/crew1/singlePilotScreen.h"

#include "gui/gui2_overlay.h"

#include "gui/gui2_autolayout.h"
#include "gui/gui2_panel.h"
#include "gui/gui2_label.h"
#include "gui/gui2_listbox.h"

class GuiKeyValueDisplay;
class GuiAutoLayout;
class GuiButton;
class GuiToggleButton;
class GuiSlider;
class GuiLabel;
class GuiTextEntry;
class GuiListbox;
class GuiCustomShipFunctions;

class DroneOperatorScreen : public GuiOverlay
{
private:
    enum EMode
    {
        DroneSelection,
        Piloting,
        NoDrones
    };
    EMode mode;
    P<PlayerSpaceship> selected_drone;

    SinglePilotScreen* single_pilot_screen;
    GuiOverlay* background_crosses;
    GuiAutoLayout* droneSelection;
    GuiLabel* connection_label;
    GuiButton* disconnect_button;
    GuiLabel* no_drones_label;
    GuiListbox* drone_list;
    GuiCustomShipFunctions* custom_functions;
public:
    DroneOperatorScreen(GuiContainer* owner);

    virtual void onDraw(sf::RenderTarget& window);
private:
    void disconnected();
    bool isOperated(P<PlayerSpaceship> ship);
    bool isConnectableDrone(P<PlayerSpaceship> ship);
    bool isShip(P<PlayerSpaceship> ship);
    float getConnectionQuality(P<PlayerSpaceship> ship);
};

#endif//DRONE_OPERATOR_SCREEN_H
