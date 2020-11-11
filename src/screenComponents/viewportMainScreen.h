#ifndef VIEWPORT_MAIN_SCREEN_H
#define VIEWPORT_MAIN_SCREEN_H

#include "viewport3d.h"
#include "spaceObjects/scanProbe.h" //Tdelc

class GuiViewportMainScreen : public GuiViewport3D
{
public:
    GuiViewportMainScreen(GuiContainer* owner, string id);

    virtual void onDraw(sf::RenderTarget& window);

    bool first_person = false;

    //Tdelc
    float angle;
    float rotatetime;
    P<ScanProbe> probe;

};

#endif//VIEWPORT_MAIN_SCREEN_H
