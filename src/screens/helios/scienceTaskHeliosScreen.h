#ifndef SCIENCE_TASK_HELIOS_SCREEN_H
#define SCIENCE_TASK_HELIOS_SCREEN_H

#include "gui/gui2_element.h"
#include "screenComponents/signalQualityIndicator.h"
#include "gameGlobalInfo.h"
#include "gui/gui2_overlay.h"

#define HACKER_LiNES 20

class GuiPanel;
class GuiLabel;
class GuiProgressbar;
class GuiKeyValueDisplay;
class GuiTextEntry;
class GuiAdvancedScrollText;

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
    GuiPanel* scanner;
    GuiLabel* signal_label;
    GuiLabel* locked_label;
    GuiLabel* no_target_label;
    GuiSignalQualityIndicator* signal_quality;
    GuiProgressbar* scanner_bars[max_bars];
// hacker
    GuiPanel* hacker;
    GuiLabel* hacked_label;
    GuiAdvancedScrollText* hacker_target;
    GuiAdvancedScrollText* hacker_current;
    GuiTextEntry* hacker_input;

// tasks
    EScienceTaskType simulation;
    int task_id;
// scanner
    bool scanning;
    float scanner_targets[max_bars];
    bool locked;
    float lock_start_time;
    int scan_depth;
// hacker
    unsigned int hacker_next_line;
    float hack_time;

    float last_line_start;
    float add_next_line_time;
    float avg_line_type_time;

public:
    ScienceTaskHeliosScreen(GuiContainer* owner, ECrewPosition crew_position=scienceHeliosScreen);

    virtual void onDraw(sf::RenderTarget& window);
    virtual bool onJoystickAxis(const AxisAction& axisAction) override;
    virtual void onHotkey(const HotkeyResult& key) override;

private:
    P<SpaceShip> getTarget();
    EScienceTaskType getTaskType();
    void completeTask();
// scanner    
    void initScanner();
    void resetScanner();
    void setupParametersScanner();
    void updateSignalScanner();
    void onDrawScanner(sf::RenderTarget& window);

// hacker
    void initHacker();
    void resetHacker();
    void setupParametersHacker();
    void addCodeLine();
    void onDrawHacker(sf::RenderTarget& window);
};

#endif//SCIENCE_TASK_HELIOS_SCREEN_H
