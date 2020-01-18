#ifndef PLAYER_SPACESHIP_H
#define PLAYER_SPACESHIP_H

#include "spaceship.h"
#include "commsScriptInterface.h"
#include "playerInfo.h"
#include <iostream>
#include "preferenceManager.h"
#include "spaceshipParts/scienceTask.h"

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
protected:
    static const int16_t CMD_SET_SHIELDS = 0x0009;
    static const int16_t CMD_SET_MAIN_SCREEN_SETTING = 0x000A; // Overlay is 0x0027
    static const int16_t CMD_SCAN_OBJECT = 0x000B;
    static const int16_t CMD_SCAN_DONE = 0x000C;
    static const int16_t CMD_SCAN_CANCEL = 0x000D;
    static const int16_t CMD_SET_SYSTEM_POWER_REQUEST = 0x000E;
    static const int16_t CMD_SET_SYSTEM_COOLANT_REQUEST = 0x000F;
    static const int16_t CMD_OPEN_TEXT_COMM = 0x0012; //TEXT communication
    static const int16_t CMD_CLOSE_TEXT_COMM = 0x0013;
    static const int16_t CMD_SEND_TEXT_COMM = 0x0014;
    static const int16_t CMD_SEND_TEXT_COMM_PLAYER = 0x0015;
    static const int16_t CMD_ANSWER_COMM_HAIL = 0x0016;
    static const int16_t CMD_SET_AUTO_REPAIR = 0x0017;
    static const int16_t CMD_ADD_WAYPOINT = 0x001B;
    static const int16_t CMD_REMOVE_WAYPOINT = 0x001C;
    static const int16_t CMD_MOVE_WAYPOINT = 0x001D;
    static const int16_t CMD_ACTIVATE_SELF_DESTRUCT = 0x001E;
    static const int16_t CMD_CANCEL_SELF_DESTRUCT = 0x001F;
    static const int16_t CMD_CONFIRM_SELF_DESTRUCT = 0x0020;
    static const int16_t CMD_SET_ALERT_LEVEL = 0x0024;
    static const int16_t CMD_SET_SCIENCE_LINK = 0x0025;
    static const int16_t CMD_SET_PROBE_3D_LINK = 0x0026;
    static const int16_t CMD_SET_MAIN_SCREEN_OVERLAY = 0x0028;
    static const int16_t CMD_CUSTOM_FUNCTION = 0x002A;
    static const int16_t CMD_SET_AUTO_REPAIR_SYSTEM_TARGET = 0x0030;
    static const int16_t CMD_SET_ENGINEERING_CONTROL = 0x0037;
    static const int16_t CMD_SCIENCE_QUERY_TO_BRIDGE_DB = 0x0039;
    static const int16_t CMD_HACK_TASK = 0x003A;
    static const int16_t CMD_SCAN_TASK = 0x003B;
    static const int16_t CMD_TASK_COMPLETED = 0x003C;
    static const int16_t CMD_OPEN_MID_RANGE_COMM = 0x003D; //VOICE communication
    static const int16_t CMD_SEND_QUERY = 0x003E; //VOICE communication
    static const int16_t CMD_SET_ALL_SYSTEMS_COOLANT_REQUESTS = 0x003F;
    static const int16_t CMD_ADD_ROUTE_WAYPOINT = 0x0040;
    static const int16_t CMD_REMOVE_ROUTE_WAYPOINT = 0x0041;
    static const int16_t CMD_MOVE_ROUTE_WAYPOINT = 0x0042;
    static const int16_t CMD_CLEAR_TASKS = 0x0043;

