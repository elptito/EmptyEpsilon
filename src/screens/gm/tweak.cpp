#include "tweak.h"
#include "playerInfo.h"
#include "spaceObjects/spaceship.h"
#include "spaceObjects/spaceObject.h"

#include "gui/gui2_listbox.h"
#include "gui/gui2_autolayout.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_label.h"
#include "gui/gui2_textentry.h"
#include "gui/gui2_selector.h"
#include "gui/gui2_slider.h"
#include "gui/gui2_togglebutton.h"

GuiObjectTweak::GuiObjectTweak(GuiContainer* owner, ETweakType tweak_type)
: GuiPanel(owner, "GM_TWEAK_DIALOG")
{
    setPosition(0, -100, ABottomCenter);
    setSize(1000, 600);

    GuiListbox* list = new GuiListbox(this, "", [this](int index, string value)
    {
        for(GuiTweakPage* page : pages)
            page->hide();
        pages[index]->show();
    });

    list->setSize(300, GuiElement::GuiSizeMax);
    list->setPosition(25, 25, ATopLeft);

    pages.push_back(new GuiObjectTweakBase(this));
    list->addEntry("Base", "");

    if (tweak_type == TW_Template || tweak_type == TW_Ship || tweak_type == TW_Player)
    {
        pages.push_back(new GuiTemplateTweak(this));
        list->addEntry("Structure fixe", "");
        pages.push_back(new GuiShipTweakShields(this));
        list->addEntry("Boucliers", "");
    }

    if (tweak_type == TW_Ship || tweak_type == TW_Player)
    {
        pages.push_back(new GuiShipTweak(this));
        list->addEntry("Structure mobile", "");
        pages.push_back(new GuiShipTweakMissileTubes(this));
        list->addEntry("Tubes", "");
        pages.push_back(new GuiShipTweakMissileWeapons(this));
        list->addEntry("Missiles", "");
        pages.push_back(new GuiShipTweakBeamweapons(this));
        list->addEntry("Lasers", "");
        pages.push_back(new GuiShipTweakSystems(this));
        list->addEntry("Systemes", "");
    }

    if (tweak_type == TW_Player)
    {
        pages.push_back(new GuiShipTweakPlayer(this));
        list->addEntry("Joueur", "");
        pages.push_back(new GuiShipTweakMessages(this));
        list->addEntry("Messages", "");
    }

    for(GuiTweakPage* page : pages)
    {
        page->setSize(700, 600)->setPosition(0, 0, ABottomRight)->hide();
    }

    pages[0]->show();
    list->setSelectionIndex(0);

    (new GuiButton(this, "CLOSE_BUTTON", "Fin", [this]() {
        hide();
    }))->setTextSize(20)->setPosition(-10, 0, ATopRight)->setSize(70, 30);
}

void GuiObjectTweak::open(P<SpaceObject> target)
{
    this->target = target;

    for(GuiTweakPage* page : pages)
        page->open(target);

    show();
}

void GuiObjectTweak::onDraw(sf::RenderTarget& window)
{
    GuiPanel::onDraw(window);

    if (!target)
        hide();
}

