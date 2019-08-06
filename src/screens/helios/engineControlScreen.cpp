#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "engineControlScreen.h"

#include "screenComponents/shipInternalView.h"
#include "screenComponents/selfDestructButton.h"
#include "screenComponents/alertOverlay.h"
#include "screenComponents/customShipFunctions.h"
#include "screenComponents/shipsLogControl.h"

#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_autolayout.h"
#include "gui/gui2_togglebutton.h"
#include "gui/gui2_slider.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_arrow.h"
#include "gui/gui2_image.h"
#include "gui/gui2_panel.h"

EngineControlScreen::EngineControlScreen(GuiContainer* owner, ECrewPosition crewPosition)
: GuiOverlay(owner, "ENGINEERING_SCREEN", colorConfig.background), crew_position(crewPosition)
{
    new_warp_frequency = my_spaceship->getWarpFrequency();
    // Render the background decorations.
    background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));

    GuiAutoLayout* global_info_layout = new GuiAutoLayout(this, "GLOBAL_INFO", GuiAutoLayout::LayoutVerticalTopToBottom);
    global_info_layout->setPosition(40, 40, ATopLeft)->setSize(240, 200);

    energy_display = new GuiKeyValueDisplay(global_info_layout, "ENERGY_DISPLAY", 0.45, "Energy", "");
    energy_display->setIcon("gui/icons/energy")->setTextSize(20)->setSize(GuiElement::GuiSizeMax, 40);
    hull_display = new GuiKeyValueDisplay(global_info_layout, "HULL_DISPLAY", 0.45, "Hull", "");
    hull_display->setIcon("gui/icons/hull")->setTextSize(20)->setSize(GuiElement::GuiSizeMax, 40);
    front_shield_display = new GuiKeyValueDisplay(global_info_layout, "SHIELDS_DISPLAY", 0.45, "Front", "");
    front_shield_display->setIcon("gui/icons/shields-fore")->setTextSize(20)->setSize(GuiElement::GuiSizeMax, 40);
    rear_shield_display = new GuiKeyValueDisplay(global_info_layout, "SHIELDS_DISPLAY", 0.45, "Rear", "");
    rear_shield_display->setIcon("gui/icons/shields-aft")->setTextSize(20)->setSize(GuiElement::GuiSizeMax, 40);
    engineering_control_display = new GuiKeyValueDisplay(global_info_layout, "ENGINEERING_CONTROL_DISPLAY", 0.45, "Control", "");
    engineering_control_display->setIcon("gui/icons/station-engineering")->setTextSize(20)->setSize(GuiElement::GuiSizeMax, 40);
    warp_frequency_display = new GuiKeyValueDisplay(global_info_layout, "WARP_FREQUENCY_DISPLAY", 0.45, "Frequency", "");
    warp_frequency_display->setIcon("gui/icons/system_warpdrive")->setTextSize(20)->setSize(GuiElement::GuiSizeMax, 40);
    new_warp_frequency_display = new GuiKeyValueDisplay(global_info_layout, "new_warp_frequency_DISPLAY", 0.45, "Next Frequency", "");
    new_warp_frequency_display->setTextSize(20)->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiCustomShipFunctions(this, crew_position, "CUSTOM_FUNCTIONS", my_spaceship))
        ->setPosition(340, 40, ATopLeft)->setSize(250, 200);

    GuiAutoLayout* system_row_layouts = new GuiAutoLayout(this, "SYSTEM_ROWS", GuiAutoLayout::LayoutVerticalBottomToTop);
    system_row_layouts->setPosition(40, -40, ABottomLeft)->setSize(800, SYS_COUNT * 50);
    for(int n=0; n<SYS_COUNT; n++)
    {
        string id = "SYSTEM_ROW_" + getSystemName(ESystem(n));
        SystemRow info;
        info.layout = new GuiAutoLayout(system_row_layouts, id, GuiAutoLayout::LayoutHorizontalLeftToRight);
        info.layout->setSize(GuiElement::GuiSizeMax, 50);

        info.name_label = new GuiLabel(info.layout, id + "_NAME_LABEL", getSystemName(ESystem(n)), 20);
        info.name_label->addBackground()->setSize(200, GuiElement::GuiSizeMax);

        info.repair_label = new GuiLabel(info.layout, id + "_REPAIR_LABEL", "", 20);
        info.repair_label->addBackground()->setSize(70, GuiElement::GuiSizeMax);
        info.repair_icon = new GuiImage(info.repair_label, "", "gui/icons/station-engineering");
        info.repair_icon->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

        info.damage_bar = new GuiProgressbar(info.layout, id + "_DAMAGE", 0.0, 1.0, 0.0);
        info.damage_bar->setSize(100, GuiElement::GuiSizeMax);
        info.damage_label = new GuiLabel(info.damage_bar, id + "_DAMAGE_LABEL", "...", 20);
        info.damage_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
        info.heat_bar = new GuiProgressbar(info.layout, id + "_HEAT", 0.0, 1.0, 0.0);
        info.heat_bar->setSize(100, GuiElement::GuiSizeMax);
        info.heat_arrow = new GuiArrow(info.heat_bar, id + "_HEAT_ARROW", 0);
        info.heat_arrow->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
        info.heat_icon = new GuiImage(info.heat_bar, "", "gui/icons/status_overheat");
        info.heat_icon->setColor(colorConfig.overlay_overheating)->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMatchHeight, GuiElement::GuiSizeMax);
        info.power_bar = new GuiProgressbar(info.layout, id + "_POWER", 0.0, 3.0, 0.0);
        info.power_bar->setColor(sf::Color(192, 192, 32, 128))->setSize(100, GuiElement::GuiSizeMax);
        info.coolant_bar = new GuiProgressbar(info.layout, id + "_COOLANT", 0.0, 10.0, 0.0);
        info.coolant_bar->setColor(sf::Color(32, 128, 128, 128))->setSize(100, GuiElement::GuiSizeMax);
        info.effectiveness_bar = new GuiProgressbar(info.layout, id + "_EFFECTIVENESS", 0.0, 3.0, 0.0);
        info.effectiveness_bar->setSize(100, GuiElement::GuiSizeMax);
        
        info.layout->moveToBack();
        system_rows.push_back(info);
    }

    GuiAutoLayout* headers_layout = new GuiAutoLayout(system_row_layouts, "", GuiAutoLayout::LayoutHorizontalLeftToRight);
    headers_layout->setSize(GuiElement::GuiSizeMax, 50);
    (new GuiLabel(headers_layout, "NAME_TITLE", "Name", 20))->setSize(200, GuiElement::GuiSizeMax);
    (new GuiLabel(headers_layout, "REPAIR_TITLE", "Repairing", 20))->setSize(70, GuiElement::GuiSizeMax);
    (new GuiLabel(headers_layout, "HEALTH_TITLE", "Health", 20))->setSize(100, GuiElement::GuiSizeMax);
    (new GuiLabel(headers_layout, "HEAT_TITLE", "Heat", 20))->setSize(100, GuiElement::GuiSizeMax);
    (new GuiLabel(headers_layout, "POWER_TITLE", "Power", 20))->setSize(100, GuiElement::GuiSizeMax);
    (new GuiLabel(headers_layout, "COOLANT_TITLE", "Coolant", 20))->setSize(100, GuiElement::GuiSizeMax);
    (new GuiLabel(headers_layout, "EFFECTIVENESS_TITLE", "Effectiveness", 20))->setSize(100, GuiElement::GuiSizeMax);

    system_effects_container = new GuiAutoLayout(this, "SYSTEM_EFFECTS", GuiAutoLayout::LayoutVerticalTopToBottom);
    system_effects_container->setPosition(-50, 50, ATopRight)->setSize(270, GuiElement::GuiSizeMax);

    energy_deriv.last_measurement = 0.0;
    energy_deriv.average = 0.0;
    last_measurement_time = 0.0;
}

