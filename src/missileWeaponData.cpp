#include "missileWeaponData.h"

#define mine 100
#define slow 200
#define medium 350
#define fast 500
#define fastest 600
#define _range(speed, range) range * 1000.f / speed

MissileWeaponData missile_data[MW_Count] = {
    //                speed, turnrate, lifetime, color, homing_range
    MissileWeaponData(fast, 12.f, _range(fast, 20), sf::Color(255, 50, 255), 4500.0, "sfx/rlaunch.wav"),/*MW_Cruise*/
    MissileWeaponData(slow, 9.f, _range(slow, 20), sf::Color(255, 255, 0), 2000.0, "sfx/rlaunch.wav"),/*MW_Torpedo*/
    MissileWeaponData(medium, 10.f, _range(medium, 10), sf::Color(255, 0, 0), 2000.0, "sfx/rlaunch.wav"),/*MW_Heavy*/
    MissileWeaponData(medium, 10.f, _range(medium, 10), sf::Color(255, 255, 255), 1000.0, "sfx/rlaunch.wav"),/*MW_EMP*/
    MissileWeaponData(fast, 10.f,_range(fast, 10), sf::Color(0, 0, 255), 1000.0, "sfx/rlaunch.wav"),/*MW_Nuke*/
    MissileWeaponData(fastest,  0.f, _range(fastest, 5), sf::Color(128, 128, 128), 0.0, "sfx/hvli_fire.wav"),/*MW_HVLI*/
    MissileWeaponData(mine,  0.f, _range(mine, 1), sf::Color(255, 255, 255), 0.0, "missile_launch.wav")/*MW_Mine, lifetime is used at time which the mine is ejecting from the ship*/
};
MissileWeaponData::MissileWeaponData(float speed, float turnrate, float lifetime, sf::Color color, float homing_range, string fire_sound)
: speed(speed), turnrate(turnrate), lifetime(lifetime), color(color), homing_range(homing_range), fire_sound(fire_sound)
{
}

const MissileWeaponData& MissileWeaponData::getDataFor(EMissileWeapons type)
{
    if (type == MW_None)
        return missile_data[0];
    return missile_data[type];
}

#ifndef _MSC_VER
// MFC: GCC does proper external template instantiation, VC++ doesn't.
#include "missileWeaponData.hpp"
#endif