GuiObjectTweakBase::GuiObjectTweakBase(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    // Left column
    (new GuiLabel(left_col, "", "ID:", 30))->setSize(GuiElement::GuiSizeMax, 50);

    callsign = new GuiTextEntry(left_col, "", "");
    callsign->setSize(GuiElement::GuiSizeMax, 50);
    callsign->callback([this](string text) {
        target->callsign = text;
    });

    // Edit object's description.
    // TODO: Fix long strings in GuiTextEntry, or make a new GUI element for
    // editing long strings.
    (new GuiLabel(left_col, "", "Description base:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    description = new GuiTextEntry(left_col, "", "");
    description->setSize(GuiElement::GuiSizeMax, 50);
    description->callback([this](string text) {
        target->object_description.not_scanned = text;
        //target->setDescription(text);
    });
    (new GuiLabel(left_col, "", "Description simple:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    description_scan = new GuiTextEntry(left_col, "", "");
    description_scan->setSize(GuiElement::GuiSizeMax, 50);
    description_scan->callback([this](string text) {
        target->object_description.simple_scan = text;
    });
    (new GuiLabel(left_col, "", "Description complete:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    description_full_scan = new GuiTextEntry(left_col, "", "");
    description_full_scan->setSize(GuiElement::GuiSizeMax, 50);
    description_full_scan->callback([this](string text) {
        target->object_description.full_scan = text;
    });

    (new GuiLabel(left_col, "", "Carlingue:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    hull_slider = new GuiSlider(left_col, "", 0.0, 500, 0.0, [this](float value) {
        target->hull = value;
    });
    hull_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", "Axe-Z:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    zaxis_slider = new GuiSlider(left_col, "", -100.0, 100, 0.0, [this](float value) {
        target->translate_z = value;
    });
    zaxis_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    // Right column
	// Radar signature
	//(new GuiLabel(right_col, "", "Radar Signature", 30))->setSize(GuiElement::GuiSizeMax, 50);
	(new GuiLabel(right_col, "", "Signature Gravitationnelle", 30))->setSize(GuiElement::GuiSizeMax, 50);
	gravity_s_slider = new GuiSlider(right_col, "", 0.0, 100.0, 0.0, [this](float value) {
        target->radar_signature.gravity = value / 100.0f;
    });
    gravity_s_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 50);
 	(new GuiLabel(right_col, "", "Signature Energetique", 30))->setSize(GuiElement::GuiSizeMax, 50);
	electrical_s_slider = new GuiSlider(right_col, "", 0.0, 100.0, 0.0, [this](float value) {
        target->radar_signature.electrical = value / 100.0f;
    });
    electrical_s_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 50);
 	(new GuiLabel(right_col, "", "Signature Biologique", 30))->setSize(GuiElement::GuiSizeMax, 50);
	biological_s_slider = new GuiSlider(right_col, "", 0.0, 100.0, 0.0, [this](float value) {
        target->radar_signature.biological = value / 100.0f;
    });
    biological_s_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 50);

    // Scanning complexity
	(new GuiLabel(right_col, "", "Complexite du Scan", 30))->setSize(GuiElement::GuiSizeMax, 50);
    scanning_complexity_selector = new GuiSlider(right_col, "", 0, 4, 0, [this](int value) {
        target->setScanningParameters(value,target->scanning_depth_value);
//        target->scanning_complexity_value = value;
    });
    scanning_complexity_selector->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
 	// Scanning Channel Depth
	(new GuiLabel(right_col, "", "Longueur du scan", 30))->setSize(GuiElement::GuiSizeMax, 50);
    scanning_channel_depth_selector = new GuiSlider(right_col, "", 0, 4, 0, [this](int value) {
        target->setScanningParameters(target->scanning_complexity_value,value);
//        target->scanning_depth_value = value;
    });
    scanning_channel_depth_selector->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

}

void GuiObjectTweakBase::onDraw(sf::RenderTarget& window)
{
    hull_slider->setValue(target->hull);

    scanning_complexity_selector->setValue(target->scanning_complexity_value);
	scanning_channel_depth_selector->setValue(target->scanning_depth_value);
}

void GuiObjectTweakBase::open(P<SpaceObject> target)
{
    this->target = target;

    callsign->setText(target->callsign);
    description->setText(target->object_description.not_scanned);
    description_scan->setText(target->object_description.simple_scan);
    description_full_scan->setText(target->object_description.full_scan);

    gravity_s_slider->setValue(target->radar_signature.gravity * 100.0f);
    electrical_s_slider->setValue(target->getRadarSignatureElectrical() * 100.0f);
	biological_s_slider->setValue(target->getRadarSignatureBiological() * 100.0f);

	scanning_complexity_selector->setValue(target->scanning_complexity_value);
	scanning_channel_depth_selector->setValue(target->scanning_depth_value);

	zaxis_slider->setValue(target->translate_z);
}

GuiTemplateTweak::GuiTemplateTweak(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);
    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    (new GuiLabel(left_col, "", "Direction:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    heading_slider = new GuiSlider(left_col, "", 0.0, 359.9, 0.0, [this](float value) {
        target->setHeading(value);

        // If the target is a player, also set its target rotation.
        P<PlayerSpaceship> player = target;
        if (player)
            player->commandTargetRotation(value - 90.0f);
    });
    heading_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    // Right column
    // Set type name. Does not change ship type.
    (new GuiLabel(right_col, "", "Type de vaisseau:", 30))->setSize(GuiElement::GuiSizeMax, 50);

    type_name = new GuiTextEntry(right_col, "", "");
    type_name->setSize(GuiElement::GuiSizeMax, 50);
    type_name->callback([this](string text) {
        target->setTypeName(text);
    });

    // Hull max and state sliders
    (new GuiLabel(right_col, "", "Carlingue max:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    hull_max_slider = new GuiSlider(right_col, "", 0.0, 2000, 0.0, [this](float value) {
        target->hull_max = round(value);
        target->hull_strength = std::min(target->hull_strength, target->hull_max);
    });
    hull_max_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", "Carlingue:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    hull_slider = new GuiSlider(right_col, "", 0.0, 2000, 0.0, [this](float value) {
        target->hull_strength = std::min(roundf(value), target->hull_max);
    });
    hull_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

   // Can be destroyed bool
   can_be_destroyed_toggle = new GuiToggleButton(right_col, "", "Peut etre detruit", [this](bool value) {
       target->setCanBeDestroyed(value);
   });
   can_be_destroyed_toggle->setSize(GuiElement::GuiSizeMax, 40);
}
 void GuiTemplateTweak::onDraw(sf::RenderTarget& window)
{
    heading_slider->setValue(target->getHeading());
    hull_slider->setValue(target->hull_strength);
}

 void GuiTemplateTweak::open(P<SpaceObject> target)
{
    P<ShipTemplateBasedObject> shipTemplate = target;
    this->target = shipTemplate;

    type_name->setText(shipTemplate->getTypeName());
    hull_max_slider->setValue(shipTemplate->hull_max);
    hull_max_slider->clearSnapValues()->addSnapValue(shipTemplate->ship_template->hull, 5.0f);
    can_be_destroyed_toggle->setValue(shipTemplate->getCanBeDestroyed());
}

GuiShipTweakShields::GuiShipTweakShields(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    for(int n=0; n<max_shield_count; n++)
    {
        (new GuiLabel(left_col, "", "Bouclier " + string(n + 1) + " max:", 20))->setSize(GuiElement::GuiSizeMax, 30);
        shield_max_slider[n] = new GuiSlider(left_col, "", 0.0, 1000, 0.0, [this, n](float value) {

            target->shield_max[n] = roundf(value);
            target->shield_level[n] = std::min(target->shield_level[n], target->shield_max[n]);

            int actual_shield_count = 0;
            for(int k=0; k<max_shield_count; k++)
                if (target->shield_max[k] > 0)
                    actual_shield_count = k+1;
            target->setShieldCount(actual_shield_count);
        });
        shield_max_slider[n]->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
    }

    for(int n=0; n<max_shield_count; n++)
    {
        (new GuiLabel(right_col, "", "Bouclier " + string(n + 1) + ":", 20))->setSize(GuiElement::GuiSizeMax, 30);
        shield_slider[n] = new GuiSlider(right_col, "", 0.0, 1000, 0.0, [this, n](float value) {
            target->shield_level[n] = std::min(roundf(value), target->shield_max[n]);
        });
        shield_slider[n]->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
    }
}

void GuiShipTweakShields::onDraw(sf::RenderTarget& window)
{
    for(int n=0; n<max_shield_count; n++)
    {
        shield_slider[n]->setValue(target->shield_level[n]);
    }
}

void GuiShipTweakShields::open(P<SpaceObject> target)
{
    P<SpaceShip> ship = target;
    P<SpaceStation> station = target;

    if (ship)
    {
        this->target = ship;

        for(int n = 0; n < max_shield_count; n++)
        {
            shield_max_slider[n]->setValue(ship->shield_max[n]);
            shield_max_slider[n]->clearSnapValues()->addSnapValue(ship->ship_template->shield_level[n], 5.0f);
        }
    }

    if (station)
        {
        this->target = station;

        for(int n = 0; n < max_shield_count; n++)
        {
            shield_max_slider[n]->setValue(station->shield_max[n]);
            shield_max_slider[n]->clearSnapValues()->addSnapValue(station->ship_template->shield_level[n], 5.0f);
        }
    }


}

GuiShipTweak::GuiShipTweak(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);
    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    // Left column
    (new GuiLabel(left_col, "", "Vitesse subluminique:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    impulse_speed_slider = new GuiSlider(left_col, "", 0.0, 250, 0.0, [this](float value) {
        target->impulse_max_speed = value;
    });
    impulse_speed_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", "Vitesse de rotation:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    turn_speed_slider = new GuiSlider(left_col, "", 0.0, 35, 0.0, [this](float value) {
        target->turn_speed = value;
    });
    turn_speed_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    // Display Boost/Strafe speed sliders
    (new GuiLabel(left_col, "", "Manoeuvre de combat avant:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    combat_maneuver_boost_speed_slider = new GuiSlider(left_col, "", 0.0, 1000, 0.0, [this](float value) {
        target->combat_maneuver_boost_speed = value;
    });
    combat_maneuver_boost_speed_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", "Manoeuvre de combat lateral:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    combat_maneuver_strafe_speed_slider = new GuiSlider(left_col, "", 0.0, 1000, 0.0, [this](float value) {
        target->combat_maneuver_strafe_speed = value;
    });
    combat_maneuver_strafe_speed_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    // Warp and jump drive toggles
    (new GuiLabel(left_col, "", "Moteurs speciaux:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    warp_toggle = new GuiToggleButton(left_col, "", "Moteur WARP", [this](bool value) {
        target->setWarpDrive(value);
    });
    warp_toggle->setSize(GuiElement::GuiSizeMax, 40);

    jump_toggle = new GuiToggleButton(left_col, "", "Moteur JUMP", [this](bool value) {
        target->setJumpDrive(value);
    });
    jump_toggle->setSize(GuiElement::GuiSizeMax, 40);

    reactor_toggle = new GuiToggleButton(left_col, "", "Reacteur", [this](bool value) {
        target->setHasReactor(value);
    });
    reactor_toggle->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", "JUMP, distance Min :", 30))->setSize(GuiElement::GuiSizeMax, 50);
    jump_drive_min_distance_slider = new GuiSlider(right_col, "", 0.0, 50000, 0.0, [this](float value) {
        target->jump_drive_min_distance = round(value / 1000) * 1000000;
        target->jump_drive_max_distance = std::max(target->jump_drive_min_distance, target->jump_drive_max_distance);
    });
    jump_drive_min_distance_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", "JUMP, distance Max :", 30))->setSize(GuiElement::GuiSizeMax, 50);
    jump_drive_max_distance_slider = new GuiSlider(right_col, "", 0.0, 100000, 0.0, [this](float value) {
        target->jump_drive_max_distance = round(value / 1000) * 1000000;
        target->jump_drive_min_distance = std::min(target->jump_drive_min_distance, target->jump_drive_max_distance);
   });
    jump_drive_max_distance_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", "JUMP, charge :", 30))->setSize(GuiElement::GuiSizeMax, 50);
    jump_drive_charge_slider = new GuiSlider(right_col, "", 0.0, 100.0, 0.0, [this](float value) {
        target->jump_drive_charge = value / 100.0 * target->jump_drive_max_distance;
    });
    jump_drive_charge_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", "Jump charge complete :", 30))->setSize(GuiElement::GuiSizeMax, 50);
    jump_drive_charge_time_slider = new GuiSlider(right_col, "", 0.0, 90*60, 0.0, [this](float value) {
        target->jump_drive_charge_time = value;
    });
    jump_drive_charge_time_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", "Jump, energie :", 30))->setSize(GuiElement::GuiSizeMax, 50);
    jump_drive_energy_slider = new GuiSlider(right_col, "", 0.0, 100, 0.0, [this](float value) {
        target->jump_drive_energy_per_km_charge = value;
    });
    jump_drive_energy_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", "Jump, delai :", 30))->setSize(GuiElement::GuiSizeMax, 50);
    jump_delay_slider = new GuiSlider(right_col, "", 0.0, 60*20, 0.0, [this](float value) {
        if (target->jump_delay > 0)
            target->jump_delay = value;
    });
    jump_delay_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

}
 void GuiShipTweak::onDraw(sf::RenderTarget& window)
{
    jump_drive_charge_slider->setValue(target->jump_drive_charge / target->jump_drive_max_distance * 100.0);
    jump_drive_min_distance_slider->setValue(round(target->jump_drive_min_distance / 1000000)*1000);
    jump_drive_max_distance_slider->setValue(round(target->jump_drive_max_distance / 1000000)*1000);
    jump_delay_slider->setValue(target->jump_delay);
}

 void GuiShipTweak::open(P<SpaceObject> target)
{
    P<SpaceShip> ship = target;
    this->target = ship;

    warp_toggle->setValue(ship->has_warp_drive);
    jump_toggle->setValue(ship->hasJumpDrive());
    reactor_toggle->setValue(ship->HasReactor());
    impulse_speed_slider->setValue(ship->impulse_max_speed);
    impulse_speed_slider->clearSnapValues()->addSnapValue(ship->ship_template->impulse_speed, 5.0f);
    turn_speed_slider->setValue(ship->turn_speed);
    turn_speed_slider->clearSnapValues()->addSnapValue(ship->ship_template->turn_speed, 1.0f);

    // Set and snap boost speed slider to current value
    combat_maneuver_boost_speed_slider->setValue(ship->combat_maneuver_boost_speed);
    combat_maneuver_boost_speed_slider->clearSnapValues()->addSnapValue(ship->combat_maneuver_boost_speed, 20.0f);

    // Set and snap strafe speed slider to current value
    combat_maneuver_strafe_speed_slider->setValue(ship->combat_maneuver_strafe_speed);
    combat_maneuver_strafe_speed_slider->clearSnapValues()->addSnapValue(ship->combat_maneuver_strafe_speed, 20.0f);

    jump_drive_charge_time_slider->setValue(ship->jump_drive_charge_time);
    jump_drive_energy_slider->setValue(ship->jump_drive_energy_per_km_charge);
}

GuiShipTweakMissileWeapons::GuiShipTweakMissileWeapons(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    // Left column
    (new GuiLabel(left_col, "", "Stockage max:", 30))->setSize(GuiElement::GuiSizeMax, 40);

    for(int n=0; n<MW_Count; n++)
    {
        (new GuiLabel(left_col, "", getMissileWeaponName(EMissileWeapons(n)) + ":", 20))->setSize(GuiElement::GuiSizeMax, 30);
        missile_storage_amount_slider[n] = new GuiSlider(left_col, "", 0.0, 50, 0.0, [this, n](float value) {
            target->weapon_storage_max[n] = int(round(value));
            target->weapon_storage[n] = std::min(target->weapon_storage[n], target->weapon_storage_max[n]);
        });
        missile_storage_amount_slider[n]->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
    }

    // Right column
    (new GuiLabel(right_col, "", "Stockage:", 30))->setSize(GuiElement::GuiSizeMax, 40);

    for(int n=0; n<MW_Count; n++)
    {
        (new GuiLabel(right_col, "", getMissileWeaponName(EMissileWeapons(n)) + ":", 20))->setSize(GuiElement::GuiSizeMax, 30);
        missile_current_amount_slider[n] = new GuiSlider(right_col, "", 0.0, 50, 0.0, [this, n](float value) {
            target->weapon_storage[n] = std::min(int(round(value)), target->weapon_storage_max[n]);
        });
        missile_current_amount_slider[n]->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
    }
}

void GuiShipTweakMissileWeapons::onDraw(sf::RenderTarget& window)
{
    for(int n=0; n<MW_Count; n++)
    {
        if (target->weapon_storage[n] != int(missile_current_amount_slider[n]->getValue()))
            missile_current_amount_slider[n]->setValue(float(target->weapon_storage[n]));
    }
}

void GuiShipTweakMissileWeapons::open(P<SpaceObject> target)
{
    P<SpaceShip> ship = target;
    this->target = ship;

    for(int n = 0; n < MW_Count; n++)
        missile_storage_amount_slider[n]->setValue(float(ship->weapon_storage_max[n]));
}

GuiShipTweakMissileTubes::GuiShipTweakMissileTubes(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    // Left column
    (new GuiLabel(left_col, "", "Nombre de tubes:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    missile_tube_amount_selector = new GuiSelector(left_col, "", [this](int index, string value) {
        target->weapon_tube_count = index;
    });
    for(int n=0; n<max_weapon_tubes; n++)
        missile_tube_amount_selector->addEntry(string(n), "");
    missile_tube_amount_selector->setSize(GuiElement::GuiSizeMax, 40);

    // Right column
    tube_index = 0;
    index_selector = new GuiSelector(right_col, "", [this](int index, string value)
    {
        if (index >= target->weapon_tube_count)
        {
            if (index == max_weapon_tubes - 1)
                index = std::max(0, target->weapon_tube_count - 1);
            else
                index = 0;
            index_selector->setSelectionIndex(index);
        }
        tube_index = index;
    });
    index_selector->setSize(GuiElement::GuiSizeMax, 40);
    for(int n=0; n<max_weapon_tubes; n++)
        index_selector->addEntry("Tube: " + string(n + 1), "");
    index_selector->setSelectionIndex(0);

    (new GuiLabel(right_col, "", "Direction:", 20))->setSize(GuiElement::GuiSizeMax, 30);
    direction_slider = new GuiSlider(right_col, "", -180.0, 180, 0.0, [this](float value) {
        target->weapon_tube[tube_index].setDirection(roundf(value));
    });
    direction_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", "Temps de chargement:", 20))->setSize(GuiElement::GuiSizeMax, 30);
    load_time_slider = new GuiSlider(right_col, "", 0.0, 60.0, 0.0, [this](float value) {
        target->weapon_tube[tube_index].setLoadTimeConfig(roundf(value * 10) / 10);
    });
    load_time_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", "Autorisations:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    for(int n=0; n<MW_Count; n++)
    {
        allowed_use[n] = new GuiToggleButton(right_col, "", getMissileWeaponName(EMissileWeapons(n)), [this, n](bool value) {
            if (value)
                target->weapon_tube[tube_index].allowLoadOf(EMissileWeapons(n));
            else
                target->weapon_tube[tube_index].disallowLoadOf(EMissileWeapons(n));
        });
        allowed_use[n]->setSize(GuiElement::GuiSizeMax, 40);
    }
}

void GuiShipTweakMissileTubes::onDraw(sf::RenderTarget& window)
{
    direction_slider->setValue(sf::angleDifference(0.0f, target->weapon_tube[tube_index].getDirection()));
    load_time_slider->setValue(target->weapon_tube[tube_index].getLoadTimeConfig());
    for(int n=0; n<MW_Count; n++)
    {
        allowed_use[n]->setValue(target->weapon_tube[tube_index].canLoad(EMissileWeapons(n)));
    }
}

void GuiShipTweakMissileTubes::open(P<SpaceObject> target)
{
    P<SpaceShip> ship = target;
    this->target = ship;

    missile_tube_amount_selector->setSelectionIndex(ship->weapon_tube_count);
}

GuiShipTweakBeamweapons::GuiShipTweakBeamweapons(GuiContainer* owner)
: GuiTweakPage(owner)
{
    beam_index = 0;

    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);
    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    GuiSelector* index_selector = new GuiSelector(left_col, "", [this](int index, string value)
    {
        beam_index = index;
    });
    index_selector->setSize(GuiElement::GuiSizeMax, 40);
    for(int n=0; n<max_beam_weapons; n++)
        index_selector->addEntry("Laser: " + string(n + 1), "");
    index_selector->setSelectionIndex(0);

    (new GuiLabel(right_col, "", "Arc de tir:", 20))->setSize(GuiElement::GuiSizeMax, 30);
    arc_slider = new GuiSlider(right_col, "", 0.0, 360.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setArc(roundf(value));
    });
    arc_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(right_col, "", "Direction:", 20))->setSize(GuiElement::GuiSizeMax, 30);
    direction_slider = new GuiSlider(right_col, "", -180.0, 180.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setDirection(roundf(value));
    });
    direction_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(right_col, "", "Arc de portee:", 20))->setSize(GuiElement::GuiSizeMax, 30);
    turret_arc_slider = new GuiSlider(right_col, "", 0.0, 360.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setTurretArc(roundf(value));
    });
    turret_arc_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(right_col, "", "Direction:", 20))->setSize(GuiElement::GuiSizeMax, 30);
    turret_direction_slider = new GuiSlider(right_col, "", -180.0, 180.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setTurretDirection(roundf(value));
    });
    turret_direction_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(right_col, "", "Rotation du laser:", 20))->setSize(GuiElement::GuiSizeMax, 30);
    // 25 is an arbitrary limit to add granularity; values greater than 25
    // result in practicaly instantaneous turret rotation anyway.
    turret_rotation_rate_slider = new GuiSlider(right_col, "", 0.0, 250.0, 0.0, [this](float value) {
        // Divide a large value for granularity.
        if (value > 0)
            target->beam_weapons[beam_index].setTurretRotationRate(value / 10.0);
        else
            target->beam_weapons[beam_index].setTurretRotationRate(0.0);
    });
    turret_rotation_rate_slider->setSize(GuiElement::GuiSizeMax, 30);
    // Override overlay label.
    turret_rotation_rate_overlay_label = new GuiLabel(turret_rotation_rate_slider, "", "", 30);
    turret_rotation_rate_overlay_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    (new GuiLabel(right_col, "", "Portee:", 20))->setSize(GuiElement::GuiSizeMax, 30);
    range_slider = new GuiSlider(right_col, "", 0.0, 5000.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setRange(roundf(value / 100) * 100);
    });
    range_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(right_col, "", "Vitesse de tir:", 20))->setSize(GuiElement::GuiSizeMax, 30);
    cycle_time_slider = new GuiSlider(right_col, "", 0.1, 20.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setCycleTime(value);
    });
    cycle_time_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(right_col, "", "Degat:", 20))->setSize(GuiElement::GuiSizeMax, 30);
    damage_slider = new GuiSlider(right_col, "", 0.1, 50.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setDamage(value);
    });
    damage_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);
}

