#ifndef DOCKING_DIALOG_H
#define DOCKING_DIALOG_H

#include "gui/gui2_element.h"
#include "signalQualityIndicator.h"
#include "gameGlobalInfo.h"

class GuiPanel;
class GuiLabel;
class GuiSlider;
class GuiButton;
class GuiImage;
class GuiOverlay;

class GuiDockingDialog : public GuiElement
{
private:
    static constexpr int max_bars = 4;
    static constexpr float lock_delay = 2.0f;

    sf::RenderTexture background_texture;
    sf::RenderTexture forground_texture;
    
    GuiPanel* box;
    GuiLabel* docking_label;
    GuiLabel* locked_label;
    GuiElement* docking_map;
    GuiImage* ship_icon;
    GuiImage* target_icon;
    GuiElement* dock[max_bars];
    GuiOverlay* left_dock[max_bars];
    GuiOverlay* right_dock[max_bars];
    GuiButton* cancel_button;
    
    string ship_radar = "radar_dread";
    string target_radar = "radartrace_hugestation";
    sf::Color ship_color = sf::Color::White;
    sf::Color target_color = sf::Color::White;
    float angle = 90.0f;
    
    // ship position
    float ship_position_x;
    float ship_position_y;
    
    // docking position
    float dock_center[max_bars];
    float dock_width = 25;
    
    bool locked;
    float lock_start_time;
public:
    GuiDockingDialog(GuiContainer* owner, string id);

    virtual void onDraw(sf::RenderTarget& window) override;
    virtual void onHotkey(const HotkeyResult& key) override;
    
    void setupParameters();
    void updateDocking();
};

#endif//DOCKING_DIALOG_H
