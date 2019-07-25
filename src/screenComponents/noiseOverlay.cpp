#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "noiseOverlay.h"

GuiNoiseOverlay::GuiNoiseOverlay(GuiContainer* owner, string id, bool onShipDestroyed)
: GuiElement(owner, id), onShipDestroyed(onShipDestroyed)
{
    color = sf::Color::White;
    setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
}

void GuiNoiseOverlay::setAlpha(int alpha){
    color.a = std::max(0, std::min(255, alpha));
}

void GuiNoiseOverlay::onDraw(sf::RenderTarget& window)
{
    // if (elements.size() > 0){
    //     LOG(WARNING) << "elements.size: " << elements.size();
    // }
    // for some strange reason, fake elements are sometimes being added to the list
    elements = std::list<GuiElement*>(); //.clear(); 

    if (onShipDestroyed && my_spaceship)
        return;
    
    sf::Sprite staticDisplay;
    textureManager.getTexture("noise.png")->setRepeated(true);
    textureManager.setTexture(staticDisplay, "noise.png");
    staticDisplay.setTextureRect(sf::IntRect(0, 0, 2048, 2048));
    staticDisplay.setOrigin(sf::Vector2f(1024, 1024));
    staticDisplay.setScale(3.0, 3.0);
    staticDisplay.setPosition(sf::Vector2f(random(-512, 512), random(-512, 512)));
    staticDisplay.setColor(color);
    window.draw(staticDisplay);
}
