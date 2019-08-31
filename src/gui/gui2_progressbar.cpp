#include "gui2_progressbar.h"

GuiProgressbar::GuiProgressbar(GuiContainer* owner, string id, float min_value, float max_value, float start_value)
: GuiElement(owner, id), min_value(min_value), max_value(max_value), value(start_value), color(sf::Color(255, 255, 255, 64)), text_color(sf::Color::White), drawBackground(true), bi_directional(false)
{
}

void GuiProgressbar::onDraw(sf::RenderTarget& window)
{
    float f = (value - min_value) / (max_value - min_value);

    if (drawBackground)
        drawStretched(window, rect, "gui/ProgressbarBackground");

    sf::FloatRect fill_rect = rect;
    if (rect.width >= rect.height)
    {
        if (bi_directional) {
            if (f > 0.5){
                fill_rect.width *= f - 0.5;
                fill_rect.left = rect.left + rect.width / 2;
            } else {
                fill_rect.width *= 0.5 - f;
                fill_rect.left = rect.left + rect.width / 2 - fill_rect.width;
            }
        } else{
            fill_rect.width *= f;
        }
        drawStretchedH(window, fill_rect, "gui/ProgressbarFill", color);
    } else {
        if (bi_directional) {
            if (f > 0.5){
                fill_rect.height *= f - 0.5;
                fill_rect.top = rect.top + rect.height / 2 - fill_rect.height;
            } else {
                fill_rect.height *= 0.5 - f;
                fill_rect.top = rect.top + rect.height / 2;
            }
        } else{
            fill_rect.height *= f;
            fill_rect.top = rect.top + rect.height - fill_rect.height;
        }
        drawStretchedV(window, fill_rect, "gui/ProgressbarFill", color);
    }
    drawText(window, rect, text, ACenter, 30.f, main_font, text_color);
}

GuiProgressbar* GuiProgressbar::setValue(float value)
{
    this->value = value;
    return this;
}

GuiProgressbar* GuiProgressbar::setRange(float min_value, float max_value)
{
    this->min_value = min_value;
    this->max_value = max_value;
    return this;
}

GuiProgressbar* GuiProgressbar::setText(string text)
{
    this->text = text;
    return this;
}

GuiProgressbar* GuiProgressbar::setColor(sf::Color color)
{
    this->color = color;
    return this;
}

GuiProgressbar* GuiProgressbar::setTextColor(sf::Color color)
{
    this->text_color = color;
    return this;
}

GuiProgressbar* GuiProgressbar::setDrawBackground(bool drawBackground)
{
    this->drawBackground = drawBackground;
    return this;
}

GuiProgressbar* GuiProgressbar::setBiDirectional(bool biDirectional)
{
    this->bi_directional = biDirectional;
    return this;
}
