#ifndef COMMS_OVERLAY_H
#define COMMS_OVERLAY_H

#include "gui/gui2_element.h"

class GuiPanel;
class GuiElement;
class GuiProgressbar;
class GuiButton;
class GuiToggleButton;
class GuiLabel;
class GuiScrollText;
class GuiListbox;
class GuiTextEntry;

class GuiCommsOverlay : public GuiElement
{
private:
    GuiPanel* opening_box;
    GuiProgressbar* opening_progress;
    GuiButton* opening_cancel;

    GuiPanel* hailed_box;
    GuiLabel* hailed_label;
    GuiButton* hailed_answer;
    GuiButton* hailed_ignore;

    GuiPanel* no_response_box;
    GuiPanel* broken_box;
    GuiPanel* closed_box;

    GuiElement* chat_comms_title;
    GuiPanel* chat_comms_box;
    GuiTextEntry* chat_comms_message_entry;
    GuiScrollText* chat_comms_text;
    GuiButton* chat_comms_send_button;
    GuiButton* chat_comms_close_button;
    GuiToggleButton* chat_minimize_button;

    GuiPanel* script_comms_box;
    GuiScrollText* script_comms_text;
    GuiListbox* script_comms_options;
    GuiButton* script_comms_close;

    bool minimized;
    float original_height;

public:
    GuiCommsOverlay(GuiContainer* owner);

    virtual void onDraw(sf::RenderTarget& window);
    void minimize(bool minimize=true);
    void clearElements();
};

#endif//COMMS_OVERLAY_H
