#include "EMPMissile.h"
#include "particleEffect.h"
#include "spaceObjects/electricExplosionEffect.h"

REGISTER_MULTIPLAYER_CLASS(EMPMissile, "EMPMissile");
EMPMissile::EMPMissile()
: MissileWeapon("EMPMissile", MissileWeaponData::getDataFor(MW_EMP), DT_EMP)
{
}

void EMPMissile::hitObject(P<SpaceObject> object)
{
    DamageInfo info(owner, damage_type, getPosition());
    //Tdelc/Tsht : damage_multiplier (qui fait doublon avec la categorie)
    SpaceObject::damageArea(getPosition(), category_modifier * blastRange, category_modifier* damageAtEdge * damage_multiplier, category_modifier * damageAtCenter * damage_multiplier, info, getRadius());

    P<ElectricExplosionEffect> e = new ElectricExplosionEffect();
    e->setSize(category_modifier * blastRange);
    e->setPosition(getPosition());
    e->setOnRadar(true);
}
