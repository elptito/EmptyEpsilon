#ifndef GAME_GLOBAL_INFO_H
#define GAME_GLOBAL_INFO_H

#include "spaceObjects/playerSpaceship.h"
#include "script.h"
//#include "GMScriptCallback.h"
#include "GMMessage.h"
#include "gameStateLogger.h"

class GameStateLogger;
class GameGlobalInfo;
extern P<GameGlobalInfo> gameGlobalInfo;

class NebulaInfo
{
public:
    sf::Vector3f vector;
    string textureName;
};

/*
class TerrainInfo
{
public:
    bool defined;
    string textureName;
    sf::Vector2f coordinates;
    float scale; 
};
*/

class MapLayer
{
public:
    bool defined;
    string title;
    string textureName;
    sf::Vector2f coordinates;
    float scale; 
    sf::Image image; // server only (no registerMemberReplication) pixels
};

enum EPlayerWarpJumpDrive
{
    PWJ_ShipDefault = 0,
    PWJ_WarpDrive,
    PWJ_JumpDrive,
    PWJ_WarpAndJumpDrive,
    PWJ_None,
    PWJ_MAX,
};
enum EScanningComplexity
{
    SC_None = 0,
    SC_Simple,
    SC_Normal,
    SC_Advanced,
};
enum EHackingGames
{
    HG_Mine,
    HG_Lights,
    HG_All
};

class GameGlobalInfo : public MultiplayerObject, public Updatable
{
    P<GameStateLogger> state_logger;
public:
    /*!
     * \brief Maximum number of player ships.
     */
    static const int max_player_ships = 32;
    /*!
     * \brief Maximum number of visual background nebulas.
     */
    static const int max_nebulas = 32;
    /*!
     * \brief Maximum number of warp layers
     */
    static const int max_map_layers = 10;
    /*!
     * \size of a sector.
     */
    static const int sector_size = 20000;
private:
    int victory_faction;
    int32_t playerShipId[max_player_ships];
    int callsign_counter;
    /*!
     * \brief List of known scripts
     */
    PVector<Script> script_list;
    sf::Image terrainImage; // server only (no registerMemberReplication) terrain image data
public:
    string global_message;
    float global_message_timeout;

    string banner_string;

    std::vector<float> reputation_points;
    EPlayerWarpJumpDrive player_warp_jump_drive_setting;
    EScanningComplexity scanning_complexity;
    //Hacking difficulty ranges from 0 to 3
    int hacking_difficulty;
    EHackingGames hacking_games;
    bool use_beam_shield_frequencies;
    bool use_system_damage;
    bool use_warp_terrain;
    bool use_complex_radar_signatures;
    bool use_advanced_sector_system;
    bool allow_main_screen_tactical_radar;
    bool allow_main_screen_long_range_radar;
    bool allow_main_screen_far_range_radar;
    bool allow_main_screen_target_analysis;
    bool use_nano_repair_crew;
    string gm_control_code;
    bool color_by_faction;
    bool all_can_be_targeted;
    bool logs_by_station;
    float elapsed_time;
    string scenario;
    string variation = "None";

    //List of script functions that can be called from the GM interface
    std::list<GMMessage> gm_messages;
    std::vector<ScriptSimpleCallback> gm_callback_functions;
    //List of names of gm_callback_functions scripts (replicated to clients)
    std::vector<string> gm_callback_names;
    //When active, all comms request goto the GM as chat, and normal scripted converstations are disabled. This does not disallow player<->player ship comms.
    bool intercept_all_comms_to_gm; 

    //Callback called when a new player ship is created on the ship selection screen.
    ScriptSimpleCallback on_new_player_ship;
    bool allow_new_player_ships = true;
    std::function<void(sf::Vector2f)> on_gm_click;

    MapLayer layer[max_map_layers];

    GameGlobalInfo();
    virtual ~GameGlobalInfo();

    P<PlayerSpaceship> getPlayerShip(int index);
    void setPlayerShip(int index, P<PlayerSpaceship> ship);

    int findPlayerShip(P<PlayerSpaceship> ship);
    int insertPlayerShip(P<PlayerSpaceship> ship);
    /*!
     * \brief Set a faction to victorious.
     * \param string Name of the faction that won.
     */
    void setVictory(string faction_name) { victory_faction = FactionInfo::findFactionId(faction_name); }
    /*!
     * \brief Get ID of faction that won.
     * \param int
     */
    int getVictoryFactionId() { return victory_faction; }

    void addScript(P<Script> script);
    //Reset the global game state (called when we want to load a new scenario, and clear out this one)
    void reset();
    void startScenario(string filename);

    virtual void update(float delta);
    virtual void destroy();
    
    // Local sector name
    std::vector<std::pair<string, string> > locals_name;

    string getNextShipCallsign();
    void setMapLayer(int layerId, string textureName, sf::Vector2f coordinates, float scale, string title);
    sf::Color getLayerPixel(int layerId, sf::Vector2f coordinates);
};

string playerWarpJumpDriveToString(EPlayerWarpJumpDrive player_warp_jump_drive);
string getSectorName(sf::Vector2f position, int scale_magnitude = 0, bool show_all = false);

REGISTER_MULTIPLAYER_ENUM(EScanningComplexity);
REGISTER_MULTIPLAYER_ENUM(EHackingGames);

template<> int convert<EScanningComplexity>::returnType(lua_State* L, EScanningComplexity complexity);
template<> int convert<EHackingGames>::returnType(lua_State* L, EHackingGames games);

#endif//GAME_GLOBAL_INFO_H
