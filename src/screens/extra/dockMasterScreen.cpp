#include "dockMasterScreen.h"

#include "playerInfo.h"
#include "spaceObjects/shipTemplateBasedObject.h"
#include "spaceObjects/playerSpaceship.h"
#include "screenComponents/shipsLogControl.h"
#include "screenComponents/customShipFunctions.h"

#include "gui/gui2_listbox.h"
#include "gui/gui2_autolayout.h"
#include "gui/gui2_element.h"
#include "gui/gui2_panel.h"
#include "gui/gui2_label.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_slider.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_button.h"
#include "gui/gui2_selector.h"
#include "screenComponents/powerDamageIndicator.h"

#include "screenComponents/rotatingModelView.h"

const int ROW_SIZE = 4;
const int ROW_HEIGHT = 200;
const int BOX_WIDTH = 290;
const int COLUMN_WIDTH = 400;

DockMasterScreen::DockMasterScreen(GuiContainer *owner)
    : GuiOverlay(owner, "DOCK_MASTER_SCREEN", colorConfig.background)
{
    GuiOverlay *background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    GuiAutoLayout *rootLayout = new GuiAutoLayout(this, "ROOT_LAYOUT", GuiAutoLayout::LayoutHorizontalLeftToRight);
    rootLayout->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax)->setPosition(0, 0, ATopLeft);

    model = new GuiRotatingModelView(this, "MODEL_VIEW", nullptr);
    model->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    docks = new GuiListbox(rootLayout, "DOCKS_LIST", [this](int index, string value) {
        selectDock(index);
    });
    docks->setMargins(20, 20, 20, 20)->setSize(COLUMN_WIDTH, GuiElement::GuiSizeMax);

    // the index in the button list is assumed to equal the index of the dock
    for (int n = 0; n < max_docks_count; n++)
    {
        if (my_spaceship->docks[n].dock_type != Dock_Disabled)
        {
            string state = my_spaceship ? " (" + getDockStateName(my_spaceship->docks[n].state) + ")" : "";
            docks->addEntry("dock-" + std::to_string(n + 1) + state, "dock-" + std::to_string(n + 1) + " " + getDockTypeName(my_spaceship->docks[n].dock_type));
        }
    }

    GuiElement *rightSide = new GuiElement(rootLayout, "RIGHT_SIDE");
    rightSide->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    title = new GuiLabel(rightSide, "TITLE", "dock x", 30);
    title->addBackground()
        ->setAlignment(ACenter)
        ->setPosition(0, 0, ATopCenter)
        ->setSize(GuiElement::GuiSizeMax, 50);

    GuiAutoLayout *dockPanel = new GuiAutoLayout(rightSide, "DOCK_PANEL", GuiAutoLayout::LayoutVerticalColumns);
    dockPanel->setSize(GuiElement::GuiSizeMax, 50)->setPosition(0, 50, ATopCenter);

    sideBar = new GuiAutoLayout(rightSide, "SIDE_BAR", GuiAutoLayout::LayoutVerticalTopToBottom);
    sideBar->setSize(COLUMN_WIDTH, GuiElement::GuiSizeMax)->setPosition(0, 100, ATopRight);

    move_dest = new GuiAutoLayout(dockPanel, "", GuiAutoLayout::LayoutVerticalColumns);
    (new GuiLabel(move_dest, "MOVE_DEST_LABEL", "Transfert vers:", 30))->setAlignment(ACenterRight);
    move_dest_selector = new GuiSelector(move_dest, "MOVE_DEST_SELECTOR", [this](int _idx, string value) {
        if (my_spaceship)
            my_spaceship->commandSetDockMoveTarget(index, value.toInt());
    });

    move_button = new GuiButton(dockPanel, "MOVE_BUTTON", "Transferer", [this]() {
        if (my_spaceship)
        {
            if (my_spaceship->getSystemEffectiveness(SYS_Docks) > 0)
                my_spaceship->commandMoveCargo(index);
        }
    });
    move_button->setSize(COLUMN_WIDTH, 40);
    (new GuiPowerDamageIndicator(move_button, "DOCKS_DPI", SYS_Docks, ATopCenter, my_spaceship))->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    launch_button = new GuiButton(sideBar, "LAUNCH_DRONE_BUTTON", "Lancer drone", [this]() {
        if (my_spaceship)
        {
            if (my_spaceship->getSystemEffectiveness(SYS_Docks) > 0)
                my_spaceship->commandLaunchCargo(index);
        }
    });
    launch_button->setSize(COLUMN_WIDTH, 40);
    (new GuiPowerDamageIndicator(launch_button, "DOCKS_DPI", SYS_Docks, ABottomCenter, my_spaceship))->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    GuiElement *energyControl = new GuiElement(sideBar, "ENERGY_CONTROL");
    energyControl->setSize(COLUMN_WIDTH, 40);

    energy_slider = new GuiSlider(energyControl, "ENERGY_SLIDER", 0.0, 10.0, 0.0, [this](float value) {
        if (my_spaceship)
            my_spaceship->commandSetDockEnergyRequest(index, value);
    });
    energy_slider->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    (new GuiPowerDamageIndicator(energy_slider, "DOCKS_DPI", SYS_Docks, ACenterLeft, my_spaceship))->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    energy_bar = new GuiProgressbar(energyControl, "ENERGY_BAR", 0.0, 10.0, 0.0);
    energy_bar->setColor(sf::Color(192, 192, 32, 128))->setText("Energie")->setDrawBackground(false)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax)->setMargins(10, 0, 10, 0);

    weaponsControl = new GuiAutoLayout(sideBar, "WEAPONS_BAR", GuiAutoLayout::LayoutVerticalTopToBottom);
    weaponsControl->setSize(COLUMN_WIDTH, GuiElement::GuiSizeMax)->setPosition(0, 100, ABottomRight);

    weapons_layout_label = new GuiElement(weaponsControl, "WEAPONS_LAYOUT_LABEL");
    weapons_layout_label -> setSize(COLUMN_WIDTH, 40);
    (new GuiLabel(weapons_layout_label, "", "Missile", 20))->setSize(75, 50);
    (new GuiLabel(weapons_layout_label, "", "Station", 20))->setPosition(75,0)->setSize(75, 50);
    (new GuiLabel(weapons_layout_label, "", "Drone", 20))->setPosition(150,0)->setSize(75, 50);

    for(int n=0; n<MW_Count; n++)
    {
        weapons_layout[n] = new GuiElement(weaponsControl, "WEAPONS_LAYOUT");
        weapons_layout[n]->setSize(COLUMN_WIDTH, 40);

        (new GuiLabel(weapons_layout[n], "", getMissileWeaponName(EMissileWeapons(n)) + " :", 20))->setSize(75, 30);
        //(new GuiLabel(weaponsControl, "", getMissileWeaponName(EMissileWeapons(n)) + " :", 20))->setSize(GuiElement::GuiSizeMax, 30);

        weapons_stock_ship[n] = new GuiLabel(weapons_layout[n],"","0/20",20);
        weapons_stock_ship[n]->setPosition(75,0)->setSize(75, 30);
        weapons_stock_cargo[n] = new GuiLabel(weapons_layout[n],"","0/20",20);
        weapons_stock_cargo[n]->setPosition(150,0)->setSize(75, 30);

        weapons_stock_p1[n] = new GuiButton(weapons_layout[n],"","+ 1", [this,n]() {
            if (my_spaceship)
            {
                Dock &dockData = my_spaceship->docks[index];
                P<Cargo> cargo = dockData.getCargo();

                if (my_spaceship->weapon_storage[n] <= 0)
                    return;

                if (cargo->getWeaponStorageMax(EMissileWeapons(n)) == cargo->getWeaponStorage(EMissileWeapons(n)))
                    return;

                my_spaceship->weapon_storage[n] -= 1;
                cargo->setWeaponStorage(EMissileWeapons(n), cargo->getWeaponStorage(EMissileWeapons(n)) + 1);
            }
        });

        weapons_stock_p1[n]->setPosition(225,0)->setSize(75, 40);

        weapons_stock_m1[n] = new GuiButton(weapons_layout[n],"","- 1", [this,n]() {
            if (my_spaceship)
            {
                Dock &dockData = my_spaceship->docks[index];
                P<Cargo> cargo = dockData.getCargo();

                if (cargo->getWeaponStorage(EMissileWeapons(n)) <= 0)
                    return;

                if (my_spaceship->weapon_storage[n] == my_spaceship->weapon_storage_max[n])
                    return;

                my_spaceship->weapon_storage[n] += 1;
                cargo->setWeaponStorage(EMissileWeapons(n), cargo->getWeaponStorage(EMissileWeapons(n)) - 1);
            }
        });
        weapons_stock_m1[n]->setPosition(300,0)->setSize(75, 40);
    }


    cargoInfo = new GuiAutoLayout(sideBar, "CARGO_INFO", GuiAutoLayout::LayoutVerticalTopToBottom);
    cargoInfo->setSize(COLUMN_WIDTH, GuiElement::GuiSizeMax);

    (new GuiCustomShipFunctions(this, dockMaster, "CUSTOM_FUNCTIONS", my_spaceship))->setPosition(20, 550, ATopLeft)->setSize(360, GuiElement::GuiSizeMax);

    overlay = new GuiOverlay(this, "OVERLAY", sf::Color(0, 0, 0, 128));
    overlay->setBlocking(true)->setPosition(COLUMN_WIDTH, 100, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    overlay_label = new GuiLabel(overlay, "OVERLAY_LABEL", "Transporting cargo out", 30);
    overlay_label->setPosition(0, 0, ACenter)->setSize(COLUMN_WIDTH, 50);
    distance_bar = new GuiProgressbar(overlay, "DISTANCE_BAR", 0.0, 1.0, 0.0);
    distance_bar->setPosition(0, 50, ACenter)->setSize(COLUMN_WIDTH, 50);
    (new GuiPowerDamageIndicator(distance_bar, "DOCKS_DPI", SYS_Docks, ATopCenter, my_spaceship))->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    cancel_move_button = new GuiButton(overlay, "CANCEL_MOVE_BUTTON", "Annuler transfert", [this]() {
        my_spaceship->commandCancelMoveCargo(index);
    });
    cancel_move_button->setPosition(0, 100, ACenter)->setSize(COLUMN_WIDTH, 50);

    selectDock(0);
    model->moveToBack();
    background_crosses->moveToBack();

    new ShipsLog(this,"docks");
}

