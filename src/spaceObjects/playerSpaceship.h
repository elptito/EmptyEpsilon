#ifndef PLAYER_SPACESHIP_H
#define PLAYER_SPACESHIP_H

#include "spaceship.h"
#include "commsScriptInterface.h"
#include "playerInfo.h"
#include "spaceshipParts/dock.h"
#include <iostream>

enum ECommsState
{
    CS_Inactive,          // No active comms
    CS_OpeningChannel,    // Opening a comms channel
    CS_BeingHailed,       // Receiving a hail from an object
    CS_BeingHailedByGM,   //                   ... the GM
    CS_ChannelOpen,       // Comms open to an object
    CS_ChannelOpenPlayer, //           ... another player
    CS_ChannelOpenGM,     //           ... the GM
    CS_ChannelFailed,     // Comms failed to connect
    CS_ChannelBroken,     // Comms broken by other side
    CS_ChannelClosed      // Comms manually closed
};

enum EAlertLevel
{
    AL_Normal,      // No alert state
    AL_YellowAlert, // Yellow
    AL_RedAlert,    // Red
    AL_MAX          // ?
};

class PlayerSpaceship : public SpaceShip
{
public:
    // Power consumption and generation base rates
    constexpr static float energy_shield_use_per_second = 1.5f;
    constexpr static float energy_warp_per_second = 1.0f;
    constexpr static float system_heatup_per_second = 0.05f;
    constexpr static float system_power_level_change_per_second = 0.3;
    constexpr static float energy_transfer_per_second = 5;
    constexpr static float heat_transfer_per_second = 0.1;
    constexpr static float cargo_repair_per_second = 0.1;

    // Coolant change rate
    constexpr static float system_coolant_level_change_per_second = 1.2;
    // Repair change rate
    constexpr static float system_repair_level_change_per_second = 1.2;
    // Repair effectiveness
    constexpr static float system_repair_effect_per_second = 0.005;
    // Total coolant
    float max_coolant_per_system = 10.0f;
    float max_coolant;
    // Total repair
    float max_repair_per_system = 3.0f;
    float max_repair;
    // Overheat subsystem damage rate
    constexpr static float damage_per_second_on_overheat = 0.08f;
    // Base time it takes to perform an action
    constexpr static float shield_calibration_time = 25.0f;
    constexpr static float comms_channel_open_time = 2.0;
    constexpr static float scan_probe_charge_time = 10.0f;
    constexpr static float max_scanning_delay = 6.0;
    // Maximum number of self-destruction confirmation codes
    constexpr static int max_self_destruct_codes = 3;
    // Subsystem effectiveness base rates
    static float system_power_user_factor[];
    // Waypoints and routes
    constexpr static int max_routes = 7;
    constexpr static int max_waypoints_in_route = 20;
    constexpr static int max_waypoints = 99;
    // Engineering presets
    constexpr static int max_engineer_presets_number = 9;
    // Scanning noise and capability
    float scanning_noise = 0.0f;

    constexpr static int16_t CMD_PLAY_CLIENT_SOUND = 0x0001;

    // Content of a line in the ship's log
    class ShipLogEntry
    {
    public:
        string prefix;
        string text;
        sf::Color color;
        ECrewPosition position;

        ShipLogEntry() {}
        ShipLogEntry(string prefix, string text, sf::Color color, ECrewPosition position)
        : prefix(prefix), text(text), color(color), position(position) {}

        bool operator!=(const ShipLogEntry& e) { return prefix != e.prefix || text != e.text || color != e.color || position != e.position; }
    };

    class CustomShipFunction
    {
    public:
        enum class Type
        {
            Info,
            Button,
            Message
        };
        Type type;
        string name;
        string caption;
        ECrewPosition crew_position;
        ScriptSimpleCallback callback;

        bool operator!=(const CustomShipFunction& csf) { return type != csf.type || name != csf.name || caption != csf.caption || crew_position != csf.crew_position; }
    };

