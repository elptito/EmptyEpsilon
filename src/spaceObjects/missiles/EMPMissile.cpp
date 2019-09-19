#include "EMPMissile.h"
#include "particleEffect.h"
#include "spaceObjects/electricExplosionEffect.h"

REGISTER_MULTIPLAYER_CLASS(EMPMissile, "EMPMissile");
EMPMissile::EMPMissile()
: MissileWeapon("EMPMissile", MissileWeaponData::getDataFor(MW_EMP), DT_EMP)
{
}

void EMPMissile::explode()
{
    DamageInfo info(owner, damage_type, getPosition());
    //Tdelc/Tsht : damage_multiplier (qui fait doublon avec la categorie)
    SpaceObject::damageArea(getPosition(), category_modifier * blast_range, category_modifier* damage_at_edge * damage_multiplier, category_modifier * damage_at_center * damage_multiplier, info, getRadius());

    P<ElectricExplosionEffect> e = new ElectricExplosionEffect();
    e->setSize(category_modifier * blast_range);
    e->setPosition(getPosition());
    e->setOnRadar(true);
}


void EMPMissile::hitObject(P<SpaceObject> object)
{
    explode();
}

void EMPMissile::lifeEnded()
{
    explode();
}


