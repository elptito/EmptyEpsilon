#include "heavyMissile.h"
#include "particleEffect.h"
#include "explosionEffect.h"

REGISTER_MULTIPLAYER_CLASS(HeavyMissile, "HeavyMissile");
HeavyMissile::HeavyMissile()
: MissileWeapon("HeavyMissile", MissileWeaponData::getDataFor(MW_Heavy))
{
}

void HeavyMissile::hitObject(P<SpaceObject> object)
{
    DamageInfo info(owner, DT_Kinetic, getPosition());
    object->takeDamage(35, info);
    P<ExplosionEffect> e = new ExplosionEffect();
    e->setSize(30);
    e->setPosition(getPosition());
    e->setOnRadar(true);
}
