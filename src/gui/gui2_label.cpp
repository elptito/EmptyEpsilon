#include "gui2_label.h"

GuiLabel::GuiLabel(GuiContainer* owner, string id, string text, float text_size)
: GuiElement(owner, id), text(text), text_size(text_size), text_color(selectColor(colorConfig.label.forground)), bgColor(selectColor(colorConfig.label.background)),text_alignment(ACenter), background(false), bold(false), vertical(false)
{
}

void GuiLabel::onDraw(sf::RenderTarget& window)
{
    if (background)
        drawStretched(window, rect, "gui/LabelBackground", bgColor);
    sf::Font* font = main_font;
    if (bold)
        font = bold_font;
    if (vertical)
        drawVerticalText(window, rect, text, text_alignment, text_size, font, text_color);
    else
        drawText(window, rect, text, text_alignment, text_size, font, text_color);
}

GuiLabel* GuiLabel::setText(string text)
{
    this->text = text;
    return this;
}

string GuiLabel::getText() const
{
    return text;
}

GuiLabel* GuiLabel::setAlignment(EGuiAlign alignment)
{
    text_alignment = alignment;
    return this;
}
GuiLabel* GuiLabel::removeBackground()
{
    background = false;
    return this;
}
GuiLabel* GuiLabel::addBackground()
{
    background = true;
    return this;
}

GuiLabel* GuiLabel::setVertical()
{
    vertical = true;
    return this;
}

GuiLabel* GuiLabel::setBold(bool bold)
{
    this->bold = bold;
    return this;
}


GuiLabel* GuiLabel::setColor(sf::Color color)
{
    this->bgColor = color;
    return this;
}

GuiLabel* GuiLabel::setTextColor(sf::Color color){
    this->text_color = color; 
    return this;
};
