#include "dockingDialog.h"
#include "spaceObjects/playerSpaceship.h"
#include "playerInfo.h"
#include "factionInfo.h"
#include "gui/gui2_panel.h"
#include "gui/gui2_label.h"
#include "gui/gui2_slider.h"
#include "gui/gui2_button.h"
#include "gui/gui2_image.h"
#include "gui/gui2_overlay.h"

GuiDockingDialog::GuiDockingDialog(GuiContainer* owner, string id)
: GuiElement(owner, id)
{
    locked = false;
    lock_start_time = 0;

    setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    
    box = new GuiPanel(this, id + "_BOX");
    box->setSize(500, 500)->setPosition(0, 0, ACenter);
    
    docking_label = new GuiLabel(box, id + "_LABEL", "Docking procedure", 30);
    docking_label->addBackground()->setPosition(0, 20, ATopCenter)->setSize(450, 50);
    
    docking_map = new GuiElement(box, id + "_MAP");
    docking_map->setPosition(0, 80, ATopCenter)->setSize(490, 300);

    if (my_spaceship)
    {
        ship_radar = my_spaceship->radar_trace;
        ship_color = factionInfo[my_spaceship->getFactionId()]->gm_color;
        //target_color = factionInfo[my_spaceship->docking_target->getFactionId()]->gm_color;
        //P<SpaceShip> target = my_spaceship->docking_target;
        //if (target)
        //    target_radar = target->radar_trace;
        //P<SpaceStation> target_alt = my_spaceship->docking_target;
        //if (target_alt)
        //    target_radar = target_alt->radar_trace;
    }
        
    ship_icon = new GuiImage(docking_map, "", ship_radar);
    ship_icon->setColor(ship_color)->setPosition(490/2, 0, ATopCenter)->setSize(25,25);
    
    locked_label = new GuiLabel(docking_map, id + "_LOCK_LABEL", "Docking in progress...", 40);
    locked_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    
    for(int n=0; n<max_bars; n++)
    {
        dock[n] = new GuiElement(docking_map, id + "_" + string(n) + "_DOCK");
        dock[n]->setPosition(0, 100 + 75*n, ATopCenter)->setSize(GuiElement::GuiSizeMax, 25);
        
        left_dock[n] = new GuiOverlay(dock[n], id + "_" + string(n) + "_LEFT_DOCK", sf::Color(128, 128, 128, 128));
        left_dock[n]->setSize(200, GuiElement::GuiSizeMax)->setPosition(0, 0, ATopLeft);
        
        right_dock[n] = new GuiOverlay(dock[n], id + "_" + string(n) + "_RIGHT_DOCK", sf::Color(128, 128, 128, 128));
        right_dock[n]->setSize(200, GuiElement::GuiSizeMax)->setPosition(0, 0, ATopRight);
    }
    
    target_icon = new GuiImage(box, "", target_radar);
    target_icon->setColor(target_color)->setPosition(0, -80, ABottomCenter)->setSize(30,30);
        
    cancel_button = new GuiButton(box, id + "_CANCEL", "Cancel", []() {
        if (my_spaceship)
            my_spaceship->commandAbortDock();
    });
    cancel_button->setPosition(0, -20, ABottomCenter)->setSize(300, 50);

    setupParameters();
}

void GuiDockingDialog::onDraw(sf::RenderTarget& window)
{
    updateDocking();
    
    if (my_spaceship)
    {
        if (my_spaceship->docking_complexity > 0)
        {
            if (!box->isVisible())
            {
                box->show();
                setupParameters();
            }
            if (angle < 270)
            {
                angle += 1.0f;
                ship_icon->setAngle(angle);
            }
            if (locked && engine->getElapsedTime() - lock_start_time > lock_delay)
                my_spaceship->docking_complexity = 0;
        }else{
            box->hide();
        }
    }
}

void GuiDockingDialog::setupParameters()
{
    if (!my_spaceship)
        return;
        
    for(int n=0; n<max_bars; n++)
    {
        if (n < my_spaceship->docking_complexity)
        {
            dock[n]->show();
            dock_center[n] = random(25, 465); // max 490 +/- 25*2
            left_dock[n]->setSize(dock_center[n] - dock_width, GuiElement::GuiSizeMax);
            right_dock[n]->setSize(490 - dock_center[n] - dock_width, GuiElement::GuiSizeMax);
        }
        else
            dock[n]->hide();
    }
    box->setSize(500, 300 + 100 * my_spaceship->docking_complexity);
    
    docking_map->setSize(490, 100 + 100 * my_spaceship->docking_complexity);

    ship_icon->setAngle(90);
    locked = false;
    locked_label->hide();
}

void GuiDockingDialog::updateDocking()
{
    float random1 = random(-2, 2);
    float random2 = random(-2, 2);
    if (my_spaceship)
    {
        random1 *= std::max(0.0f,1.5f-my_spaceship->getSystemEffectiveness(SYS_Maneuver));
        random2 *= std::max(0.0f,1.5f-my_spaceship->getSystemEffectiveness(SYS_Impulse));
    }
    ship_position_x = ship_icon->getPositionOffset().x + random1;
    ship_position_y = ship_icon->getPositionOffset().y + random2;
    ship_position_x = std::max(std::min(ship_position_x,465.0f),0.0f);
    ship_position_y = std::max(ship_position_y,0.0f);
    
    ship_icon->setPosition(ship_position_x,ship_position_y);
    
    float success_y;
    if (my_spaceship)
        success_y = 75 + 100 * my_spaceship->docking_complexity;
        
    if (ship_position_y > success_y && !locked)
    {
        ship_icon->hide();
        target_icon->hide();
        for(int n=0; n<max_bars; n++)
            dock[n]->hide();
        lock_start_time = engine->getElapsedTime();
        locked = true;
        locked_label->show();
    }

    if (!my_spaceship)
        return;
    
    for(int n=0; n<max_bars; n++)
    {
        if (n < my_spaceship->docking_complexity)
        {
            float diff_x = ship_position_x - dock_center[n];
            float diff_y = ship_position_y - dock[n]->getPositionOffset().y;
            
            if ((diff_x < -25 || diff_x > 0) && abs(diff_y) < 25)
            {
                ship_position_y -= 10;
                ship_icon->setPosition(ship_position_x,ship_position_y);
                DamageInfo info(NULL, DT_Kinetic, my_spaceship->getPosition());
                my_spaceship->takeDamage(10, info);
            }
        }
        else
            dock[n]->hide();
    }
}

void GuiDockingDialog::onHotkey(const HotkeyResult& key)
{
    if (key.category == "HELMS" && my_spaceship)
    {
        ship_position_x = ship_icon->getPositionOffset().x;
        ship_position_y = ship_icon->getPositionOffset().y;
        
        float variation = my_spaceship->getSystemEffectiveness(SYS_Maneuver)*my_spaceship->getSystemEffectiveness(SYS_Impulse);
        
        if (key.hotkey == "INC_IMPULSE")
            ship_position_y = ship_position_y - variation;
        else if (key.hotkey == "DEC_IMPULSE")
            ship_position_y = ship_position_y + variation;
        else if (key.hotkey == "TURN_LEFT")
            ship_position_x = ship_position_x - variation;
        else if (key.hotkey == "TURN_RIGHT")
            ship_position_x = ship_position_x + variation;
        ship_icon->setPosition(ship_position_x,ship_position_y);
    }
}
