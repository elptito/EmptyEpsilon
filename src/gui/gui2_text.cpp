#include "main.h"
#include "gui2_text.h"



GuiText::GuiText(GuiContainer* owner, string id, string text)
: GuiElement(owner, id), text(text),text_size(30) {}

GuiText* GuiText::setText(string text)
{
    this->text = text;
    // calcTextSize();
    return this;
}

GuiText* GuiText::setTextSize(float text_size) { 
    this->text_size = text_size; 
    // calcTextSize();
    return this; 
}

string GuiText::getText() const
{
    return text;
}

// void GuiText::calcTextSize()
// {
//     std::vector<int> sizes(RESOLUTION);
//     IncGenerator f(max_text_size);
//     std::generate(sizes.begin(), sizes.end(), f);
    
//     auto upper = std::lower_bound(sizes.begin(), sizes.end(), this->text, 
//     [this](float size, string value){
//         LOG(INFO) << "checking " << size;
//         return doLineWrap(value, size, rect.width).line_count * text_size < rect.height * float(RESOLUTION-1) / float(RESOLUTION);
//     });
//     text_size = *upper;
// }

void GuiText::onDraw(sf::RenderTarget& window)
{
    // if (lastKnowRect != rect){    
    //     calcTextSize();
    // }
    LineWrapResult wrap = doLineWrap(this->text, text_size, rect.width);
    drawText(window, sf::FloatRect(rect.left, rect.top, rect.width, rect.height), wrap.text, ATopLeft, text_size, main_font, selectColor(colorConfig.textbox.forground));
}