    // Visual indicators of hull damage and in-progress jumps
    float hull_damage_indicator;
    float jump_indicator;
    // Time in seconds it takes to recalibrate shields
    float shield_calibration_delay;
    // Ship automation features, mostly for single-person ships like fighters
    bool auto_repair_enabled;
    bool auto_coolant_enabled;
    // Whether shields are up (true) or down
    bool shields_active;
    // Whether fire are active (true) or not
    bool lock_fire;
    // Password to join a ship. Default is empty.
    string control_code;

private:
    bool on_new_player_ship_called=false;
    // Comms variables
    ECommsState comms_state;
    float comms_open_delay;
    string comms_target_name;
    string comms_incomming_message;
    P<SpaceObject> comms_target; // Server only
    std::vector<int> comms_reply_id;
    std::vector<string> comms_reply_message;
    CommsScriptInterface comms_script_interface; // Server only
    std::vector<string> scan_labels;
    // Ship's log container
    std::vector<ShipLogEntry> ships_log;
    std::vector<ShipLogEntry> sub_log;

    float far_range_radar_range = 50000.0f;
    float long_range_radar_range = 30000.0f;
    float short_range_radar_range = 5000.0f;
public:
    std::vector<CustomShipFunction> custom_functions;

    sf::Vector2f waypoints[max_routes][max_waypoints_in_route];

    // Presets for engeneering screen
    int active_engineer_presets_number;
    float power_presets[SYS_COUNT][max_engineer_presets_number];
    float coolant_presets[SYS_COUNT][max_engineer_presets_number];

    // Ship functionality
    // Capable of scanning a target
    bool can_scan = true;
    // Target of a scan. Server-only value
    P<SpaceObject> scanning_target;
    // Time in seconds to scan an object if scanning_complexity is 0 (none)
    float scanning_delay = 0.0;
    // Number of sliders during a scan
    int scanning_complexity = 0;
    // Number of times an object must be scanned to achieve a fully scanned
    // state
    int scanning_depth = 0;

    // Capable of hacking a target
    bool can_hack = true;
    // Capable of docking with a target
    bool can_dock = true;
    // Capable of combat maneuvers
    bool can_combat_maneuver = true;

    // Capable of self-destruction
    bool can_self_destruct = true;
    bool activate_self_destruct = false;
    uint32_t self_destruct_code[max_self_destruct_codes];
    bool self_destruct_code_confirmed[max_self_destruct_codes];
    ECrewPosition self_destruct_code_entry_position[max_self_destruct_codes];
    ECrewPosition self_destruct_code_show_position[max_self_destruct_codes];
    float self_destruct_countdown = 0.0;
    float self_destruct_damage = 150.0;
    float self_destruct_size = 1500.0;

    // Capable of probe launches
    bool can_launch_probe = true;
    int max_scan_probes = 8;
    int scan_probe_stock;
    float scan_probe_recharge = 0.0;
    ScriptSimpleCallback on_probe_launch;

    // Main screen content
    EMainScreenSetting main_screen_setting;
    // Content overlaid on the main screen, such as comms
    EMainScreenOverlay main_screen_overlay;
    
    string texture_front;
    string texture_back;
    string texture_left;
    string texture_right;
    string texture_top;
    string texture_bottom;
    float texture_r;
    float texture_g;
    float texture_b;
    float texture_a;

    EAlertLevel alert_level;

    //int32_t linked_science_probe_id;
    int32_t linked_science_probe_id = -1;
    int32_t linked_analysis_object_id = -1;

    PlayerSpaceship();
    virtual ~PlayerSpaceship();