void GuiShipTweakBeamweapons::onDraw(sf::RenderTarget& window)
{
    target->drawOnRadar(window, sf::Vector2f(rect.left - 150.0f + rect.width / 2.0f, rect.top + rect.height * 0.66), 300.0f / 5000.0f, false);

    arc_slider->setValue(target->beam_weapons[beam_index].getArc());
    direction_slider->setValue(sf::angleDifference(0.0f, target->beam_weapons[beam_index].getDirection()));
    range_slider->setValue(target->beam_weapons[beam_index].getRange());
    turret_arc_slider->setValue(target->beam_weapons[beam_index].getTurretArc());
    turret_direction_slider->setValue(sf::angleDifference(0.0f, target->beam_weapons[beam_index].getTurretDirection()));
    turret_rotation_rate_slider->setValue(target->beam_weapons[beam_index].getTurretRotationRate() * 10.0f);
    turret_rotation_rate_overlay_label->setText(string(target->beam_weapons[beam_index].getTurretRotationRate()));
    cycle_time_slider->setValue(target->beam_weapons[beam_index].getCycleTime());
    damage_slider->setValue(target->beam_weapons[beam_index].getDamage());
}

void GuiShipTweakBeamweapons::open(P<SpaceObject> target)
{
    P<SpaceShip> ship = target;
    this->target = ship;
}

