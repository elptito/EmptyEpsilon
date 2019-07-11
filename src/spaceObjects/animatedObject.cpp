
#include <SFML/OpenGL.hpp>
#include "animatedObject.h"
#include "explosionEffect.h"
#include "main.h"
#include "pathPlanner.h"

#include "scriptInterface.h"

REGISTER_SCRIPT_SUBCLASS(AnimatedObject, SpaceObject)
{
    REGISTER_SCRIPT_CLASS_FUNCTION(AnimatedObject, setSize);
}

REGISTER_MULTIPLAYER_CLASS(AnimatedObject, "AnimatedObject");
AnimatedObject::AnimatedObject()
: SpaceObject(random(110, 130), "AnimatedObject")
{
    setRotation(random(0, 360));
    rotation_speed = random(0.1, 0.8);
    z = random(-50, 50);
    hull = random(25,75);
    size = getRadius();
    model_number = 1;
    setRadarSignatureInfo(0.05, 0, 0);

    registerMemberReplication(&z);
    registerMemberReplication(&size);

    PathPlannerManager::getInstance()->addAvoidObject(this, 300);
}

void AnimatedObject::draw3D()
{
#if FEATURE_3D_RENDERING
//    if (size != getRadius())
//        setRadius(size);

    glTranslatef(0, 0, z);
    glRotatef(engine->getElapsedTime() * rotation_speed, 0, 0, 1);
//    glScalef(getRadius(), getRadius(), getRadius());
    glScalef(size*50, size*50, size*50);
    sf::Shader* shader = ShaderManager::getShader("objectShaderBS");
    shader->setParameter("baseMap", *textureManager.getTexture("animation/baleine/Whale Normals.tga"));
    shader->setParameter("specularMap", *textureManager.getTexture("animation/baleine/Whale Diffuse.tga"));
    shader->setParameter("illuminationMap", *textureManager.getTexture("animation/baleine/Whale Emissive.tga"));
    sf::Shader::bind(shader);

    std::string new_string = std::string(6 - string(model_number).length(), '0') + string(model_number);
    Mesh* m = Mesh::getMesh("animation/baleine/whale3_"+ string(new_string) +".obj");
    m->render();
#endif//FEATURE_3D_RENDERING
}

void AnimatedObject::drawOnRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, bool long_range)
{
    if (size != getRadius())
        setRadius(size);

    sf::Sprite object_sprite;
    textureManager.setTexture(object_sprite, "astero-1.png");
    object_sprite.setRotation(getRotation());
    object_sprite.setPosition(position);
//    object_sprite.setColor(sf::Color(255, 200, 100));
    float size = getRadius() * scale / object_sprite.getTextureRect().width * 2;
    if (size < 0.2)
        size = 0.2;
    object_sprite.setScale(size, size);
    window.draw(object_sprite);
}

void AnimatedObject::collide(Collisionable* target, float force)
{
    if (!isServer())
        return;
    P<SpaceObject> hit_object = P<Collisionable>(target);
    if (!hit_object || !hit_object->canBeTargetedBy(nullptr))
        return;

    DamageInfo info(nullptr, DT_Kinetic, getPosition());
    hit_object->takeDamage(35, info);

    P<ExplosionEffect> e = new ExplosionEffect();
    e->setSize(getRadius());
    e->setPosition(getPosition());
    destroy();
}

void AnimatedObject::update(float delta)
{
    model_number += 1;
    if (model_number > 400) model_number = 1;
}

void AnimatedObject::setModel(int model_number)
{
    model_number = model_number;
}

void AnimatedObject::setSize(float new_size)
{
    size = new_size;
}
