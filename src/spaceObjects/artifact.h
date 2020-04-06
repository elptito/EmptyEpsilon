#ifndef ARTIFACT_H
#define ARTIFACT_H

#include "spaceObject.h"

class Artifact : public SpaceObject, public Updatable
{
private:
    string current_model_data_name;
    string model_data_name;
    float artifact_spin=0.0;
    bool allow_pickup;
    float size;
    ScriptSimpleCallback on_pickup_callback;
    int32_t orbit_target_id;
    float orbit_time;
    float orbit_distance;
public:
    Artifact();

    virtual void update(float delta) override;

    virtual void draw3D() override;

    virtual void drawOnRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, float rotation, bool long_range) override;

    virtual void collide(Collisionable* target, float force) override;

    void setModel(string name);
    void setSpin(float spin=0.0);
    void explode();
    void allowPickup(bool allow);
    void setOrbit(P<SpaceObject> target, float orbit_time);
    void setOrbitDistance(float orbit_distance);
    void setHull(float hull) {this->hull = hull;}

    virtual string getExportLine() override;
    void onPickUp(ScriptSimpleCallback callback);
};

#endif//ARTIFACT_H
