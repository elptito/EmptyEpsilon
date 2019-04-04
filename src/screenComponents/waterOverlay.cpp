#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "waterOverlay.h"

GuiWaterOverlay::GuiWaterOverlay(GuiContainer* owner)
: GuiElement(owner, "WATER_OVERLAY")
{
    setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
}

void GuiWaterOverlay::onDraw(sf::RenderTarget& window)
{
//    if ((my_spaceship && engine->getGameSpeed() == 1.0f) || game_server)
//        return;

    sf::Sprite staticDisplay;
    textureManager.getTexture("water2.png")->setRepeated(true);
    textureManager.setTexture(staticDisplay, "water2.png");
    staticDisplay.setTextureRect(sf::IntRect(0, 0, 2048, 2048));
    staticDisplay.setOrigin(sf::Vector2f(1024, 1024));
    staticDisplay.setScale(3.0, 3.0);
    staticDisplay.setPosition(sf::Vector2f(x, y));
    window.draw(staticDisplay);
}

void GuiWaterOverlay::update(float delta)
{
    angle_x += delta * irandom(-1,1);
    angle_y += delta * irandom(-1,1);
    x += angle_x*delta*10;
    y += angle_y*delta*10;


    x = fmod(x,1024);
    y = fmod(y,1024);
}
