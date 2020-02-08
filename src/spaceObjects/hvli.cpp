#include "hvli.h"
#include "particleEffect.h"
#include "explosionEffect.h"

REGISTER_MULTIPLAYER_CLASS(HVLI, "HVLI");
HVLI::HVLI()
: MissileWeapon("HVLI", MissileWeaponData::getDataFor(MW_HVLI), DT_Kinetic)
{
}

void HVLI::hitObject(P<SpaceObject> object)
{
    DamageInfo info(owner, damage_type, getPosition());
    float alive_for = MissileWeaponData::getDataFor(MW_HVLI).lifetime - lifetime;
    if (alive_for > 2.0)
        object->takeDamage(6 * damage_multiplier, info);
    else
        object->takeDamage(6 * damage_multiplier * (alive_for / 2.0), info);
    P<ExplosionEffect> e = new ExplosionEffect();
    e->setSize(20);
    e->setPosition(getPosition());
    e->setOnRadar(true);
}