GuiShipTweakSystems::GuiShipTweakSystems(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* col_1 = new GuiAutoLayout(this, "LAYOUT_1", GuiAutoLayout::LayoutVerticalTopToBottom);
    col_1->setPosition(50, 25, ATopLeft)->setSize(150, GuiElement::GuiSizeMax);
    GuiAutoLayout* col_2 = new GuiAutoLayout(this, "LAYOUT_2", GuiAutoLayout::LayoutVerticalTopToBottom);
    col_2->setPosition(-75, 25, ATopCenter)->setSize(150, GuiElement::GuiSizeMax);
    GuiAutoLayout* col_3 = new GuiAutoLayout(this, "LAYOUT_3", GuiAutoLayout::LayoutVerticalTopToBottom);
    col_3->setPosition(100, 25, ATopCenter)->setSize(150, GuiElement::GuiSizeMax);
    GuiAutoLayout* col_4 = new GuiAutoLayout(this, "LAYOUT_4", GuiAutoLayout::LayoutVerticalTopToBottom);
    col_4->setPosition(-25, 25, ATopRight)->setSize(150, GuiElement::GuiSizeMax);

    (new GuiLabel(col_1, "", "etat", 20))->setSize(GuiElement::GuiSizeMax, 30);
    (new GuiLabel(col_2, "", "max", 20))->setSize(GuiElement::GuiSizeMax, 30);
    (new GuiLabel(col_3, "", "chaleur", 20))->setSize(GuiElement::GuiSizeMax, 30);
    (new GuiLabel(col_4, "", "Hack", 20))->setSize(GuiElement::GuiSizeMax, 30);

    for(int n=0; n<SYS_COUNT; n++)
    {
        ESystem system = ESystem(n);

        system_name[n] = new GuiLabel(col_1, "", getSystemName(system), 20);
        system_name[n]->setSize(GuiElement::GuiSizeMax, 30);

        temp_1[n] = new GuiLabel(col_2, "", " ", 20);
        temp_1[n]->setSize(GuiElement::GuiSizeMax, 30);

        temp_2[n] = new GuiLabel(col_3, "", " ", 20);
        temp_2[n]->setSize(GuiElement::GuiSizeMax, 30);

        temp_3[n] = new GuiLabel(col_4, "", " ", 20);
        temp_3[n]->setSize(GuiElement::GuiSizeMax, 30);

        system_damage[n] = new GuiSlider(col_1, "", -1.0, 1.0, 0.0, [this, n](float value) {
            target->systems[n].health = std::min(value,target->systems[n].health_max);
        });
        system_damage[n]->setSize(GuiElement::GuiSizeMax, 30);
        system_damage[n]->addSnapValue(-1.0, 0.01);
        system_damage[n]->addSnapValue( 0.0, 0.01);
        system_damage[n]->addSnapValue( 1.0, 0.01);

        system_health[n] = new GuiSlider(col_2, "", 0.0, 1.0, 0.0, [this, n](float value) {
            target->systems[n].health_max = value;
            target->systems[n].health = std::min(value,target->systems[n].health);
        });
        system_health[n]->setSize(GuiElement::GuiSizeMax, 30);
        system_health[n]->setPosition(0,30);
        system_health[n]->addSnapValue( 0.0, 0.01);
        system_health[n]->addSnapValue( 1.0, 0.01);

        system_heat[n] = new GuiSlider(col_3, "", 0.0, 1.0, 0.0, [this, n](float value) {
            target->systems[n].heat_level = value;
        });
        system_heat[n]->setSize(GuiElement::GuiSizeMax, 30);
        system_heat[n]->addSnapValue( 0.0, 0.01);
        system_heat[n]->addSnapValue( 1.0, 0.01);

        system_hack[n] = new GuiSlider(col_4, "", 0.0, 1.0, 0.0, [this, n](float value) {
            target->systems[n].hacked_level = value;
        });
        system_hack[n]->setSize(GuiElement::GuiSizeMax, 30);
        system_hack[n]->addSnapValue( 0.0, 0.01);
        system_hack[n]->addSnapValue( 1.0, 0.01);
    }
}

