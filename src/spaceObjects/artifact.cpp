#include <SFML/OpenGL.hpp>
#include "artifact.h"
#include "explosionEffect.h"
#include "playerSpaceship.h"
#include "main.h"

#include "scriptInterface.h"

/// An artifact.
/// Can be used for mission scripting.
REGISTER_SCRIPT_SUBCLASS(Artifact, SpaceObject)
{
    /// Set the 3D model used for this artifact.
    /// Example: setModel("artifact6"), setModel("shield_generator"), setModel("ammo_box").
    /// Check model_data.lua for all possible options.
    REGISTER_SCRIPT_CLASS_FUNCTION(Artifact, setModel);
    /// Have this object explode with a visual explosion. The Artifact is destroyed by this action.
    REGISTER_SCRIPT_CLASS_FUNCTION(Artifact, explode);
    /// Set if this artifact can be picked up or not. When it is picked up, this artifact will be destroyed.
    REGISTER_SCRIPT_CLASS_FUNCTION(Artifact, allowPickup);
    /// Set a function that will be called if a player picks up the artifact.
    /// First argument given to the function will be the artifact, the second the player.
    REGISTER_SCRIPT_CLASS_FUNCTION(Artifact, onPickUp);
    REGISTER_SCRIPT_CLASS_FUNCTION(Artifact, setOrbit);
    REGISTER_SCRIPT_CLASS_FUNCTION(Artifact, setOrbitDistance);
    REGISTER_SCRIPT_CLASS_FUNCTION(Artifact, setHull);
}

REGISTER_MULTIPLAYER_CLASS(Artifact, "Artifact");
Artifact::Artifact()
: SpaceObject(120, "Artifact")
{
    registerMemberReplication(&model_data_name);

    setRotation(random(0, 360));

    current_model_data_name = "artifact" + string(irandom(1, 8));
    model_data_name = current_model_data_name;
    model_info.setData(current_model_data_name);

    allow_pickup = false;
    hull = random(5,10);

    orbit_target_id = -1;
    orbit_time = 0.0f;
    orbit_distance = 0.0f;
    registerMemberReplication(&orbit_target_id);
    registerMemberReplication(&orbit_time);
    registerMemberReplication(&orbit_distance);
}

void Artifact::update(float delta)
{
    if (current_model_data_name != model_data_name)
    {
        current_model_data_name = model_data_name;
        model_info.setData(current_model_data_name);
    }

    if (orbit_distance > 0.0f)
    {
        P<SpaceObject> orbit_target;
        if (game_server)
            orbit_target = game_server->getObjectById(orbit_target_id);
        else
            orbit_target = game_client->getObjectById(orbit_target_id);
        if (orbit_target)
        {
            float angle = sf::vector2ToAngle(getPosition() - orbit_target->getPosition());
            angle += delta / orbit_time * 360.0f;
            setPosition(orbit_target->getPosition() + sf::vector2FromAngle(angle) * orbit_distance);
        }
    }
}

void Artifact::setOrbit(P<SpaceObject> target, float orbit_time)
{
    if (!target)
        return;
    this->orbit_target_id = target->getMultiplayerId();
    this->orbit_distance = sf::length(getPosition() - target->getPosition());
    this->orbit_time = orbit_time;
}

void Artifact::setOrbitDistance(float orbit_distance)
{
    this->orbit_distance = orbit_distance;
}

void Artifact::drawOnRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, bool long_range)
{
    sf::Sprite object_sprite;
    textureManager.setTexture(object_sprite, "RadarBlip.png");
    object_sprite.setRotation(getRotation());
    object_sprite.setPosition(position);
    object_sprite.setColor(sf::Color(255, 255, 255));
    float size = getRadius() * scale / object_sprite.getTextureRect().width * 2;
    if (size < 0.2)
        size = 0.2;
    object_sprite.setScale(size, size);
    window.draw(object_sprite);
}

void Artifact::collide(Collisionable* target, float force)
{
    if (!isServer() || !allow_pickup)
        return;
    P<SpaceObject> hit_object = P<Collisionable>(target);
    P<PlayerSpaceship> player = hit_object;
    if (player)
    {
        if (on_pickup_callback.isSet())
        {
            on_pickup_callback.call(P<Artifact>(this), player);
        }
        destroy();
        player->addToShipLog("Objet recupere en soute",sf::Color::White,"generic");
    }
}

void Artifact::setModel(string model_name)
{
    model_data_name = model_name;
}

void Artifact::explode()
{
    P<ExplosionEffect> e = new ExplosionEffect();
    e->setSize(getRadius());
    e->setPosition(getPosition());
    destroy();
}

void Artifact::allowPickup(bool allow)
{
    allow_pickup = allow;
}

void Artifact::onPickUp(ScriptSimpleCallback callback)
{
    this->allow_pickup = 1;
    this->on_pickup_callback = callback;
}

string Artifact::getExportLine()
{
    string ret = "Artifact():setPosition(" + string(getPosition().x, 0) + ", " + string(getPosition().y, 0) + ")";
    ret += ":setModel(\"" + model_data_name + "\")";
    if (allow_pickup)
        ret += ":allowPickup(true)";
    return ret;
}
