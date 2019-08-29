#ifndef MISSILE_WEAPON_H
#define MISSILE_WEAPON_H

#include "spaceObjects/spaceObject.h"
#include "spaceObjects/spaceship.h"

/* Base class for all the missile weapons. Handles missile generic stuff like targeting, lifetime, etc... */
class MissileWeapon : public SpaceObject, public Updatable
{
protected:
    const MissileWeaponData& data;

    float lifetime; //sec

    bool launch_sound_played;

public:
    P<SpaceObject> owner; //Only valid on server.
    int32_t target_id;
    float target_angle;
    float speed;
    sf::Color color;
    // Damage modifier for this missile which indicates it's size. (eg; Missiles by fighters have a low modifier), missiles from
    // capital ships have a high modifier.
    float category_modifier;  
    //Tdelc/Tsht : damage_multiplier est principalement la meme chose que category_modifier (mais fait avant). 
    //Les deux sont cumulables, et damage_multiplier est finement customisable
    float damage_multiplier;
    EDamageType damage_type;
    //

    MissileWeapon(string multiplayer_name, const MissileWeaponData& data, const EDamageType &i_damage_type);

    virtual void drawOnRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, bool long_range);
    virtual void update(float delta);

    virtual void collide(Collisionable* target, float force) override;
    virtual void takeDamage(float damage_amount, DamageInfo info) override;

    //Called when the missile hits something (could be the target, or something else). Missile is destroyed afterwards.
    virtual void hitObject(P<SpaceObject> object) = 0;
    //Called when the missile's lifetime is up. Missile is destroyed afterwards.
    virtual void lifeEnded() {}

private:
    void updateMovement();
};

#endif//MISSILE_WEAPON_H
