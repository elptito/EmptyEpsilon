#include "torpedo.h"
#include "particleEffect.h"
#include "explosionEffect.h"

REGISTER_MULTIPLAYER_CLASS(Torpedo, "Torpedo");
Torpedo::Torpedo()
: MissileWeapon("Torpedo", MissileWeaponData::getDataFor(MW_Torpedo))
{
}

void Torpedo::hitObject(P<SpaceObject> object)
{
    DamageInfo info(owner, DT_Kinetic, getPosition());
    object->takeDamage(80, info);
    P<ExplosionEffect> e = new ExplosionEffect();
    e->setSize(30);
    e->setPosition(getPosition());
    e->setOnRadar(true);
}
