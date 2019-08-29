#include "homingMissile.h"
#include "particleEffect.h"
#include "spaceObjects/explosionEffect.h"

REGISTER_MULTIPLAYER_CLASS(HomingMissile, "HomingMissile");
HomingMissile::HomingMissile()
: MissileWeapon("HomingMissile", MissileWeaponData::getDataFor(MW_Homing), DT_Kinetic)
{
}

void HomingMissile::hitObject(P<SpaceObject> object)
{
    DamageInfo info(owner, damage_type, getPosition());
    object->takeDamage(35 * damage_multiplier, info);
    P<ExplosionEffect> e = new ExplosionEffect();
    e->setSize(30);
    e->setPosition(getPosition());
    e->setOnRadar(true);
}
