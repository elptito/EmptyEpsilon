#ifndef PLANET_H
#define PLANET_H

#include "nebula.h"
#include "spaceObject.h"
#include "pathPlanner.h"

class Planet : public SpaceObject, public Updatable
{
public:
    Planet();

#if FEATURE_3D_RENDERING
    virtual void draw3D() override;
    virtual void draw3DTransparent() override;
#endif//FEATURE_3D_RENDERING
    virtual void drawOnRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, float rotation, bool long_range) override;
    virtual void drawOnGMRadar(sf::RenderTarget& window, sf::Vector2f draw_position, float scale, float rotation, bool long_range) override;
    virtual void update(float delta) override;
    virtual void collide(Collisionable* target, float force) override;
    virtual bool canHideInNebula()  override { return false; }

    //Ajout Tdelc
    virtual bool canBeDockedBy(P<SpaceObject> obj) { return true; }
    virtual bool canBeLandedOn(P<SpaceObject> obj) { return false; }

    
    float getPlanetRadius();
    float getCollisionSize();

    void setPlanetAtmosphereColor(float r, float g, float b);
    void setPlanetAtmosphereTexture(string texture_name);
    void setPlanetSurfaceTexture(string texture_name);
    string getPlanetSurfaceTexture();
    void setPlanetIcon(string texture_name);
    void setPlanetCloudTexture(string texture_name);
    void setPlanetRadius(float size);
    void setPlanetCloudRadius(float size);
    void setPlanetAtmosphereRadius(float size);
    void setDistanceFromMovementPlane(float distance_from_movement_plane);
    void setAxialRotationTime(float time);
    void setAxialRotation(float axis);
    void setOrbit(P<SpaceObject> target, float orbit_time);
    
    virtual string getExportLine() override { return "Planet():setPosition(" + string(getPosition().x, 0) + ", " + string(getPosition().y, 0) + "):setPlanetRadius(" + string(getPlanetRadius(), 0) + ")"; }

private:
    //Config:
    float planet_size;
    float cloud_size;
    float atmosphere_size;
    string planet_texture;
    string planet_icon;
    string cloud_texture;
    string atmosphere_texture;
    sf::Color atmosphere_color;
    float distance_from_movement_plane;

    float axial_rotation_time;
    float rotation_axis;
    int32_t orbit_target_id;
    float orbit_time;
    float orbit_distance;

    float collision_size;

    void updateCollisionSize();
};

#endif//PLANET_H
