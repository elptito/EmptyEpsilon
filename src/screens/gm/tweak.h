#ifndef TWEAK_H
#define TWEAK_H

#include "gui/gui2_panel.h"
#include "missileWeaponData.h"
#include "shipTemplate.h"
#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "spaceObjects/warpJammer.h"
#include "spaceObjects/asteroid.h"

class SpaceShip;
class GuiKeyValueDisplay;
class GuiLabel;
class GuiTextEntry;
class GuiSlider;
class GuiSelector;
class GuiToggleButton;
class GuiButton;
class GuiProgressbar;

enum ETweakType
{
    TW_Object,  // TODO: Space object
    TW_Jammer,  // WarpJammer
    TW_Ship,    // Ships
    TW_Station, // TODO: Space stations
    TW_Player,  // Player ships
    TW_Asteroid // Asteroid
};

class GuiTweakPage : public GuiElement
{
public:
    GuiTweakPage(GuiContainer* owner) : GuiElement(owner, "") {}

    virtual void open(P<SpaceObject> target) = 0;
};

class GuiObjectTweak : public GuiPanel
{
public:
    GuiObjectTweak(GuiContainer* owner, ETweakType tweak_type);

    void open(P<SpaceObject> target);

    virtual void onDraw(sf::RenderTarget& window) override;
private:
    P<SpaceObject> target;
    std::vector<GuiTweakPage*> pages;
};

class GuiTweakShip : public GuiTweakPage
{
private:
    P<SpaceShip> target;

    GuiTextEntry* type_name;
    GuiToggleButton* reactor_toggle;
    GuiToggleButton* oxygen_generator_toggle;
    GuiToggleButton* warp_toggle;
    GuiToggleButton* jump_toggle;
    GuiSlider* impulse_speed_slider;
    GuiSlider* warp_speed_slider;
    GuiSlider* turn_speed_slider;
    GuiSlider* hull_max_slider;
    GuiSlider* hull_slider;
    GuiSlider* jump_charge_slider;
    GuiSlider* jump_min_distance_slider;
    GuiSlider* jump_max_distance_slider;
    GuiToggleButton* can_be_destroyed_toggle;
public:
    GuiTweakShip(GuiContainer* owner);

    virtual void onDraw(sf::RenderTarget& window) override;

    virtual void open(P<SpaceObject> target) override;
};

class GuiJammerTweak : public GuiTweakPage
{
private:
    P<WarpJammer> target;

    GuiSlider* jammer_range_slider;
public:
    GuiJammerTweak(GuiContainer* owner);

    virtual void onDraw(sf::RenderTarget& window) override;

    virtual void open(P<SpaceObject> target) override;
};

class GuiAsteroidTweak : public GuiTweakPage
{
private:
    P<Asteroid> target;

    GuiSlider* asteroid_size_slider;
public:
    GuiAsteroidTweak(GuiContainer* owner);

    virtual void onDraw(sf::RenderTarget& window) override;

    virtual void open(P<SpaceObject> target) override;
};

class GuiShipTweakMissileWeapons : public GuiTweakPage
{
private:
    P<SpaceShip> target;

    GuiSlider* missile_storage_amount_slider[MW_Count];
    GuiSlider* missile_current_amount_slider[MW_Count];
public:
    GuiShipTweakMissileWeapons(GuiContainer* owner);

    virtual void onDraw(sf::RenderTarget& window) override;

    virtual void open(P<SpaceObject> target) override;
};

class GuiShipTweakMissileTubes : public GuiTweakPage
{
private:
    P<SpaceShip> target;

    int tube_index;
    GuiSelector* index_selector;
    GuiSelector* missile_tube_amount_selector;
    GuiSlider* direction_slider;
    GuiSlider* load_time_slider;
    GuiSelector* size_selector;
    GuiToggleButton* allowed_use[MW_Count];
public:
    GuiShipTweakMissileTubes(GuiContainer* owner);

    virtual void onDraw(sf::RenderTarget& window) override;

    virtual void open(P<SpaceObject> target) override;
};

class GuiShipTweakShields : public GuiTweakPage
{
private:
    P<ShipTemplateBasedObject> target;

    GuiSlider* shields_count;
    GuiSlider* shield_max_slider[max_shield_count];
    GuiSlider* shield_slider[max_shield_count];
public:
    GuiShipTweakShields(GuiContainer* owner);

    virtual void onDraw(sf::RenderTarget& window) override;

    virtual void open(P<SpaceObject> target) override;
};

class GuiShipTweakBeamweapons : public GuiTweakPage
{
private:
    P<SpaceShip> target;

    int beam_index;
    GuiSlider* tractor_range_slider;
    GuiSlider* arc_slider;
    GuiSlider* direction_slider;
    GuiSlider* range_slider;
    GuiSlider* turret_arc_slider;
    GuiSlider* turret_direction_slider;
    GuiSlider* turret_rotation_rate_slider;
    GuiLabel* turret_rotation_rate_overlay_label;
    GuiSlider* cycle_time_slider;
    GuiSlider* damage_slider;
    GuiSelector* damage_type_slider;
public:
    GuiShipTweakBeamweapons(GuiContainer* owner);

    virtual void open(P<SpaceObject> target) override;

    virtual void onDraw(sf::RenderTarget& window) override;
};

class GuiShipTweakSystems : public GuiTweakPage
{
private:
    P<SpaceShip> target;

