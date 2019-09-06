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
class GuiButton;
class GuiScanTargetButton;
class GuiToggleButton;
class GuiLabel;
class GuiProgressbar;

class ScienceHeliosScreen : public GuiOverlay
{
public:
    TargetsContainer targets;
    GuiRadarView* science_radar;
    GuiRadarView* probe_radar;
    GuiProgressbar* zoom_bar;
    GuiScanTargetButton* scan_button;
    GuiLabel* info_callsign;
    GuiKeyValueDisplay* info_distance;
    GuiKeyValueDisplay* info_heading;
    GuiKeyValueDisplay* info_relspeed;
    GuiKeyValueDisplay* info_duration;

    GuiKeyValueDisplay* info_faction;
    GuiKeyValueDisplay* info_type;
    GuiButton* info_type_button;
    GuiKeyValueDisplay* info_shields;
    GuiKeyValueDisplay* info_hull;
    GuiText* info_description;
    GuiFrequencyCurve* info_shield_frequency;
    GuiFrequencyCurve* info_beam_frequency;
    GuiKeyValueDisplay* info_system[SYS_COUNT];

    GuiToggleButton* probe_view_button;
    P<ScanProbe> observation_point;
public:
    ScienceHeliosScreen(GuiContainer* owner, ECrewPosition crew_position=scienceHeliosScreen);

    virtual void onDraw(sf::RenderTarget& window) override;
    virtual void onHotkey(const HotkeyResult& key) override;
    virtual bool onJoystickAxis(const AxisAction& axisAction) override{ return false;};
private:
    void iterateTagrets(bool forward);
};

#endif//SCIENCE_HELIOS_SCREEN_H
