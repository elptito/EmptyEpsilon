#ifndef HACK_DIALOG_H
#define HACK_DIALOG_H

#include "gui/gui2_overlay.h"

class GuiElement;
class GuiPanel;
class GuiProgressbar;
class GuiButton;
class GuiLabel;
class GuiScrollText;
class GuiListbox;
class GuiTextEntry;
class GuiToggleButton;

class GuiHackDialog : public GuiOverlay
{
private:
    GuiElement* hack_title;
    GuiLabel* hack_label;
    GuiPanel* hack_box;
    GuiTextEntry* hack_message_entry;
    GuiScrollText* hack_comms_text;
    GuiButton* hack_send_button;
    GuiButton* hack_close_button;
    GuiToggleButton* hack_minimize_button;
    GuiProgressbar* progress;

    P<SpaceShip> hack_target;
    //P<SpaceShip> ship;
    string target_system;
    string hack_message;
    int hack_step = 0;
    float hack_delay = 5.0;
    float hack_time = 0.0;
    string hack_code = "";
    bool minimized;
    float original_height;
//    P<SpaceShip> hack_target;

public:
    GuiHackDialog(GuiContainer* owner, string id);

    //void open(P<SpaceObject> target);
//    void open(P<SpaceShip> target);
    void open();
    void minimize(bool minimize=true);
    virtual void onDraw(sf::RenderTarget& window) override;
    void clearElements();
    void commandHack();
    void defineHackCode(int hackDiff);
    int verifHackCode(int hackDiff, string hack_text, string hack_code);
};

#endif//HACK_DIALOG_H
