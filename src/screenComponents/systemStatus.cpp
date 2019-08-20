#include "systemStatus.h"
#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "spaceObjects/warpJammer.h"

#define NUM_ICONS 6

GuiSystemStatus::GuiSystemStatus(GuiContainer* owner, string name, ESystem system, P<PlayerSpaceship> targetSpaceship)
: GuiElement(owner, name), system(system), text_size(20), target_spaceship(targetSpaceship)
{
}

void GuiSystemStatus::drawIcon(sf::RenderTarget& window, sf::FloatRect iconRect, string icon_name, sf::Color color)
{
    sf::Sprite icon;
    textureManager.setTexture(icon, icon_name);
    float scale = iconRect.height / icon.getTextureRect().height;
    icon.setScale(scale, scale);
    icon.setPosition(iconRect.left + iconRect.width / 2, iconRect.top + iconRect.height / 2);
    icon.setColor(color);
    window.draw(icon);
}

void GuiSystemStatus::onDraw(sf::RenderTarget& window)
{
    if (!target_spaceship)
        return;
    
    icon_size = std::min(rect.width, rect.height) * 0.8;

    drawStretched(window, rect, "gui/LabelBackground", selectColor(colorConfig.label.background));
    sf::Font* font = main_font;
    sf::FloatRect textRect(rect.left, rect.top, rect.width - NUM_ICONS * icon_size, rect.height);

    sf::Color textColor = getSystemEffectivenessColor(target_spaceship->getSystemEffectiveness(system));

    drawText(window, textRect, getSystemName(system), ACenter, text_size, font, textColor);

    float power = target_spaceship->systems[system].power_level;
    float health = target_spaceship->systems[system].health;
    float heat = target_spaceship->systems[system].heat_level;
    float hacked_level = target_spaceship->systems[system].hacked_level;

    sf::FloatRect iconRect(rect.left + rect.width - icon_size, rect.top, icon_size, rect.height);
    if (health <= 0.0) {
        drawIcon(window, iconRect, "gui/icons/status_damaged", colorConfig.overlay_damaged);
    } else {
        drawText(window, iconRect, "OK" , ACenter, text_size, font, colorConfig.overlay_ok);
    }
    iconRect.left -= icon_size;
    if (power == 0.0) {
        drawIcon(window, iconRect, "gui/icons/status_no_power", colorConfig.overlay_no_power);
    } else if (power < 0.3) {
        drawIcon(window, iconRect, "gui/icons/status_low_power", colorConfig.overlay_low_power);
    } else {
        drawText(window, iconRect, "OK" , ACenter, text_size, font, colorConfig.overlay_ok);
    }
    iconRect.left -= icon_size;
    if (target_spaceship->energy_level < 10) {
        drawIcon(window, iconRect, "gui/icons/status_low_energy", colorConfig.overlay_low_energy);
    } else {
        drawText(window, iconRect, "OK" , ACenter, text_size, font, colorConfig.overlay_ok);
    }
    iconRect.left -= icon_size;
    if (heat > 0.90) {
        drawIcon(window, iconRect, "gui/icons/status_overheat", colorConfig.overlay_overheating);
    } else {
        drawText(window, iconRect, "OK" , ACenter, text_size, font, colorConfig.overlay_ok);
    }
    iconRect.left -= icon_size;
    if (hacked_level > 0.1) {
        drawIcon(window, iconRect, "gui/icons/self-destruct", colorConfig.overlay_hacked);
    } else {
        drawText(window, iconRect, "OK" , ACenter, text_size, font, colorConfig.overlay_ok);
    }
    if (system == SYS_Warp || system == SYS_JumpDrive){
        iconRect.left -= icon_size;
        if (WarpJammer::isWarpJammed(target_spaceship->getPosition())) {
            drawIcon(window, iconRect, "gui/icons/status_jammed", colorConfig.overlay_jammed);
        } else {
            drawText(window, iconRect, "OK" , ACenter, text_size, font, colorConfig.overlay_ok);
        }
    }
}