void GuiShipTweakSystems::onDraw(sf::RenderTarget& window)
{
    for(int n=0; n<SYS_COUNT; n++)
    {
        system_damage[n]->setValue(target->systems[n].health);
        system_health[n]->setValue(target->systems[n].health_max);
        system_heat[n]->setValue(target->systems[n].heat_level);
        system_hack[n]->setValue(target->systems[n].hacked_level);

        if (!target->hasSystem(ESystem(n)))
        {
            system_name[n]->hide();
            temp_1[n]->hide();
            temp_2[n]->hide();
            temp_3[n]->hide();
            system_damage[n]->hide();
            system_health[n]->hide();
            system_heat[n]->hide();
            system_hack[n]->hide();
        }
        else
        {
            system_name[n]->show();
            temp_1[n]->show();
            temp_2[n]->show();
            temp_3[n]->show();
            system_damage[n]->show();
            system_health[n]->show();
            system_heat[n]->show();
            system_hack[n]->show();
        }
    }
}

void GuiShipTweakSystems::open(P<SpaceObject> target)
{
    P<SpaceShip> ship = target;
    this->target = ship;
}

GuiShipTweakPlayer::GuiShipTweakPlayer(GuiContainer* owner)
: GuiTweakPage(owner)
{
    // TODO: Add more player ship tweaks here.
    // -   Ship-to-ship player transfer
    // -   Reputation

    // Add two columns.
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    // Left column
    // Edit control code.
    (new GuiLabel(left_col, "", "code de securite:", 30))->setSize(GuiElement::GuiSizeMax, 50);

    control_code = new GuiTextEntry(left_col, "", "");
    control_code->setSize(GuiElement::GuiSizeMax, 50);
    control_code->callback([this](string text) {
        target->control_code = text;
    });

    // Edit reputation.
    (new GuiLabel(left_col, "", "Nombre de droides:", 30))->setSize(GuiElement::GuiSizeMax, 50);
     repair_team_slider = new GuiSlider(left_col, "", 0.0, 15, 0.0, [this](float value) {
        target->setRepairCrewCount(value);
    });
    repair_team_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    // Edit oxygen.
    (new GuiLabel(left_col, "", "Oxygen:", 30))->setSize(GuiElement::GuiSizeMax, 50);
     oxygen_point_slider = new GuiSlider(left_col, "", 0.0, 500.0, 0.0, [this](float value) {
//        target->setOxygenPoints(value);
        target->oxygen_points = std::min(value, target->oxygen_max);
    });
    oxygen_point_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    // Edit oxygen.
    (new GuiLabel(left_col, "", "Oxygen Max:", 30))->setSize(GuiElement::GuiSizeMax, 50);
     max_oxygen_point_slider = new GuiSlider(left_col, "", 0.0, 500.0, 0.0, [this](float value) {
        target->setOxygenMax(value);
        target->oxygen_points = std::min(target->oxygen_points, target->oxygen_max);
    });
    max_oxygen_point_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    // Edit energy level.
    (new GuiLabel(left_col, "", "Energie max:", 30))->setSize(GuiElement::GuiSizeMax, 50);

    max_energy_level_slider = new GuiSlider(left_col, "", 0.0, 5000, 0.0, [this](float value) {
        target->max_energy_level = value;
        target->energy_level = std::min(target->energy_level, target->max_energy_level);
    });
    max_energy_level_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", "Energie:", 30))->setSize(GuiElement::GuiSizeMax, 50);

    energy_level_slider = new GuiSlider(left_col, "", 0.0, 5000, 0.0, [this](float value) {
        target->energy_level = std::min(value, target->max_energy_level);
    });
    energy_level_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    // Right column

    // Radar Capabilities
    (new GuiLabel(right_col, "", "Radar Capabilities:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    gravity_toggle = new GuiToggleButton(right_col, "", "Gravity Sensor", [this](bool value) {
        target->setGravitySensor(value);
    });
    gravity_toggle->setSize(GuiElement::GuiSizeMax, 40);
    electrical_toggle = new GuiToggleButton(right_col, "", "Electrical Sensor", [this](bool value) {
        target->setElectricalSensor(value);
    });
    electrical_toggle->setSize(GuiElement::GuiSizeMax, 40);
    biological_toggle = new GuiToggleButton(right_col, "", "Biological Sensor", [this](bool value) {
        target->setBiologicalSensor(value);
    });
    biological_toggle->setSize(GuiElement::GuiSizeMax, 40);

    // Count and list ship positions and whether they're occupied.
    position_count = new GuiLabel(right_col, "", "Postes occupes: ", 30);
    position_count->setSize(GuiElement::GuiSizeMax, 50);

    for(int n = 0; n < max_crew_positions; n++)
    {
        string position_name = getCrewPositionName(ECrewPosition(n));

        position[n] = new GuiKeyValueDisplay(right_col, "CREW_POSITION_" + position_name, 0.5, position_name, "-");
        position[n]->setSize(GuiElement::GuiSizeMax, 30);
    }
}

