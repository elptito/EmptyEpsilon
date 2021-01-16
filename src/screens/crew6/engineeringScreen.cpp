#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "engineeringScreen.h"
#include "preferenceManager.h"

#include "screenComponents/shipInternalView.h"
#include "screenComponents/selfDestructButton.h"
#include "screenComponents/alertOverlay.h"
#include "screenComponents/customShipFunctions.h"
#include "screenComponents/systemEffectsList.h"
#include "screenComponents/powerDamageIndicator.h"
#include "screenComponents/commsOverlay.h"
#include "screenComponents/shipsLogControl.h"

#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_autolayout.h"
#include "gui/gui2_togglebutton.h"
#include "gui/gui2_slider.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_progressslider.h"
#include "gui/gui2_arrow.h"
#include "gui/gui2_image.h"
#include "gui/gui2_panel.h"

EngineeringScreen::EngineeringScreen(GuiContainer* owner, ECrewPosition crew_position)
: GuiOverlay(owner, "ENGINEERING_SCREEN", colorConfig.background), selected_system(SYS_None)
{
    // Render the background decorations.
    background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));
    int i = 80;
    energy_bar = new GuiProgressbar(this, "ENERGY_BAR", 0.0, 1.0, 0.0);
    energy_bar->setColor(sf::Color(96, 96, 96, 128));
    energy_bar->setPosition(20, i, ATopLeft)->setSize(240, 40);
    energy_display = new GuiKeyValueDisplay(energy_bar, "ENERGY_DISPLAY", 0.45, tr("Energy"), "");
    energy_display->isBackground(false)->setIcon("gui/icons/energy")->setTextSize(20)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    i += 40;
    hull_bar = new GuiProgressbar(this, "HULL_BAR", 0.0, 1.0, 0.0);
    hull_bar->setColor(sf::Color(96, 96, 96, 128));
    hull_bar->setPosition(20, i, ATopLeft)->setSize(240, 40);
    hull_display = new GuiKeyValueDisplay(hull_bar, "HULL_DISPLAY", 0.45, tr("health","Hull"), "");
    hull_display->isBackground(false)->setIcon("gui/icons/hull")->setTextSize(20)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    i += 40;
    shield_bar = new GuiProgressbar(this, "SHIELDS_BAR", 0.0, 1.0, 0.0);
    shield_bar->setColor(sf::Color(96, 96, 96, 128));
    shield_bar->setPosition(20, i, ATopLeft)->setSize(240, 40);
    shield_display = new GuiKeyValueDisplay(shield_bar, "SHIELD_DISPLAY", 0.45, tr("shields", "Shields"), "");
    shield_display->isBackground(false)->setIcon("gui/icons/shields-all")->setTextSize(20)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);    
    i += 40;
    oxygen_bar = new GuiProgressbar(this, "OXYGEN_BAR", 0.0, 1.0, 0.0);
    oxygen_bar->setColor(sf::Color(96, 96, 96, 128));
    oxygen_bar->setPosition(20, i, ATopLeft)->setSize(240, 40);
    oxygen_display = new GuiKeyValueDisplay(oxygen_bar, "OXYGEN_DISPLAY", 0.45, tr("total","Oxygen"), "");
    oxygen_display->isBackground(false)->setIcon("gui/icons/system_oxygen")->setTextSize(20)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    i += 60;
    coolant_display = new GuiKeyValueDisplay(this, "COOLANT_DISPLAY", 0.75, tr("total","Total of Coolant"), "");
    coolant_display->setIcon("gui/icons/coolant")->setTextSize(20)->setPosition(20, i, ATopLeft)->setSize(240, 40);
    if (gameGlobalInfo->use_nano_repair_crew and gameGlobalInfo->use_system_damage)
    {
        i += 40;
        repair_display = new GuiKeyValueDisplay(this, "REPAIR_DISPLAY", 0.75, tr("total","Total of Repair"), "");
        repair_display->setIcon("gui/icons/system_health")->setTextSize(20)->setPosition(20, i, ATopLeft)->setSize(240, 40);
    }

    self_destruct_button = new GuiSelfDestructButton(this, "SELF_DESTRUCT");
    self_destruct_button->setPosition(20, 20, ATopLeft)->setSize(240, 100)->setVisible(my_spaceship && my_spaceship->getCanSelfDestruct());

    GuiElement* system_config_container = new GuiElement(this, "");
    system_config_container->setPosition(0, -60, ABottomCenter)->setSize(750 + 300, GuiElement::GuiSizeMax);
    GuiAutoLayout* system_row_layouts = new GuiAutoLayout(system_config_container, "SYSTEM_ROWS", GuiAutoLayout::LayoutVerticalBottomToTop);
    system_row_layouts->setPosition(0, 0, ABottomLeft);
    system_row_layouts->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    for(int n=0; n<SYS_COUNT; n++)
    {
        string id = "SYSTEM_ROW_" + getSystemName(ESystem(n));
        SystemRow info;
        info.layout = new GuiAutoLayout(system_row_layouts, id, GuiAutoLayout::LayoutHorizontalLeftToRight);
        info.layout->setSize(GuiElement::GuiSizeMax, 40);

        info.button = new GuiToggleButton(info.layout, id + "_SELECT", getLocaleSystemName(ESystem(n)), [this, n](bool value){
            selectSystem(ESystem(n));
        });
        info.button->setSize(300, GuiElement::GuiSizeMax);
        info.damage_bar = new GuiProgressbar(info.layout, id + "_DAMAGE", 0.0, 1.0, 0.0);
        info.damage_bar->setSize(150, GuiElement::GuiSizeMax);
        info.damage_icon = new GuiImage(info.damage_bar, "", "gui/icons/hull");
        info.damage_icon->setColor(colorConfig.overlay_damaged)->setPosition(0, 0, ACenterRight)->setSize(GuiElement::GuiSizeMatchHeight, GuiElement::GuiSizeMax);
        info.damage_label = new GuiLabel(info.damage_bar, id + "_DAMAGE_LABEL", "...", 20);
        info.damage_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
        info.heat_bar = new GuiProgressbar(info.layout, id + "_HEAT", 0.0, 1.0, 0.0);
        info.heat_bar->setSize(100, GuiElement::GuiSizeMax);
        info.heat_arrow = new GuiArrow(info.heat_bar, id + "_HEAT_ARROW", 0);
        info.heat_arrow->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
        info.heat_icon = new GuiImage(info.heat_bar, "", "gui/icons/status_overheat");
        info.heat_icon->setColor(colorConfig.overlay_overheating)->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMatchHeight, GuiElement::GuiSizeMax);
        info.heat_label = new GuiLabel(info.heat_bar, id + "_HEAT_LABEL", "...", 20);
        info.heat_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
        info.power_bar = new GuiProgressSlider(info.layout, id + "_POWER", 0.0, 3.0, 0.0, [this,n](float value){
            if (my_spaceship)
                my_spaceship->commandSetSystemPowerRequest(ESystem(n), value);
        });
        info.power_bar->setColor(sf::Color(192, 192, 32, 128))->setSize(100, GuiElement::GuiSizeMax);
        info.power_label = new GuiLabel(info.power_bar, id + "_POWER_LABEL", "...", 20);
        info.power_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
        info.coolant_bar = new GuiProgressSlider(info.layout, id + "_COOLANT", 0.0, 10.0, 0.0, [this,n](float value){
            if (my_spaceship)
                my_spaceship->commandSetSystemCoolantRequest(ESystem(n), value);
        });
        info.coolant_bar->setColor(sf::Color(32, 128, 128, 128))->setSize(100, GuiElement::GuiSizeMax);
        info.coolant_label = new GuiLabel(info.coolant_bar, id + "_COOLANT_LABEL", "...", 20);
        info.coolant_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
        if (!gameGlobalInfo->use_system_damage){
            info.damage_bar->hide();
            info.heat_bar->setSize(150, GuiElement::GuiSizeMax);
            info.power_bar->setSize(150, GuiElement::GuiSizeMax);
            info.coolant_bar->setSize(150, GuiElement::GuiSizeMax);
        }
        if (gameGlobalInfo->use_nano_repair_crew)
        {
            info.damage_bar->setSize(150, GuiElement::GuiSizeMax);
            info.heat_bar->setSize(150, GuiElement::GuiSizeMax);
            info.power_bar->setSize(150, GuiElement::GuiSizeMax);
            info.coolant_bar->setSize(150, GuiElement::GuiSizeMax);
            if (gameGlobalInfo->use_system_damage)
            {
                info.repair_bar = new GuiProgressSlider(info.layout, id + "_REPAIR", 0.0, 10.0, 0.0, [this,n](float value){
                    if (my_spaceship)
                        my_spaceship->commandSetSystemRepairRequest(ESystem(n), value);
                });
                info.repair_bar->setColor(sf::Color(32, 128, 32, 128))->setSize(150, GuiElement::GuiSizeMax);
                info.repair_label = new GuiLabel(info.repair_bar, id + "_REPAIR_LABEL", "...", 20);
                info.repair_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
            }
        }

        info.layout->moveToBack();
        system_rows.push_back(info);
    }

    GuiAutoLayout* icon_layout = new GuiAutoLayout(system_row_layouts, "", GuiAutoLayout::LayoutHorizontalLeftToRight);
    icon_layout->setSize(GuiElement::GuiSizeMax, 48);
    (new GuiElement(icon_layout, "FILLER"))->setSize(300, GuiElement::GuiSizeMax);
    if (gameGlobalInfo->use_system_damage){
        if (gameGlobalInfo->use_nano_repair_crew)
        {
            (new GuiKeyValueDisplay(icon_layout, "SYSTEM_HEALTH", 0.9, tr("title", "health"), ""))->setIcon("gui/icons/hull")->setTextSize(30)->setSize(150, GuiElement::GuiSizeMax);
            (new GuiKeyValueDisplay(icon_layout, "SYSTEM_HEAT", 0.9, tr("title", "heat"), ""))->setIcon("gui/icons/status_overheat")->setTextSize(30)->setSize(150, GuiElement::GuiSizeMax);
            (new GuiKeyValueDisplay(icon_layout, "SYSTEM_POWER", 0.9, tr("title", "power"), ""))->setIcon("gui/icons/energy")->setTextSize(30)->setSize(150, GuiElement::GuiSizeMax);
            (new GuiKeyValueDisplay(icon_layout, "SYSTEM_COOLANT", 0.9, tr("title", "coolant"), ""))->setIcon("gui/icons/coolant")->setTextSize(30)->setSize(150, GuiElement::GuiSizeMax);
            (new GuiKeyValueDisplay(icon_layout, "SYSTEM_REPAIR", 0.9, tr("title", "repair"), ""))->setIcon("gui/icons/system_health")->setTextSize(30)->setSize(150, GuiElement::GuiSizeMax);
        } else {
            (new GuiImage(icon_layout, "SYSTEM_HEALTH_ICON", "gui/icons/hull"))->setSize(150, GuiElement::GuiSizeMax);
            (new GuiImage(icon_layout, "HEAT_ICON", "gui/icons/status_overheat"))->setSize(100, GuiElement::GuiSizeMax);
            (new GuiImage(icon_layout, "POWER_ICON", "gui/icons/energy"))->setSize(100, GuiElement::GuiSizeMax);
            (new GuiImage(icon_layout, "COOLANT_ICON", "gui/icons/coolant"))->setSize(100, GuiElement::GuiSizeMax);
        }
    } else {
        (new GuiImage(icon_layout, "HEAT_ICON", "gui/icons/status_overheat"))->setSize(150, GuiElement::GuiSizeMax);
        (new GuiImage(icon_layout, "POWER_ICON", "gui/icons/energy"))->setSize(150, GuiElement::GuiSizeMax);
        (new GuiImage(icon_layout, "COOLANT_ICON", "gui/icons/coolant"))->setSize(150, GuiElement::GuiSizeMax);
    }

    system_rows[SYS_Reactor].button->setIcon("gui/icons/system_reactor");
    system_rows[SYS_Oxygen].button->setIcon("gui/icons/system_oxygen");
    system_rows[SYS_BeamWeapons].button->setIcon("gui/icons/system_beam");
    system_rows[SYS_MissileSystem].button->setIcon("gui/icons/system_missile");
    system_rows[SYS_Maneuver].button->setIcon("gui/icons/system_maneuver");
    system_rows[SYS_Impulse].button->setIcon("gui/icons/system_impulse");
    system_rows[SYS_Warp].button->setIcon("gui/icons/system_warpdrive");
    system_rows[SYS_JumpDrive].button->setIcon("gui/icons/system_jumpdrive");
    system_rows[SYS_FrontShield].button->setIcon("gui/icons/shields-fore");
    system_rows[SYS_RearShield].button->setIcon("gui/icons/shields-aft");
    system_rows[SYS_Docks].button->setIcon("gui/icons/docking");
    system_rows[SYS_Scanner].button->setIcon("gui/icons/station-relay");

    for(int n=0; n<SYS_COUNT; n++)
    {
        (new GuiPowerDamageIndicator(system_rows[n].button, n + "_INDICATOR", ESystem(n), ACenterLeft, my_spaceship, false))->setPosition(0, 0, ABottomLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    }

    if (gameGlobalInfo->use_nano_repair_crew)
    {
        system_effects_container = new GuiSystemEffectsList(this,"",GuiAutoLayout::LayoutVerticalBottomToTop);
        system_effects_container->setPosition(-20, -20, ATopRight)->setSize(270, 340);
    } else {
        system_effects_container = new GuiSystemEffectsList(system_config_container,"",GuiAutoLayout::LayoutVerticalBottomToTop);
        system_effects_container->setPosition(0, -400, ABottomRight)->setSize(270, 400);
    }

    GuiPanel* box = new GuiPanel(system_config_container, "POWER_COOLANT_BOX");
    box->setPosition(0, 0, ABottomRight)->setSize(270, 400);
    power_label = new GuiLabel(box, "POWER_LABEL", tr("slider", "Power"), 30);
    power_label->setVertical()->setAlignment(ACenterLeft)->setPosition(20, 20, ATopLeft)->setSize(30, 360);
    coolant_label = new GuiLabel(box, "COOLANT_LABEL", tr("slider", "Coolant"), 30);
    coolant_label->setVertical()->setAlignment(ACenterLeft)->setPosition(110, 20, ATopLeft)->setSize(30, 360);

    power_slider = new GuiSlider(box, "POWER_SLIDER", 3.0, 0.0, 1.0, [this](float value) {
        if (my_spaceship && selected_system != SYS_None)
            my_spaceship->commandSetSystemPowerRequest(selected_system, value);
    });
    power_slider->setPosition(50, 20, ATopLeft)->setSize(60, 360);
    for(float snap_point = 0.0; snap_point <= 3.0; snap_point += 0.5)
        power_slider->addSnapValue(snap_point, snap_point == 1.0 ? 0.1 : 0.01);
    power_slider->disable();
    coolant_slider = new GuiSlider(box, "COOLANT_SLIDER", 10.0, 0.0, 0.0, [this](float value) {
        if (my_spaceship && selected_system != SYS_None)
            my_spaceship->commandSetSystemCoolantRequest(selected_system, value);
    });
    coolant_slider->setPosition(140, 20, ATopLeft)->setSize(60, 360);
    coolant_slider->disable();

    if (!gameGlobalInfo->use_nano_repair_crew)
        (new GuiShipInternalView(system_row_layouts, "SHIP_INTERNAL_VIEW", 48.0f))->setShip(my_spaceship)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
        
    if (gameGlobalInfo->use_nano_repair_crew)
    {
        box->setPosition(0, 20, ATopCenter)->setSize(450, 320);
        power_label->setHorizontal()->setPosition(20, 20, ATopLeft)->setSize(400, 20);
        power_slider->setRange(0.0, 3.0)->setPosition(20, 60, ATopLeft)->setSize(400, 40);
        coolant_label->setHorizontal()->setPosition(20, 120, ATopLeft)->setSize(400, 20);
        coolant_slider->setRange(0.0, 10.0)->setPosition(20, 160, ATopLeft)->setSize(400, 40);
        
        if (gameGlobalInfo->use_system_damage)
        {
            repair_label = new GuiLabel(box, "COOLANT_LABEL", tr("slider", "Repair"), 30);
            repair_label->setAlignment(ACenterLeft)->setPosition(20, 220, ATopLeft)->setSize(400, 20);
            repair_slider = new GuiSlider(box, "COOLANT_SLIDER", 0.0, 10.0, 0.0, [this](float value) {
            if (my_spaceship && selected_system != SYS_None)
                my_spaceship->commandSetSystemRepairRequest(selected_system, value);
            });
            repair_slider->setPosition(20, 260, ATopLeft)->setSize(400, 40);
            repair_slider->disable();
        }
    }

    // Bottom layout.
    GuiAutoLayout* layout = new GuiAutoLayout(this, "", GuiAutoLayout::LayoutVerticalBottomToTop);
    layout->setPosition(-20, -20, ABottomRight)->setSize(300, GuiElement::GuiSizeMax);
    std::vector<GuiAutoLayout*> presets_buttons_layouts;

    // Presets buttons.
    presets_button = new GuiToggleButton(layout, "PRESET", tr("preset", "presets"), [this](bool value)
    {
        for(int buttonId = 0 ; buttonId < my_spaceship->max_engineer_presets_number*2 ; buttonId++)
        {
            if (buttonId < my_spaceship->active_engineer_presets_number*2)
                presets_buttons.at(buttonId)->setVisible(value);
            else
                presets_buttons.at(buttonId)->setVisible(false);
        }
    });
    presets_button->setValue(false);
    presets_button->setTextSize(20);
    presets_button->setSize(125, 25);

    for(int presetId=1; presetId < my_spaceship->max_engineer_presets_number + 1; presetId++)
    {

        GuiAutoLayout* preset_button_layout = new GuiAutoLayout(layout, "", GuiAutoLayout::LayoutHorizontalLeftToRight);
        preset_button_layout->setSize(300, GuiElement::GuiSizeMax);
        GuiButton* preset_button_apply = new GuiButton(preset_button_layout, "", tr("preset", "preset") + " " + std::to_string(presetId), [this, presetId]()
        {
            applyPreset(presetId);
            for(GuiButton* button : presets_buttons)
                button->setVisible(false);
            presets_button->setValue(false);
        });
        preset_button_apply->setVisible(false);
        preset_button_apply->setTextSize(20);
        preset_button_apply->setSize(100, 25);   
        presets_buttons.push_back(preset_button_apply);
        GuiButton* preset_button_update = new GuiButton(preset_button_layout, "", "", [this, presetId]()
        {
            updatePreset(presetId);
            //my_spaceship->saveToPreferencesEngineerPresets();
            for(GuiButton* button : presets_buttons)
                button->setVisible(false);
            presets_button->setValue(false);
        });
        preset_button_update->setVisible(false);
        preset_button_update->setTextSize(20);
        preset_button_update->setIcon("gui/icons/save-arrow");
        preset_button_update->setSize(25, 25);   
        presets_buttons.push_back(preset_button_update);

        //preset_button_layout->setVisible(false);
        preset_button_layout->setSize(125, 25);   
        presets_buttons_layouts.push_back(preset_button_layout);
    }


    new ShipsLog(this, crew_position);
    
    (new GuiCustomShipFunctions(this, crew_position, "", my_spaceship))->setPosition(-20, 120, ATopRight)->setSize(250, GuiElement::GuiSizeMax);

    previous_energy_level = 0.0;
    average_energy_delta = 0.0;
    previous_energy_measurement = 0.0;
}

