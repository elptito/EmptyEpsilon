#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "weaponsHeliosScreen.h"

#include "screenComponents/alertOverlay.h"
#include "screenComponents/customShipFunctions.h"
#include "screenComponents/systemStatus.h"
#include "screenComponents/radarView.h"

#include "gui/gui2_rotationdial.h"
#include "gui/gui2_label.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_progressbar.h"

static sf::Color grey(96, 96, 96);
WeaponsHeliosScreen::WeaponsHeliosScreen(GuiContainer* owner)
: GuiOverlay(owner, "WEAPONS_SCREEN", colorConfig.background), load_type(MW_None), missile_target_angle(0)
    , next_shields_frequency(0), manual_aim(false), target_mode_near(true), target_mode_enemy(true)
{
    // Render the radar shadow and background decorations.
    GuiOverlay* background_gradient = new GuiOverlay(this, "BACKGROUND_GRADIENT", sf::Color::White);
    background_gradient->setTextureCenter("gui/BackgroundGradient");

    GuiOverlay* background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));

    radar = new GuiRadarView(this, "HELMS_RADAR", PlayerSpaceship::weapons_radar_range, &targets, my_spaceship);
    radar->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMatchHeight, 800);
    radar->setRangeIndicatorStepSize(1000.0)->shortRange()->enableGhostDots()->enableWaypoints()->enableCallsigns()->enableHeadingIndicators()->setStyle(GuiRadarView::Circular);
    radar->setFogOfWarStyle(GuiRadarView::RadarRangeAndLineOfSight)->setAutoOrient(true)->setShowSectors(false)->enableTargetProjections(this);

    missile_aim = new GuiRotationDial(this, "MISSILE_AIM", 0, 360, 0, nullptr);
    missile_aim->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMatchHeight, 850);
    
    energy_display = new GuiKeyValueDisplay(this, "ENERGY_DISPLAY", 0.45, "Energy", "");
    energy_display->setIcon("gui/icons/energy")->setTextSize(20)->setPosition(20, 20, ATopLeft)->setSize(240, 40);
    front_shield_display = new GuiKeyValueDisplay(this, "FRONT_SHIELD_DISPLAY", 0.45, "Front", "");
    front_shield_display->setIcon("gui/icons/shields-fore")->setTextSize(20)->setPosition(20, 60, ATopLeft)->setSize(240, 40);
    rear_shield_display = new GuiKeyValueDisplay(this, "REAR_SHIELD_DISPLAY", 0.45, "Rear", "");
    rear_shield_display->setIcon("gui/icons/shields-aft")->setTextSize(20)->setPosition(20, 100, ATopLeft)->setSize(240, 40);

    (new GuiSystemStatus(this, "FRONT_STATUS", SYS_FrontShield, my_spaceship))->setPosition(-20, 20, ATopRight)->setSize(350, 40);
    (new GuiSystemStatus(this, "REAR_STATUS", SYS_RearShield, my_spaceship))->setPosition(-20, 60, ATopRight)->setSize(350, 40);
    (new GuiSystemStatus(this, "BEAMS_STATUS", SYS_BeamWeapons, my_spaceship))->setPosition(-20, 100, ATopRight)->setSize(350, 40);
    (new GuiSystemStatus(this, "MISSILES_STATUS", SYS_MissileSystem, my_spaceship))->setPosition(-20, 140, ATopRight)->setSize(350, 40);

    GuiAutoLayout* missileLoadLayout = new GuiAutoLayout(this, "MISSILE_LOAD_TYPES", GuiAutoLayout::LayoutVerticalTopToBottom);
    missileLoadLayout->setPosition(20, 180, ATopLeft)->setSize(200, 40 * MW_Count);

    for (int n = 0; n < MW_Count; n++)
    {   
        load_type_rows[n] = new GuiKeyValueDisplay(missileLoadLayout, "MISSILE_LOAD_" + string(n), 0.45, getMissileWeaponName(EMissileWeapons(n)), "");
        load_type_rows[n]->setTextSize(20)->setSize(GuiElement::GuiSizeMax, 40);
    }
    load_type_rows[MW_Cruise]->setIcon("gui/icons/weapon-homing.png");
    load_type_rows[MW_Torpedo]->setIcon("gui/icons/weapon-homing.png");
    load_type_rows[MW_Heavy]->setIcon("gui/icons/weapon-homing.png");
    load_type_rows[MW_Mine]->setIcon("gui/icons/weapon-mine.png");
    load_type_rows[MW_EMP]->setIcon("gui/icons/weapon-emp.png");
    load_type_rows[MW_Nuke]->setIcon("gui/icons/weapon-nuke.png");
    load_type_rows[MW_HVLI]->setIcon("gui/icons/weapon-hvli.png");

    GuiAutoLayout* tubesLayout = new GuiAutoLayout(this, "TUBES", GuiAutoLayout::LayoutVerticalBottomToTop);
    tubesLayout->setPosition(20, -20, ABottomLeft)->setSize(200, 50 * max_weapon_tubes);

    for (int n = max_weapon_tubes - 1; n >= 0; n--) {
        tube_rows[n] = new GuiProgressbar(tubesLayout, "TUBE_" + string(n), 0, 1.0, 0);
        tube_rows[n]->setSize(GuiElement::GuiSizeMax, 50);
    }

    if (gameGlobalInfo->use_beam_shield_frequencies || gameGlobalInfo->use_system_damage) {
        beams_display = new GuiLabel(this, "BEAMS_DISPLAY", "Beams", 30);
        beams_display->addBackground()->setPosition(-20, -100, ABottomRight)->setSize(350, 40);
    }

    shields_display = new GuiProgressbar(this, "SHIELDS", PlayerSpaceship::shield_calibration_time, 0.f, PlayerSpaceship::shield_calibration_time);
    shields_display->setPosition(-20, -60, ABottomRight)->setSize(350, 40);

    if (gameGlobalInfo->use_beam_shield_frequencies) {
        next_shields_frequency_display = new GuiKeyValueDisplay(this, "NEW_SHIELDS_FREQ", 0.55, "Calibrate Shields", "");
        next_shields_frequency_display->setPosition(-20, -20, ABottomRight)->setSize(350, 40);
    }

    GuiAutoLayout* targetInfoLayout = new GuiAutoLayout(this, "TARGET_INFO", GuiAutoLayout::LayoutVerticalTopToBottom);
    targetInfoLayout->setPosition(-20, 0, ACenterRight)->setSize(150, 40 * 5);

    GuiAutoLayout* targetModeLayout = new GuiAutoLayout(targetInfoLayout, "TARGET_MODE", GuiAutoLayout::LayoutVerticalColumns);
    targetModeLayout->setSize(GuiAutoLayout::GuiSizeMax, 40);

    target_mode_enemy_display = new GuiLabel(targetModeLayout, "TARGET_ENEMY", "Enemy", 30);
    target_mode_enemy_display->addBackground()->setMargins(10, 0)->setSize(GuiAutoLayout::GuiSizeMax, 40);
    target_mode_near_display = new GuiLabel(targetModeLayout, "TARGET_NEAR", "Near", 30);
    target_mode_near_display->addBackground()->setMargins(10, 0)->setSize(GuiAutoLayout::GuiSizeMax, 40);

    target_callsign = new GuiLabel(targetInfoLayout, "TARGET_CALLSIGN", "", 30);
    target_callsign->addBackground()->setSize(GuiAutoLayout::GuiSizeMax, 40);
    target_distance = new GuiKeyValueDisplay(targetInfoLayout, "TARGET_DISTANCE", 0.45, "Distance", "");
    target_distance->setSize(GuiAutoLayout::GuiSizeMax, 40);
    target_heading = new GuiKeyValueDisplay(targetInfoLayout, "TARGET_HEADING", 0.45, "Heading", "");
    target_heading->setSize(GuiAutoLayout::GuiSizeMax, 40);

    (new GuiCustomShipFunctions(this, weaponsOfficer, "", my_spaceship))->setPosition(-20, 120, ATopRight)->setSize(250, GuiElement::GuiSizeMax);
}

