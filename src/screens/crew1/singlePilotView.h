#ifndef SINGLE_PILOT_VIEW_H
#define SINGLE_PILOT_VIEW_H

#include "gui/gui2_overlay.h"
#include "screenComponents/targetsContainer.h"
#include "gui/joystickConfig.h"

class GuiMissileTubeControls;
class GuiRadarView;
class GuiKeyValueDisplay;
class GuiToggleButton;
class GuiRotationDial;
class GuiWarpControls;
class GuiJumpControls;
class GuiCombatManeuver;
class GuiImpulseControls;
class GuiDockingButton;
class GuiShieldsEnableButton;
class GuiCustomShipFunctions;
class AimLockButton;
class GuiLandingButton;

class SinglePilotView : public GuiElement, JoystickMappable
{
private:
    P<PlayerSpaceship> target_spaceship;
    GuiOverlay* background_gradient;

    GuiKeyValueDisplay* heat_display;
    GuiKeyValueDisplay* hull_display;
    GuiKeyValueDisplay* energy_display;
    GuiKeyValueDisplay* heading_display;
    GuiKeyValueDisplay* velocity_display;
    GuiKeyValueDisplay* shields_display;
    GuiWarpControls* warp_controls;
    GuiJumpControls* jump_controls;

    TargetsContainer targets;
    GuiRadarView* radar;
    GuiCombatManeuver* combat_maneuver;
    GuiRotationDial* missile_aim;
    GuiMissileTubeControls* tube_controls;
    GuiImpulseControls* impulse_controls;
    GuiDockingButton* docking_button;
    GuiLandingButton* landing_button;
    GuiShieldsEnableButton* shields_enable_button;
    GuiCustomShipFunctions* custom_ship_functions;
    AimLockButton* lock_aim;
    GuiToggleButton* lock_fire;

public:
    SinglePilotView(GuiContainer* owner, P<PlayerSpaceship> targetSpaceship);

    virtual void onDraw(sf::RenderTarget& window);
    virtual void onHotkey(const HotkeyResult& key) override;
    void setTargetSpaceship(P<PlayerSpaceship> targetSpaceship);
    virtual void onJoystickAxis(AxisAction& axisAction) override;
};

#endif//SINGLE_PILOT_VIEW_H
