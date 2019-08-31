#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "weaponsHeliosScreen.h"

#include "screenComponents/beamFrequencySelector.h"
#include "screenComponents/beamTargetSelector.h"
#include "screenComponents/powerDamageIndicator.h"
#include "screenComponents/shieldFreqencySelect.h"
#include "screenComponents/shieldsEnableButton.h"
#include "screenComponents/alertOverlay.h"
#include "screenComponents/customShipFunctions.h"
#include "screenComponents/systemStatus.h"

#include "gui/gui2_rotationdial.h"
#include "gui/gui2_label.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_progressbar.h"

sf::Color grey(128, 128, 128);
WeaponsHeliosScreen::WeaponsHeliosScreen(GuiContainer* owner)
: GuiOverlay(owner, "WEAPONS_SCREEN", colorConfig.background), load_type(MW_None), manual_aim(false), missile_target_angle(0)
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
    load_type_rows[MW_Homing]->setIcon("gui/icons/weapon-homing.png");
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
        beams_display = new GuiLabel(this, "BEAMS_DISPLAY", "Beams", 20);
        beams_display->setBold()->addBackground()->setPosition(-20, 180, ATopRight)->setSize(350, 40);
    }

    if (gameGlobalInfo->use_beam_shield_frequencies) {
        //The shield frequency selection includes a shield enable button.
        // TODO: 
        // shields_display = new GuiProgressbar()
        // new_shields_frequency_display = GuiKeyValueDisplay()

        (new GuiShieldFrequencySelect(this, "SHIELD_FREQ"))->setPosition(-20, -20, ABottomRight)->setSize(280, 100);
    }else{
        (new GuiShieldsEnableButton(this, "SHIELDS_ENABLE", my_spaceship))->setPosition(-20, -20, ABottomRight)->setSize(280, 50);
    }

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
        targets.set(my_spaceship->getTarget());
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
        missile_aim->setValue(missile_target_angle - my_spaceship->getRotation());
    }
    GuiOverlay::onDraw(window);
}

//         targets.setToClosestTo(position, 250, TargetsContainer::Targetable);
void WeaponsHeliosScreen::onHotkey(const HotkeyResult& key)
{
    if (key.category == "WEAPONS" && my_spaceship) {
        if (key.hotkey == "NEXT_ENEMY_TARGET") {
            bool current_found = false;
            foreach(SpaceObject, obj, space_object_list)
            {
                if (obj == targets.get())
                {
                    current_found = true;
                    continue;
                }
                if (current_found && sf::length(obj->getPosition() - my_spaceship->getPosition()) < 5000 && my_spaceship->isEnemy(obj) && my_spaceship->getScannedStateFor(obj) >= SS_FriendOrFoeIdentified && obj->canBeTargetedBy(my_spaceship))
                {
                    targets.set(obj);
                    my_spaceship->commandSetTarget(targets.get());
                    return;
                }
            }
            foreach(SpaceObject, obj, space_object_list)
            {
                if (obj == targets.get())
                {
                    continue;
                }
                if (my_spaceship->isEnemy(obj) && sf::length(obj->getPosition() - my_spaceship->getPosition()) < 5000 && my_spaceship->getScannedStateFor(obj) >= SS_FriendOrFoeIdentified && obj->canBeTargetedBy(my_spaceship))
                {
                    targets.set(obj);
                    my_spaceship->commandSetTarget(targets.get());
                    return;
                }
            }
        } else if (key.hotkey == "NEXT_TARGET") {
            bool current_found = false;
            foreach(SpaceObject, obj, space_object_list)
            {
                if (obj == targets.get())
                {
                    current_found = true;
                    continue;
                }
                if (obj == my_spaceship)
                    continue;
                if (current_found && sf::length(obj->getPosition() - my_spaceship->getPosition()) < 5000 && obj->canBeTargetedBy(my_spaceship))
                {
                    targets.set(obj);
                    my_spaceship->commandSetTarget(targets.get());
                    return;
                }
            }
            foreach(SpaceObject, obj, space_object_list)
            {
                if (obj == targets.get() || obj == my_spaceship)
                    continue;
                if (sf::length(obj->getPosition() - my_spaceship->getPosition()) < 5000 && obj->canBeTargetedBy(my_spaceship))
                {
                    targets.set(obj);
                    my_spaceship->commandSetTarget(targets.get());
                    return;
                }
            }
        } else if (key.hotkey == "SELECT_MISSILE_TYPE_HOMING") {
            load_type = MW_Homing;
        } else if (key.hotkey == "SELECT_MISSILE_TYPE_NUKE") {
            load_type = MW_Nuke;
        } else if (key.hotkey == "SELECT_MISSILE_TYPE_MINE") {
            load_type = MW_Mine;
        } else if (key.hotkey == "SELECT_MISSILE_TYPE_EMP") {
            load_type = MW_EMP;
        } else if (key.hotkey == "SELECT_MISSILE_TYPE_HVLI") {
            load_type = MW_HVLI;
        } else if (key.hotkey == "BEAM_FREQUENCY_INCREASE") {
            my_spaceship->commandSetBeamFrequency((my_spaceship->beam_frequency + 1) % SpaceShip::max_frequency);
        } else if (key.hotkey == "BEAM_FREQUENCY_DECREASE") {
            my_spaceship->commandSetBeamFrequency((SpaceShip::max_frequency + my_spaceship->beam_frequency - 1) % SpaceShip::max_frequency);
        }
        if (gameGlobalInfo->use_system_damage){
            // const int targetSysCount = SYS_COUNT +1;
            if (key.hotkey == "BEAM_SUBSYSTEM_TARGET_NEXT") {
                // ESystem system = ESystem(((my_spaceship->beam_system_target +2) % targetSysCount) -1);
                int system = my_spaceship->beam_system_target + 1;
                if (system == SYS_JumpDrive && gameGlobalInfo->player_warp_jump_drive_setting == PWJ_WarpDrive) {
                    system = ESystem(system + 1);
                } else if (system >= SYS_COUNT) {
                    system = -1;
                }
                my_spaceship->commandSetBeamSystemTarget(ESystem(system));
            } else if (key.hotkey == "BEAM_SUBSYSTEM_TARGET_PREV") {
                // ESystem system = ESystem(((targetSysCount + my_spaceship->beam_system_target) % targetSysCount) -1);
                int system = my_spaceship->beam_system_target - 1;
                if (system == SYS_JumpDrive && gameGlobalInfo->player_warp_jump_drive_setting == PWJ_WarpDrive) {
                    system = ESystem(system - 1);
                } else if (system <= -2) {
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
                manual_aim = false;
            } else if (key.hotkey == "DISABLE_AIM_LOCK") {
                manual_aim = true;
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