void WeaponsHeliosScreen::onDraw(sf::RenderTarget& window)
{
    if (my_spaceship)
    {
        energy_display->setValue(string(int(my_spaceship->energy_level)));
        front_shield_display->setValue(string(my_spaceship->getShieldPercentage(0)) + "%");
        rear_shield_display->setValue(string(my_spaceship->getShieldPercentage(1)) + "%");
        string beamsSystemName = my_spaceship->beam_system_target == SYS_None? "Hull" : getSystemName(ESystem(my_spaceship->beam_system_target));
        beams_display->setText(frequencyToString(my_spaceship->beam_frequency) + " Beams: " + beamsSystemName);
        string shieldsPrefix = "";
        if (gameGlobalInfo->use_beam_shield_frequencies){
            shieldsPrefix = frequencyToString(my_spaceship->shield_frequency) + " ";
            next_shields_frequency_display->setValue(frequencyToString(next_shields_frequency));
        }
        if (my_spaceship->shield_calibration_delay > 0.0) {
            shields_display->setValue(my_spaceship->shield_calibration_delay);
            shields_display->setText(shieldsPrefix + "Calibrating")->setColor(grey)->setTextColor(sf::Color::White);
        } else if (my_spaceship->shields_active){
            shields_display->setValue(0);
            shields_display->setText(shieldsPrefix + "Shields: ON")->setColor(sf::Color::White)->setTextColor(sf::Color::Black);
        } else {
            shields_display->setValue(0);
            shields_display->setText(shieldsPrefix + "Shields: OFF")->setColor(grey)->setTextColor(sf::Color::White);
        }
        P<SpaceObject> target = my_spaceship->getTarget();
        if (target){
            targets.set(target);
            target_callsign->setText(target->getCallSign());
            target_distance->setValue(string(sf::length(target->getPosition()-my_spaceship->getPosition()) / 1000));
            float angle = std::fmod(450 + vector2ToAngle(target->getPosition()-my_spaceship->getPosition()), 360); // add 90 deg and normalize
            target_heading->setValue(string(angle));
        } else {
            targets.clear();
            target_callsign->setText("-");
            target_distance->setValue("-");
            target_heading->setValue("-");
        }

        for (int n = 0; n < MW_Count; n++)
        {
            sf::Color color = n == load_type? sf::Color::Yellow :  sf::Color::White ;
            if (my_spaceship->weapon_storage[n] == 0){
                color *= grey;
            }
            load_type_rows[n]->setValue(" [" + string(my_spaceship->weapon_storage[n]) + "/" + string(my_spaceship->weapon_storage_max[n]) + "]");
            load_type_rows[n]->setVisible(my_spaceship->weapon_storage_max[n] > 0);
            load_type_rows[n]->setColor(color)->setContentColor(color);
        }

        for (int n = 0; n < my_spaceship->weapon_tube_count; n++) {
            WeaponTube& tube = my_spaceship->weapon_tube[n];
            tube_rows[n]->show();
            
            if(tube.isEmpty()) {
                tube_rows[n]->setText(tube.getTubeName() + ": Empty");
                tube_rows[n]->setValue(0.f);
                tube_rows[n]->setTextColor(tube.canLoad(load_type)? sf::Color::Yellow : sf::Color::White);
            } else if(tube.isLoaded()) {
                tube_rows[n]->setText(tube.getTubeName() + ": " + getMissileWeaponName(tube.getLoadType()));
                tube_rows[n]->setValue(1.0f);
                tube_rows[n]->setColor(sf::Color::Green)->setTextColor(sf::Color::Black);
            } else if(tube.isLoading()) {
                tube_rows[n]->setText(tube.getTubeName() + ": " + getMissileWeaponName(tube.getLoadType()));
                tube_rows[n]->setValue(tube.getLoadProgress());
                tube_rows[n]->setColor(sf::Color::Yellow)->setTextColor(grey);
            } else if(tube.isUnloading()) {
                tube_rows[n]->setText(getMissileWeaponName(tube.getLoadType()));
                tube_rows[n]->setValue(1.f - tube.getLoadProgress());
                tube_rows[n]->setColor(grey)->setTextColor(sf::Color::White);
            } else if(tube.isFiring()) {
                tube_rows[n]->setText("Firing");
                tube_rows[n]->setValue(0.f);
                tube_rows[n]->setColor(grey)->setTextColor(sf::Color::White);
            }
        }
        for(int n=my_spaceship->weapon_tube_count; n<max_weapon_tubes; n++)
            tube_rows[n]->hide();
        missile_aim->setValue(missile_target_angle - my_spaceship->getRotation())->setVisible(manual_aim);

        sf::Color color = target_mode_near? sf::Color::Yellow : grey;
        target_mode_near_display->setColor(color)->setTextColor(color);
        color = target_mode_enemy? sf::Color::Yellow : grey;
        target_mode_enemy_display->setColor(color)->setTextColor(color);
    }
    GuiOverlay::onDraw(window);
}

