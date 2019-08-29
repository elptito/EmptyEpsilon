#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "helmsHeliosScreen.h"

#include "screenComponents/radarView.h"
#include "screenComponents/impulseControls.h"
#include "screenComponents/warpControls.h"
#include "screenComponents/jumpControls.h"
#include "screenComponents/dockingButton.h"
#include "screenComponents/alertOverlay.h"
#include "screenComponents/customShipFunctions.h"
#include "screenComponents/systemStatus.h"

#include "gui/gui2_label.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_togglebutton.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_autolayout.h"

sf::Color halfTransparent(0, 0, 0, 128);

HelmsHeliosScreen::HelmsHeliosScreen(GuiContainer* owner)
: GuiOverlay(owner, "HELMS_SCREEN", colorConfig.background)
{
    // Render the radar shadow and background decorations.
    GuiOverlay* background_gradient = new GuiOverlay(this, "BACKGROUND_GRADIENT", sf::Color::White);
    background_gradient->setTextureCenter("gui/BackgroundGradient");

    GuiOverlay* background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));

    radar = new GuiRadarView(this, "HELMS_RADAR", 5000.0, nullptr, my_spaceship);
    radar->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMatchHeight, 800);
    radar->setRangeIndicatorStepSize(1000.0)->shortRange()->enableGhostDots()->enableWaypoints()->enableCallsigns()->enableHeadingIndicators()->setStyle(GuiRadarView::Circular);
    radar->enableMissileTubeIndicators()->setFogOfWarStyle(GuiRadarView::RadarRangeAndLineOfSight)->setAutoOrient(true)->setShowSectors(false);

    heading_hint = new GuiLabel(this, "HEADING_HINT", "", 30);
    heading_hint->setAlignment(ACenter)->setSize(0, 0);

    energy_display = new GuiKeyValueDisplay(this, "ENERGY_DISPLAY", 0.45, "Energy", "");
    energy_display->setIcon("gui/icons/energy")->setTextSize(20)->setPosition(20, 20, ATopLeft)->setSize(240, 40);
    heading_display = new GuiKeyValueDisplay(this, "HEADING_DISPLAY", 0.45, "Heading", "");
    heading_display->setIcon("gui/icons/heading")->setTextSize(20)->setPosition(20, 60, ATopLeft)->setSize(240, 40);
    velocity_display = new GuiKeyValueDisplay(this, "VELOCITY_DISPLAY", 0.45, "Speed", "");
    velocity_display->setIcon("gui/icons/speed")->setTextSize(20)->setPosition(20, 100, ATopLeft)->setSize(240, 40);
    maneuver_charge_bar = new GuiProgressbar(this, "MANEUVER_CHARGE", 0.0, 1.0, 0.0);
    maneuver_charge_bar->setColor(sf::Color(192, 192, 192, 64));
    maneuver_charge_bar->setPosition(20, 140, ATopLeft)->setSize(240, 40);
    (new GuiLabel(maneuver_charge_bar, "MANEUVER_CHARGE_LABEL", "Combat maneuver", 20))->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    (new GuiSystemStatus(this, "IMPULSE_STATUS", SYS_Impulse, my_spaceship))->setPosition(-20, 20, ATopRight)->setSize(300, 40);
    (new GuiSystemStatus(this, "MANEUVER_STATUS", SYS_Maneuver, my_spaceship))->setPosition(-20, 60, ATopRight)->setSize(300, 40);
    (new GuiSystemStatus(this, "WARP_STATUS", SYS_Warp, my_spaceship))->setPosition(-20, 100, ATopRight)->setSize(300, 40);

    docking_indicator = new GuiLabel(this, "DOCKING_INDICATOR", "Request Docking", 20);
    docking_indicator->addBackground()->setPosition(-20, 140, ATopRight)->setSize(240, 40);

    GuiElement* engine_layout = new GuiElement(this, "ENGINE_CONTAINER");
    engine_layout->setPosition(0, 220, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    
    warp_bar = new GuiProgressbar(engine_layout, "WARP_BAR", 0.0, 4.0, 0.0);
    warp_bar->setPosition(20, 0, ABottomLeft)->setSize(100, GuiElement::GuiSizeMax);
    impulse_bar = new GuiProgressbar(engine_layout, "WARP_BAR", -1.0, 1.0, 0.0);
    impulse_bar->setBiDirectional(true)->setPosition(-20, 0, ABottomRight)->setSize(100, GuiElement::GuiSizeMax);

    (new GuiCustomShipFunctions(this, helmsOfficer, "", my_spaceship))->setPosition(-20, 120, ATopRight)->setSize(250, GuiElement::GuiSizeMax);
}