    // Comms functions
    bool isCommsInactive() { return comms_state == CS_Inactive; }
    bool isCommsOpening() { return comms_state == CS_OpeningChannel; }
    bool isCommsBeingHailed() { return comms_state == CS_BeingHailed || comms_state == CS_BeingHailedByGM; }
    bool isCommsBeingHailedByGM() { return comms_state == CS_BeingHailedByGM; }
    bool isCommsFailed() { return comms_state == CS_ChannelFailed; }
    bool isCommsBroken() { return comms_state == CS_ChannelBroken; }
    bool isCommsClosed() { return comms_state == CS_ChannelClosed; }
    bool isCommsChatOpen() { return comms_state == CS_ChannelOpenPlayer || comms_state == CS_ChannelOpenGM; }
    bool isCommsChatOpenToGM() { return comms_state == CS_ChannelOpenGM; }
    bool isCommsChatOpenToPlayer() { return comms_state == CS_ChannelOpenPlayer; }
    bool isCommsScriptOpen() { return comms_state == CS_ChannelOpen; }
    ECommsState getCommsState() { return comms_state; }
    float getCommsOpeningDelay() { return comms_open_delay; }
    const std::vector<string>& getCommsReplyOptions() const { return comms_reply_message; }
    P<SpaceObject> getCommsTarget() { return comms_target; }
    const string& getCommsTargetName() { return comms_target_name; }
    const string& getCommsIncommingMessage() { return comms_incomming_message; }
    bool hailCommsByGM(string target_name);
    bool hailByObject(P<SpaceObject> object, string opening_message);
    void setCommsMessage(string message);
    void addCommsIncommingMessage(string message);
    void addCommsOutgoingMessage(string message);
    void addCommsReply(int32_t id, string message);
    void switchCommsToGM();
    void closeComms();
    void setScanLabels(std::vector<string> labels);
    const std::vector<string> getScanLabels();

    void setEnergyLevel(float amount) { energy_level = std::max(0.0f, std::min(max_energy_level, amount)); }
    void setEnergyLevelMax(float amount) { max_energy_level = std::max(0.0f, amount); energy_level = std::min(energy_level, max_energy_level); }
    float getEnergyLevel() { return energy_level; }
    float getEnergyLevelMax() { return max_energy_level; }

    void setActivePresetNumber(int amount) { if (amount >= 0 and amount <= max_engineer_presets_number) active_engineer_presets_number = amount; }
    int getActivePresetNumber() { return active_engineer_presets_number; }

    void setCanScan(bool enabled) { can_scan = enabled; }
    bool getCanScan() { return can_scan; }
    void setCanHack(bool enabled) { can_hack = enabled; }
    bool getCanHack() { return can_hack; }
    void setCanDock(bool enabled) { can_dock = enabled; }
    bool getCanDock() { return can_dock; }
    void setCanCombatManeuver(bool enabled) { can_combat_maneuver = enabled; }
    bool getCanCombatManeuver() { return can_combat_maneuver; }
    void setCanSelfDestruct(bool enabled) { can_self_destruct = enabled; }
    bool getCanSelfDestruct() { return can_self_destruct && self_destruct_size > 0 && self_destruct_damage > 0; }
    void setCanLaunchProbe(bool enabled) { can_launch_probe = enabled; }
    bool getCanLaunchProbe() { return can_launch_probe; }

    void setSelfDestructDamage(float amount) { self_destruct_damage = std::max(0.0f, amount); }
    float getSelfDestructDamage() { return self_destruct_damage; }
    void setSelfDestructSize(float size) { self_destruct_size = std::max(0.0f, size); }
    float getSelfDestructSize() { return self_destruct_size; }

    void setScanProbeCount(int amount) { scan_probe_stock = std::max(0, std::min(amount, max_scan_probes)); }
    int getScanProbeCount() { return scan_probe_stock; }
    void setMaxScanProbeCount(int amount) { max_scan_probes = std::max(0, amount); scan_probe_stock = std::min(scan_probe_stock, max_scan_probes); }
    int getMaxScanProbeCount() { return max_scan_probes; }

    void onProbeLaunch(ScriptSimpleCallback callback);

    void addCustomButton(ECrewPosition position, string name, string caption, ScriptSimpleCallback callback);
    void addCustomInfo(ECrewPosition position, string name, string caption);
    void addCustomMessage(ECrewPosition position, string name, string caption);
    void addCustomMessageWithCallback(ECrewPosition position, string name, string caption, ScriptSimpleCallback callback);
    void removeCustom(string name);

