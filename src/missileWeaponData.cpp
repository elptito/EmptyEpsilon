#include "missileWeaponData.h"
namespace {

    MissileWeaponData missile_data[MW_Count] =
    {
        //                speed, turnrate, lifetime, color, homing_range
        MissileWeaponData(200.0f, 10.f, 54.0f, sf::Color(255, 200, 0), 1200.0, "sfx/rlaunch.wav", MW_Homing, DT_Kinetic),/*MW_Homing*/
        MissileWeaponData(500.0f,  0.f, 27.0f, sf::Color(200, 200, 200), 0.0, "sfx/hvli_fire.wav", MW_HVLI, DT_Kinetic),/*MW_HVLI*/
        MissileWeaponData(200.0f, 10.f, 54.0f, sf::Color(100, 32, 255), 500.0, "sfx/rlaunch.wav", MW_EMP, DT_EMP),/*MW_EMP*/
        MissileWeaponData(200.0f, 10.f, 54.0f, sf::Color(255, 100, 32), 500.0, "sfx/rlaunch.wav", MW_Nuke, DT_Kinetic),/*MW_Nuke*/
        MissileWeaponData(100.0f,  0.f, 30.0f, sf::Color(255, 255, 255), 0.0, "missile_launch.wav", MW_Mine, DT_Kinetic),/*MW_Mine, lifetime is used at time which the mine is ejecting from the ship*/
    };
}

MissileWeaponData::MissileWeaponData(float speed, float turnrate, float lifetime, sf::Color color, float homing_range, string fire_sound, EMissileWeapons base_type, EDamageType dt)
: speed(speed), turnrate(turnrate), lifetime(lifetime), color(color), homing_range(homing_range), fire_sound(fire_sound), damage_multiplier(1), basetype(base_type), damage_type(dt)
{
}

const MissileWeaponData& MissileWeaponData::getDataFor(const EMissileWeapons& type)
{
    if (type == MW_None)
        return missile_data[0];
    return missile_data[type];
}

bool isNumber(const std::string& s)
{
   return !s.empty() && s.find_first_not_of("-.0123456789") == std::string::npos;
}


const MissileWeaponData& MissileWeaponData::getDataFor(const string& type)
{
    if(isNumber(type))
    {
        EMissileWeapons typeNum = (EMissileWeapons)std::stoi(type);
        return MissileWeaponData::getDataFor(typeNum);
    }

    return CustomMissileWeaponRegistry::getMissileWeapon(type);

}

void CustomMissileWeaponRegistry::createMissileWeapon(const EMissileWeapons &iBaseType, const std::string &iNewName, const float &iDamageMultiplier, const float &iSpeed, const EDamageType &iDT)
{
    MissileWeaponData base = MissileWeaponData::getDataFor(iBaseType);
    MissileWeaponData copyMWD = base;
    copyMWD.damage_multiplier = iDamageMultiplier;
    copyMWD.speed = iSpeed;
    copyMWD.basetype = iBaseType;
    copyMWD.damage_type = iDT;

    getCustomMissileWeapons().insert(MissileWeaponMap::value_type(iNewName,copyMWD));
}
auto CustomMissileWeaponRegistry::getMissileWeapon(const std::string &iName) -> MissileWeaponData&
{
    auto found = getCustomMissileWeapons().find(iName);

    if (found == getCustomMissileWeapons().end())
        assert(0 && "not found ");
    return found->second;
}
#ifndef _MSC_VER
// MFC: GCC does proper external template instantiation, VC++ doesn't.
#include "missileWeaponData.hpp"
#endif
