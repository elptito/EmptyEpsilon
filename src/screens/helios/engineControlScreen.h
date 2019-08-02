#ifndef ENGINE_CONTROL_SCREEN_H
#define ENGINE_CONTROL_SCREEN_H

#include "gui/gui2_overlay.h"
#include "shipTemplate.h"
#include "playerInfo.h"

class GuiKeyValueDisplay;
class GuiLabel;
class GuiSlider;
class GuiAutoLayout;
class GuiImage;
class GuiArrow;
class GuiToggleButton;
class GuiProgressbar;

class EngineControlScreen : public GuiOverlay
{
private:
    GuiOverlay* background_gradient;
    GuiOverlay* background_crosses;

    GuiKeyValueDisplay* energy_display;
    GuiKeyValueDisplay* hull_display;
    GuiKeyValueDisplay* front_shield_display;
    GuiKeyValueDisplay* rear_shield_display;
    
    
    class RollingDeriviateAvg {
        public:
        float last_measurement;
        float average;

        void apply(float deltaTime, float currentMeasurement);
    };

    class SystemRow
    {
    public:
        GuiAutoLayout* layout;
        GuiLabel* name_label;
        GuiProgressbar* damage_bar;
        GuiLabel* damage_label;
        GuiProgressbar* heat_bar;
        GuiArrow* heat_arrow;
        GuiImage* heat_icon;
        GuiProgressbar* power_bar;
        GuiProgressbar* coolant_bar;
        GuiProgressbar* effectiveness_bar;
        GuiLabel* repair_label;
        GuiImage* repair_icon;
        RollingDeriviateAvg heat_deriv;
    };
    std::vector<SystemRow> system_rows;
    GuiAutoLayout* system_effects_container;
    std::vector<GuiKeyValueDisplay*> system_effects;
    unsigned int system_effects_index;
    float last_measurement_time;
    RollingDeriviateAvg energy_deriv;
    // float previous_energy_level;
    // float average_energy_delta;
    
    void addSystemEffect(string key, string value);
public:
    EngineControlScreen(GuiContainer* owner, ECrewPosition crew_position=engineering);
    
    virtual void onDraw(sf::RenderTarget& window) override;
    virtual void onHotkey(const HotkeyResult& key) override;
    virtual bool onJoystickAxis(const AxisAction& axisAction) override;
};

#endif//ENGINE_CONTROL_SCREEN_H
