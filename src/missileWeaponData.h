#ifndef MISSILE_WEAPON_DATA_H
#define MISSILE_WEAPON_DATA_H

#include "engine.h"
#include "EDamageType.h"


enum EMissileWeapons
{
    MW_None = -1,
    MW_Homing = 0,
    MW_HVLI,
    MW_EMP,
    MW_Nuke,
    MW_Mine,
    MW_Count
};

enum EMissileSizes
{
    MS_Small = 0,
    MS_Medium = 1,
    MS_Large = 2,
};
REGISTER_MULTIPLAYER_ENUM(EMissileSizes);
string getMissileSizeString(EMissileSizes size);

/* Define script conversion function for the EMissileWeapons enum. */
template<> void convert<EMissileWeapons>::param(lua_State* L, int& idx, EMissileWeapons& es);
template<> int convert<EMissileWeapons>::returnType(lua_State* L, EMissileWeapons es);
/* Define script conversion function for the EMissileSizes enum. */
template <> void convert<EMissileSizes>::param(lua_State* L, int& idx, EMissileSizes& es);
template<> int convert<EMissileSizes>::returnType(lua_State* L, EMissileSizes es);

/* data container for missile weapon data, contains information about different missile weapon types. */
class MissileWeaponData
{
public:
    MissileWeaponData(float speed, float turnrate, float lifetime, sf::Color color, float homing_range, string fire_sound, EMissileWeapons base_type, EDamageType dt);

    EMissileWeapons basetype; //for custom

    float speed; //meter/sec
    float turnrate; //deg/sec

    float lifetime; //sec
    sf::Color color;
    float homing_range;

    string fire_sound;

    float damage_multiplier;
    int fire_count;
    int line_count;
    EDamageType damage_type;

    static const MissileWeaponData& getDataFor(const EMissileWeapons& type);
    static const MissileWeaponData& getDataFor(const string& type);
};

class CustomMissileWeaponRegistry
{
private:
    using MissileWeaponMap = std::map<string,MissileWeaponData>;
    using MissileWeaponTypeMap = std::map<string, uint32_t>;

public:

    static auto getCustomMissileWeapons() -> MissileWeaponMap&
    {
        static MissileWeaponMap custom_missile_data{};
        return custom_missile_data;
    }

    static auto getCustomMissileWeaponTypes() -> MissileWeaponTypeMap&
    {
        static MissileWeaponTypeMap custom_missile_data_types{};
        return custom_missile_data_types;
    }

    static auto getMissileWeapon (const std::string &iName) -> MissileWeaponData& ;
    static auto getMissileWeaponType (const std::string &iName) -> uint32_t& ;
    static void createMissileWeapon(const EMissileWeapons &iBaseType, const std::string &iNewName, const float &iDamageMultiplier, const float &iSpeed, const EDamageType &iDT);

};

#ifdef _MSC_VER
// MFC: GCC does proper external template instantiation, VC++ doesn't.
#include "missileWeaponData.hpp"
#endif

#endif//MISSILE_WEAPON_DATA_H