void HelmsHeliosScreen::onDraw(sf::RenderTarget& window)
{
    if (my_spaceship)
    {
        energy_display->setValue(string(int(my_spaceship->energy_level)));
        heading_display->setValue(string(my_spaceship->getHeading(), 1));
        float velocity = sf::length(my_spaceship->getVelocity()) / 1000 * 60;
        velocity_display->setValue(string(velocity, 1) + DISTANCE_UNIT_1K + "/min");
        warp_bar->setVisible(my_spaceship->has_warp_drive);

        warp_bar->setValue(my_spaceship->current_warp);
        warp_bar->setText("Warp\n" + string(my_spaceship->current_warp, 1));
        impulse_bar->setValue(my_spaceship->current_impulse);
        impulse_bar->setText("Impulse\n" + string(int(my_spaceship->current_impulse * 100)) + "%");
        maneuver_charge_bar->setValue(my_spaceship->combat_maneuver_charge)->show();

        switch(my_spaceship->docking_state)
        {
        case DS_NotDocking:
            if (!my_spaceship->canStartDocking())
                docking_indicator->setText("Docking Disabled")->setTextColor(sf::Color::Yellow - halfTransparent);
            else if (findDockingTarget())
                docking_indicator->setText("Request Dock")->setTextColor(sf::Color::Yellow);
            else
                docking_indicator->setText("No Dock Target")->setTextColor(sf::Color::Yellow - halfTransparent);
            break;
        case DS_Docking:
            docking_indicator->setText("Cancel Docking")->setTextColor(sf::Color::Red);
            break;
        case DS_Docked:
            docking_indicator->setText("Undock")->setTextColor(sf::Color::Yellow);
            break;
        }
        if (my_spaceship->current_warp >= 1.f){
            radar->setDistance(35000.f)->setRangeIndicatorStepSize(5000.0)->longRange()->setStyle(GuiRadarView::CircularSector);
        } else {
            radar->setDistance(5000.f)->setRangeIndicatorStepSize(1000.0)->shortRange()->setStyle(GuiRadarView::Circular);
        }
    }
    GuiOverlay::onDraw(window);
}

P<SpaceObject> HelmsHeliosScreen::findDockingTarget()
{
    PVector<Collisionable> obj_list = CollisionManager::queryArea(my_spaceship->getPosition() - sf::Vector2f(1000, 1000), my_spaceship->getPosition() + sf::Vector2f(1000, 1000));
    P<SpaceObject> dock_object;
    foreach(Collisionable, obj, obj_list)
    {
        dock_object = obj;
        if (dock_object && dock_object->canBeDockedBy(my_spaceship) && (dock_object->getPosition() - my_spaceship->getPosition()) < 1000.0f + dock_object->getRadius())
            break;
        dock_object = NULL;
    }
    return dock_object;
}

bool HelmsHeliosScreen::onJoystickAxis(const AxisAction& axisAction){
    if(my_spaceship){
        if (axisAction.category == "HELMS"){
            if (axisAction.action == "IMPULSE"){
                my_spaceship->commandImpulse(axisAction.value);  
                return true;
            } 
            if (axisAction.action == "ROTATE"){
                my_spaceship->commandTurnSpeed(axisAction.value);
                return true;
            } 
            if (axisAction.action == "STRAFE"){
                my_spaceship->commandCombatManeuverStrafe(axisAction.value);
                return true;
            } 
            if (axisAction.action == "BOOST"){
                my_spaceship->commandCombatManeuverBoost(axisAction.value);
                return true;
            }
        }
    }
    return false;
}

void HelmsHeliosScreen::onHotkey(const HotkeyResult& key)
{
    if (key.category == "HELMS" && my_spaceship)
    {
        if (key.hotkey == "TURN_LEFT")
            my_spaceship->commandTargetRotation(my_spaceship->getRotation() - 5.0f);
        else if (key.hotkey == "TURN_RIGHT")
            my_spaceship->commandTargetRotation(my_spaceship->getRotation() + 5.0f);
        else if (key.hotkey == "WARP_0")
            my_spaceship->commandWarp(0);
        else if (key.hotkey == "WARP_1")
            my_spaceship->commandWarp(1);
        else if (key.hotkey == "WARP_2")
            my_spaceship->commandWarp(2);
        else if (key.hotkey == "WARP_3")
            my_spaceship->commandWarp(3);
        else if (key.hotkey == "WARP_4")
            my_spaceship->commandWarp(4);
        else if (key.hotkey == "INC_IMPULSE")
            my_spaceship->commandImpulse(my_spaceship->current_impulse + 0.1f);
        else if (key.hotkey == "DEC_IMPULSE")
            my_spaceship->commandImpulse(my_spaceship->current_impulse - 0.1f);
        else if (key.hotkey == "ZERO_IMPULSE")
            my_spaceship->commandImpulse(0.0f);
        else if (key.hotkey == "MAX_IMPULSE")
            my_spaceship->commandImpulse(1.0f);
        else if (key.hotkey == "MIN_IMPULSE")
            my_spaceship->commandImpulse(-1.0f);
        else if (key.hotkey == "COMBAT_LEFT")
            my_spaceship->commandCombatManeuverStrafe(my_spaceship->combat_maneuver_strafe_request - 0.1f);
        else if (key.hotkey == "COMBAT_RIGHT")
            my_spaceship->commandCombatManeuverStrafe(my_spaceship->combat_maneuver_strafe_request + 0.1f);
        else if (key.hotkey == "COMBAT_BOOST")
            my_spaceship->commandCombatManeuverBoost(my_spaceship->combat_maneuver_boost_request + 0.1f);
        else if (key.hotkey == "COMBAT_BACK")
            my_spaceship->commandCombatManeuverBoost(my_spaceship->combat_maneuver_boost_request - 0.1f);
        else if (my_spaceship->docking_state == DS_NotDocking && (key.hotkey == "DOCK_REQUEST" || key.hotkey == "DOCK_ACTION"))
            my_spaceship->commandDock(findDockingTarget());
        else if (my_spaceship->docking_state == DS_Docking && (key.hotkey == "DOCK_ABORT" || key.hotkey == "DOCK_ACTION"))
            my_spaceship->commandAbortDock();
        else if (my_spaceship->docking_state == DS_Docked && (key.hotkey == "UNDOCK" || key.hotkey == "DOCK_ACTION"))
            my_spaceship->commandUndock();
    }
}
