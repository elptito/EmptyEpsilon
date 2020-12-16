#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "landingButton.h"

GuiLandingButton::GuiLandingButton(GuiContainer* owner, string id, P<PlayerSpaceship> targetSpaceship)
: GuiButton(owner, id, "", [this]() { click(); }), landing_spaceship(targetSpaceship)
{
    setIcon("gui/icons/docking");
}

void GuiLandingButton::click()
{
    if (!landing_spaceship)
        return;
    if(landing_spaceship->docking_state != DS_NotDocking)
        return;
    switch(landing_spaceship->landing_state)
    {
    case LS_NotLanding:
        landing_spaceship->commandLand(findLandingTarget());
        landing_spaceship->commandSetLandingTarget(findLandingTarget());
        break;
    case LS_Landing:
        landing_spaceship->commandAbortLanding();
        landing_spaceship->commandSetLandingTarget(NULL);
        break;
//    case DS_Docked:
//        landing_spaceship->commandUndock();
//        landing_spaceship->commandSetDockTarget(NULL);
//        break;
    }
}

void GuiLandingButton::onDraw(sf::RenderTarget& window)
{
    if (landing_spaceship)
    {
        switch(landing_spaceship->landing_state)
        {
        case LS_NotLanding:
            //setText("Atterrissage demande");
            if ((landing_spaceship->getSystemEffectiveness(SYS_Hangar) > 0) 
                && landing_spaceship->canStartLanding() 
                && findLandingTarget())
            {
                enable();
                setText("Se poser : " + findLandingTarget()->callsign);
            }
            else if((landing_spaceship->getSystemEffectiveness(SYS_Hangar) <= 0)
                && landing_spaceship->canStartLanding() 
                && findLandingTarget())
            {
                disable();
                setText("Hangar HS");
            }
            else
            {
                disable();
                setText("Se poser");
            }
            break;
        case LS_Landing:
            setText("Annuler se poser");
            enable();
            break;
//        case DS_Docked:
//            setText("Se Dedocker");
//            enable();
//            break;
        }

        setVisible(landing_spaceship->hasSystem(SYS_Impulse));
    }
    GuiButton::onDraw(window);
}

void GuiLandingButton::onHotkey(const HotkeyResult& key)
{
    if (key.category == "HELMS" && landing_spaceship && landing_spaceship->docking_state == DS_NotDocking)
    {
        if (key.hotkey == "LAND_ACTION")
        {
            switch(landing_spaceship->landing_state)
            {
            case LS_NotLanding:
                landing_spaceship->commandLand(findLandingTarget());
                break;
            case LS_Landing:
                landing_spaceship->commandAbortLanding();
                break;
//            case DS_Docked:
//                landing_spaceship->commandUndock();
//                break;
            }
        }
        else if (key.hotkey == "LAND_REQUEST")
            landing_spaceship->commandLand(findLandingTarget());
        else if (key.hotkey == "LAND_ABORT")
            landing_spaceship->commandAbortLanding();
//        else if (key.hotkey == "UNDOCK")
//            landing_spaceship->commandUndock();
    }
}

P<SpaceObject> GuiLandingButton::findLandingTarget()
{
    PVector<Collisionable> obj_list = CollisionManager::queryArea(landing_spaceship->getPosition() - sf::Vector2f(1000, 1000), landing_spaceship->getPosition() + sf::Vector2f(1000, 1000));
    P<SpaceObject> land_object;
    P<SpaceShip> land_ship;
    P<SpaceStation> land_station;
    foreach(Collisionable, obj, obj_list)
    {
        land_object = obj;
        if (landing_spaceship->id_galaxy != land_object->id_galaxy) // ??? da fuck pareil copier coller docking
        {
            land_object = NULL;
            continue;
        }
        land_ship = obj;
        if(!land_ship)
        {
            land_object = NULL;
            continue;
        }
        //land_station = obj;
        if (land_object
            &&(land_ship->landing_state == LS_NotLanding)
            &&(land_ship->docking_state == DS_NotDocking)
            &&(land_object != landing_spaceship)
            &&(land_object->canBeLandedOn(landing_spaceship))
            &&(land_object->getPosition() - landing_spaceship->getPosition()) < 1000.0f + land_object->getRadius())
            break;
        land_object = NULL;
    }
    return land_object;
}