void DockMasterScreen::selectDock(int index)
{
    title->setText(docks->getEntryValue(index));
    this->index = index;
    docks->setSelectionIndex(index);
    auto &dockData = my_spaceship->docks[index];
    launch_button->setVisible(dockData.dock_type == Dock_Launcher);
    energy_bar->setVisible(dockData.dock_type == Dock_Energy);
    energy_slider->setVisible(dockData.dock_type == Dock_Energy);
    weaponsControl->setVisible(dockData.dock_type == Dock_Energy);
}

void DockMasterScreen::onDraw(sf::RenderTarget &window)
{
    GuiOverlay::onDraw(window);
    if (my_spaceship)
    {
        move_dest_selector->setOptions({});
        for (int n = 0; n < max_docks_count; n++)
        {
            Dock &dockData = my_spaceship->docks[n];
            if (dockData.dock_type != Dock_Disabled)
            {
                string state = " (" + getDockStateName(dockData.state) + ")";
                //string dockName = "d" + std::to_string(n + 1) + "-" + getDockTypeName(dockData.dock_type)[0];
                string dockName = getDockTypeName(dockData.dock_type) + " - " + std::to_string(n + 1);
                docks->setEntryName(n, dockName + state);
                if (n != index)
                    move_dest_selector->addEntry(dockName, string(n));
            }
        }

        Dock &dockData = my_spaceship->docks[index];
        move_dest_selector->setSelectionIndex(move_dest_selector->indexByValue(string(dockData.move_target_index)));

        switch (dockData.state)
        {
        case Empty:
            sideBar->setVisible(false);
            model->setModel(nullptr);
            overlay->setVisible(true);
            overlay_label->setText("Vide");
            distance_bar->setVisible(false);
            cancel_move_button->setVisible(false);
            move_button->setVisible(false);
            move_dest->setVisible(false);
            break;
        case MovingIn:
            displayDroneDetails(dockData);
            overlay->setVisible(true);
            overlay_label->setText("Transfert en cours");
            distance_bar->setVisible(true);
            distance_bar->setValue(dockData.current_distance);
            cancel_move_button->setVisible(false);
            move_button->setVisible(false);
            move_dest->setVisible(false);
            break;
        case Docked:
            displayDroneDetails(dockData);
            move_button->setVisible(true);
            move_dest->setVisible(true);
            overlay->setVisible(false);
            break;
        case MovingOut:
            displayDroneDetails(dockData);
            overlay->setVisible(true);
            overlay_label->setText("Transfert en cours");
            distance_bar->setVisible(true);
            distance_bar->setValue(dockData.current_distance);
            cancel_move_button->setVisible(true);
            move_button->setVisible(false);
            move_dest->setVisible(false);
            break;
        }
    }
}

