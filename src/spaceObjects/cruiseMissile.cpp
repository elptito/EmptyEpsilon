#include "cruiseMissile.h"
#include "particleEffect.h"
#include "explosionEffect.h"

REGISTER_MULTIPLAYER_CLASS(CruiseMissile, "CruiseMissile");
CruiseMissile::CruiseMissile()
: MissileWeapon("CruiseMissile", MissileWeaponData::getDataFor(MW_Cruise))
{
}

void CruiseMissile::hitObject(P<SpaceObject> object)
{
    DamageInfo info(owner, DT_Kinetic, getPosition());
    object->takeDamage(40, info);
    P<ExplosionEffect> e = new ExplosionEffect();
    e->setSize(30);
    e->setPosition(getPosition());
    e->setOnRadar(true);
}
