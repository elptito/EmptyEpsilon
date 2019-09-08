#ifndef SCAN_HELIOS_SCREEN_H
#define SCAN_HELIOS_SCREEN_H

#include "gui/gui2_element.h"
#include "screenComponents/signalQualityIndicator.h"
#include "gameGlobalInfo.h"
#include "gui/gui2_overlay.h"

class GuiPanel;
class GuiLabel;
class GuiProgressbar;

class ScanHeliosScreen : public GuiOverlay
{
private:
    static constexpr int max_bars = EScanningComplexity::SC_MAX;
    static constexpr float lock_delay = 5.0f;

    GuiPanel* box;
    GuiLabel* signal_label;
    GuiLabel* locked_label;
    GuiLabel* no_target_label;
    GuiSignalQualityIndicator* signal_quality;
    GuiProgressbar* bars[max_bars];
    
    bool scanning;
    float target[max_bars];
    bool locked;
    float lock_start_time;
    int scan_depth;
public:
    ScanHeliosScreen(GuiContainer* owner, ECrewPosition crew_position=scienceHeliosScreen);

    virtual void onDraw(sf::RenderTarget& window);
    virtual bool onJoystickAxis(const AxisAction& axisAction) override;
    
    void reset();
    void setupParameters();
    void updateSignal();
};

#endif//SCAN_HELIOS_SCREEN_H
