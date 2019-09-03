#ifndef HEAVY_MISSLE_H
#define HEAVY_MISSLE_H

#include "missileWeapon.h"

class HeavyMissile : public MissileWeapon
{
public:
    HeavyMissile();
    
    virtual void hitObject(P<SpaceObject> object);
};

#endif//HEAVY_MISSLE_H