public:

    // Subsystem effectiveness base rates
    static float system_power_user_factor[];
    
    static void load(){
        PlayerSpaceship::weapons_radar_range = PreferencesManager::get("weapons_radar_range", "10000.0").toFloat();
        PlayerSpaceship::warp_terrain_cap = PreferencesManager::get("warp_terrain_cap", "2.0").toFloat();
        PlayerSpaceship::energy_warp_per_second = PreferencesManager::get("energy_warp_per_second", "1.2").toFloat();
        PlayerSpaceship::energy_shield_use_per_second = PreferencesManager::get("energy_shield_use_per_second", "1.5").toFloat();
        PlayerSpaceship::system_heatup_per_second = PreferencesManager::get("system_heatup_per_second", "0.05").toFloat();
        PlayerSpaceship::system_power_level_change_per_second = PreferencesManager::get("system_power_level_change_per_second", "0.3").toFloat();
        PlayerSpaceship::energy_transfer_per_second = PreferencesManager::get("energy_transfer_per_second", "5").toFloat();
        PlayerSpaceship::heat_transfer_per_second = PreferencesManager::get("heat_transfer_per_second", "0.1").toFloat();
        PlayerSpaceship::repair_per_second = PreferencesManager::get("repair_per_second", "0.007").toFloat();
        PlayerSpaceship::cargo_repair_per_second = PreferencesManager::get("cargo_repair_per_second", "0.1").toFloat();
        PlayerSpaceship::system_coolant_level_change_per_second = PreferencesManager::get("system_coolant_level_change_per_second", "1.2").toFloat();
        PlayerSpaceship::max_coolant_per_system = PreferencesManager::get("max_coolant_per_system ", "10").toFloat();
        PlayerSpaceship::damage_per_second_on_overheat = PreferencesManager::get("damage_per_second_on_overheat", "0.08").toFloat();
        PlayerSpaceship::shield_calibration_time = PreferencesManager::get("shield_calibration_time", "25.0").toFloat();
        PlayerSpaceship::warp_calibration_time = PreferencesManager::get("warp_calibration_time", "10.0").toFloat();
        PlayerSpaceship::comms_channel_open_time = PreferencesManager::get("comms_channel_open_time", "2.0").toFloat();
        PlayerSpaceship::scan_probe_charge_time = PreferencesManager::get("scan_probe_charge_time", "10.0").toFloat();
        PlayerSpaceship::max_scanning_delay = PreferencesManager::get("max_scanning_delay", "6.0").toFloat();
        PlayerSpaceship::warp_calibration_penalty_heat_factor = PreferencesManager::get("warp_calibration_penalty_heat_factor", "1.0").toFloat();
        PlayerSpaceship::over_fix_heat_factor = PreferencesManager::get("over_fix_heat_factor", "4").toFloat();
        PlayerSpaceship::drones_energy_factor = PreferencesManager::get("drones_energy_factor", "0.25").toFloat();
    }
    // Power consumption and generation base rates
    static float weapons_radar_range;
    static float energy_warp_per_second;
    static float energy_shield_use_per_second;
    static float system_heatup_per_second;
    static float system_power_level_change_per_second ;
    static float energy_transfer_per_second;
    static float heat_transfer_per_second;
    static float repair_per_second;
    static float cargo_repair_per_second;

    // Coolant change rate
    static float system_coolant_level_change_per_second;
    // Total coolant
    static float max_coolant_per_system;
    float max_coolant;
    // Overheat subsystem damage rate
    static float damage_per_second_on_overheat;
    // Base time it takes to perform an action
    static float shield_calibration_time;
    static float warp_calibration_time;
    static float comms_channel_open_time;
    static float scan_probe_charge_time;
    static float max_scanning_delay;
    static float warp_calibration_penalty_heat_factor;
    static float over_fix_heat_factor;
    static float drones_energy_factor;
    // Maximum number of self-destruction confirmation codes
    constexpr static int max_self_destruct_codes = 3;
    constexpr static int max_science_tasks = 10;
    constexpr static int max_routes = 7;
    constexpr static int max_waypoints_in_route = 20;
    constexpr static int max_waypoints = 99;

    static float warp_terrain_cap; 

    constexpr static int16_t CMD_PLAY_CLIENT_SOUND = 0x0001;

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

    ScienceTask scienceTasks[max_science_tasks];
    // Visual indicators of hull damage and in-progress jumps
    float hull_damage_indicator;
    float jump_indicator;
    /// MultiplayerObjectID of the targeted object for scan, or -1 when no target is selected.
    int32_t scanning_target_id;
    // Time in seconds to scan an object if scanning_complexity is 0 (none)
    float scanning_delay;
    // Time in seconds it takes to recalibrate shields
    float shield_calibration_delay;
    // Time in seconds it takes to recalibrate warp engine
    float warp_calibration_delay;
    // Ship automation features, mostly for single-person ships like fighters
    bool auto_repair_enabled;
    bool auto_coolant_enabled;
    // Whether shields are up (true) or down
    bool shields_active;
    // Password to join a ship. Default is empty.
    string control_code;
    int warp_indicator;

