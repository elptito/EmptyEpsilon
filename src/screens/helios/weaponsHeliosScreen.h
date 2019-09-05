#ifndef WEAPONS_HELIOS_SCREEN_H
#define WEAPONS_HELIOS_SCREEN_H

#include "gui/gui2_overlay.h"
#include "screenComponents/missileAim.h"
#include "gui/joystickConfig.h"
#include "screenComponents/targetsContainer.h"

// class GuiMissileTubeControls;
class GuiKeyValueDisplay;
class GuiRotationDial;
class GuiProgressbar;
class GuiLabel;
class GuiRadarView;
class GuiAutoLayout;

class WeaponsHeliosScreen : public GuiOverlay, public MissileAim
{
private:

    GuiRadarView* radar;
    GuiRotationDial* missile_aim;

    GuiKeyValueDisplay* energy_display;
    GuiKeyValueDisplay* front_shield_display;
    GuiKeyValueDisplay* rear_shield_display;
    GuiLabel* beams_display;
    GuiProgressbar* shields_display;
    GuiKeyValueDisplay* next_shields_frequency_display;

    GuiKeyValueDisplay* load_type_rows[MW_Count];
    GuiProgressbar* tube_rows[max_weapon_tubes];

    GuiLabel* target_mode_near_display;
    GuiLabel* target_mode_enemy_display;
    GuiLabel* target_callsign;
    GuiKeyValueDisplay* target_distance;
    GuiKeyValueDisplay* target_heading;

    TargetsContainer targets;
    EMissileWeapons load_type;
    float missile_target_angle;
    int next_shields_frequency;
    bool manual_aim;
    bool target_mode_near;
    bool target_mode_enemy;
public:
    WeaponsHeliosScreen(GuiContainer* owner);
    
    virtual void onDraw(sf::RenderTarget& window) override;
    virtual void onHotkey(const HotkeyResult& key) override;
    virtual bool onJoystickAxis(const AxisAction& axisAction) override;

    virtual float getMissileTargetAngle() override;
    virtual bool getManualAim() override;
    void iterateTagrets(bool forward, bool enemiesOnly, bool nearOnly);
};

#endif//WEAPONS_HELIOS_SCREEN_H
