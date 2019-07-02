#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "weaponsScreen.h"

#include "screenComponents/missileTubeControls.h"
#include "screenComponents/aimLock.h"
#include "screenComponents/beamFrequencySelector.h"
#include "screenComponents/beamTargetSelector.h"
#include "screenComponents/powerDamageIndicator.h"
#include "screenComponents/shieldFreqencySelect.h"
#include "screenComponents/shieldsEnableButton.h"
#include "screenComponents/alertOverlay.h"
#include "screenComponents/customShipFunctions.h"

#include "gui/gui2_rotationdial.h"
#include "gui/gui2_label.h"
#include "gui/gui2_keyvaluedisplay.h"

WeaponsScreen::WeaponsScreen(GuiContainer* owner)
: GuiOverlay(owner, "WEAPONS_SCREEN", colorConfig.background)
{
    // Render the radar shadow and background decorations.
    background_gradient = new GuiOverlay(this, "BACKGROUND_GRADIENT", sf::Color::White);
    background_gradient->setTextureCenter("gui/BackgroundGradient");

    background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));

    radar = new GuiRadarView(this, "HELMS_RADAR", my_spaceship->tactical_radar_range, &targets, my_spaceship);
    radar->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMatchHeight, 800);
    radar->setRangeIndicatorStepSize(1000.0)->shortRange()->enableCallsigns()->enableHeadingIndicators()->setStyle(GuiRadarView::Circular);
    radar->setCallbacks(
        [this](sf::Vector2f position) {
            targets.setToClosestTo(position, 250, TargetsContainer::Targetable, my_spaceship);
            if (my_spaceship && targets.get())
                my_spaceship->commandSetTarget(targets.get());
            else if (my_spaceship)
                my_spaceship->commandSetTarget(NULL);
        }, nullptr, nullptr
    );
    missile_aim = new GuiRotationDial(this, "MISSILE_AIM", -90, 360 - 90, 0, [this](float value){
        tube_controls->setMissileTargetAngle(value);
    });
    missile_aim->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMatchHeight, 850);

    tube_controls = new GuiMissileTubeControls(this, "MISSILE_TUBES", my_spaceship);
    tube_controls->setPosition(20, -20, ABottomLeft);
    radar->enableTargetProjections(tube_controls);

    lock_aim = new AimLockButton(this, "LOCK_AIM", tube_controls, missile_aim, my_spaceship);
    lock_aim->setPosition(250, 20, ATopCenter)->setSize(200, 50);

    lock_fire = new GuiToggleButton(this, "TOOGLE_FIRE", "Feu", [this](bool value){
        if (my_spaceship)
            my_spaceship->lock_fire = value;
    });
    lock_fire->setIcon("gui/icons/lock");
    lock_fire->setPosition(-250, 20, ATopCenter)->setSize(200, 50);
    if (my_spaceship)
        lock_fire->setValue(true);

    if (gameGlobalInfo->use_beam_shield_frequencies || gameGlobalInfo->use_system_damage)
    {
        if (my_spaceship && my_spaceship->beam_weapons_count > 0)
        {
            GuiElement* beam_info_box = new GuiElement(this, "BEAM_INFO_BOX");
            beam_info_box->setPosition(-20, -120, ABottomRight)->setSize(280, 150);
            (new GuiLabel(beam_info_box, "BEAM_INFO_LABEL", "Info Laser", 30))->addBackground()->setSize(GuiElement::GuiSizeMax, 50);
            (new GuiPowerDamageIndicator(beam_info_box, "", SYS_BeamWeapons, ACenterLeft, my_spaceship))->setSize(GuiElement::GuiSizeMax, 50);
            (new GuiBeamFrequencySelector(beam_info_box, "BEAM_FREQUENCY_SELECTOR"))->setPosition(0, 0, ABottomRight)->setSize(GuiElement::GuiSizeMax, 50);
            (new GuiBeamTargetSelector(beam_info_box, "BEAM_TARGET_SELECTOR", my_spaceship))->setPosition(0, -50, ABottomRight)->setSize(GuiElement::GuiSizeMax, 50);

            if (!gameGlobalInfo->use_beam_shield_frequencies)
            {   //If we do have system damage, but no shield frequencies, we can partially overlap this with the shield button.
                //So move the beam configuration a bit down.
                beam_info_box->setPosition(-20, -50, ABottomRight);
            }
        }
    }

    energy_display = new GuiKeyValueDisplay(this, "ENERGY_DISPLAY", 0.45, "Energie", "");
    energy_display->setIcon("gui/icons/energy")->setTextSize(20)->setPosition(20, 100, ATopLeft)->setSize(240, 40);
    target_display = new GuiKeyValueDisplay(this, "TARGET_DISPLAY", 0.45, "Cible", "");
    target_display->setIcon("gui/icons/lock")->setTextSize(20)->setPosition(20, 180, ATopLeft)->setSize(240, 40);
    shields_display = new GuiKeyValueDisplay(this, "SHIELDS_DISPLAY", 0.45, "Boucliers", "");
    shields_display->setIcon("gui/icons/shields")->setTextSize(20)->setPosition(20, 140, ATopLeft)->setSize(240, 40);

    if (my_spaceship && my_spaceship->getShieldCount() > 0)
    {
         if (gameGlobalInfo->use_beam_shield_frequencies)
        {
            //The shield frequency selection includes a shield enable button.
            (new GuiShieldFrequencySelect(this, "SHIELD_FREQ"))->setPosition(-20, -20, ABottomRight)->setSize(280, 100);
        }else{
            (new GuiShieldsEnableButton(this, "SHIELDS_ENABLE", my_spaceship))->setPosition(-20, -20, ABottomRight)->setSize(280, 50);
        }
    }

    (new GuiCustomShipFunctions(this, weaponsOfficer, "", my_spaceship))->setPosition(-20, 120, ATopRight)->setSize(250, GuiElement::GuiSizeMax);
}

