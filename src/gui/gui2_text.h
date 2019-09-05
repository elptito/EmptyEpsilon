#ifndef GUI_TEXT_H
#define GUI_TEXT_H

#include "gui2_element.h"

class GuiText : public GuiElement
{ 
// private:
//     static const int RESOLUTION = 32;
// struct IncGenerator {
//     int i;
//     float max_text_size;
//     IncGenerator (float text_size) : i(1), max_text_size(text_size){}
//     int operator() () { return  max_text_size * i++ / RESOLUTION; }
// };
protected:
    string text;
    // float max_text_size;
    float text_size;
    // sf::FloatRect lastKnowRect;
public:
    GuiText(GuiContainer* owner, string id, string text);

    GuiText* setText(string text);
    string getText() const;
    GuiText* setTextSize(float text_size);

    virtual void onDraw(sf::RenderTarget& window);
// private:
//     void calcTextSize();
};

#endif//GUI_TEXT_H
