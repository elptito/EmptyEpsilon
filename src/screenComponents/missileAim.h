#ifndef MISSILE_AIM_H
#define MISSILE_AIM_H

class MissileAim
{
public:
    virtual float getMissileTargetAngle()=0;
    virtual bool getManualAim()=0;
};

#endif//MISSILE_AIM_H
