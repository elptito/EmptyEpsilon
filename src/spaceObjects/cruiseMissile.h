#ifndef CRUISE_MISSLE_H
#define CRUISE_MISSLE_H

#include "missileWeapon.h"

class CruiseMissile : public MissileWeapon
{
public:
    CruiseMissile();
    
    virtual void hitObject(P<SpaceObject> object);
};

#endif//CRUISE_MISSLE_H
