#include "EMPMissile.h"
#include "particleEffect.h"
#include "electricExplosionEffect.h"

REGISTER_MULTIPLAYER_CLASS(EMPMissile, "EMPMissile");
EMPMissile::EMPMissile()
: MissileWeapon("EMPMissile", MissileWeaponData::getDataFor(MW_EMP), DT_EMP)
{
}

void EMPMissile::hitObject(P<SpaceObject> object)
{
    DamageInfo info(owner, damage_type, getPosition());
    SpaceObject::damageArea(getPosition(), blastRange, damageAtEdge * damage_multiplier, damageAtCenter * damage_multiplier, info, getRadius());

    P<ElectricExplosionEffect> e = new ElectricExplosionEffect();
    e->setSize(blastRange);
    e->setPosition(getPosition());
    e->setOnRadar(true);
}