void EngineeringScreen::applyPreset(int preset)
{
    if (my_spaceship)
    {
        for(int n = 0; n < SYS_COUNT; n++)
        {
            ESystem system = ESystem(n);
            my_spaceship->commandSetSystemPowerRequest(system, my_spaceship->power_presets[n][preset-1]);
            my_spaceship->commandSetSystemCoolantRequest(system, my_spaceship->coolant_presets[n][preset-1]);
        }
        my_spaceship->addToShipLog(tr("preset","Preset {id_preset} loaded").format({{"id_preset", string(preset)}}), colorConfig.log_receive_neutral, engineering);
    }
}

void EngineeringScreen::updatePreset(int preset)
{
    if (my_spaceship)
    {
        for(int n = 0; n < SYS_COUNT; n++)
        {
            ESystem system = ESystem(n);
            my_spaceship->commandSetSystemPowerPreset(system, preset, my_spaceship->systems[n].power_request);
            my_spaceship->commandSetSystemCoolantPreset(system, preset, my_spaceship->systems[n].coolant_request);
        }
    }
    // Save preset to options.ini file
    string power_saved_presets = "";
    string coolant_saved_presets = "";
    for(int n = 0; n < SYS_COUNT-1; n++)
    {
        power_saved_presets += string(my_spaceship->systems[n].power_request, 2) + "|";
        coolant_saved_presets += string(my_spaceship->systems[n].coolant_request, 2) + "|";
    }
    power_saved_presets += string(my_spaceship->systems[SYS_COUNT-1].power_request, 2);
    coolant_saved_presets += string(my_spaceship->systems[SYS_COUNT-1].coolant_request, 2);
    
    PreferencesManager::set("ENGINEERING.PRESET_POWER_"+string(preset), power_saved_presets);
    PreferencesManager::set("ENGINEERING.PRESET_COOLANT_"+string(preset), coolant_saved_presets);

    //Save preferences
    if (getenv("HOME"))
        PreferencesManager::save(string(getenv("HOME")) + "/.emptyepsilon/options.ini");
    else
        PreferencesManager::save("options.ini");
}