    int system_index;
    GuiElement* system_box[SYS_COUNT];
    GuiToggleButton* system_selector[SYS_COUNT];
    GuiSlider* system_factor[SYS_COUNT];
    GuiSlider* system_damage[SYS_COUNT];
    GuiSlider* system_health_max[SYS_COUNT];
    GuiSlider* system_heat[SYS_COUNT];
    GuiProgressbar* system_coolant_bar[SYS_COUNT];
    GuiSlider* system_coolant_slider[SYS_COUNT];
    GuiSlider* system_hacked[SYS_COUNT];
    GuiProgressbar* system_power_bar[SYS_COUNT];
    GuiSlider* system_power_slider[SYS_COUNT];
    GuiProgressbar* system_repair_bar[SYS_COUNT];
    GuiSlider* system_repair_slider[SYS_COUNT];
    GuiSlider* system_instability_factor[SYS_COUNT];
    GuiSlider* system_instability_difficulty[SYS_COUNT];
    GuiProgressbar* system_instability_level[SYS_COUNT];
public:
    GuiShipTweakSystems(GuiContainer* owner);

    virtual void open(P<SpaceObject> target) override;

    virtual void onDraw(sf::RenderTarget& window) override;
};

class GuiShipTweakOxygen : public GuiTweakPage
{
private:
    P<SpaceShip> target;

    int zone_index;
    GuiTextEntry* zone_label;
    GuiElement* zone_box[10];
    GuiToggleButton* zone_selector[10];
    GuiSlider* zone_oxygen_level[10];
    GuiSlider* zone_oxygen_max[10];
    GuiSlider* zone_recharge_rate[10];
    GuiLabel* zone_recharge_rate_label[10];
    GuiSlider* zone_discharge_rate[10];
    GuiLabel* zone_discharge_rate_label[10];
public:
    GuiShipTweakOxygen(GuiContainer* owner);

    virtual void open(P<SpaceObject> target) override;

    virtual void onDraw(sf::RenderTarget& window) override;
};

class GuiShipTweakPlayer : public GuiTweakPage
{
private:
    P<PlayerSpaceship> target;

    GuiTextEntry* control_code;
    GuiSlider* reputation_point_slider;
    GuiSlider* energy_level_slider;
    GuiSlider* max_energy_level_slider;
    GuiSlider* combat_maneuver_boost_speed_slider;
    GuiSlider* combat_maneuver_strafe_speed_slider;
    GuiSlider* scanning_noise_slider;
    GuiLabel* scanning_noise_label;
    GuiSlider* scanning_capability_slider;
    GuiLabel* scanning_capability_label;
    GuiLabel* position_count;
    GuiKeyValueDisplay* position[max_crew_positions];
public:
    GuiShipTweakPlayer(GuiContainer* owner);

    virtual void open(P<SpaceObject> target) override;

    virtual void onDraw(sf::RenderTarget& window) override;
};

class GuiShipTweakPlayer2 : public GuiTweakPage
{
private:
    P<PlayerSpaceship> target;

    GuiSlider* coolant_slider;
    GuiSlider* coolant_per_system_slider;
    GuiSlider* repair_slider;
    GuiSlider* repair_per_system_slider;
    GuiSlider* short_range_radar_slider;
    GuiSlider* long_range_radar_slider;
    GuiSlider* far_range_radar_slider;
    GuiSlider* engineering_presets_slider;
    GuiSlider* max_scan_probes_slider;
    GuiSlider* scan_probes_slider;
    GuiToggleButton* can_scan;
    GuiToggleButton* can_hack;
    GuiToggleButton* can_dock;
    GuiToggleButton* can_combat_maneuver;
    GuiToggleButton* can_self_destruct;
    GuiToggleButton* can_launch_probe;
    GuiToggleButton* auto_coolant_enabled;
    GuiToggleButton* auto_repair_enabled;
public:
    GuiShipTweakPlayer2(GuiContainer* owner);

    virtual void open(P<SpaceObject> target) override;

    virtual void onDraw(sf::RenderTarget& window) override;
};

class GuiShipTweakMessages : public GuiTweakPage
{
private:
    P<PlayerSpaceship> target;

    sf::Color color_message;
    string message;

    GuiSelector* color_selector;
    GuiTextEntry* message_entry;
    GuiButton* message_delete;
    GuiButton* send_message_log;
    GuiToggleButton* message_all_toggle;

public:
    GuiShipTweakMessages(GuiContainer* owner);

    virtual void open(P<SpaceObject> target);

    virtual void onDraw(sf::RenderTarget& window) override;
};

class GuiObjectTweakBase : public GuiTweakPage
{
private:
    P<SpaceObject> target;

    GuiSlider* heading_slider;
    GuiSlider* position_z_slider;
    GuiSlider* hull_slider;
    GuiLabel* hull_label;
    GuiSlider* scanning_complexity_slider;
    GuiSlider* scanning_depth_slider;
    GuiSlider* gravity_slider;
    GuiSlider* electrical_slider;
    GuiSlider* biological_slider;
public:
    GuiObjectTweakBase(GuiContainer* owner);

    virtual void open(P<SpaceObject> target) override;

    virtual void onDraw(sf::RenderTarget& window) override;
};

class GuiShipTweakDescription : public GuiTweakPage
{
private:
    P<SpaceObject> target;

    GuiTextEntry* callsign;
    GuiTextEntry* unscanned_description;
    GuiTextEntry* friend_or_foe_description;
    GuiTextEntry* simple_scan_description;
    GuiTextEntry* full_scan_description;
    GuiTextEntry* infos_label[10];
    GuiTextEntry* infos_value[10];

public:
    GuiShipTweakDescription(GuiContainer* owner);

    virtual void open(P<SpaceObject> target);

    virtual void onDraw(sf::RenderTarget& window) override;
};

#endif//TWEAK_H
