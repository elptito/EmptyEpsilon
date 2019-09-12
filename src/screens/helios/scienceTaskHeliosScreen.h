#ifndef SCIENCE_TASK_HELIOS_SCREEN_H
#define SCIENCE_TASK_HELIOS_SCREEN_H

#include "gui/gui2_element.h"
#include "screenComponents/signalQualityIndicator.h"
#include "gameGlobalInfo.h"
#include "gui/gui2_overlay.h"

class GuiPanel;
class GuiLabel;
class GuiProgressbar;
class GuiKeyValueDisplay;

class ScienceTaskHeliosScreen : public GuiOverlay
{
private:
    static constexpr int max_bars = EScanningComplexity::SC_MAX;
    static constexpr float lock_delay = 5.0f;

// tasks
    GuiLabel* current_task_title;
    GuiLabel* tasks_queue_title;
    GuiKeyValueDisplay* tasks_queue[PlayerSpaceship::max_science_tasks];

// scanner
    GuiPanel* box;
    GuiLabel* signal_label;
    GuiLabel* locked_label;
    GuiLabel* no_target_label;
    GuiSignalQualityIndicator* signal_quality;
    GuiProgressbar* bars[max_bars];
    
// tasks
    int task_id;
// scanner
    bool scanning;
    float bars_target[max_bars];
    bool locked;
    float lock_start_time;
    int scan_depth;
public:
    ScienceTaskHeliosScreen(GuiContainer* owner, ECrewPosition crew_position=scienceHeliosScreen);

    virtual void onDraw(sf::RenderTarget& window);
    virtual bool onJoystickAxis(const AxisAction& axisAction) override;
    virtual void onHotkey(const HotkeyResult& key) override;

private:
    P<SpaceShip> getTarget();
    EScienceTaskType getTaskType();

// scanner    
    void initScanner();
    void resetScanner();
    void setupParametersScanner();
    void updateSignalScanner();
    void onDrawScanner(sf::RenderTarget& window);
};

#endif//SCIENCE_TASK_HELIOS_SCREEN_H