void EngineeringScreen::onDraw(sf::RenderTarget& window)
{
    if (my_spaceship)
    {
        // Update presets number
        for(int buttonId = 0 ; buttonId < my_spaceship->max_engineer_presets_number*2; buttonId++)
        {
            if (buttonId < my_spaceship->active_engineer_presets_number*2)
                presets_buttons.at(buttonId)->setVisible(presets_button->getValue());
            else
                presets_buttons.at(buttonId)->setVisible(false);
        }
        presets_button->setVisible(my_spaceship->active_engineer_presets_number > 0);

        // Update the energy usage.
        if (previous_energy_measurement == 0.0)
        {
            previous_energy_level = my_spaceship->energy_level;
            previous_energy_measurement = engine->getElapsedTime();
        }else{
            if (previous_energy_measurement != engine->getElapsedTime())
            {
                float delta_t = engine->getElapsedTime() - previous_energy_measurement;
                float delta_e = my_spaceship->energy_level - previous_energy_level;
                float delta_e_per_second = delta_e / delta_t;
                average_energy_delta = average_energy_delta * 0.99 + delta_e_per_second * 0.01;

                previous_energy_level = my_spaceship->energy_level;
                previous_energy_measurement = engine->getElapsedTime();
            }
        }

        energy_display->setValue(toNearbyIntString(my_spaceship->energy_level) + " (" + tr("{energy}/min").format({{"energy", toNearbyIntString(average_energy_delta * 60.0f)}}) + ")");
        if (my_spaceship->energy_level < 100)
            energy_display->setColor(sf::Color::Red);
        else
            energy_display->setColor(sf::Color::White);
        energy_bar->setValue(my_spaceship->energy_level / my_spaceship->max_energy_level);
        hull_display->setValue(toNearbyIntString(100 * my_spaceship->hull_strength / my_spaceship->hull_max) + "%");
        if (my_spaceship->hull_strength < my_spaceship->hull_max / 4.0f)
            hull_display->setColor(sf::Color::Red);
        else
            hull_display->setColor(sf::Color::White);
        hull_bar->setValue(my_spaceship->hull_strength / my_spaceship->hull_max);
        string shields = "";
        shields += string(my_spaceship->getShieldPercentage(0)) + "%";
        if (my_spaceship->getShieldPercentage(1) > 0)
            shields += "/" + string(my_spaceship->getShieldPercentage(1)) + "%";
        shield_display->setValue(shields);
        shield_bar->setValue((my_spaceship->getShieldPercentage(0) / 100.0 + my_spaceship->getShieldPercentage(1) / 100.0) / 2.0);
		coolant_display->setValue(toNearbyIntString(my_spaceship->max_coolant * 10) + "%");
        if (gameGlobalInfo->use_nano_repair_crew)
        {
            coolant_display->setValue(string(int(my_spaceship->max_coolant)));
            if (gameGlobalInfo->use_system_damage)
                repair_display->setValue(string(int(my_spaceship->max_repair)));
        }
        float oxygen_level = 0;
        float oxygen_max = 0;
        for(int n=0; n<10; n++)
        {
            if (my_spaceship->oxygen_zones[n].oxygen_max > 0)
            {
                oxygen_level += my_spaceship->oxygen_zones[n].oxygen_level;
                oxygen_max += my_spaceship->oxygen_zones[n].oxygen_max;
            }
        }
        oxygen_display->setValue(string(oxygen_level / oxygen_max * 100,1) + "%");
        oxygen_bar->setVisible(oxygen_max > 0);
        oxygen_bar->setValue(oxygen_level / oxygen_max);

        for(int n=0; n<SYS_COUNT; n++)
        {
            SystemRow info = system_rows[n];
            info.layout->setVisible(my_spaceship->hasSystem(ESystem(n)));

            float health = my_spaceship->systems[n].health;
            if (health < 0.0)
                info.damage_bar->setValue(-health)->setColor(sf::Color(128, 32, 32, 192));
            else
                info.damage_bar->setValue(health)->setColor(sf::Color(64, 128 * health, 64 * health, 192));
            info.damage_label->setText(toNearbyIntString(health * 100) + "%");
            info.heat_label->setText(toNearbyIntString(my_spaceship->systems[n].heat_level * 100) + "%");
            info.heat_label->setVisible(my_spaceship->systems[n].heat_level > 0.0);
            info.power_label->setText(toNearbyIntString(my_spaceship->systems[n].power_level * 100) + "%");
            info.coolant_label->setText(string(my_spaceship->systems[n].coolant_level, 1));
            info.coolant_label->setVisible(my_spaceship->systems[n].coolant_level > 0.0);
            if (gameGlobalInfo->use_nano_repair_crew and gameGlobalInfo->use_system_damage)
            {
                info.repair_label->setText(string(my_spaceship->systems[n].repair_level, 1));
                info.repair_label->setVisible(my_spaceship->systems[n].repair_level > 0.0);
            }
            float health_max = my_spaceship->systems[n].health_max;
            if (health_max < 1.0)
                info.damage_icon->show();
            else
                info.damage_icon->hide();

            float heat = my_spaceship->systems[n].heat_level;
            info.heat_bar->setValue(heat)->setColor(sf::Color(128, 32 + 96 * (1.0 - heat), 32, 192));
            float heating_diff = my_spaceship->systems[n].getHeatingDelta();
            if (heating_diff > 0)
                info.heat_arrow->setAngle(90);
            else
                info.heat_arrow->setAngle(-90);
            info.heat_arrow->setVisible(heat > 0);
            info.heat_arrow->setColor(sf::Color(255, 255, 255, std::min(255, int(255 * fabs(heating_diff)))));
            if (heat > 0.9 && fmod(engine->getElapsedTime(), 0.5) < 0.25)
                info.heat_icon->show();
            else
                info.heat_icon->hide();

            info.power_bar->setValue(my_spaceship->systems[n].power_level);
            info.coolant_bar->setValue(my_spaceship->systems[n].coolant_level);
            info.coolant_bar->setRange(0.0, my_spaceship->max_coolant_per_system);
            if (gameGlobalInfo->use_nano_repair_crew and gameGlobalInfo->use_system_damage)
            {
                info.repair_bar->setRange(0.0, my_spaceship->max_repair_per_system);
                info.repair_bar->setValue(my_spaceship->systems[n].repair_level);
            }
        }

        if (selected_system != SYS_None)
        {
            ShipSystem& system = my_spaceship->systems[selected_system];
            power_label->setText(tr("Power: {power_level}%/{power_request}%").format({{"power_level", toNearbyIntString(system.power_request * 100)},{"power_request", toNearbyIntString(system.power_request * 100)}}));
            power_slider->setValue(system.power_request);
            coolant_label->setText(tr("Coolant: {coolant_level}%/{coolant_request}%").format({{"coolant_level", toNearbyIntString(system.coolant_level / my_spaceship->max_coolant_per_system * 100)},{"coolant_request", toNearbyIntString(std::min(system.coolant_request, my_spaceship->max_coolant) / my_spaceship->max_coolant_per_system * 100)}}));
            coolant_slider->setEnable(!my_spaceship->auto_coolant_enabled);
            coolant_slider->setRange(0.0, my_spaceship->max_coolant_per_system);
            coolant_slider->setValue(std::min(system.coolant_request, my_spaceship->max_coolant));

            if (gameGlobalInfo->use_nano_repair_crew)
            {
                coolant_label->setText(tr("Coolant: {coolant_level} / {coolant_max}\t\t (Target: {coolant_request})").format({{"coolant_level", string(system.coolant_level, 1)},{"coolant_max", string(my_spaceship->max_coolant_per_system, 1)},{"coolant_request", string(system.coolant_request, 1)}}));
                power_label->setText(tr("Power: {power_level}% \t\t (Target: {power_request}%)").format({{"power_level", toNearbyIntString(system.power_level * 100)},{"power_request", toNearbyIntString(system.power_request * 100)}}));
                if (gameGlobalInfo->use_system_damage)
                {
                    repair_label->setText(tr("Repair: {repair_level} / {repair_max}\t\t (Target: {repair_request})").format({{"repair_level", string(system.repair_level, 1)},{"repair_max", string(my_spaceship->max_repair_per_system, 1)},{"repair_request", string(system.repair_request, 1)}}));
                    repair_slider->setEnable(!my_spaceship->auto_repair_enabled);
                    repair_slider->setRange(0.0, my_spaceship->max_repair_per_system);
                    repair_slider->setValue(std::min(system.repair_request, my_spaceship->max_repair));
                }
            }
        }
    }
    GuiOverlay::onDraw(window);
}