void WeaponsScreen::onDraw(sf::RenderTarget& window)
{
    if (my_spaceship)
    {
        energy_display->setValue(string(int(my_spaceship->energy_level/my_spaceship->max_energy_level * 100)) + "%");
        if (my_spaceship->energy_level < 100)
            energy_display->setColor(sf::Color::Red);
        else
            energy_display->setColor(sf::Color::White);

        lock_aim->setVisible(my_spaceship->getWeaponTubeCount() > 0);

        targets.set(my_spaceship->getTarget());
        if (targets.get())
            target_display->setValue(string(my_spaceship->getTarget()->getCallSign()));
        else
            target_display->setValue("-");

        if (my_spaceship->getShieldCount() > 0)
        {
            shields_display->show();
            string shields_info = "";
            for(int n=0; n<my_spaceship->getShieldCount(); n++)
                shields_info += string(my_spaceship->getShieldPercentage(n)) + "% ";
            shields_display->setValue(shields_info);
        }
        else
            shields_display->hide();

        missile_aim->setVisible(tube_controls->getManualAim());
    }
    GuiOverlay::onDraw(window);
}

void WeaponsScreen::onHotkey(const HotkeyResult& key)
{
    if (key.category == "WEAPONS" && my_spaceship)
    {
        if (key.hotkey == "NEXT_ENEMY_TARGET")
        {
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
        }
        if (key.hotkey == "NEXT_TARGET")
        {
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
        }
        if (key.hotkey == "AIM_MISSILE_LEFT")
        {
            missile_aim->setValue(missile_aim->getValue() - 5.0f);
            tube_controls->setMissileTargetAngle(missile_aim->getValue());
        }
        if (key.hotkey == "AIM_MISSILE_RIGHT")
        {
            missile_aim->setValue(missile_aim->getValue() + 5.0f);
            tube_controls->setMissileTargetAngle(missile_aim->getValue());
        }
    }
}
void WeaponsScreen::onJoystickAxis(AxisAction& axisAction){
    if (axisAction.category == "WEAPONS" && my_spaceship){
        if (axisAction.action == "AIM_MISSILE"){
            missile_aim->setValue(axisAction.value * 180);
            tube_controls->setMissileTargetAngle(missile_aim->getValue());
        } 
    }
}