void EngineControlScreen::RollingDeriviateAvg::apply(float deltaTime, float currentMeasurement){
    float delta_e = currentMeasurement - last_measurement;
    float delta_e_per_second = delta_e / deltaTime;
    average = average * 0.99 + delta_e_per_second * 0.01;
    last_measurement = currentMeasurement;
}

void EngineControlScreen::onDraw(sf::RenderTarget& window)
{
    if (my_spaceship)
    {
        //Update the energy usage.
        if (last_measurement_time == 0.0) {
            energy_deriv.last_measurement = my_spaceship->energy_level;
        } else if (last_measurement_time != engine->getElapsedTime()) {
            float delta_t = engine->getElapsedTime() - last_measurement_time;
            energy_deriv.apply(delta_t, my_spaceship->energy_level);
            for(int n=0; n<SYS_COUNT; n++) {
                system_rows[n].heat_deriv.apply(delta_t, my_spaceship->systems[n].heat_level);
            }
        }
        last_measurement_time = engine->getElapsedTime();

        energy_display->setValue(string(int(my_spaceship->energy_level)) + " (" + string(int(energy_deriv.average * 60.0f)) + "/m)");
        if (my_spaceship->energy_level < 100)
            energy_display->setColor(sf::Color::Red);
        else
            energy_display->setColor(sf::Color::White);
        hull_display->setValue(string(int(100 * my_spaceship->hull_strength / my_spaceship->hull_max)) + "%");
        if (my_spaceship->hull_strength < my_spaceship->hull_max / 4.0f)
            hull_display->setColor(sf::Color::Red);
        else
            hull_display->setColor(sf::Color::White);
        front_shield_display->setValue(string(my_spaceship->getShieldPercentage(0)) + "%");
        rear_shield_display->setValue(string(my_spaceship->getShieldPercentage(1)) + "%");
        engineering_control_display->setValue(my_spaceship->engineering_control_from_bridge? "bridge" : "ECR");
        warp_frequency_display->setValue(frequencyToString(my_spaceship->shield_frequency));
        new_warp_frequency_display->setValue(frequencyToString(new_warp_frequency));

        system_effects_index = 0;
        for(int n=0; n<SYS_COUNT; n++)
        {
            ESystem selected_system = ESystem(n);
            ShipSystem& system = my_spaceship->systems[n];
            SystemRow info = system_rows[n];
            info.layout->setVisible(my_spaceship->hasSystem(selected_system));

            info.repair_icon->setVisible(my_spaceship->auto_repairing_system == selected_system);
            
            float health = system.health;
            if (health < 0.0)
                info.damage_bar->setValue(-health)->setColor(sf::Color(128, 32, 32, 192));
            else
                info.damage_bar->setValue(health)->setColor(sf::Color(64, 128 * health, 64 * health, 192));
            info.damage_label->setText(string(int(health * 100)) + "%");

            float heat = system.heat_level;
            info.heat_bar->setValue(heat)->setColor(sf::Color(128, 32 + 96 * (1.0 - heat), 32, 192));
            float heating_diff = info.heat_deriv.average;
            if (heating_diff > 0)
                info.heat_arrow->setAngle(90);
            else
                info.heat_arrow->setAngle(-90);
            info.heat_arrow->setVisible(heat > 0);
            info.heat_arrow->setColor(sf::Color(255, 255, 255, std::min(255, int(255 * fabs(heating_diff / PlayerSpaceship::system_heatup_per_second)))));
            if (heat > 0.9 && fmod(engine->getElapsedTime(), 0.5) < 0.25)
                info.heat_icon->show();
            else
                info.heat_icon->hide();

            info.power_bar->setValue(system.power_level);
            info.coolant_bar->setValue(system.coolant_level);
            
            float effectiveness = my_spaceship->getSystemEffectiveness(selected_system);
            info.effectiveness_bar->setValue(effectiveness);

            switch(selected_system)
            {
            case SYS_Reactor:
                if (effectiveness > 1.0f)
                    effectiveness = (1.0f + effectiveness) / 2.0f;
                addSystemEffect("Energy production", string(effectiveness * -PlayerSpaceship::system_power_user_factor[SYS_Reactor] * 60.0, 1) + "/m");
                break;
            case SYS_BeamWeapons:
                addSystemEffect("Beams Firing rate", string(int(effectiveness * 100)) + "%");
                // If the ship has a turret, also note that the rotation rate
                // is affected.
                for(int n = 0; n < max_beam_weapons; n++)
                {
                    if (my_spaceship->beam_weapons[n].getTurretArc() > 0)
                    {
                        addSystemEffect("Turret rotation rate", string(int(effectiveness * 100)) + "%");
                        break;
                    }
                }
                break;
            case SYS_MissileSystem:
                addSystemEffect("Missile Reload rate", string(int(effectiveness * 100)) + "%");
                break;
            case SYS_Maneuver:
                addSystemEffect("Turning speed", string(int(effectiveness * 100)) + "%");
                if (my_spaceship->combat_maneuver_boost_speed > 0.0 || my_spaceship->combat_maneuver_strafe_speed)
                    addSystemEffect("Combat recharge rate", string(int(((my_spaceship->getSystemEffectiveness(SYS_Maneuver) + my_spaceship->getSystemEffectiveness(SYS_Impulse)) / 2.0) * 100)) + "%");
                break;
            case SYS_Impulse:
                addSystemEffect("Impulse speed", string(int(effectiveness * 100)) + "%");
                break;
            case SYS_Warp:
                addSystemEffect("Warp drive speed", string(int(my_spaceship->warp_boost_factor * effectiveness * 100)) + "%");
                break;
            case SYS_JumpDrive:
                addSystemEffect("Jump drive recharge rate", string(int(my_spaceship->getJumpDriveRechargeRate() * 100)) + "%");
                addSystemEffect("Jump drive jump speed", string(int(effectiveness * 100)) + "%");
                break;
            case SYS_FrontShield:
                if (gameGlobalInfo->use_beam_shield_frequencies)
                    addSystemEffect("Shield Calibration speed", string(int((my_spaceship->getSystemEffectiveness(SYS_FrontShield) + my_spaceship->getSystemEffectiveness(SYS_RearShield)) / 2.0 * 100)) + "%");
                addSystemEffect("Front Charge rate", string(int(effectiveness * 100)) + "%");
                {
                    DamageInfo di;
                    di.type = DT_Kinetic;
                    float damage_negate = 1.0f - my_spaceship->getShieldDamageFactor(di, 0);
                    if (damage_negate < 0.0)
                        addSystemEffect("Front Extra damage", string(int(-damage_negate * 100)) + "%");
                    else
                        addSystemEffect("Front Damage negate", string(int(damage_negate * 100)) + "%");
                }
                break;
            case SYS_RearShield:
                addSystemEffect("Rear Charge rate", string(int(effectiveness * 100)) + "%");
                {
                    DamageInfo di;
                    di.type = DT_Kinetic;
                    float damage_negate = 1.0f - my_spaceship->getShieldDamageFactor(di, my_spaceship->shield_count - 1);
                    if (damage_negate < 0.0)
                        addSystemEffect("Rear Extra damage", string(int(-damage_negate * 100)) + "%");
                    else
                        addSystemEffect("Rear Damage negate", string(int(damage_negate * 100)) + "%");
                }
                break;
            case SYS_Docks:
                addSystemEffect("Cargo move speed", string(int(effectiveness * 100)) + "%");
                addSystemEffect("Energy transfer speed", string(effectiveness * PlayerSpaceship::energy_transfer_per_second) + "/s");
                addSystemEffect("Tractor beam drag speed", string(int(effectiveness * 100)) + "%");
                break;
            case SYS_Drones:
                addSystemEffect("Drones control range", string(my_spaceship->getDronesControlRange(),1) + "U");
                break;
            default:
                break;
            }
        }
        for(unsigned int idx=system_effects_index; idx<system_effects.size(); idx++)
            system_effects[idx]->hide();
    }
    GuiOverlay::onDraw(window);
}