private:
    // soundManager index of the shield object
    int shield_sound;
    // Comms variables
    ECommsState comms_state;
    ECommsState mid_range_comms_state;
    float comms_open_delay;
    string mid_range_comms_target_name;
    string comms_target_name;
    string comms_incomming_message;
    P<SpaceObject> mid_range_comms_target; // Server only
    P<SpaceObject> comms_target; // Server only
    std::vector<int> comms_reply_id;
    std::vector<string> comms_reply_message;
    CommsScriptInterface comms_script_interface;  //Server only
    bool intern_in_query;
    bool excalibur_in_query;

public:
    ESystem auto_repairing_system;
    std::vector<CustomShipFunction> custom_functions;

    sf::Vector2f waypoints[max_waypoints];
    sf::Vector2f routes[max_routes][max_waypoints_in_route];
    
    // Scan probe capacity
    int max_scan_probes;
    int scan_probe_stock;
    float scan_probe_recharge;

    // Main screen content
    EMainScreenSetting main_screen_setting;
    // Content overlaid on the main screen, such as comms
    EMainScreenOverlay main_screen_overlay;
    bool engineering_control_from_bridge;
    bool activate_self_destruct;
    uint32_t self_destruct_code[max_self_destruct_codes];
    bool self_destruct_code_confirmed[max_self_destruct_codes];
    ECrewPosition self_destruct_code_entry_position[max_self_destruct_codes];
    ECrewPosition self_destruct_code_show_position[max_self_destruct_codes];
    float self_destruct_countdown;

    EAlertLevel alert_level;

    int32_t linked_science_probe_id;
    int32_t linked_probe_3D_id;
    string science_query_to_bridge_db;

    PlayerSpaceship();

    // Comms functions
    bool isMidRangeCommsInactive() { return mid_range_comms_state == CS_Inactive; }
    bool isMidRangeCommsOpening() { return mid_range_comms_state == CS_OpeningChannel; }
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
    P<SpaceObject> getScanTarget();
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
    void closeComms();

    void acceptedMidRangeCall(string response);
    void closeMidRangeComms(string response);
    void setEnergyLevel(float amount) { energy_level = std::max(0.0f, std::min(max_energy_level, amount)); }
    void setEnergyLevelMax(float amount) { max_energy_level = std::max(0.0f, amount); energy_level = std::min(energy_level, max_energy_level); }
    float getEnergyLevel() { return energy_level; }
    float getEnergyLevelMax() { return max_energy_level; }
    
    void setScanProbeCount(int amount) { scan_probe_stock = std::max(0, std::min(amount, max_scan_probes)); }
    int getScanProbeCount() { return scan_probe_stock; }
    void setMaxScanProbeCount(int amount) { max_scan_probes = std::max(0, amount); scan_probe_stock = std::min(scan_probe_stock, max_scan_probes); }
    int getMaxScanProbeCount() { return max_scan_probes; }

    void addCustomButton(ECrewPosition position, string name, string caption, ScriptSimpleCallback callback);
    void addCustomInfo(ECrewPosition position, string name, string caption);
    void addCustomMessage(ECrewPosition position, string name, string caption);
    void addCustomMessageWithCallback(ECrewPosition position, string name, string caption, ScriptSimpleCallback callback);
    void removeCustom(string name);

    // Client command functions
    virtual void handleClientCommand(int32_t client_id, int16_t command, sf::Packet& packet) override;
    void commandSetScienceLink(int32_t id);
    void commandSetProbe3DLink(int32_t id);
    void commandSetShields(bool enabled);
    void commandMainScreenSetting(EMainScreenSetting mainScreen);
    void commandMainScreenOverlay(EMainScreenOverlay mainScreen);
    void commandScan(P<SpaceObject> object);
    void commandSetSystemPowerRequest(ESystem system, float power_level);
    void commandSetSystemCoolantRequest(ESystem system, float coolant_level);
    void commandSetAllSystemsCoolantRequests(float coolant_level[SYS_COUNT]);
    void commandOpenVoiceComm(P<SpaceObject> obj);
    void commandOpenTextComm(P<SpaceObject> obj);
    void commandCloseVoiceComm();
    void commandCloseTextComm();
    void commandAnswerCommHail(bool awnser);
    void commandSendComm(uint8_t index);
    void commandSendCommPlayer(string message);
    void commandSetAutoRepair(bool enabled);
    void commandSetShieldFrequency(int32_t frequency);
    void commandAddWaypoint(sf::Vector2f position);
    void commandRemoveWaypoint(int32_t index);
    void commandMoveWaypoint(int32_t index, sf::Vector2f position);
    void commandAddRouteWaypoint(int route, sf::Vector2f position);
    void commandRemoveRouteWaypoint(int route, int index);
    void commandMoveRouteWaypoint(int route, int index, sf::Vector2f position);
    void commandActivateSelfDestruct();
    void commandCancelSelfDestruct();
    void commandConfirmDestructCode(int8_t index, uint32_t code);
    void commandLaunchProbe(sf::Vector2f target_position);
    void commandScanDone();
    void commandScanCancel();
    void commandSetAlertLevel(EAlertLevel level);
    void commandCustomFunction(string name);
    void commandSetAutoRepairSystemTarget(ESystem system);
    void commandSetEngineeringControlToBridge();
    void commandSetEngineeringControlToECR();
    void commandSendScienceQueryToBridgeDB(string entryName);
    void commandAddHackTask(P<SpaceShip> target, ESystem target_system);
    void commandAddScanTask(P<SpaceObject> object);
    void commandClearAllTasks();
    void commandCompleteScienceTask(int taskIndex, bool success);
    void commandSendQuery(string message, string station);
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
    int getRepairCrewCount();
    void setRepairCrewCount(int amount);
    EAlertLevel getAlertLevel() { return alert_level; }

    // Ship update functions
    virtual void update(float delta) override;
    virtual bool useEnergy(float amount) override;
    virtual void addHeat(ESystem system, float amount) override;
    
    // Call on the server to play a sound on the main screen.
    void playSoundOnMainScreen(string sound_name);

    float getNetSystemEnergyUsage();

    // Ship's crew functions
    void transferPlayersToShip(P<PlayerSpaceship> other_ship);
    void transferPlayersAtPositionToShip(ECrewPosition position, P<PlayerSpaceship> other_ship);
    bool hasPlayerAtPosition(ECrewPosition position);

    // Ship shields functions
    virtual bool getShieldsActive() override { return shields_active; }
    void setShieldsActive(bool active) { shields_active = active; }

    // Ship control code/password setter
    void setControlCode(string code) { control_code = code; }

    // Script export function
    virtual string getExportLine();
};
REGISTER_MULTIPLAYER_ENUM(ECommsState);
template<> int convert<EAlertLevel>::returnType(lua_State* L, EAlertLevel l);
template<> void convert<EAlertLevel>::param(lua_State* L, int& idx, EAlertLevel& al);
REGISTER_MULTIPLAYER_ENUM(EAlertLevel);

static inline sf::Packet& operator << (sf::Packet& packet, const PlayerSpaceship::CustomShipFunction& csf) { return packet << uint8_t(csf.type) << uint8_t(csf.crew_position) << csf.name << csf.caption; } \
static inline sf::Packet& operator >> (sf::Packet& packet, PlayerSpaceship::CustomShipFunction& csf) { int8_t tmp; packet >> tmp; csf.type = PlayerSpaceship::CustomShipFunction::Type(tmp); packet >> tmp; csf.crew_position = ECrewPosition(tmp); packet >> csf.name >> csf.caption; return packet; }

string alertLevelToString(EAlertLevel level);

static const sf::Vector2f empty_waypoint = sf::Vector2f(FLT_MAX, FLT_MAX);
static const sf::Color routeColors[PlayerSpaceship::max_routes] = {
    sf::Color::Red, sf::Color::White, sf::Color::Green, 
    sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta, 
    sf::Color::Cyan};

#ifdef _MSC_VER
#include "playerSpaceship.hpp"
#endif /* _MSC_VER */

#endif//PLAYER_SPACESHIP_H
