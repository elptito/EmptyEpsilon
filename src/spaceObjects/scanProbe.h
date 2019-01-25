#ifndef SCAN_PROBE_H
#define SCAN_PROBE_H

#include "spaceObject.h"

class ScanProbe : public SpaceObject, public Updatable
{
private:
    constexpr static float probe_speed = 5000.0f;
    float lifetime;
    sf::Vector2f target_position;
public:
    int owner_id;

    ScanProbe();

    virtual void update(float delta);

    virtual void drawOnRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, bool long_range) override;
    virtual void collide(Collisionable* target, float force) override;

    void setTarget(sf::Vector2f target) { target_position = target; }
    sf::Vector2f getTarget() { return target_position; }
    void setOwner(P<SpaceObject> owner);
};

#endif//SCAN_PROBE_H