bool EngineeringScreen::onJoystickAxis(const AxisAction& axisAction){
    if(my_spaceship){
        if (axisAction.category == "ENGINEERING"){
            if (selected_system != SYS_None){
                if (axisAction.action == "POWER" || axisAction.action == std::string("POWER_") + getSystemName(selected_system)){
                    power_slider->setValue((axisAction.value + 1) * 3.0 / 2.0);
                    my_spaceship->commandSetSystemPowerRequest(selected_system, power_slider->getValue());
                    return true;
                }
                if (axisAction.action == "COOLANT" || axisAction.action == std::string("COOLANT_") + getSystemName(selected_system)){
                    coolant_slider->setValue((axisAction.value + 1) * 10.0 / 2.0);
                    my_spaceship->commandSetSystemCoolantRequest(selected_system, coolant_slider->getValue());
                    return true;
                }
            } else {
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
        }
    }
    return false;
}

void EngineeringScreen::onHotkey(const HotkeyResult& key)
{
    if (key.category == "ENGINEERING" && my_spaceship)
    {
        if (key.hotkey == "SELECT_REACTOR") selectSystem(SYS_Reactor);
        if (key.hotkey == "SELECT_BEAM_WEAPONS") selectSystem(SYS_BeamWeapons);
        if (key.hotkey == "SELECT_MISSILE_SYSTEM") selectSystem(SYS_MissileSystem);
        if (key.hotkey == "SELECT_MANEUVER") selectSystem(SYS_Maneuver);
        if (key.hotkey == "SELECT_IMPULSE") selectSystem(SYS_Impulse);
        if (key.hotkey == "SELECT_WARP") selectSystem(SYS_Warp);
        if (key.hotkey == "SELECT_JUMP_DRIVE") selectSystem(SYS_JumpDrive);
        if (key.hotkey == "SELECT_FRONT_SHIELDS") selectSystem(SYS_FrontShield);
        if (key.hotkey == "SELECT_REAR_SHIELDS") selectSystem(SYS_RearShield);
        if (key.hotkey == "SELECT_DOCKS") selectSystem(SYS_Docks);
        if (key.hotkey == "SELECT_SCANNER") selectSystem(SYS_Scanner);

        if (key.hotkey == "SELECT_NEXT_SYSTEM")
        {
            if (selected_system == SYS_None)
                selectSystem(SYS_Reactor);
            else
            {
                int n = ESystem(int(selected_system) + 1);

                while (n<SYS_COUNT)                
                {
                    if (my_spaceship->hasSystem(ESystem(n)))
                    {
                        selectSystem(ESystem(n));
                        break;
                    }
                    n++;
                }
                if (n == SYS_COUNT)
                    selectSystem(SYS_Reactor);
            }
        }
        if (key.hotkey == "SELECT_PREVIOUS_SYSTEM")
        {
            if (selected_system == SYS_None)
                selectSystem(ESystem(SYS_COUNT-1));
            else
            {
                int n = ESystem(int(selected_system) - 1);

                while (n>=0)                
                {
                    if (my_spaceship->hasSystem(ESystem(n)))
                    {
                        selectSystem(ESystem(n));
                        break;
                    }
                    n--;
                }
                if (n < 0)
                    selectSystem(ESystem(SYS_COUNT-1));
            }
        }

        if (key.hotkey == "RESET_SYSTEMS")
        {
            int n_systems = 0;
            for(int n=0; n<SYS_COUNT; n++)
                if (my_spaceship->hasSystem(ESystem(n)))
                    n_systems ++;
                    
            for(int n=0; n<SYS_COUNT; n++)
            {
                ESystem system = ESystem(n);
                power_slider->setValue(1.0f);
                coolant_slider->setValue(0.0f);
                my_spaceship->commandSetSystemPowerRequest(system, 1.0f);
                my_spaceship->commandSetSystemCoolantRequest(system, my_spaceship->max_coolant / n_systems);
                if (gameGlobalInfo->use_nano_repair_crew)
                {
                    repair_slider->setValue(0.0f);
                    my_spaceship->commandSetSystemRepairRequest(system, my_spaceship->max_repair / n_systems);
                }
            }
        }

        if (selected_system != SYS_None)
        {
            // Note the code duplication with extra/powerManagement
            if (key.hotkey == "SET_POWER_000")
            {
                power_slider->setValue(0.0f);
                my_spaceship->commandSetSystemPowerRequest(selected_system, power_slider->getValue());
            }
            if (key.hotkey == "SET_POWER_030")
            {
                power_slider->setValue(0.3f);
                my_spaceship->commandSetSystemPowerRequest(selected_system, power_slider->getValue());
            }
            if (key.hotkey == "SET_POWER_050")
            {
                power_slider->setValue(0.5f);
                my_spaceship->commandSetSystemPowerRequest(selected_system, power_slider->getValue());
            }
            if (key.hotkey == "SET_POWER_100")
            {
                power_slider->setValue(1.0f);
                my_spaceship->commandSetSystemPowerRequest(selected_system, power_slider->getValue());
            }
            if (key.hotkey == "SET_POWER_150")
            {
                power_slider->setValue(1.5f);
                my_spaceship->commandSetSystemPowerRequest(selected_system, power_slider->getValue());
            }
            if (key.hotkey == "SET_POWER_200")
            {
                power_slider->setValue(2.0f);
                my_spaceship->commandSetSystemPowerRequest(selected_system, power_slider->getValue());
            }
            if (key.hotkey == "SET_POWER_250")
            {
                power_slider->setValue(2.5f);
                my_spaceship->commandSetSystemPowerRequest(selected_system, power_slider->getValue());
            }
            if (key.hotkey == "SET_POWER_300")
            {
                power_slider->setValue(3.0f);
                my_spaceship->commandSetSystemPowerRequest(selected_system, power_slider->getValue());
            }
            if (key.hotkey == "INCREASE_POWER")
            {
                power_slider->setValue(my_spaceship->systems[selected_system].power_request + 0.1f);
                my_spaceship->commandSetSystemPowerRequest(selected_system, power_slider->getValue());
            }
            if (key.hotkey == "DECREASE_POWER")
            {
                power_slider->setValue(my_spaceship->systems[selected_system].power_request - 0.1f);
                my_spaceship->commandSetSystemPowerRequest(selected_system, power_slider->getValue());
            }
            if (key.hotkey == "INCREASE_COOLANT")
            {
                coolant_slider->setValue(my_spaceship->systems[selected_system].coolant_request + 0.5f);
                my_spaceship->commandSetSystemCoolantRequest(selected_system, coolant_slider->getValue());
            }
            if (key.hotkey == "DECREASE_COOLANT")
            {
                coolant_slider->setValue(my_spaceship->systems[selected_system].coolant_request - 0.5f);
                my_spaceship->commandSetSystemCoolantRequest(selected_system, coolant_slider->getValue());
            }
            if (key.hotkey == "COOLANT_MAX")
            {
                coolant_slider->setValue(my_spaceship->max_coolant_per_system);
                my_spaceship->commandSetSystemCoolantRequest(selected_system, coolant_slider->getValue());
            }
            if (key.hotkey == "COOLANT_MIN")
            {
                coolant_slider->setValue(0.0f);
                my_spaceship->commandSetSystemCoolantRequest(selected_system, coolant_slider->getValue());
            }
            if (gameGlobalInfo->use_nano_repair_crew && key.hotkey == "INCREASE_REPAIR")
            {
                repair_slider->setValue(my_spaceship->systems[selected_system].repair_request + 0.5f);
                my_spaceship->commandSetSystemRepairRequest(selected_system, repair_slider->getValue());
            }
            if (gameGlobalInfo->use_nano_repair_crew && key.hotkey == "DECREASE_REPAIR")
            {
                repair_slider->setValue(my_spaceship->systems[selected_system].repair_request - 0.5f);
                my_spaceship->commandSetSystemRepairRequest(selected_system, repair_slider->getValue());
            }
            if (gameGlobalInfo->use_nano_repair_crew && key.hotkey == "REPAIR_MAX")
            {
                repair_slider->setValue(my_spaceship->max_repair_per_system);
                my_spaceship->commandSetSystemRepairRequest(selected_system, repair_slider->getValue());
            }
            if (gameGlobalInfo->use_nano_repair_crew && key.hotkey == "REPAIR_MIN")
            {
                repair_slider->setValue(0.0f);
                my_spaceship->commandSetSystemRepairRequest(selected_system, repair_slider->getValue());
            }
        }

        for(int presetId=1; presetId < 10; presetId++) 
        {
            if (presetId <= my_spaceship->active_engineer_presets_number)
            {
                if (key.hotkey == "PRESET_APPLY" + string(presetId))
                    applyPreset(presetId);
                if (key.hotkey == "PRESET_UPDATE" + string(presetId))
                    updatePreset(presetId);
            }
        }
    }
}

void EngineeringScreen::selectSystem(ESystem system)
{
    if (my_spaceship && !my_spaceship->hasSystem(system))
        return;

    for(int idx=0; idx<SYS_COUNT; idx++)
    {
        system_rows[idx].button->setValue(idx == system);
    }
    selected_system = system;
    system_effects_container->selectSystem(system);
    power_slider->enable();
    if (my_spaceship)
    {
        power_slider->setValue(my_spaceship->systems[system].power_request);
        coolant_slider->setValue(my_spaceship->systems[system].coolant_request);
        if (gameGlobalInfo->use_nano_repair_crew and gameGlobalInfo->use_system_damage)
            repair_slider->setValue(my_spaceship->systems[system].repair_request);
    }
}

string EngineeringScreen::toNearbyIntString(float value)
{
    return string(int(nearbyint(value)));
}
