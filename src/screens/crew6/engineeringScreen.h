#ifndef ENGINEERING_SCREEN_H
#define ENGINEERING_SCREEN_H

#include "gui/gui2_overlay.h"
#include "shipTemplate.h"
#include "playerInfo.h"

class GuiSelfDestructButton;
class GuiKeyValueDisplay;
class GuiLabel;
class GuiSlider;
class GuiAutoLayout;
class GuiImage;
class GuiArrow;
class GuiToggleButton;
class GuiButton; 
class GuiProgressbar;
class GuiProgressSlider;
class GuiSystemEffectsList;

class EngineeringScreen : public GuiOverlay
{
private:
    GuiOverlay* background_crosses;

    GuiKeyValueDisplay* energy_display;
    GuiProgressbar* energy_bar;
    GuiKeyValueDisplay* hull_display;
    GuiProgressbar* hull_bar;
    GuiKeyValueDisplay* shield_display;
    GuiProgressbar* shield_bar;
    GuiKeyValueDisplay* oxygen_display;
    GuiProgressbar* oxygen_bar;
    GuiKeyValueDisplay* coolant_display;
    GuiKeyValueDisplay* repair_display;
    GuiSelfDestructButton* self_destruct_button;
    GuiSystemEffectsList* system_effects_container;
    GuiLabel* power_label;
    GuiSlider* power_slider;
    GuiLabel* coolant_label;
    GuiSlider* coolant_slider;
    GuiLabel* repair_label;
    GuiSlider* repair_slider;
    GuiToggleButton* presets_button;
    std::vector<GuiButton*> presets_buttons;

    class SystemRow
    {
    public:
        GuiAutoLayout* layout;
        GuiToggleButton* button;
        GuiProgressbar* damage_bar;
        GuiImage* damage_icon;
        GuiLabel* damage_label;
        GuiProgressbar* heat_bar;
        GuiArrow* heat_arrow;
        GuiImage* heat_icon;
        GuiLabel* heat_label;
        GuiProgressSlider* power_bar;
        GuiLabel* power_label;
        GuiProgressSlider* coolant_bar;
        GuiLabel* coolant_label;
        GuiProgressSlider* repair_bar;
        GuiLabel* repair_label;
    };
    std::vector<SystemRow> system_rows;

    ESystem selected_system;
    
    float previous_energy_measurement;
    float previous_energy_level;
    float average_energy_delta;
    void selectSystem(ESystem system);

    string toNearbyIntString(float value);
public:
    EngineeringScreen(GuiContainer* owner, ECrewPosition crew_position=engineering);

    virtual void onDraw(sf::RenderTarget& window) override;
    virtual void onHotkey(const HotkeyResult& key) override;
    virtual bool onJoystickAxis(const AxisAction& axisAction) override;
    virtual void applyPreset(int preset);
    virtual void updatePreset(int preset);
};

#endif//ENGINEERING_SCREEN_H
