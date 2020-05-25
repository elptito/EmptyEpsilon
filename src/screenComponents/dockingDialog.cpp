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
    started = false;
    locked = false;
    finished = false;

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
    }
        
    ship_icon = new GuiImage(docking_map, "", ship_radar);
    ship_icon->setColor(ship_color)->setPosition(490/2, 100, ATopCenter)->setSize(25,25);
    
    locked_label = new GuiLabel(docking_map, id + "_LOCK_LABEL", "Docking in progress...", 40);
    locked_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    
    for(int n=0; n<max_bars; n++)
    {
        dock[n] = new GuiElement(docking_map, id + "_" + string(n) + "_DOCK");
        dock[n]->setPosition(0, 200 + 5*n, ATopCenter)->setSize(GuiElement::GuiSizeMax, 5);
        
        left_dock[n] = new GuiOverlay(dock[n], id + "_" + string(n) + "_LEFT_DOCK", sf::Color(128, 128, 128, 128));
        left_dock[n]->setSize(200, GuiElement::GuiSizeMax)->setPosition(0, 0, ATopLeft);
        
        right_dock[n] = new GuiOverlay(dock[n], id + "_" + string(n) + "_RIGHT_DOCK", sf::Color(128, 128, 128, 128));
        right_dock[n]->setSize(200, GuiElement::GuiSizeMax)->setPosition(0, 0, ATopRight);
    }
    
    target_icon = new GuiImage(box, "", target_radar);
    target_icon->setColor(target_color)->setPosition(0, -80, ABottomCenter)->setSize(30,30);
        
    cancel_button = new GuiButton(box, id + "_CANCEL", "Cancel", [this]() {
        if (my_spaceship)
            my_spaceship->commandAbortDock();
        this->setupParameters();
    });
    cancel_button->setPosition(0, -20, ABottomCenter)->setSize(300, 50);

    setupParameters();
}

void GuiDockingDialog::onDraw(sf::RenderTarget& window)
{   
    if (my_spaceship)
    {
        if (my_spaceship->docking_complexity > 0)
        {
            if (!box->isVisible())
            {
                box->show();
                setupParameters();
            }
            updateDocking();
            if (finished)
            {
                my_spaceship->docking_complexity = 0;
                setupParameters();
            }
        }else{
            box->hide();
        }
    }
}

void GuiDockingDialog::setupParameters()
{
    if (!my_spaceship)
        return;
    
    ship_angle = 90;
    ship_icon->setPosition(490/2, 100, ATopCenter);
    ship_position_x = ship_icon->getPositionOffset().x;
    ship_position_y = ship_icon->getPositionOffset().y;
    ship_icon->setPosition(ship_position_x,ship_position_y);
    
    started = false;
    locked = false;
    finished = false;
    locked_label->hide();
    dock_complexity = 50 - 10 * my_spaceship->docking_complexity;
    
    for(int n=0; n<max_bars; n++)
    {
        dock[n]->setPosition(0, 200 + 5*n, ATopCenter);
        // Initial visibility
        float dock_position = dock[n]->getPositionOffset().y;
        if (dock_position > 300)
             dock[n]->hide();
        else
            dock[n]->show();
            
        if (n == 0)
        {
            dock_direction[n] = (random(0, 100)<50)? 1 : 0;
            dock_width[n] = 75;
            dock_center[n] = 245;
            left_dock[n]->setSize(dock_center[n] - dock_width[n], GuiElement::GuiSizeMax);
            right_dock[n]->setSize(490 - dock_center[n] - dock_width[n], GuiElement::GuiSizeMax);
        }
        else
        {
            dock_direction[n] = (random(0, 100)<90)? dock_direction[n-1] : 1 - dock_direction[n-1];
            dock_width[n] = dock_width[n-1] + random(-3, 2);
            dock_width[n] = (dock_width[n] > dock_complexity)? dock_width[n] : dock_complexity;
            dock_center[n] = dock_center[n-1] + (dock_direction[n] * random(0, 5));
            left_dock[n]->setSize(dock_center[n] - dock_width[n], GuiElement::GuiSizeMax);
            right_dock[n]->setSize(490 - dock_center[n] - dock_width[n], GuiElement::GuiSizeMax);
        }
    }

}

void GuiDockingDialog::updateDocking()
{
    if (ship_angle < 270)
    {
        ship_angle += 1.0f;
        ship_icon->setAngle(ship_angle);
    }
    if (abs(270 - ship_angle) < 1 && !started)
        started = true;
        
    if (!started)
        return;
    
    // Ship position
    ship_position_x = ship_icon->getPositionOffset().x;
    ship_position_y = ship_icon->getPositionOffset().y;
    
    // Ship position noise
    float random1 = random(-2, 2);
    float random2 = random(-2, 2);
    if (my_spaceship)
    {
        random1 *= std::max(0.0f,1.5f-my_spaceship->getSystemEffectiveness(SYS_Maneuver));
        random2 *= std::max(0.0f,1.5f-my_spaceship->getSystemEffectiveness(SYS_Impulse));
    }
    if (!locked)
    {
        ship_position_x += random1;
        ship_position_y += random2;
        ship_position_x = std::max(std::min(ship_position_x,465.0f),0.0f);
        ship_position_y = std::max(ship_position_y,0.0f);
        ship_icon->setPosition(ship_position_x,ship_position_y);
    }
    
    // Finish mini game
    if (locked && !finished)
    {
        ship_position_x += (ship_position_x < 245)? 0.5 : -0.5;
        ship_position_y += (ship_position_y < 300)? 0.5 : -0.5;
        ship_icon->setPosition(ship_position_x,ship_position_y);
        if (abs(ship_position_x - 245) < 25 && abs(ship_position_y - 300) < 25)
            finished = true;
    }
    
    // Bars move
    for(int n=0; n<max_bars; n++)
    {
        float dock_position = dock[n]->getPositionOffset().y;
        if (dock_position > 300 || dock_position < 0)
             dock[n]->hide();
        else
            dock[n]->show();
            
        // Lock mini game
        if (n == max_bars - 1 && dock_position < ship_position_y && !locked)
        {
            locked = true;
            locked_label->show();
        }
        
        // Dock move
        dock[n]-> setPosition(0, dock_position - 0.5, ATopCenter);
        
        // Damage
        // Only if y near
        float diff_y = ship_position_y - dock[n]->getPositionOffset().y;
        if (abs(diff_y) < 30)
        {
            // Left damage
            float left_x = ship_position_x - (dock_center[n] - dock_width[n]);
            if (left_x < 0 && my_spaceship)
            {
                ship_position_x += 5;
                ship_icon->setPosition(ship_position_x,ship_position_y);
                DamageInfo info(NULL, DT_Kinetic, my_spaceship->getPosition());
                my_spaceship->takeDamage(10, info);
            }
            // Right damage
            float right_x = (dock_center[n] + dock_width[n]) - (ship_position_x + 25);
            if (right_x < 0 && my_spaceship)
            {
                ship_position_x -= 5;
                ship_icon->setPosition(ship_position_x,ship_position_y);
                DamageInfo info(NULL, DT_Kinetic, my_spaceship->getPosition());
                my_spaceship->takeDamage(10, info);
            }
        }
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
