#include "missileWeapon.h"
#include "particleEffect.h"
#include "spaceObjects/explosionEffect.h"

MissileWeapon::MissileWeapon(string multiplayer_name, const MissileWeaponData& data, const EDamageType &itype)
: SpaceObject(10, multiplayer_name), data(data),damage_type(itype)
{
    target_id = -1;
    target_angle = 0;
    category_modifier = 1;
    lifetime = data.lifetime;
    hull = 5;
    speed = 0;
    color = data.color;

    registerMemberReplication(&target_id);
    registerMemberReplication(&target_angle);
    registerMemberReplication(&damage_multiplier);
    registerMemberReplication(&damage_type);
    registerMemberReplication(&color);
    registerMemberReplication(&category_modifier);

    launch_sound_played = false;
}

void MissileWeapon::drawOnRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, bool long_range)
{
    if (long_range) return;

    sf::Sprite object_sprite;
    textureManager.setTexture(object_sprite, "RadarArrow.png");
    object_sprite.setRotation(getRotation());
    object_sprite.setPosition(position);
    object_sprite.setColor(color);
    object_sprite.setScale(0.25 + 0.25 * category_modifier, 0.25 + 0.25 * category_modifier);
    window.draw(object_sprite);
}

void MissileWeapon::update(float delta)
{
    updateMovement();
    
    // Small missiles have a larger speed & rotational speed, large ones are slower and turn less fast
    float size_speed_modifier = 1 / category_modifier;

    P<SpaceObject> target;
    if (game_server)
        target = game_server->getObjectById(target_id);
    else
        target = game_client->getObjectById(target_id);

    if (target)
    {
        if (translate_z < target->translate_z)
            translate_z += delta;
        if (translate_z > target->translate_z)
            translate_z -= delta;
    }

    if (!launch_sound_played)
    {
        soundManager->playSound(data.fire_sound, getPosition(), 200.0, 1.0, 1.0f + random(-0.2f, 0.2f));
        launch_sound_played = true;
    }
    
    // Since we do want the range to remain the same, ensure that slow missiles don't die down as fast.
    lifetime -= delta * size_speed_modifier;
    if (lifetime < 0)
    {
        lifeEnded();
        destroy();
    }
    setVelocity(sf::vector2FromAngle(getRotation()) * speed * size_speed_modifier);

    if (delta > 0)
    {
        ParticleEngine::spawn(sf::Vector3f(getPosition().x, getPosition().y, translate_z), sf::Vector3f(getPosition().x, getPosition().y, translate_z), sf::Vector3f(1, 0.8, 0.8), sf::Vector3f(0, 0, 0), 5, 20, 5.0);
    }
}

void MissileWeapon::collide(Collisionable* target, float force)
{
    if (!game_server)
    {
        return;
    }
    P<SpaceObject> object = P<Collisionable>(target);
    if (!object || object == owner || !object->canBeTargetedBy(owner))
    {
        return;
    }
    P<SpaceShip> ship = object;
    if (ship && ship->isDockedWith(owner))
    {
        return;
    }

    hitObject(object);
    destroy();
}

void MissileWeapon::takeDamage(float damage_amount, DamageInfo info)
{
    // If no hull, then it could no be destroyed
    if(hull <= 0)
        return;
    if (info.type == DT_EMP)
        return;
    if (random(1,100)<80-(damage_amount*2))
        return;

    hull -= damage_amount;
    if (hull <= 0)
    {
        P<ExplosionEffect> e = new ExplosionEffect();
        e->setSize(getRadius());
        e->setPosition(getPosition());
        destroy();
    }
}

void MissileWeapon::updateMovement()
{
    if (data.turnrate > 0.0)
    {
        if (data.homing_range > 0)
        {
            P<SpaceObject> target;
            if (game_server)
            {
                target = game_server->getObjectById(target_id);
            }
            else
            {
                target = game_client->getObjectById(target_id);
            }

            if (target && (target->getPosition() - getPosition()) < data.homing_range + target->getRadius())
            {
                target_angle = sf::vector2ToAngle(target->getPosition() - getPosition());
            }
        }
        // Small missiles have a larger speed & rotational speed, large ones are slower and turn less fast
        float size_speed_modifier = 1 / category_modifier;

        float angle_diff = sf::angleDifference(getRotation(), target_angle);

        if (angle_diff > 1.0)
            setAngularVelocity(data.turnrate * size_speed_modifier);
        else if (angle_diff < -1.0)
            setAngularVelocity(data.turnrate * -1.0f * size_speed_modifier);
        else
            setAngularVelocity(angle_diff * data.turnrate * size_speed_modifier);
    }
}