bool EngineControlScreen::onJoystickAxis(const AxisAction& axisAction){
    if (hasControl() && axisAction.category == "ENGINEERING"){
        for(int n=0; n<SYS_COUNT; n++)
        {
            ESystem system = ESystem(n);
            if (axisAction.action == std::string("POWER_") + getSystemName(system)){
                my_spaceship->commandSetSystemPowerRequest(system, (axisAction.value + 1) * 3.0 / 2.0);
                return true;
            } 
            if (axisAction.action == std::string("COOLANT_") + getSystemName(system)){
                my_spaceship->commandSetSystemCoolantRequest(system, (axisAction.value + 1) * 10.0 / 2.0);
                return true;
            }
        }
    }
    return false;
}

void EngineControlScreen::onHotkey(const HotkeyResult& key)
{
    if (key.category == "ENGINEERING") {
        if (hasControl()){
            if (key.hotkey == std::string("SET_CONTROL_BRIDGE")) {
                my_spaceship->commandSetEngineeringControlToBridge();
            } else if (key.hotkey == std::string("SET_CONTROL_ECR")) {
                my_spaceship->commandSetEngineeringControlToECR();
            } else {
                for(int n=0; n<SYS_COUNT; n++) {
                    ESystem system = ESystem(n);
                    if (key.hotkey == std::string("REPAIR_") + getSystemName(system)) {
                        my_spaceship->commandSetAutoRepairSystemTarget(system);
                    }
                }
            }
        } else if (my_spaceship && crew_position == engineControlScreen) {
            if (key.hotkey == std::string("SET_CONTROL_ECR")) {
                my_spaceship->commandSetEngineeringControlToECR();
            } else if (key.hotkey == std::string("WARP_CAL_INC")) {
                new_warp_frequency = (new_warp_frequency + 1) % SpaceShip::max_frequency;
            } else if (key.hotkey == std::string("WARP_CAL_DEC")) {
                new_warp_frequency = (new_warp_frequency + SpaceShip::max_frequency - 1) % SpaceShip::max_frequency;
            } else if (key.hotkey == std::string("WARP_CAL_START")) {
                my_spaceship->commandSetWarpFrequency(new_warp_frequency);
            }
        }
    }
}

bool EngineControlScreen::hasControl(){
    return my_spaceship && 
        ((crew_position == bridgeEngineeringScreen && my_spaceship->engineering_control_from_bridge) ||
        (crew_position == engineControlScreen && !my_spaceship->engineering_control_from_bridge));
}

void EngineControlScreen::addSystemEffect(string key, string value)
{
    if (system_effects_index == system_effects.size())
    {
        GuiKeyValueDisplay* item = new GuiKeyValueDisplay(system_effects_container, "", 0.75, key, value);
        item->setTextSize(20)->setSize(GuiElement::GuiSizeMax, 40);
        system_effects.push_back(item);
    }else{
        system_effects[system_effects_index]->setKey(key);
        system_effects[system_effects_index]->setValue(value);
        system_effects[system_effects_index]->show();
    }
    system_effects_index++;
}