    ESystem getBeamSystemTarget(){ return beam_system_target; }
    string getBeamSystemTargetName(){ return getSystemName(beam_system_target); }
    // Client command functions
    virtual void onReceiveClientCommand(int32_t client_id, sf::Packet& packet) override;
    void commandTargetRotation(float target);
    void commandTurnSpeed(float turnSpeed);
    void commandImpulse(float target);
    void commandWarp(int8_t target);
    void commandSetWarpFrequency(int32_t frequency);
    void commandJump(float distance);
    void commandSetTarget(P<SpaceObject> target, int8_t station = 0);
    void commandSetScienceLink(int32_t id);
    void commandSetAnalysisLink(int32_t id);
    void commandLoadTube(int8_t tubeNumber, EMissileWeapons missileType);
    void commandUnloadTube(int8_t tubeNumber);
    void commandFireTube(int8_t tubeNumber, float missile_target_angle);
    void commandFireTubeAtTarget(int8_t tubeNumber, P<SpaceObject> target);
    void commandSetShields(bool enabled);
    void commandLockFire(bool enabled);
    void commandMainScreenSetting(EMainScreenSetting mainScreen);
    void commandMainScreenOverlay(EMainScreenOverlay mainScreen);
    void commandScan(P<SpaceObject> object);
    void commandSetSystemPowerRequest(ESystem system, float power_level);
    void commandSetSystemPowerPreset(ESystem system, int preset, float power_level);
    void commandSetSystemCoolantRequest(ESystem system, float coolant_level);
    void commandSetSystemCoolantPreset(ESystem system, int preset, float coolant_level);
    void commandSetSystemInstability(ESystem system, int slider, float instability);
    void commandSetSystemRepairRequest(ESystem system, float repair_level);
    void commandDock(P<SpaceObject> station);
    void commandUndock();
    void commandAbortDock();
    void commandOpenTextComm(P<SpaceObject> obj);
    void commandCloseTextComm();
    void commandAnswerCommHail(bool awnser);
    void commandSendComm(uint8_t index);
    void commandSendCommPlayer(string message);
    void commandSetAutoRepair(bool enabled);
    void commandSetBeamFrequency(int32_t frequency);
    void commandSetBeamSystemTarget(ESystem system);
    void commandSetShieldFrequency(int32_t frequency);
    void commandAddWaypoint(sf::Vector2f position, int route = 0);
    void commandRemoveWaypoint(int index, int route = 0);
    void commandMoveWaypoint(int index, sf::Vector2f position, int route = 0);
    void commandActivateSelfDestruct();
    void commandCancelSelfDestruct();
    void commandConfirmDestructCode(int8_t index, uint32_t code);
    void commandCombatManeuverBoost(float amount);
    void commandCombatManeuverStrafe(float strafe);
    void commandLaunchProbe(sf::Vector2f target_position);
    void commandLaunchCargo(int index);
    void commandMoveCargo(int index);
    void commandCancelMoveCargo(int index);
    void commandSetDockMoveTarget(int srcIdx, int destIdx);
    void commandSetDockEnergyRequest(int index, float value);
    void commandScanDone();
    void commandScanCancel();
    void commandSetAlertLevel(EAlertLevel level);
    void commandHackingFinished(P<SpaceObject> target, string target_system, float value);
    void commandCustomFunction(string name);
    void commandSetTractorBeamDirection(float direction);
    void commandSetTractorBeamArc(float arc);
    void commandSetTractorBeamRange(float range);
    void commandSetTractorBeamMode(ETractorBeamMode range);
    
    void setTexture(string t_front, string t_back, string t_left, string t_right, string t_top, string t_bottom){
        texture_front = t_front;
        texture_back = t_back;
        texture_left = t_left;
        texture_right = t_right;
        texture_top = t_top;
        texture_bottom = t_bottom;
    };
    void setTextureColor(float r, float g, float b, float a){
        texture_r = r;
        texture_g = g;
        texture_b = b;
        texture_a = a;
    };
    
    virtual void onReceiveServerCommand(sf::Packet& packet) override;

    // Template function
    virtual void applyTemplateValues() override;

    // Ship status functions
    virtual void executeJump(float distance) override;
    virtual void takeHullDamage(float damage_amount, DamageInfo& info) override;
    void setSystemCoolantRequest(ESystem system, float request);
    void setMaxCoolant(float coolant);
    float getMaxCoolant() { return max_coolant; }
    void setAutoCoolant(bool active) { auto_coolant_enabled = active; }
    void setMaxCoolantPerSystem(float coolant){ max_coolant_per_system = coolant; }
    float getMaxCoolantPerSystem() { return max_coolant_per_system; }
    void setSystemRepairRequest(ESystem system, float request);
    void setMaxRepair(float repair);
    float getMaxRepair() { return max_repair; }
    void setMaxRepairPerSystem(int amount){ max_repair_per_system = (float) amount; }
    float getMaxRepairPerSystem() { return max_repair_per_system; }
    void setAutoRepair(bool active) { auto_repair_enabled = active; }
    int getRepairCrewCount();
    void setRepairCrewCount(int amount);
    EAlertLevel getAlertLevel() { return alert_level; }

