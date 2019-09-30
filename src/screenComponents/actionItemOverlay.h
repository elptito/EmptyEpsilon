#ifndef ACTION_ITEM_OVERLAY_H
#define ACTION_ITEM_OVERLAY_H

#include "gui/gui2_overlay.h"

class GuiLabel;
class GuiPanel;
class GuiTextEntry;
class GuiButton;
class ActionItemOverlay : public GuiOverlay
{
private:
    GuiLabel* title_label;
    GuiLabel* query_label; // TODO GuiAdvancedScrollText?
    GuiTextEntry* response_text;
    GuiButton* close_button;
    GuiButton* accept_button;
    GuiButton* decline_button;

    int index;
public:
    ActionItemOverlay(GuiContainer* owner);
    void setIndex(int index);
};

#endif //ACTION_ITEM_OVERLAY_H