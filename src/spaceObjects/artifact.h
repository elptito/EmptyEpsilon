#ifndef ARTIFACT_H
#define ARTIFACT_H

#include "spaceObject.h"

class Artifact : public SpaceObject, public Updatable
{
private:
    string current_model_data_name;
    string model_data_name;
    bool allow_pickup;
    float size;
    ScriptSimpleCallback on_pickup_callback;
    int32_t orbit_target_id;
    float orbit_time;
    float orbit_distance;
public:
    Artifact();

    virtual void update(float delta);

    virtual void drawOnRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, bool long_range);

    virtual void collide(Collisionable* target, float force) override;

    void setModel(string name);
    void explode();
    void allowPickup(bool allow);
    void setOrbit(P<SpaceObject> target, float orbit_time);

    virtual string getExportLine();
    void onPickUp(ScriptSimpleCallback callback);
};

#endif//ARTIFACT_H
