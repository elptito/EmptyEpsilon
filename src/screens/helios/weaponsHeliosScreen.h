#ifndef WEAPONS_HELIOS_SCREEN_H
#define WEAPONS_HELIOS_SCREEN_H

#include "gui/gui2_overlay.h"
#include "screenComponents/radarView.h"
#include "screenComponents/targetsContainer.h"
#include "screenComponents/missileAim.h"
#include "gui/joystickConfig.h"

// class GuiMissileTubeControls;
class GuiKeyValueDisplay;
class GuiRotationDial;
class GuiProgressbar;
class GuiLabel;

class WeaponsHeliosScreen : public GuiOverlay, public MissileAim
{
private:

    GuiRadarView* radar;
    TargetsContainer targets;
    GuiRotationDial* missile_aim;

    GuiKeyValueDisplay* energy_display;
    GuiKeyValueDisplay* front_shield_display;
    GuiKeyValueDisplay* rear_shield_display;
    GuiLabel* beams_display;
    GuiProgressbar* shields_display;
    GuiKeyValueDisplay* new_shields_frequency_display;

    GuiKeyValueDisplay* load_type_rows[MW_Count];
    GuiProgressbar* tube_rows[max_weapon_tubes];

    EMissileWeapons load_type;
    bool manual_aim;
    float missile_target_angle;
public:
    WeaponsHeliosScreen(GuiContainer* owner);
    
    virtual void onDraw(sf::RenderTarget& window) override;
    virtual void onHotkey(const HotkeyResult& key) override;
    virtual bool onJoystickAxis(const AxisAction& axisAction) override;

    virtual float getMissileTargetAngle() override;
    virtual bool getManualAim() override;
};

#endif//WEAPONS_HELIOS_SCREEN_H