void GuiShipTweakPlayer::onDraw(sf::RenderTarget& window)
{
    // Update position list.
    int position_counter = 0;

    // Update the status of each crew position.
    for(int n = 0; n < max_crew_positions; n++)
    {
        string position_name = getCrewPositionName(ECrewPosition(n));
        string position_state = "-";

        if (target->hasPlayerAtPosition(ECrewPosition(n)))
        {
            position_state = "Occupe";
            position_counter += 1;
        }

        position[n]->setValue(position_state);
    }

    // Update the total occupied position count.
    position_count->setText("Postes occupes: " + string(position_counter));

    // Update the ship's energy level.
    energy_level_slider->setValue(target->energy_level);
    max_energy_level_slider->setValue(target->max_energy_level);

    // Update reputation points.
    repair_team_slider->setValue(target->getRepairCrewCount());

    // Update oxygen points.
    oxygen_point_slider->setValue(target->getOxygenPoints());
    max_oxygen_point_slider->setValue(target->getOxygenMax());
}

void GuiShipTweakPlayer::open(P<SpaceObject> target)
{
    P<PlayerSpaceship> player = target;
    this->target = player;

    if (player)
    {
        // Read ship's control code.
        control_code->setText(player->control_code);

        gravity_toggle->setValue(player->has_gravity_sensor);
        electrical_toggle->setValue(player->has_electrical_sensor);
        biological_toggle->setValue(player->has_biological_sensor);
    }
}