void DockMasterScreen::displayDroneDetails(Dock &dockData)
{
    P<Cargo> cargo = dockData.getCargo();
    sideBar->setVisible(true);

    unsigned int cnt = 0;
    for(std::tuple<string, string, string> e : cargo->getEntries())
    {
        if (cnt == cargoInfoItems.size())
        {
            cargoInfoItems.push_back(new GuiKeyValueDisplay(cargoInfo, "INFO_" + string(cnt), 0.5, "", ""));
            cargoInfoItems[cnt]->setSize(COLUMN_WIDTH, 40);
        }else{
            cargoInfoItems[cnt]->show();
        }
        cargoInfoItems[cnt]->setIcon(std::get<0>(e))->setKey(std::get<1>(e))->setValue(std::get<2>(e));
        cnt++;
    }
    while(cnt < cargoInfoItems.size())
    {
        cargoInfoItems[cnt]->hide();
        cnt++;
    }

    energy_bar->setValue(cargo->getEnergy());
    energy_bar->setRange(cargo->getMinEnergy(), cargo->getMaxEnergy());
    energy_slider->setRange(cargo->getMinEnergy(), cargo->getMaxEnergy());
    energy_slider->setValue(dockData.energy_request);
    for(int n = 0; n < MW_Count; n++)
    {
        weapons_stock_ship[n]->setText(string(cargo->getWeaponStorage(EMissileWeapons(n))) + " / " + string(cargo->getWeaponStorageMax(EMissileWeapons(n))));
        if (my_spaceship)
            weapons_stock_cargo[n]->setText(string(my_spaceship->getWeaponStorage(EMissileWeapons(n))) + " / " + string(my_spaceship->getWeaponStorageMax(EMissileWeapons(n))));
    }
    model->setModel(cargo->getModel());
}
