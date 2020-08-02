#ifndef WARP_JAMMER_H
#define WARP_JAMMER_H

#include "spaceObject.h"

class WarpJammer : public SpaceObject
{
    static PVector<WarpJammer> jammer_list;

    float range;
    float hull;

    ScriptSimpleCallback on_destruction;
    ScriptSimpleCallback on_taking_damage;
public:
    WarpJammer();
    ~WarpJammer();

    void setRange(float range) { this->range = range; }
    float getRange() { return range; }

    virtual void drawOnRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, float rotation, bool long_range) override;

    static bool isWarpJammed(sf::Vector2f position);
    static sf::Vector2f getFirstNoneJammedPosition(sf::Vector2f start, sf::Vector2f end);

    void onTakingDamage(ScriptSimpleCallback callback);
    void onDestruction(ScriptSimpleCallback callback);

    //Ajout post Tdelc. TODO : verifier si on peut merger avec la fonction de SpaceObject, 
    //la modif de Tdelc consistait a rendre ces deux methodes communes pour tout objet qui a une coque (hull)
    virtual bool canBeTargetedBy(P<SpaceObject> other)  override { return true; }
    virtual void takeDamage(float damage_amount, DamageInfo info) override;
    
    virtual string getExportLine() override { return "WarpJammer():setFaction(\"" + getFaction() + "\"):setPosition(" + string(getPosition().x, 0) + ", " + string(getPosition().y, 0) + ")"; }
};

#endif//WARP_JAMMER_H
