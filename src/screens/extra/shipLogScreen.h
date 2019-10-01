#ifndef SHIP_LOG_SCREEN_H
#define SHIP_LOG_SCREEN_H

#include "gui/gui2_overlay.h"

class GuiAdvancedScrollText;
class GuiTextEntry;
class GuiToggleButton;

class ShipLogScreen : public GuiOverlay
{
private:
    string station;
    GuiAdvancedScrollText* log_text;
    GuiTextEntry* new_line;
    GuiToggleButton* mode;
public:
    ShipLogScreen(GuiContainer* owner, string station, bool queries = false, bool log = true);
    
    void onDraw(sf::RenderTarget& window) override;
};

#endif//SHIP_LOG_SCREEN_H
