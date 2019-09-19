#include "nuke.h"
#include "particleEffect.h"
#include "spaceObjects/explosionEffect.h"

REGISTER_MULTIPLAYER_CLASS(Nuke, "Nuke");
Nuke::Nuke()
: MissileWeapon("Nuke", MissileWeaponData::getDataFor(MW_Nuke), DT_Kinetic)
{
}

void Nuke::explode()
{
    DamageInfo info(owner, damage_type, getPosition());
    SpaceObject::damageArea(getPosition(), category_modifier * blast_range, category_modifier * damage_at_edge * damage_multiplier, category_modifier * damage_at_center * damage_multiplier, info, getRadius());

    P<ExplosionEffect> e = new ExplosionEffect();
    e->setSize(category_modifier * blast_range);
    e->setPosition(getPosition());
    e->setOnRadar(true);
    e->setExplosionSound("sfx/nuke_explosion.wav");
}

void Nuke::hitObject(P<SpaceObject> object)
{
    explode();
}

void Nuke::lifeEnded()
{
    explode();
}
