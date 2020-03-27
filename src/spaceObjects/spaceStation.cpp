#include "spaceObjects/explosionEffect.h"
#include "spaceObjects/spaceStation.h"
#include "spaceObjects/spaceship.h"
#include "spaceObjects/playerSpaceship.h"
#include "shipTemplate.h"
#include "playerInfo.h"
#include "factionInfo.h"
#include "mesh.h"
#include "main.h"
#include "pathPlanner.h"

#include "scriptInterface.h"
REGISTER_SCRIPT_SUBCLASS(SpaceStation, ShipTemplateBasedObject)
{
}

REGISTER_MULTIPLAYER_CLASS(SpaceStation, "SpaceStation");
SpaceStation::SpaceStation()
: ShipTemplateBasedObject(300, "SpaceStation")
{
    restocks_scan_probes = true;
    comms_script_name = "comms_station.lua";
    setRadarSignatureInfo(0.2, 0.5, 0.5);
    setScanningParameters(2,2);

    callsign = "DS" + string(getMultiplayerId());
}

void SpaceStation::drawOnRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, float rotation, bool long_range)
{
    sf::Sprite objectSprite;
    // If the object is a station that hasn't been scanned, draw the default icon.
    // Otherwise, draw the station-specific icon.
	float sprite_scale = 0.2;
	if (my_spaceship && (getScannedStateFor(my_spaceship) == SS_NotScanned || getScannedStateFor(my_spaceship) == SS_FriendOrFoeIdentified) && getFactionId() != my_spaceship->getFactionId())
    {
        objectSprite.setColor(sf::Color(192, 192, 192));
        textureManager.setTexture(objectSprite, "RadarBlip.png");
    }
    else
    {
        objectSprite.setColor(factionInfo[getFactionId()]->gm_color);
        textureManager.setTexture(objectSprite, radar_trace);
		//sprite_scale = scale * getRadius() * 1.5 / objectSprite.getTextureRect().width;
		sprite_scale = scale * getRadius() * 2 / objectSprite.getTextureRect().width;
    }
    objectSprite.setPosition(position);

    if (!long_range)
    {
        sprite_scale *= 0.7;
        drawShieldsOnRadar(window, position, scale, sprite_scale, true);
    }
    sprite_scale = std::max(0.15f, sprite_scale);
    objectSprite.setScale(sprite_scale, sprite_scale);

    window.draw(objectSprite);
}

void SpaceStation::applyTemplateValues()
{
    PathPlannerManager::getInstance()->addAvoidObject(this, getRadius() * 1.5f);
}

void SpaceStation::destroyedByDamage(DamageInfo& info)
{
    ExplosionEffect* e = new ExplosionEffect();
    e->setSize(getRadius());
    e->setPosition(getPosition());
    e->setRadarSignatureInfo(0.0, 0.4, 0.4);
    
    if (info.instigator)
    {
        float points = 0;
        if (shield_count > 0)
        {
            for(int n=0; n<shield_count; n++)
            {
                points += shield_max[n] * 0.1;
            }
            points /= shield_count;
        }
        points += hull_max * 0.1;
        if (isEnemy(info.instigator))
            info.instigator->addReputationPoints(points);
        else
            info.instigator->removeReputationPoints(points);
    }
}

bool SpaceStation::canBeDockedBy(P<SpaceObject> obj)
{
    if (isEnemy(obj))
        return false;
    P<SpaceShip> ship = obj;
    if (!ship)
        return false;
    return true;
}

bool SpaceStation::canBeLandedOn(P<SpaceObject> obj)
{
    if (isEnemy(obj) || !ship_template)
        return false;
    P<SpaceShip> ship = obj;
    if (!ship || !ship->ship_template)
        return false;
    if (ship->getFactionId() != getFactionId())
        return false;
    // return ship_template->can_be_docked_by_class.count(ship->ship_template->getClass()) > 0;
    return true;
}

string SpaceStation::getExportLine()
{
    return "SpaceStation():setTemplate(\"" + template_name + "\"):setFaction(\"" + getFaction() + "\"):setCallSign(\"" + getCallSign() + "\"):setPosition(" + string(getPosition().x, 0) + ", " + string(getPosition().y, 0) + ")";
}
