#ifndef TORPEDO_H
#define TORPEDO_H

#include "missileWeapon.h"

class Torpedo : public MissileWeapon
{
public:
    Torpedo();
    
    virtual void hitObject(P<SpaceObject> object);
};

#endif//TORPEDO_H
