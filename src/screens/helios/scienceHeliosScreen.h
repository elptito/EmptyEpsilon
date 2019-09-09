#ifndef SCIENCE_HELIOS_SCREEN_H
#define SCIENCE_HELIOS_SCREEN_H

#include "screenComponents/targetsContainer.h"
#include "gui/gui2_overlay.h"
#include "spaceObjects/scanProbe.h"
#include "playerInfo.h"

class GuiRadarView;
class GuiKeyValueDisplay;
class GuiFrequencyCurve;
class GuiText;
class GuiAutoLayout;
class GuiLabel;
class GuiProgressbar;

class ScienceHeliosScreen : public GuiOverlay
{
private:
    TargetsContainer targets;
    GuiRadarView* radar;
    GuiProgressbar* zoom_bar;
    GuiLabel* info_callsign;
    GuiKeyValueDisplay* info_distance;
    GuiKeyValueDisplay* info_heading;
    GuiKeyValueDisplay* info_relspeed;
    GuiKeyValueDisplay* info_duration;

    GuiKeyValueDisplay* info_faction;
    GuiKeyValueDisplay* info_type;
    GuiKeyValueDisplay* info_shields;
    GuiKeyValueDisplay* info_hull;
    GuiText* info_description;
    GuiFrequencyCurve* info_shield_frequency;
    GuiFrequencyCurve* info_beam_frequency;
    GuiKeyValueDisplay* info_system[SYS_COUNT];

    GuiLabel* target_mode_waypoints_display;

    GuiLabel* main_view_display;
    GuiLabel* probe_view_display;
    GuiLabel* scan_status;
    GuiLabel* query_action;

    P<SpaceObject> radar_pov;
    bool probe_view;
    bool target_waypoints;
    float zoom;
public:
    ScienceHeliosScreen(GuiContainer* owner, ECrewPosition crew_position=scienceHeliosScreen);

    virtual void onDraw(sf::RenderTarget& window) override;
    virtual void onHotkey(const HotkeyResult& key) override;
    virtual bool onJoystickAxis(const AxisAction& axisAction) override;
private:
    void iterateTagrets(bool forward);
    void showTargetInfo();
    void showWaypointInfo();
    void showNoInfo();
};

#endif//SCIENCE_HELIOS_SCREEN_H