static bool compareSpaceObjects(P<SpaceObject> o1, P<SpaceObject> o2) { 
    return (o1->getPosition() - my_spaceship->getPosition()) < (o2->getPosition() - my_spaceship->getPosition());
} 

void WeaponsHeliosScreen::iterateTagrets(bool forward, bool enemiesOnly, bool nearOnly){
    float range = nearOnly? radar->getDistance() : my_spaceship->getRadarRange();
    PVector<SpaceObject> potentialTargetsUnfiltered = GuiRadarView::getVisibleObjects(my_spaceship->getPosition(), my_spaceship->getFactionId(), GuiRadarView::RadarRangeAndLineOfSight, range);
    PVector<SpaceObject> potentialTargets;
    foreach(SpaceObject, obj, potentialTargetsUnfiltered) {
        if(obj != my_spaceship &&
            obj->canBeTargetedBy(my_spaceship) &&
            (!enemiesOnly || my_spaceship->isKnownEnemy(obj))) {
            potentialTargets.push_back(obj);
        }
    }
    std::sort(potentialTargets.begin(), potentialTargets.end(), compareSpaceObjects); 
    targets.next(potentialTargets, forward);
    my_spaceship->commandSetTarget(targets.get());
}

void WeaponsHeliosScreen::onHotkey(const HotkeyResult& key)
{
    if (key.category == "WEAPONS" && my_spaceship) {
        if (key.hotkey == "BEAM_FREQUENCY_INCREASE") {
            my_spaceship->commandSetBeamFrequency((my_spaceship->beam_frequency + 1) % SpaceShip::max_frequency);
        } else if (key.hotkey == "BEAM_FREQUENCY_DECREASE") {
            my_spaceship->commandSetBeamFrequency((SpaceShip::max_frequency + my_spaceship->beam_frequency - 1) % SpaceShip::max_frequency);
        } else if (gameGlobalInfo->use_system_damage){
            // const int targetSysCount = SYS_COUNT +1;
            if (key.hotkey == "BEAM_SUBSYSTEM_TARGET_NEXT") {
                // ESystem system = ESystem(((my_spaceship->beam_system_target +2) % targetSysCount) -1);
                int system = my_spaceship->beam_system_target + 1;
                if (system == SYS_JumpDrive) {
                    system = ESystem(SYS_JumpDrive + 1);
                } else if (system >= SYS_COUNT) {
                    system = SYS_None;
                }
                my_spaceship->commandSetBeamSystemTarget(ESystem(system));
            } else if (key.hotkey == "BEAM_SUBSYSTEM_TARGET_PREV") {
                // ESystem system = ESystem(((targetSysCount + my_spaceship->beam_system_target) % targetSysCount) -1);
                int system = my_spaceship->beam_system_target - 1;
                if (system == SYS_JumpDrive) {
                    system = ESystem(SYS_JumpDrive - 1);
                } else if (system < SYS_None) {
                    system = SYS_COUNT -1;
                }
                my_spaceship->commandSetBeamSystemTarget(ESystem(system));
            } 
        }
        if (my_spaceship->ship_template->has_manual_aim) {
            if (key.hotkey == "AIM_MISSILE_LEFT") {
                missile_target_angle = missile_target_angle - 5.0f;
            } else if (key.hotkey == "AIM_MISSILE_RIGHT") {
                missile_target_angle = missile_target_angle + 5.0f;
            } else if (key.hotkey == "ENABLE_AIM_LOCK") {
                missile_target_angle = 0;
                manual_aim = false;
            } else if (key.hotkey == "DISABLE_AIM_LOCK") {
                manual_aim = true;
            }
        }
        for(int n=0; n<MW_Count; n++){
            if (key.hotkey == "SELECT_MISSILE_TYPE_" + getMissileWeaponName(EMissileWeapons(n)).upper()) {
                load_type = EMissileWeapons(n);
            }
        }
        for(int n=0; n<my_spaceship->weapon_tube_count; n++)
        {
            WeaponTube& tube = my_spaceship->weapon_tube[n];
            if (key.hotkey == "LOAD_TUBE_" + string(n+1) && tube.isEmpty())
                my_spaceship->commandLoadTube(n, load_type);
            if (key.hotkey == "UNLOAD_TUBE_" + string(n+1) && tube.isLoaded())
                my_spaceship->commandUnloadTube(n);
            if (key.hotkey == "FIRE_TUBE_" + string(n+1) && tube.isLoaded())
            {
                float target_angle = missile_target_angle;
                if (!manual_aim)
                {
                    target_angle = my_spaceship->weapon_tube[n].calculateFiringSolution(my_spaceship->getTarget());
                    if (target_angle == std::numeric_limits<float>::infinity())
                        target_angle = my_spaceship->getRotation() + my_spaceship->weapon_tube[n].getDirection();
                }
                my_spaceship->commandFireTube(n, target_angle);
            }
        }
    } else if (key.category == "ENGINEERING" && my_spaceship){
        if (key.hotkey == "SHIELD_CAL_INC") {
            next_shields_frequency = (next_shields_frequency + 1) % SpaceShip::max_frequency;
        } else if (key.hotkey == "SHIELD_CAL_DEC") {
            next_shields_frequency = (SpaceShip::max_frequency + next_shields_frequency - 1) % SpaceShip::max_frequency;
        } else if (key.hotkey == "SHIELD_CAL_START") {
            my_spaceship->commandSetShieldFrequency(next_shields_frequency);
        } 
    } else if (key.category == "TARGET" && my_spaceship){
        if (key.hotkey == "NEXT_TARGET") {
            iterateTagrets(true, target_mode_enemy, target_mode_near);
        } else if (key.hotkey == "PREV_TARGET") {
            iterateTagrets(false, target_mode_enemy, target_mode_near);
        } else if (key.hotkey == "TARGET_NEAR_TOGGLE") {
            target_mode_near = !target_mode_near;
        } else if (key.hotkey == "TARGET_ENEMY_TOGGLE") {
            target_mode_enemy = !target_mode_enemy;
        }
    }
}

bool WeaponsHeliosScreen::onJoystickAxis(const AxisAction& axisAction){
    if (axisAction.category == "WEAPONS" && my_spaceship){
        if (axisAction.action == "AIM_MISSILE"){
            missile_target_angle = axisAction.value * 180;
            return true;
        } 
    }
    return false;
}

float WeaponsHeliosScreen::getMissileTargetAngle()
{
    return missile_target_angle;
}

bool WeaponsHeliosScreen::getManualAim()
{
    return manual_aim;
}