    // Ship noise scanning functions
    float getScanningNoise() { return scanning_noise; }
    void setScanningNoise(float value){ scanning_noise = value; }
    // Ship scanning capability
    EScannedState getScanningCapability() { return scanning_capability; }
    void setScanningCapability(EScannedState value){ scanning_capability = value; }

    // Ship update functions
    virtual void update(float delta) override;
    virtual bool useEnergy(float amount) override;
    virtual void addHeat(ESystem system, float amount) override;
    virtual void updateInstability(ESystem system);

    // Call on the server to play a sound on the main screen.
    void playSoundOnMainScreen(string sound_name);

    float getNetSystemEnergyUsage();

    // Ship's log functions
    void addToShipLog(string message, sf::Color color, ECrewPosition position = relayOfficer);
    void addToShipLogBy(string message, P<SpaceObject> target);
    std::vector<ShipLogEntry>& getShipsLog(ECrewPosition position);

    // Ship's crew functions
    void transferPlayersToShip(P<PlayerSpaceship> other_ship);
    void transferPlayersAtPositionToShip(ECrewPosition position, P<PlayerSpaceship> other_ship);
    bool hasPlayerAtPosition(ECrewPosition position);

    // Ship shields functions
    virtual bool getShieldsActive() override { return shields_active; }
    void setShieldsActive(bool active) { shields_active = active; }

    // Waypoint functions
    int getWaypointCount(int route = 0);
    sf::Vector2f getWaypoint(int index, int route = 0) { if (route > -1 && route <= max_routes && index > 0 && index <= max_waypoints_in_route) return waypoints[route][index - 1]; return sf::Vector2f(0, 0); }

    // Ship control code/password setter
    void setControlCode(string code) { control_code = code.upper(); }

    // Radar function
    virtual void drawOnGMRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, float rotation, bool long_range) override;

    // Radar range
    float getFarRangeRadarRange();
    float getLongRangeRadarRange();
    float getShortRangeRadarRange();
    void setFarRangeRadarRange(float range);
    void setLongRangeRadarRange(float range);
    void setShortRangeRadarRange(float range);

    // Script export function
    virtual string getExportLine() override;
};
REGISTER_MULTIPLAYER_ENUM(ECommsState);
template<> int convert<EAlertLevel>::returnType(lua_State* L, EAlertLevel l);
template<> void convert<EAlertLevel>::param(lua_State* L, int& idx, EAlertLevel& al);
REGISTER_MULTIPLAYER_ENUM(EAlertLevel);

static inline sf::Packet& operator << (sf::Packet& packet, const PlayerSpaceship::CustomShipFunction& csf) { return packet << uint8_t(csf.type) << uint8_t(csf.crew_position) << csf.name << csf.caption; } \
static inline sf::Packet& operator >> (sf::Packet& packet, PlayerSpaceship::CustomShipFunction& csf) { int8_t tmp; packet >> tmp; csf.type = PlayerSpaceship::CustomShipFunction::Type(tmp); packet >> tmp; csf.crew_position = ECrewPosition(tmp); packet >> csf.name >> csf.caption; return packet; }

string alertLevelToString(EAlertLevel level);
string alertLevelToLocaleString(EAlertLevel level);

static const sf::Vector2f empty_waypoint = sf::Vector2f(FLT_MAX, FLT_MAX);
static const sf::Color routeColors[PlayerSpaceship::max_routes] = {
    sf::Color::White, sf::Color::Magenta, sf::Color::Green, 
    sf::Color::Red, sf::Color::Yellow, sf::Color::Blue, 
    sf::Color::Cyan};

#ifdef _MSC_VER
#include "playerSpaceship.hpp"
#endif /* _MSC_VER */
#endif//PLAYER_SPACESHIP_H
