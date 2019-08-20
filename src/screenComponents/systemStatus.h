#ifndef SYSTEM_STATUS_H
#define SYSTEM_STATUS_H

#include "gui/gui2_element.h"
#include "shipTemplate.h"
#include "spaceObjects/playerSpaceship.h"

class PlayerSpaceship;
class GuiSystemStatus : public GuiElement
{
public:
    static sf::Color getSystemEffectivenessColor(const float effectiveness){
        const float f = effectiveness;
        if (f <= 1.f){
            return sf::Color(255 * (1 - f), 255 * f, 0);
        } else {
            return sf::Color(255 * (f - 1) / 2, 255, 255 * (f - 1) / 2);
        }
    }
    GuiSystemStatus(GuiContainer* owner, string name, ESystem system, P<PlayerSpaceship> targetSpaceship);
    
    virtual void onDraw(sf::RenderTarget& window);
    void setTargetSpaceship(P<PlayerSpaceship> targetSpaceship){target_spaceship = targetSpaceship;}
private:
    ESystem system;
    float text_size;
    EGuiAlign icon_align;
    P<PlayerSpaceship> target_spaceship;

    sf::Vector2f icon_position;
    sf::Vector2f icon_offset;
    float icon_size;
    
    void drawIcon(sf::RenderTarget& window, sf::FloatRect iconRect, string icon_name, sf::Color color);

};

#endif//SYSTEM_STATUS_H
