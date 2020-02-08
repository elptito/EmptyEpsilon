#include "nuke.h"
#include "particleEffect.h"
#include "explosionEffect.h"

REGISTER_MULTIPLAYER_CLASS(Nuke, "Nuke");
Nuke::Nuke()
: MissileWeapon("Nuke", MissileWeaponData::getDataFor(MW_Nuke), DT_Kinetic)
{
}

void Nuke::hitObject(P<SpaceObject> object)
{
    DamageInfo info(owner, damage_type, getPosition());
    SpaceObject::damageArea(getPosition(), blastRange, damageAtEdge * damage_multiplier, damageAtCenter * damage_multiplier, info, getRadius());

    P<ExplosionEffect> e = new ExplosionEffect();
    e->setSize(blastRange);
    e->setPosition(getPosition());
    e->setOnRadar(true);
    e->setExplosionSound("sfx/nuke_explosion.wav");
}
