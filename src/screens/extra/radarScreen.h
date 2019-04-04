#ifndef RADAR_SCREEN_H
#define RADAR_SCREEN_H
#include "gui/gui2_overlay.h"
class GuiRadarView;
class RadarScreen : public GuiOverlay, public Updatable
{
private:
    GuiRadarView* tactical_radar;
    GuiRadarView* science_radar;
    GuiRadarView* relay_radar;
    float sonar_step_time = 0.0;
    float sonar_parameter = 5000.0;
    bool sonar_sound_played = false;

public:
    string type;
    RadarScreen(GuiContainer* owner, string type);
    virtual void update(float delta) override;
};
#endif//RADAR_SCREEN_H
