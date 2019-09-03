#ifndef NUKE_H
#define NUKE_H

#include "missileWeapon.h"

class Nuke : public MissileWeapon
{
    constexpr static float blastRange = 5000.0f;
    constexpr static float damageAtCenter = 300.0f;
    constexpr static float damageAtEdge = 70.0f;
public:
    Nuke();

    virtual void hitObject(P<SpaceObject> object);
};

#endif//NUKE_H