GuiShipTweakMessages::GuiShipTweakMessages(GuiContainer* owner)
: GuiTweakPage(owner)
{
    color_message = sf::Color::White;
    type_log = "extern";
    message = "";

    (new GuiLabel(this, "", "Message :", 30))->setPosition(0, 30, ATopCenter);
    message_entry = new GuiTextEntry(this, "", "");
    message_entry->setSize(650, 50);
    message_entry->setPosition(0, 50, ATopCenter);
    message_entry->callback([this](string text) {
        message = text;
    });

    (new GuiButton(this, "", "Effacer message", [this]() {
        message_entry -> setText("");
        message = "";
    }))->setPosition(0, 100, ATopCenter)->setSize(300, 50);

    // Add two columns.
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 150, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 150, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    // Left column

    (new GuiLabel(left_col, "", "Dans le log", 30))->setSize(GuiElement::GuiSizeMax, 100);

    // Choose the color
    (new GuiLabel(left_col, "", "Couleur :", 30))->setSize(GuiElement::GuiSizeMax, 50);
    GuiSelector* color_selector = new GuiSelector(left_col, "", [this](int index, string value)
    {
        color_message = sf::Color::White;
        if (value == "white")
            color_message = sf::Color::White;
        if (value == "black")
            color_message = sf::Color::Black;
        if (value == "red")
            color_message = sf::Color::Red;
        if (value == "green")
            color_message = sf::Color::Green;
        if (value == "blue")
            color_message = sf::Color::Blue;
        if (value == "yellow")
            color_message = sf::Color::Yellow;
        if (value == "magenta")
            color_message = sf::Color::Magenta;
        if (value == "cyan")
            color_message = sf::Color::Cyan;
    });
    color_selector->setSize(GuiElement::GuiSizeMax, 40);
    color_selector->addEntry("white", "white");
    color_selector->addEntry("black", "black");
    color_selector->addEntry("red", "red");
    color_selector->addEntry("green", "green");
    color_selector->addEntry("blue", "blue");
    color_selector->addEntry("yellow", "yellow");
    color_selector->addEntry("magenta", "magenta");
    color_selector->addEntry("cyan", "cyan");
    color_selector->setSelectionIndex(0);

    // Choose the color
    (new GuiLabel(left_col, "", "Log:", 30))->setSize(GuiElement::GuiSizeMax, 50);
    GuiSelector* log_selector = new GuiSelector(left_col, "", [this](int index, string value)
    {
        type_log = value;
    });
    log_selector->setSize(GuiElement::GuiSizeMax, 40);
    log_selector->addEntry("extern", "extern");
    log_selector->addEntry("intern", "intern");
    log_selector->setSelectionIndex(0);

    // Send the message
    send_message_log = new GuiButton(left_col, "", "Envoyer message", [this]() {
       target -> addToShipLog(message,color_message,type_log);
    });
    send_message_log->setSize(GuiElement::GuiSizeMax, 40);

    // Right column

    (new GuiLabel(right_col, "", "Sur un ecran", 30))->setSize(GuiElement::GuiSizeMax, 100);

    (new GuiButton(right_col, "", "Pilote", [this]() {
        target->addCustomMessage(helmsOfficer,"helms_message", message);
    }))->setSize(GuiElement::GuiSizeMax, 50);
    (new GuiButton(right_col, "", "Artilleur", [this]() {
        target->addCustomMessage(weaponsOfficer,"weapons_message", message);
    }))->setSize(GuiElement::GuiSizeMax, 50);
    (new GuiButton(right_col, "", "Ingenieur", [this]() {
        target->addCustomMessage(engineering,"engineering_message", message);
    }))->setSize(GuiElement::GuiSizeMax, 50);
    (new GuiButton(right_col, "", "Analyste", [this]() {
        target->addCustomMessage(scienceOfficer,"science_message", message);
    }))->setSize(GuiElement::GuiSizeMax, 50);
    (new GuiButton(right_col, "", "Relai", [this]() {
        target->addCustomMessage(relayOfficer,"relay_message", message);
    }))->setSize(GuiElement::GuiSizeMax, 50);

    (new GuiButton(right_col, "", "Retirer message ecran", [this]() {
        target->removeCustom("helms_message");
        target->removeCustom("weapons_message");
        target->removeCustom("engineering_message");
        target->removeCustom("science_message");
        target->removeCustom("relay_message");

        message_entry -> setText("");
    }))->setSize(GuiElement::GuiSizeMax, 50);

}

void GuiShipTweakMessages::onDraw(sf::RenderTarget& window)
{

}

void GuiShipTweakMessages::open(P<SpaceObject> target)
{
    P<PlayerSpaceship> player = target;
    this->target = player;

    if (player)
    {

    }
}
