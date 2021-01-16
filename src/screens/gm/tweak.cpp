#include <i18n.h>
#include "tweak.h"
#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "spaceObjects/spaceship.h"

#include "gui/gui2_listbox.h"
#include "gui/gui2_autolayout.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_label.h"
#include "gui/gui2_textentry.h"
#include "gui/gui2_selector.h"
#include "gui/gui2_slider.h"
#include "gui/gui2_togglebutton.h"
#include "gui/gui2_button.h"
#include "gui/gui2_progressbar.h"

GuiObjectTweak::GuiObjectTweak(GuiContainer* owner, ETweakType tweak_type)
: GuiPanel(owner, "GM_TWEAK_DIALOG")
{
    setPosition(0, -100, ABottomCenter);
    setSize(1000, 700);

    GuiListbox* list = new GuiListbox(this, "", [this](int index, string value)
    {
        for(GuiTweakPage* page : pages)
            page->hide();
        pages[index]->show();
    });

    list->setSize(300, GuiElement::GuiSizeMax);
    list->setPosition(25, 25, ATopLeft);

    pages.push_back(new GuiObjectTweakBase(this));
    list->addEntry(tr("tab", "Base"), "");

    pages.push_back(new GuiShipTweakDescription(this));
    list->addEntry("Description", "");

    if (tweak_type == TW_Ship || tweak_type == TW_Player)
    {
        pages.push_back(new GuiTweakShip(this));
        list->addEntry(tr("tab", "Ship"), "");
    }

    if (tweak_type == TW_Asteroid)
    {
        pages.push_back(new GuiAsteroidTweak(this));
        list->addEntry(tr("tab","Asteroid"), "");
    }

    if (tweak_type == TW_Jammer)
    {
        pages.push_back(new GuiJammerTweak(this));
        list->addEntry(tr("tab", "Jammer"), "");
    }

    if (tweak_type == TW_Ship || tweak_type == TW_Player || tweak_type == TW_Station)
    {
        pages.push_back(new GuiShipTweakShields(this));
        list->addEntry(tr("tab", "Shields"), "");
    }

    if (tweak_type == TW_Ship || tweak_type == TW_Player)
    {
        pages.push_back(new GuiShipTweakMissileTubes(this));
        list->addEntry(tr("tab", "Tubes"), "");
        pages.push_back(new GuiShipTweakMissileWeapons(this));
        list->addEntry(tr("tab", "Missiles"), "");
        pages.push_back(new GuiShipTweakBeamweapons(this));
        list->addEntry(tr("tab", "Beams"), "");
        pages.push_back(new GuiShipTweakSystems(this));
        list->addEntry(tr("tab", "Systems"), "");
        pages.push_back(new GuiShipTweakOxygen(this));
        list->addEntry(tr("tab", "Oxygen"), "");
    }

    if (tweak_type == TW_Player)
    {
        pages.push_back(new GuiShipTweakPlayer(this));
        list->addEntry(tr("tab", "Player"), "");
        pages.push_back(new GuiShipTweakPlayer2(this));
        list->addEntry(tr("tab", "Player 2"), "");
        pages.push_back(new GuiShipTweakMessages(this));
        list->addEntry(tr("tab", "Messages"), "");
    }

    for(GuiTweakPage* page : pages)
    {
        page->setSize(700, 700)->setPosition(0, 0, ABottomRight)->hide();
    }

    pages[0]->show();
    list->setSelectionIndex(0);

    (new GuiButton(this, "CLOSE_BUTTON", tr("button", "Close"), [this]() {
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

GuiTweakShip::GuiTweakShip(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    // Left column
    (new GuiLabel(left_col, "", tr("Impulse speed:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    impulse_speed_slider = new GuiSlider(left_col, "", 0.0, 250, 0.0, [this](float value) {
        target->impulse_max_speed = value;
    });
    impulse_speed_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", tr("WARP speed:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    warp_speed_slider = new GuiSlider(left_col, "", 500.0, 2000.0, 0.0, [this](float value) {
        target->warp_speed_per_warp_level = value;
    });
    warp_speed_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", tr("Turn speed:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    turn_speed_slider = new GuiSlider(left_col, "", 0.0, 35, 0.0, [this](float value) {
        target->turn_speed = value;
    });
    turn_speed_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", tr("Jump Min Distance:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    jump_min_distance_slider= new GuiSlider(left_col, "", 0.0, 100000, 0.0, [this](float value) {
        target->setJumpDriveRange(value,target->jump_drive_max_distance);
    });
    jump_min_distance_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", tr("Jump Max Distance:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    jump_max_distance_slider= new GuiSlider(left_col, "", 0.0, 100000, 0.0, [this](float value) {
        target->setJumpDriveRange(target->jump_drive_min_distance,value);
    });
    jump_max_distance_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", tr("Jump charge:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    jump_charge_slider = new GuiSlider(left_col, "", 0.0, 100000, 0.0, [this](float value) {
        target->setJumpDriveCharge(value);
    });
    jump_charge_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    // Right column
    // Set type name. Does not change ship type.
    (new GuiLabel(right_col, "", tr("Type name:"), 30))->setSize(GuiElement::GuiSizeMax, 50);

    type_name = new GuiTextEntry(right_col, "", "");
    type_name->setSize(GuiElement::GuiSizeMax, 50);
    type_name->callback([this](string text) {
        target->setTypeName(text);
    });

    // Hull max and state sliders
    (new GuiLabel(right_col, "", tr("Hull max:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    hull_max_slider = new GuiSlider(right_col, "", 0.0, 500, 0.0, [this](float value) {
        target->hull_max = round(value);
        target->hull_strength = std::min(target->hull_strength, target->hull_max);
    });
    hull_max_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", tr("Hull current:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    hull_slider = new GuiSlider(right_col, "", 0.0, 500, 0.0, [this](float value) {
        target->hull_strength = std::min(roundf(value), target->hull_max);
    });
    hull_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

   // Can be destroyed bool
   can_be_destroyed_toggle = new GuiToggleButton(right_col, "", tr("Could be destroyed"), [this](bool value) {
       target->setCanBeDestroyed(value);
   });
   can_be_destroyed_toggle->setSize(GuiElement::GuiSizeMax, 40);

    // Warp and jump drive toggles
    (new GuiLabel(right_col, "", tr("Special drives:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    reactor_toggle = new GuiToggleButton(right_col, "", tr("Reactor Drive"), [this](bool value) {
        target->setReactor(value);
    });
    reactor_toggle->setSize(GuiElement::GuiSizeMax, 40);

    oxygen_generator_toggle = new GuiToggleButton(right_col, "", tr("Oxygen Generator"), [this](bool value) {
        target->setOxygenGenerator(value);
    });
    oxygen_generator_toggle->setSize(GuiElement::GuiSizeMax, 40);

    warp_toggle = new GuiToggleButton(right_col, "", tr("Warp Drive"), [this](bool value) {
        target->setWarpDrive(value);
    });
    warp_toggle->setSize(GuiElement::GuiSizeMax, 40);

    jump_toggle = new GuiToggleButton(right_col, "", tr("Jump Drive"), [this](bool value) {
        target->setJumpDrive(value);
    });
    jump_toggle->setSize(GuiElement::GuiSizeMax, 40);
}

void GuiTweakShip::onDraw(sf::RenderTarget& window)
{
    hull_slider->setValue(target->hull_strength);
    jump_charge_slider->setValue(target->getJumpDriveCharge());
    jump_min_distance_slider->setValue(target->jump_drive_min_distance);
    jump_max_distance_slider->setValue(target->jump_drive_max_distance);
    type_name->setText(target->getTypeName());
    reactor_toggle->setValue(target->has_reactor);
    oxygen_generator_toggle->setValue(target->has_oxygen_generator);
    warp_toggle->setValue(target->has_warp_drive);
    jump_toggle->setValue(target->hasJumpDrive());
    impulse_speed_slider->setValue(target->impulse_max_speed);
    warp_speed_slider->setValue(target->warp_speed_per_warp_level);
    turn_speed_slider->setValue(target->turn_speed);
    hull_max_slider->setValue(target->hull_max);
    can_be_destroyed_toggle->setValue(target->getCanBeDestroyed());
}

void GuiTweakShip::open(P<SpaceObject> target)
{
    P<SpaceShip> ship = target;
    this->target = ship;

    impulse_speed_slider->clearSnapValues()->addSnapValue(ship->ship_template->impulse_speed, 5.0f);
    warp_speed_slider->clearSnapValues()->addSnapValue(ship->ship_template->warp_speed, 50.0f);
    turn_speed_slider->clearSnapValues()->addSnapValue(ship->ship_template->turn_speed, 1.0f);
    hull_max_slider->clearSnapValues()->addSnapValue(ship->ship_template->hull, 5.0f);
}

GuiShipTweakMissileWeapons::GuiShipTweakMissileWeapons(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    // Left column
    (new GuiLabel(left_col, "", tr("missiles", "Storage space:"), 30))->setSize(GuiElement::GuiSizeMax, 40);

    for(int n=0; n<MW_Count; n++)
    {
        (new GuiLabel(left_col, "", getLocaleMissileWeaponName(EMissileWeapons(n)) + ":", 20))->setSize(GuiElement::GuiSizeMax, 30);
        missile_storage_amount_slider[n] = new GuiSlider(left_col, "", 0.0, 50, 0.0, [this, n](float value) {
            target->weapon_storage_max[n] = int(round(value));
            target->weapon_storage[n] = std::min(target->weapon_storage[n], target->weapon_storage_max[n]);
        });
        missile_storage_amount_slider[n]->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
    }

    // Right column
    (new GuiLabel(right_col, "", tr("missiles", "Stored amount:"), 30))->setSize(GuiElement::GuiSizeMax, 40);

    for(int n=0; n<MW_Count; n++)
    {
        (new GuiLabel(right_col, "", getLocaleMissileWeaponName(EMissileWeapons(n)) + ":", 20))->setSize(GuiElement::GuiSizeMax, 30);
        missile_current_amount_slider[n] = new GuiSlider(right_col, "", 0.0, 50, 0.0, [this, n](float value) {
            target->weapon_storage[n] = std::min(int(round(value)), target->weapon_storage_max[n]);
        });
        missile_current_amount_slider[n]->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
    }
}

GuiJammerTweak::GuiJammerTweak(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    (new GuiLabel(left_col, "", tr("Jammer Range:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    jammer_range_slider = new GuiSlider(left_col, "", 0, 50000, 0, [this](float value) {
        target->setRange(round(value/100)*100);
    });
    jammer_range_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
}

void GuiJammerTweak::open(P<SpaceObject> target)
{
    P<WarpJammer> jammer = target;
    this->target = jammer;
}

void GuiJammerTweak::onDraw(sf::RenderTarget& window)
{
    jammer_range_slider->setValue(target->getRange());
}

GuiAsteroidTweak::GuiAsteroidTweak(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    (new GuiLabel(left_col, "", tr("Asteroid Size:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    asteroid_size_slider = new GuiSlider(left_col, "", 10, 500, 0, [this](float value) {
        target->setSize(value);
    });
    asteroid_size_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
}

void GuiAsteroidTweak::open(P<SpaceObject> target)
{
    P<Asteroid> asteroid = target;
    this->target = asteroid;
}

void GuiAsteroidTweak::onDraw(sf::RenderTarget& window)
{
    asteroid_size_slider->setValue(target->getSize());
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
    (new GuiLabel(left_col, "", tr("Tube count:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
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
        index_selector->addEntry(tr("Tube: {id_tube}").format({{"id_tube", string(n + 1)}}), "");
    index_selector->setSelectionIndex(0);

    (new GuiLabel(right_col, "", tr("tube", "Direction:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
    direction_slider = new GuiSlider(right_col, "", -180.0, 180, 0.0, [this](float value) {
        target->weapon_tube[tube_index].setDirection(roundf(value));
    });
    direction_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", tr("tube", "Load time:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
    load_time_slider = new GuiSlider(right_col, "", 0.0, 60.0, 0.0, [this](float value) {
        target->weapon_tube[tube_index].setLoadTimeConfig(roundf(value * 10) / 10);
    });
    load_time_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", tr("tube", "Size:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
    size_selector=new GuiSelector(right_col, "", [this](int index, string value)
    {
        target->weapon_tube[tube_index].setSize(EMissileSizes(index));
    });
    size_selector->addEntry(tr("tube", "Small"),MS_Small);
    size_selector->addEntry(tr("tube", "Medium"),MS_Medium);
    size_selector->addEntry(tr("tube", "large"),MS_Large);
    size_selector->setSelectionIndex(MS_Medium);
    size_selector->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", tr("tube", "Allowed use:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    for(int n=0; n<MW_Count; n++)
    {
        allowed_use[n] = new GuiToggleButton(right_col, "", getLocaleMissileWeaponName(EMissileWeapons(n)), [this, n](bool value) {
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
    size_selector->setSelectionIndex(target->weapon_tube[tube_index].getSize());
}

void GuiShipTweakMissileTubes::open(P<SpaceObject> target)
{
    P<SpaceShip> ship = target;
    this->target = ship;

    missile_tube_amount_selector->setSelectionIndex(ship->weapon_tube_count);
}

GuiShipTweakShields::GuiShipTweakShields(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    (new GuiLabel(left_col, "", tr("Number of Shields"), 20))->setSize(GuiElement::GuiSizeMax, 40);
    shields_count = new GuiSlider(right_col, "", 0, 8, 0, [this](int value) {
        target->shield_count = value;
    });
    shields_count->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    for(int n=0; n<max_shield_count; n++)
    {
        (new GuiLabel(left_col, "", tr("Shield {id_shield} max:").format({{"id_shield", string(n + 1)}}), 20))->setSize(GuiElement::GuiSizeMax, 30);
        shield_max_slider[n] = new GuiSlider(left_col, "", 0.0, 500, 0.0, [this, n](float value) {
            target->shield_max[n] = round(value);
            target->shield_level[n] = std::min(target->shield_level[n], target->shield_max[n]);
        });
        shield_max_slider[n]->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
    }

    for(int n=0; n<max_shield_count; n++)
    {
        (new GuiLabel(right_col, "", tr("Shield {id_shield}:").format({{"id_shield", string(n + 1)}}), 20))->setSize(GuiElement::GuiSizeMax, 30);
        shield_slider[n] = new GuiSlider(right_col, "", 0.0, 500, 0.0, [this, n](float value) {
            target->shield_level[n] = std::min(roundf(value), target->shield_max[n]);
        });
        shield_slider[n]->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
    }
}

void GuiShipTweakShields::onDraw(sf::RenderTarget& window)
{
    shields_count->setValue(target->shield_count);
    for(int n=0; n<max_shield_count; n++)
    {
        shield_slider[n]->setValue(target->shield_level[n]);
        shield_max_slider[n]->setValue(target->shield_max[n]);
    }
}

void GuiShipTweakShields::open(P<SpaceObject> target)
{
    P<ShipTemplateBasedObject> ship = target;
    this->target = ship;

    for(int n = 0; n < max_shield_count; n++)
    {
        shield_max_slider[n]->clearSnapValues()->addSnapValue(ship->ship_template->shield_level[n], 5.0f);
    }
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
        index_selector->addEntry(tr("Beam: {id_beam}").format({{"id_beam", string(n + 1)}}), "");
    index_selector->setSelectionIndex(0);

    (new GuiLabel(left_col, "", tr("beam", "Tractor Beam Range:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
    tractor_range_slider = new GuiSlider(left_col, "", 0.0, 5000.0, 0.0, [this](float value) {
        target->tractor_beam.setMaxRange(roundf(value / 100) * 100);
    });
    tractor_range_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(right_col, "", tr("beam", "Arc:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
    arc_slider = new GuiSlider(right_col, "", 0.0, 360.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setArc(roundf(value));
    });
    arc_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(right_col, "", tr("beam", "Direction:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
    direction_slider = new GuiSlider(right_col, "", -180.0, 180.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setDirection(roundf(value));
    });
    direction_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(right_col, "", tr("beam", "Turret arc:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
    turret_arc_slider = new GuiSlider(right_col, "", 0.0, 360.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setTurretArc(roundf(value));
    });
    turret_arc_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(right_col, "", tr("beam", "Turret direction:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
    turret_direction_slider = new GuiSlider(right_col, "", -180.0, 180.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setTurretDirection(roundf(value));
    });
    turret_direction_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(right_col, "", tr("beam", "Turret rotation rate:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
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

    (new GuiLabel(right_col, "", tr("beam", "Range:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
    range_slider = new GuiSlider(right_col, "", 0.0, 5000.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setRange(roundf(value / 100) * 100);
    });
    range_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(right_col, "", tr("beam", "Cycle time:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
    cycle_time_slider = new GuiSlider(right_col, "", 0.1, 20.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setCycleTime(value);
    });
    cycle_time_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(right_col, "", tr("beam", "Damage:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
    damage_slider = new GuiSlider(right_col, "", 0.1, 50.0, 0.0, [this](float value) {
        target->beam_weapons[beam_index].setDamage(value);
    });
    damage_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);
    
    (new GuiLabel(right_col, "", tr("beam", "Damage Type:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
    damage_type_slider = new GuiSelector(right_col, "", [this](int index, string value)
    {
        target->beam_weapons[beam_index].setDamageType(EDamageType(index));
    });
    damage_type_slider->setSize(GuiElement::GuiSizeMax, 30);
    damage_type_slider->addEntry("Energy",DT_Energy);
    damage_type_slider->addEntry("Kinetic",DT_Kinetic);
    damage_type_slider->addEntry("EMP",DT_EMP);
    damage_type_slider->addEntry("Heat",DT_Heat);
    damage_type_slider->setSelectionIndex(DT_Energy);
}

void GuiShipTweakBeamweapons::onDraw(sf::RenderTarget& window)
{
    target->drawOnRadar(window, sf::Vector2f(rect.left - 150.0f + rect.width / 2.0f, rect.top + rect.height * 0.66), 300.0f / 5000.0f, 0, false);
    
    tractor_range_slider->setValue(target->tractor_beam.getMaxRange(6.0));
    arc_slider->setValue(target->beam_weapons[beam_index].getArc());
    direction_slider->setValue(sf::angleDifference(0.0f, target->beam_weapons[beam_index].getDirection()));
    range_slider->setValue(target->beam_weapons[beam_index].getRange());
    turret_arc_slider->setValue(target->beam_weapons[beam_index].getTurretArc());
    turret_direction_slider->setValue(sf::angleDifference(0.0f, target->beam_weapons[beam_index].getTurretDirection()));
    turret_rotation_rate_slider->setValue(target->beam_weapons[beam_index].getTurretRotationRate() * 10.0f);
    turret_rotation_rate_overlay_label->setText(string(target->beam_weapons[beam_index].getTurretRotationRate()));
    cycle_time_slider->setValue(target->beam_weapons[beam_index].getCycleTime());
    damage_slider->setValue(target->beam_weapons[beam_index].getDamage());
    damage_type_slider->setSelectionIndex(target->beam_weapons[beam_index].getDamageType());
}

void GuiShipTweakBeamweapons::open(P<SpaceObject> target)
{
    P<SpaceShip> ship = target;
    this->target = ship;
}

GuiShipTweakSystems::GuiShipTweakSystems(GuiContainer* owner)
: GuiTweakPage(owner)
{

    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);
    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    for(int n=0; n<SYS_COUNT; n++)
    {
        ESystem system = ESystem(n);
        
        system_selector[n] = new GuiToggleButton(left_col, "", getLocaleSystemName(system), [this, n](bool value) {
            system_index = n;
        });
        system_selector[n]->setSize(GuiElement::GuiSizeMax, 40);
        
        system_box[n] = new GuiAutoLayout(right_col, "", GuiAutoLayout::LayoutVerticalTopToBottom);
        system_box[n]->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

        // Difference between reactor and other systems
        if (n == 0)
        {
            (new GuiLabel(system_box[n], "", tr("slider", "Production factor"), 20))->setSize(GuiElement::GuiSizeMax, 30);    
            system_factor[n] = new GuiSlider(system_box[n], "", 10.0, 40.0, 0.0, [this, n](float value) {
                target->systems[n].power_user_factor = value * -0.08f;
            });
        }
        else
        {
            (new GuiLabel(system_box[n], "", tr("slider", "Consumption factor"), 20))->setSize(GuiElement::GuiSizeMax, 30);    
            system_factor[n] = new GuiSlider(system_box[n], "", 0.0, 10.0, 0.0, [this, n](float value) {
                target->systems[n].power_user_factor = value * 0.08f;
            });
        }
        system_factor[n]->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);
    
        (new GuiLabel(system_box[n], "", tr("slider", "Health level"), 20))->setSize(GuiElement::GuiSizeMax, 30);
        system_damage[n] = new GuiSlider(system_box[n], "", -1.0, 1.0, 0.0, [this, n](float value) {
            target->systems[n].health = std::min(value,target->systems[n].health_max);
        });
        system_damage[n]->setSize(GuiElement::GuiSizeMax, 30);
        system_damage[n]->addSnapValue(-1.0, 0.01);
        system_damage[n]->addSnapValue( 0.0, 0.01);
        system_damage[n]->addSnapValue( 1.0, 0.01);

        (new GuiLabel(system_box[n], "", tr("slider", "Health max"), 20))->setSize(GuiElement::GuiSizeMax, 30);
        system_health_max[n] = new GuiSlider(system_box[n], "", -1.0, 1.0, 1.0, [this, n](float value) {
            target->systems[n].health_max = value;
            target->systems[n].health = std::min(value,target->systems[n].health);
        });
        system_health_max[n]->setSize(GuiElement::GuiSizeMax, 30);
        system_health_max[n]->addSnapValue(-1.0, 0.01);
        system_health_max[n]->addSnapValue( 0.0, 0.01);
        system_health_max[n]->addSnapValue( 1.0, 0.01);

        (new GuiLabel(system_box[n], "", tr("slider", "Heat level"), 20))->setSize(GuiElement::GuiSizeMax, 30);
        system_heat[n] = new GuiSlider(system_box[n], "", 0.0, 1.0, 0.0, [this, n](float value) {
            target->systems[n].heat_level = value;
        });
        system_heat[n]->setSize(GuiElement::GuiSizeMax, 30);
        system_heat[n]->addSnapValue( 0.0, 0.01);
        system_heat[n]->addSnapValue( 1.0, 0.01);

        (new GuiLabel(system_box[n], "", tr("slider", "Hack Level"), 20))->setSize(GuiElement::GuiSizeMax, 30);
        system_hacked[n] = new GuiSlider(system_box[n], "", 0.0, 1.0, 0.0, [this, n](float value) {
            target->systems[n].hacked_level = value;
        });
        system_hacked[n]->setSize(GuiElement::GuiSizeMax, 30);
        system_hacked[n]->addSnapValue( 0.0, 0.01);
        system_hacked[n]->addSnapValue( 0.5, 0.01);
        system_hacked[n]->addSnapValue( 1.0, 0.01);

        (new GuiLabel(system_box[n], "", tr("slider", "Power request"), 20))->setSize(GuiElement::GuiSizeMax, 30);
        system_power_bar[n] = new GuiProgressbar(system_box[n], "", 0.0, 3.0, 1.0);
        system_power_bar[n]->setDrawBackground(false)->setSize(GuiElement::GuiSizeMax, 30);
        system_power_bar[n]->setColor(sf::Color(255, 255, 0, 128));
        system_power_slider[n] = new GuiSlider(system_power_bar[n], "", 0.0, 3.0, 0.0, [this, n](float value) {
            target->systems[n].power_request = value;
        });
        system_power_slider[n]->setSize(GuiElement::GuiSizeMax, 30);
        system_power_slider[n]->addSnapValue( 0.0, 0.01);
        system_power_slider[n]->addSnapValue( 0.5, 0.01);
        system_power_slider[n]->addSnapValue( 1.0, 0.01);
        system_power_slider[n]->addSnapValue( 1.5, 0.01);
        system_power_slider[n]->addSnapValue( 2.0, 0.01);
        system_power_slider[n]->addSnapValue( 3.0, 0.01);

        (new GuiLabel(system_box[n], "", tr("slider", "Coolant request"), 20))->setSize(GuiElement::GuiSizeMax, 30);
        system_coolant_bar[n] = new GuiProgressbar(system_box[n], "", 0.0, 10.0, 1.0);
        system_coolant_bar[n]->setDrawBackground(false)->setSize(GuiElement::GuiSizeMax, 30);
        system_coolant_bar[n]->setColor(sf::Color(0, 128, 255, 128));
        system_coolant_slider[n] = new GuiSlider(system_coolant_bar[n], "", 0.0, 10.0, 0.0, [this, n](float value) {
            target->systems[n].coolant_request = value;
        });
        system_coolant_slider[n]->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);
        system_coolant_slider[n]->addSnapValue( 0.0, 0.01);
        system_coolant_slider[n]->addSnapValue( 1.0, 0.01);
        system_coolant_slider[n]->addSnapValue( 2.0, 0.01);
        system_coolant_slider[n]->addSnapValue( 3.0, 0.01);
        system_coolant_slider[n]->addSnapValue( 4.0, 0.01);
        system_coolant_slider[n]->addSnapValue( 5.0, 0.01);
        
        if (gameGlobalInfo->use_nano_repair_crew)
        {
            (new GuiLabel(system_box[n], "", tr("slider", "Repair request"), 20))->setSize(GuiElement::GuiSizeMax, 30);
            system_repair_bar[n] = new GuiProgressbar(system_box[n], "", 0.0, 3.0, 1.0);
            system_repair_bar[n]->setDrawBackground(false)->setSize(GuiElement::GuiSizeMax, 30);
            system_repair_bar[n]->setColor(sf::Color(32, 128, 32, 128));
            system_repair_slider[n] = new GuiSlider(system_repair_bar[n], "", 0.0, 3.0, 0.0, [this, n](float value) {
                target->systems[n].repair_request = value;
            });
            system_repair_slider[n]->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);
            system_repair_slider[n]->addSnapValue( 0.0, 0.01);
            system_repair_slider[n]->addSnapValue( 1.0, 0.01);
            system_repair_slider[n]->addSnapValue( 2.0, 0.01);
            system_repair_slider[n]->addSnapValue( 3.0, 0.01);
            system_repair_slider[n]->addSnapValue( 4.0, 0.01);
            system_repair_slider[n]->addSnapValue( 5.0, 0.01);
        }

        (new GuiLabel(system_box[n], "", tr("slider", "Instability factor"), 20))->setSize(GuiElement::GuiSizeMax, 30);
        system_instability_factor[n] = new GuiSlider(system_box[n], "", 0.0, 0.5, 0.0, [this, n](float value) {
            target->systems[n].instability_factor = value;
        });
        system_instability_factor[n]->setSize(GuiElement::GuiSizeMax, 30);
        system_instability_factor[n]->addSnapValue( 0.0, 0.01);
        system_instability_factor[n]->addSnapValue( 0.1, 0.01);
        system_instability_factor[n]->addSnapValue( 0.2, 0.01);
        system_instability_factor[n]->addSnapValue( 0.3, 0.01);
        system_instability_factor[n]->addSnapValue( 0.4, 0.01);
        system_instability_factor[n]->addSnapValue( 0.5, 0.01);

        (new GuiLabel(system_box[n], "", tr("slider", "Instability difficulty"), 20))->setSize(GuiElement::GuiSizeMax, 30);
        system_instability_difficulty[n] = new GuiSlider(system_box[n], "", 0, 4, 0, [this, n](int value) {
            target->systems[n].instability_difficulty = value;
        });
        system_instability_difficulty[n]->addOverlay()->setSize(GuiElement::GuiSizeMax, 30); 

        (new GuiLabel(system_box[n], "", tr("slider", "Instability level"), 20))->setSize(GuiElement::GuiSizeMax, 30);
        system_instability_level[n] = new GuiProgressbar(system_box[n], "", 0.0, 1.0, 0.0);
        system_instability_level[n]->setSize(GuiElement::GuiSizeMax, 30);
    }
}

void GuiShipTweakSystems::onDraw(sf::RenderTarget& window)
{
    for(int n=0; n<SYS_COUNT; n++)
    {
        system_selector[n]->setValue(n == system_index);
        system_selector[n]->setEnable(target->hasSystem(ESystem(n)));
        
        system_box[n]->setVisible(n == system_index);
        
        if (n == 0)
            system_factor[n]->setValue(target->systems[n].power_user_factor / -0.08f);
        else
            system_factor[n]->setValue(target->systems[n].power_user_factor / 0.08f);
        system_damage[n]->setValue(target->systems[n].health);
        system_health_max[n]->setValue(target->systems[n].health_max);
        system_heat[n]->setValue(target->systems[n].heat_level);
        system_coolant_bar[n]->setValue(target->systems[n].coolant_level);
        system_coolant_slider[n]->setValue(target->systems[n].coolant_request);
        system_hacked[n]->setValue(target->systems[n].hacked_level);
        system_power_bar[n]->setValue(target->systems[n].power_level);
        system_power_slider[n]->setValue(target->systems[n].power_request);
        system_repair_bar[n]->setValue(target->systems[n].repair_level);
        system_repair_slider[n]->setValue(target->systems[n].repair_request);
        system_instability_level[n]->setValue(target->systems[n].instability_level);
        system_instability_difficulty[n]->setValue(target->systems[n].instability_difficulty);
        system_instability_factor[n]->setValue(target->systems[n].instability_factor);
    }
}

void GuiShipTweakSystems::open(P<SpaceObject> target)
{
    P<SpaceShip> ship = target;
    this->target = ship;

    for(int n=0; n<SYS_COUNT; n++)
    {
        if (n == 0)
            system_factor[n]->clearSnapValues()->addSnapValue(PlayerSpaceship::system_power_user_factor[n] / -0.08f, 0.5f);
        else
            system_factor[n]->clearSnapValues()->addSnapValue(PlayerSpaceship::system_power_user_factor[n] / 0.08f, 0.5f);
    }
}

GuiShipTweakOxygen::GuiShipTweakOxygen(GuiContainer* owner)
: GuiTweakPage(owner)
{

    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);
    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    for(int n=0; n<10; n++)
    {
        zone_selector[n] = new GuiToggleButton(left_col, "", "Zone " + string(n+1), [this, n](bool value) {
            zone_index = n;
        });
        zone_selector[n]->setSize(GuiElement::GuiSizeMax, 40);
    }
    zone_index = 0;
    
    (new GuiLabel(left_col, "", tr("Change zone name:"), 20))->setSize(GuiElement::GuiSizeMax, 30);
    zone_label = new GuiTextEntry(left_col, "", "");
    zone_label->setSize(GuiElement::GuiSizeMax, 50);
    zone_label->callback([this](string text) {
        target->oxygen_zones[zone_index].label = text.upper();
    });
        
    for(int n=0; n<10; n++)
    {
        zone_box[n] = new GuiAutoLayout(right_col, "", GuiAutoLayout::LayoutVerticalTopToBottom);
        zone_box[n]->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    
        (new GuiLabel(zone_box[n], "", tr("slider", "Oxygen level"), 20))->setSize(GuiElement::GuiSizeMax, 30);
        zone_oxygen_level[n] = new GuiSlider(zone_box[n], "", 0.0, 1000.0, 100.0, [this, n](float value) {
            target->oxygen_zones[n].oxygen_level = std::min(value,target->oxygen_zones[n].oxygen_max);
        });
        zone_oxygen_level[n]->setSize(GuiElement::GuiSizeMax, 30);
        zone_oxygen_level[n]->addSnapValue(0.0, 0.01);
        zone_oxygen_level[n]->addSnapValue(200.0, 0.01);
        zone_oxygen_level[n]->addSnapValue(400.0, 0.01);
        zone_oxygen_level[n]->addSnapValue(600.0, 0.01);
        zone_oxygen_level[n]->addSnapValue(800.0, 0.01);
        zone_oxygen_level[n]->addSnapValue(1000.0, 0.01);
        zone_oxygen_level[n]->addOverlay();

        (new GuiLabel(zone_box[n], "", tr("slider", "Oxygen max"), 20))->setSize(GuiElement::GuiSizeMax, 30);
        zone_oxygen_max[n] = new GuiSlider(zone_box[n], "", 0.0, 1000.0, 100.0, [this, n](float value) {
            target->oxygen_zones[n].oxygen_max = value;
            target->oxygen_zones[n].oxygen_level = std::min(value,target->oxygen_zones[n].oxygen_level);
        });
        zone_oxygen_max[n]->setSize(GuiElement::GuiSizeMax, 30);
        zone_oxygen_max[n]->addSnapValue(0.0, 0.01);
        zone_oxygen_max[n]->addSnapValue(200.0, 0.01);
        zone_oxygen_max[n]->addSnapValue(400.0, 0.01);
        zone_oxygen_max[n]->addSnapValue(600.0, 0.01);
        zone_oxygen_max[n]->addSnapValue(800.0, 0.01);
        zone_oxygen_max[n]->addSnapValue(1000.0, 0.01);
        zone_oxygen_max[n]->addOverlay();

        (new GuiLabel(zone_box[n], "", tr("slider", "Recharge rate per second"), 20))->setSize(GuiElement::GuiSizeMax, 30);
        zone_recharge_rate[n] = new GuiSlider(zone_box[n], "", 0.0, 10.0, 1.0, [this, n](float value) {
            target->oxygen_zones[n].recharge_rate_per_second = value / 10.0;
        });
        zone_recharge_rate[n]->setSize(GuiElement::GuiSizeMax, 30);
        // Override overlay label.
        zone_recharge_rate_label[n] = new GuiLabel(zone_recharge_rate[n], "", "", 30);
        zone_recharge_rate_label[n]->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
        
        (new GuiLabel(zone_box[n], "", tr("slider", "Discharge rate per second"), 20))->setSize(GuiElement::GuiSizeMax, 30);
        zone_discharge_rate[n] = new GuiSlider(zone_box[n], "", 0.0, 10.0, 1.0, [this, n](float value) {
            target->oxygen_zones[n].discharge_rate_per_second = value / 10.0;
        });
        zone_discharge_rate[n]->setSize(GuiElement::GuiSizeMax, 30);
        // Override overlay label.
        zone_discharge_rate_label[n] = new GuiLabel(zone_discharge_rate[n], "", "", 30);
        zone_discharge_rate_label[n]->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    }
}

void GuiShipTweakOxygen::onDraw(sf::RenderTarget& window)
{
    for(int n=0; n<10; n++)
    {
        zone_selector[n]->setValue(n == zone_index);
        zone_box[n]->setVisible(n == zone_index);
        zone_selector[n]->setText(target->oxygen_zones[n].label);
        zone_oxygen_level[n]->setValue(target->oxygen_zones[n].oxygen_level);
        zone_oxygen_max[n]->setValue(target->oxygen_zones[n].oxygen_max);
        zone_recharge_rate[n]->setValue(target->oxygen_zones[n].recharge_rate_per_second * 10.0);
        zone_recharge_rate_label[n]->setText(target->oxygen_zones[n].recharge_rate_per_second);
        zone_discharge_rate[n]->setValue(target->oxygen_zones[n].discharge_rate_per_second * 10.0);
        zone_discharge_rate_label[n]->setText(target->oxygen_zones[n].discharge_rate_per_second);
    }
    zone_label->setText(target->oxygen_zones[zone_index].label);
}

void GuiShipTweakOxygen::open(P<SpaceObject> target)
{
    this->target = target;
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
    (new GuiLabel(left_col, "", tr("Control code:"), 30))->setSize(GuiElement::GuiSizeMax, 50);

    control_code = new GuiTextEntry(left_col, "", "");
    control_code->setSize(GuiElement::GuiSizeMax, 50);
    control_code->callback([this](string text) {
        target->control_code = text.upper();
    });

    // Edit reputation.
    (new GuiLabel(left_col, "", tr("Reputation:"), 30))->setSize(GuiElement::GuiSizeMax, 40);

    reputation_point_slider = new GuiSlider(left_col, "", 0.0, 9999.0, 0.0, [this](float value) {
        target->setReputationPoints(value);
    });
    reputation_point_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    // Edit energy level.
    (new GuiLabel(left_col, "", tr("Max energy:"), 30))->setSize(GuiElement::GuiSizeMax, 40);

    max_energy_level_slider = new GuiSlider(left_col, "", 0.0, 2000, 0.0, [this](float value) {
        target->max_energy_level = value;
        target->energy_level = std::min(target->energy_level, target->max_energy_level);
    });
    max_energy_level_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(left_col, "", tr("Current energy:"), 30))->setSize(GuiElement::GuiSizeMax, 40);

    energy_level_slider = new GuiSlider(left_col, "", 0.0, 2000, 0.0, [this](float value) {
        target->energy_level = std::min(value, target->max_energy_level);
    });
    energy_level_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    // Display Boost/Strafe speed sliders
    (new GuiLabel(left_col, "", tr("Boost Speed:"), 30))->setSize(GuiElement::GuiSizeMax, 40);
    combat_maneuver_boost_speed_slider = new GuiSlider(left_col, "", 0.0, 1000, 0.0, [this](float value) {
        target->combat_maneuver_boost_speed = value;
    });
    combat_maneuver_boost_speed_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(left_col, "", tr("Strafe Speed:"), 30))->setSize(GuiElement::GuiSizeMax, 40);
    combat_maneuver_strafe_speed_slider = new GuiSlider(left_col, "", 0.0, 1000, 0.0, [this](float value) {
        target->combat_maneuver_strafe_speed = value;
    });
    combat_maneuver_strafe_speed_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 30);

    // Edit scanning noise (< 0 for help)
    (new GuiLabel(left_col, "", tr("Scanning noise (<0 for help):"), 30))->setSize(GuiElement::GuiSizeMax, 40);
    scanning_noise_slider = new GuiSlider(left_col, "", -10.0, 10.0, 0.0, [this](float value) {
        target->scanning_noise = value / 10.0;
    });
    scanning_noise_slider->setSize(GuiElement::GuiSizeMax, 30);
    scanning_noise_slider->addSnapValue(0.0, 1.0f);
    // Override overlay label.
    scanning_noise_label = new GuiLabel(scanning_noise_slider, "", "", 30);
    scanning_noise_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    (new GuiLabel(left_col, "", tr("Scanning capability :"), 30))->setSize(GuiElement::GuiSizeMax, 40);
    scanning_capability_slider = new GuiSlider(left_col, "", 0, 3, 3, [this](int value) {
        target->scanning_capability = EScannedState(value);
    });
    scanning_capability_slider->setSize(GuiElement::GuiSizeMax, 30);
    scanning_capability_slider->addSnapValue(0.0, 1.0f);
    // Override overlay label.
    scanning_capability_label = new GuiLabel(scanning_capability_slider, "", "", 30);
    scanning_capability_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    // Right column
    // Count and list ship positions and whether they're occupied.
    position_count = new GuiLabel(right_col, "", tr("Positions occupied: "), 30);
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
            position_state = tr("position", "Occupied");
            position_counter += 1;
        }

        position[n]->setValue(position_state);
    }

    // Update the total occupied position count.
    position_count->setText(tr("Positions occupied: ") + string(position_counter));

    // Update the ship's energy level.
    energy_level_slider->setValue(target->energy_level);
    max_energy_level_slider->setValue(target->max_energy_level);

    // Update reputation points.
    reputation_point_slider->setValue(target->getReputationPoints());

    // Update scanning noise and capability
    scanning_noise_slider->setValue(target->scanning_noise * 10.0);
    scanning_noise_label->setText(target->scanning_noise);
    scanning_capability_slider->setValue(target->scanning_capability);
    scanning_capability_label->setText(getScannedStateName(target->scanning_capability));
}

void GuiShipTweakPlayer::open(P<SpaceObject> target)
{
    P<PlayerSpaceship> player = target;
    this->target = player;

    if (player)
    {
        // Read ship's control code.
        control_code->setText(player->control_code);

        // Set and snap boost speed slider to current value
        combat_maneuver_boost_speed_slider->setValue(player->combat_maneuver_boost_speed);
        combat_maneuver_boost_speed_slider->clearSnapValues()->addSnapValue(player->combat_maneuver_boost_speed, 20.0f);

        // Set and snap strafe speed slider to current value
        combat_maneuver_strafe_speed_slider->setValue(player->combat_maneuver_strafe_speed);
        combat_maneuver_strafe_speed_slider->clearSnapValues()->addSnapValue(player->combat_maneuver_strafe_speed, 20.0f);

        // Set and snap scanning noise slider to current value
        scanning_noise_slider->setValue(player->scanning_noise);
    }
}

GuiShipTweakPlayer2::GuiShipTweakPlayer2(GuiContainer* owner)
: GuiTweakPage(owner)
{
    // Add two columns.
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    // Left column
    (new GuiLabel(left_col, "", tr("Coolant (per system / max):"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    
    coolant_per_system_slider = new GuiSlider(left_col, "", 0.0, 50.0, 0.0, [this](float value) {
        target->setMaxCoolantPerSystem(value);
    });
    coolant_per_system_slider->addSnapValue(10,1)->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    coolant_slider = new GuiSlider(left_col, "", 0.0, 50.0, 0.0, [this](float value) {
        target->setMaxCoolant(value);
    });
    coolant_slider->addSnapValue(10,1)->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
    
    (new GuiLabel(left_col, "", tr("Repair (per system / max):"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    
    repair_per_system_slider = new GuiSlider(left_col, "", 0, 10, 0, [this](int value) {
        target->setMaxRepairPerSystem(value);
    });
    repair_per_system_slider->addSnapValue(2,1)->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    repair_slider = new GuiSlider(left_col, "", 0, 10, 2, [this](int value) {
        target->setRepairCrewCount(value);
    });
    repair_slider->addSnapValue(2,1)->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
    
    (new GuiLabel(left_col, "", tr("Short range radar:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    short_range_radar_slider = new GuiSlider(left_col, "", 100.0, 20000.0, 0.0, [this](float value) {
        target->setShortRangeRadarRange(value);
    });
    short_range_radar_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", tr("Long range radar:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    long_range_radar_slider = new GuiSlider(left_col, "", 100.0, 100000.0, 0.0, [this](float value) {
        target->setLongRangeRadarRange(value);
    });
    long_range_radar_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", tr("Far range radar:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    far_range_radar_slider = new GuiSlider(left_col, "", 50000.0, 10000000.0, 0.0, [this](float value) {
        target->setFarRangeRadarRange(value);
    });
    far_range_radar_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", tr("Engineering presets:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    engineering_presets_slider = new GuiSlider(left_col, "", 0, 9, 0, [this](int value) {
        target->active_engineer_presets_number = value;
    });
    engineering_presets_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", tr("Max Scan Probes:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    max_scan_probes_slider = new GuiSlider(left_col, "", 0, 20, 0.0, [this](float value) {
        target->setMaxScanProbeCount(value);
    });
    max_scan_probes_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(left_col, "", tr("Scan Probes:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    scan_probes_slider = new GuiSlider(left_col, "", 0, 20, 0.0, [this](float value) {
        target->setScanProbeCount(value);
    });
    scan_probes_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    // Right column
    // Can scan bool
    can_scan = new GuiToggleButton(right_col, "", tr("button", "Can scan"), [this](bool value) {
        target->setCanScan(value);
    });
    can_scan->setSize(GuiElement::GuiSizeMax, 40);

    // Can hack bool
    can_hack = new GuiToggleButton(right_col, "", tr("button", "Can hack"), [this](bool value) {
        target->setCanHack(value);
    });
    can_hack->setSize(GuiElement::GuiSizeMax, 40);

    // Can dock bool
    can_dock = new GuiToggleButton(right_col, "", tr("button", "Can dock"), [this](bool value) {
        target->setCanDock(value);
    });
    can_dock->setSize(GuiElement::GuiSizeMax, 40);

    // Can combat maneuver bool
    can_combat_maneuver = new GuiToggleButton(right_col, "", tr("button", "Can combat maneuver"), [this](bool value) {
        target->setCanCombatManeuver(value);
    });
    can_combat_maneuver->setSize(GuiElement::GuiSizeMax, 40);

    // Can self destruct bool
    can_self_destruct = new GuiToggleButton(right_col, "", tr("button", "Can self destruct"), [this](bool value) {
        target->setCanSelfDestruct(value);
    });
    can_self_destruct->setSize(GuiElement::GuiSizeMax, 40);

    // Can launch probe bool
    can_launch_probe = new GuiToggleButton(right_col, "", tr("button", "Can launch probes"), [this](bool value) {
        target->setCanLaunchProbe(value);
    });
    can_launch_probe->setSize(GuiElement::GuiSizeMax, 40);

    auto_coolant_enabled = new GuiToggleButton(right_col, "", tr("button", "Auto coolant"), [this](bool value) {
        target->setAutoCoolant(value);
    });
    auto_coolant_enabled->setSize(GuiElement::GuiSizeMax, 40);

    auto_repair_enabled = new GuiToggleButton(right_col, "", tr("button", "Auto repair"), [this](bool value) {
        target->commandSetAutoRepair(value);
    });
    auto_repair_enabled->setSize(GuiElement::GuiSizeMax, 40);
}

void GuiShipTweakPlayer2::onDraw(sf::RenderTarget& window)
{
    coolant_slider->setValue(target->max_coolant);
    coolant_per_system_slider->setValue(target->max_coolant_per_system);
    repair_slider->setValue(target->max_repair);
    repair_per_system_slider->setValue(target->max_repair_per_system);
    short_range_radar_slider->setValue(target->getShortRangeRadarRange());
    long_range_radar_slider->setValue(target->getLongRangeRadarRange());
    far_range_radar_slider->setValue(target->getFarRangeRadarRange());
    engineering_presets_slider->setValue(target->active_engineer_presets_number);
    max_scan_probes_slider->setValue(target->getMaxScanProbeCount());
    scan_probes_slider->setValue(target->getScanProbeCount());
    can_scan->setValue(target->getCanScan());
    can_hack->setValue(target->getCanHack());
    can_dock->setValue(target->getCanDock());
    can_combat_maneuver->setValue(target->getCanCombatManeuver());
    can_self_destruct->setValue(target->getCanSelfDestruct());
    can_launch_probe->setValue(target->getCanLaunchProbe());
    auto_coolant_enabled->setValue(target->auto_coolant_enabled);
    auto_repair_enabled->setValue(target->auto_repair_enabled);
}

void GuiShipTweakPlayer2::open(P<SpaceObject> target)
{
    this->target = target;
}

GuiShipTweakMessages::GuiShipTweakMessages(GuiContainer* owner)
: GuiTweakPage(owner)
{
    color_message = sf::Color::White;
    message = "";

    (new GuiLabel(this, "", tr("message", "Message:"), 30))->setPosition(100, 50, ATopLeft);
    message_entry = new GuiTextEntry(this, "", "");
    message_entry->setSize(550, 50);
    message_entry->setPosition(50, 70, ATopLeft);
    message_entry->callback([this](string text) {
        message = text;
    });

    message_delete = new GuiButton(this, "", "", [this]() {
        message_entry -> setText("");
        message = "";
    });
    message_delete->setPosition(-25, 70, ATopRight)->setSize(50, 50);
    message_delete->setIcon("gui/icons/self-destruct");
    
    // Choose the target
    (new GuiLabel(this, "", tr("message", "Target:"), 30))->setSize(100, 40)->setPosition(50, 130, ATopLeft);
    GuiSelector* target_selector = new GuiSelector(this, "", [this](int index, string value)
    {
    });
    target_selector->setSize(300, 40)->setPosition(200, 130, ATopLeft);
    target_selector->addEntry(tr("message", "this player"), "this player");
    target_selector->addEntry(tr("message", "all players"), "all players");
    target_selector->setSelectionIndex(0);
    
    // Choose the screen
    (new GuiLabel(this, "", tr("message", "Screen:"), 30))->setSize(100, 40)->setPosition(50, 170, ATopLeft);
    GuiSelector* screen_selector = new GuiSelector(this, "", [this](int index, string value)
    {
    });
    screen_selector->setSize(300, 40)->setPosition(200, 170, ATopLeft);
    for(int n = 0; n < max_crew_positions; n++)
        screen_selector->addEntry(getCrewPositionName(ECrewPosition(n)), getCrewPositionName(ECrewPosition(n)));
    screen_selector->setSelectionIndex(0);

    // Add two columns.
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 200, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 200, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    // Left column
    (new GuiLabel(left_col, "", tr("message", "into log"), 30))->setSize(GuiElement::GuiSizeMax, 100);

    // Choose the color
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
    color_selector->addEntry(tr("message", "white"), "white");
    color_selector->addEntry(tr("message", "black"), "black");
    color_selector->addEntry(tr("message", "red"), "red");
    color_selector->addEntry(tr("message", "green"), "green");
    color_selector->addEntry(tr("message", "blue"), "blue");
    color_selector->addEntry(tr("message", "yellow"), "yellow");
    color_selector->addEntry(tr("message", "magenta"), "magenta");
    color_selector->addEntry(tr("message", "cyan"), "cyan");
    color_selector->setSelectionIndex(0);

    // Send the message
    send_message_log = new GuiButton(left_col, "", tr("message", "Send message"), [this, target_selector, screen_selector]() {
        if (target_selector->getSelectionValue() == "all players")
        {
            for(int n=0; n<GameGlobalInfo::max_player_ships; n++)
            {
                P<PlayerSpaceship> ship = gameGlobalInfo->getPlayerShip(n);
                if (ship)
                    ship -> addToShipLog(message,color_message,ECrewPosition(screen_selector->getSelectionIndex()));
            }
        }
        else
            target -> addToShipLog(message,color_message,ECrewPosition(screen_selector->getSelectionIndex()));
    });
    send_message_log->setSize(GuiElement::GuiSizeMax, 40);

    // Right column
    (new GuiLabel(right_col, "", tr("message", "On screen"), 30))->setSize(GuiElement::GuiSizeMax, 100);

    (new GuiButton(right_col, "", tr("message", "Send message"), [this, target_selector, screen_selector]() {
        if (target_selector->getSelectionValue() == "all players")
        {
            for(int n=0; n<GameGlobalInfo::max_player_ships; n++)
            {
                P<PlayerSpaceship> ship = gameGlobalInfo->getPlayerShip(n);
                if (ship)
                    ship->addCustomMessage(ECrewPosition(screen_selector->getSelectionIndex()), getCrewPositionName(ECrewPosition(screen_selector->getSelectionIndex())) + "_message", message);
            }
        }
        else
            target->addCustomMessage(ECrewPosition(screen_selector->getSelectionIndex()), getCrewPositionName(ECrewPosition(screen_selector->getSelectionIndex())) + "_message", message);
    }))->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiButton(right_col, "", tr("message", "Remove previous messages"), [this, target_selector, screen_selector]() {
        if (target_selector->getSelectionValue() == "all players")
        {
            for(int n=0; n<GameGlobalInfo::max_player_ships; n++)
            {
                P<PlayerSpaceship> ship = gameGlobalInfo->getPlayerShip(n);
                if (ship)
                {
                    for(int n = 0; n < max_crew_positions; n++)
                        ship->removeCustom(getCrewPositionName(ECrewPosition(n)) + "_message");
                }
            }
        }
        else
        {
            for(int n = 0; n < max_crew_positions; n++)
                target->removeCustom(getCrewPositionName(ECrewPosition(n)) + "_message");
        }
    }))->setSize(GuiElement::GuiSizeMax, 40);

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

GuiObjectTweakBase::GuiObjectTweakBase(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);

    GuiAutoLayout* right_col = new GuiAutoLayout(this, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 25, ATopRight)->setSize(300, GuiElement::GuiSizeMax);

    // Left column

    // Set object's heading.
    (new GuiLabel(left_col, "", tr("Heading:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    heading_slider = new GuiSlider(left_col, "", 0.0, 359.9, 0.0, [this](float value) {
        target->setHeading(value);
    });
    heading_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
    
    // Set object's z position.
    (new GuiLabel(left_col, "", tr("Z Position:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    position_z_slider = new GuiSlider(left_col, "", -300.0, 300.0, 0.0, [this](float value) {
        target->setPositionZ(value);
    });
    position_z_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
    position_z_slider->addSnapValue(-200.0, 0.01);
    position_z_slider->addSnapValue(-100.0, 0.01);
    position_z_slider->addSnapValue(0.0, 0.01);
    position_z_slider->addSnapValue(100.0, 0.01);
    position_z_slider->addSnapValue(200.0, 0.01);
    
    hull_label = new GuiLabel(left_col, "", "Hull:", 30);
    hull_label->setSize(GuiElement::GuiSizeMax, 50);
    hull_slider = new GuiSlider(left_col, "", 0.0, 500, 0.0, [this](float value) {
        target->hull = value;
    });
    hull_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
    
    // Right column
    // Radar signature
	(new GuiLabel(right_col, "", tr("Gravity signature (blue):"), 30))->setSize(GuiElement::GuiSizeMax, 50);
	gravity_slider = new GuiSlider(right_col, "", -100.0, 100.0, 0.0, [this](float value) {
        target->radar_signature.gravity = value / 100.0f;
    });
    gravity_slider->addSnapValue(0.0f, 5.0f);
    gravity_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
 	(new GuiLabel(right_col, "", tr("Energy signature (red):"), 30))->setSize(GuiElement::GuiSizeMax, 50);
	electrical_slider = new GuiSlider(right_col, "", -100.0, 100.0, 0.0, [this](float value) {
        target->radar_signature.electrical = value / 100.0f;
    });
    electrical_slider->addSnapValue(0.0f, 5.0f);
    electrical_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
 	(new GuiLabel(right_col, "", tr("Biology signature (green):"), 30))->setSize(GuiElement::GuiSizeMax, 50);
	biological_slider = new GuiSlider(right_col, "", -100.0, 100.0, 0.0, [this](float value) {
        target->radar_signature.biological = value / 100.0f;
    });
    biological_slider->addSnapValue(0.0f, 5.0f);
    biological_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", tr("Scanning Complexity:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    scanning_complexity_slider = new GuiSlider(right_col, "", 0, 4, 0, [this](float value) {
        target->setScanningParameters(value,target->scanningChannelDepth(target));
    });
    scanning_complexity_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);

    (new GuiLabel(right_col, "", tr("Scanning Depth:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    scanning_depth_slider = new GuiSlider(right_col, "", 1, 5, 0, [this](float value) {
        target->setScanningParameters(target->scanningComplexity(target),value);
    });
    scanning_depth_slider->addOverlay()->setSize(GuiElement::GuiSizeMax, 40);
}

void GuiObjectTweakBase::onDraw(sf::RenderTarget& window)
{
    heading_slider->setValue(target->getHeading());
    position_z_slider->setValue(target->getPositionZ());
    hull_slider->setValue(target->hull);
    P<ShipTemplateBasedObject> ship = target;
    hull_label->setVisible(!ship);
    hull_slider->setVisible(!ship);

    // we probably dont need to set these each onDraw
    // but doing it forces the slider to round to a integer
    scanning_complexity_slider->setValue(target->scanningComplexity(target));
    scanning_depth_slider->setValue(target->scanningChannelDepth(target));
    gravity_slider->setValue(target->radar_signature.gravity * 100.0f);
    electrical_slider->setValue(target->getRadarSignatureElectrical() * 100.0f);
	biological_slider->setValue(target->getRadarSignatureBiological() * 100.0f);
}

void GuiObjectTweakBase::open(P<SpaceObject> target)
{
    this->target = target;
}

GuiShipTweakDescription::GuiShipTweakDescription(GuiContainer* owner)
: GuiTweakPage(owner)
{
    GuiAutoLayout* left_col = new GuiAutoLayout(this, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(50, 25, ATopLeft)->setSize(300, GuiElement::GuiSizeMax);
    
    GuiAutoLayout* label_col = new GuiAutoLayout(this, "LABEL_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    label_col->setPosition(-50-130-10, 25, ATopRight)->setSize(130, GuiElement::GuiSizeMax);
    
    GuiAutoLayout* value_col = new GuiAutoLayout(this, "VALUE_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    value_col->setPosition(-50, 25, ATopRight)->setSize(130, GuiElement::GuiSizeMax);

    // Left column
    (new GuiLabel(left_col, "", tr("title", "Description"), 40))->setSize(GuiElement::GuiSizeMax, 50);
    // Edit object's callsign.
    (new GuiLabel(left_col, "", tr("Callsign:"), 30))->setSize(GuiElement::GuiSizeMax, 50);

    callsign = new GuiTextEntry(left_col, "", "");
    callsign->setSize(GuiElement::GuiSizeMax, 50);
    callsign->callback([this](string text) {
        target->callsign = text;
    });

    // Edit object's description.
    // TODO: Fix long strings in GuiTextEntry, or make a new GUI element for
    // editing long strings.
    (new GuiLabel(left_col, "", tr("Unscanned description:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    unscanned_description = new GuiTextEntry(left_col, "", "");
    unscanned_description->setSize(GuiElement::GuiSizeMax, 50);
    unscanned_description->callback([this](string text) {
        target->setDescriptionForScanState(SS_NotScanned,text);
    });

    (new GuiLabel(left_col, "", tr("Friend or Foe Description:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    friend_or_foe_description = new GuiTextEntry(left_col, "", "");
    friend_or_foe_description->setSize(GuiElement::GuiSizeMax, 50);
    friend_or_foe_description->callback([this](string text) {
        target->setDescriptionForScanState(SS_FriendOrFoeIdentified,text);
    });

    (new GuiLabel(left_col, "", tr("Simple Scan Description:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    simple_scan_description = new GuiTextEntry(left_col, "", "");
    simple_scan_description->setSize(GuiElement::GuiSizeMax, 50);
    simple_scan_description->callback([this](string text) {
        target->setDescriptionForScanState(SS_SimpleScan,text);
    });

    (new GuiLabel(left_col, "", tr("Full Scan Description:"), 30))->setSize(GuiElement::GuiSizeMax, 50);
    full_scan_description = new GuiTextEntry(left_col, "", "");
    full_scan_description->setSize(GuiElement::GuiSizeMax, 50);
    full_scan_description->callback([this](string text) {
        target->setDescriptionForScanState(SS_FullScan,text);
    });
    
    // Right column
    (new GuiLabel(label_col, "", tr("title", "Label"), 40))->setSize(GuiElement::GuiSizeMax, 50);
    (new GuiLabel(value_col, "", tr("title", "Value"), 40))->setSize(GuiElement::GuiSizeMax, 50);

    for(int n = 0; n < 10; n++)
    {
        infos_label[n] = new GuiTextEntry(label_col, "", "");
        infos_label[n]->setSize(GuiElement::GuiSizeMax, 50);
        infos_label[n]->callback([this, n](string text) {
            target->infos_label[n] = text;
        });

        infos_value[n] = new GuiTextEntry(value_col, "", "");
        infos_value[n]->setSize(GuiElement::GuiSizeMax, 50);
        infos_value[n]->callback([this, n](string text) {
            target->infos_value[n] = text;
        });
    }
}

void GuiShipTweakDescription::onDraw(sf::RenderTarget& window)
{
    // Update Callsign and description
     callsign->setText(target->callsign);
    // TODO: Fix long strings in GuiTextEntry, or make a new GUI element for
    // editing long strings.
    unscanned_description->setText(target->getDescription(SS_NotScanned));
    friend_or_foe_description->setText(target->getDescription(SS_FriendOrFoeIdentified));
    simple_scan_description->setText(target->getDescription(SS_SimpleScan));
    full_scan_description->setText(target->getDescription(SS_FullScan));
    
    // Update infos.
    for(int n = 0; n < 10; n++)
    {
        infos_label[n]->setText(target->infos_label[n]);
        infos_value[n]->setText(target->infos_value[n]);
    }
}

void GuiShipTweakDescription::open(P<SpaceObject> target)
{
    this->target = target;
}
