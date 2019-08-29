#ifndef HELMS_HELIOS_SCREEN_H
#define HELMS_HELIOS_SCREEN_H

#include "gui/gui2_overlay.h"

class GuiKeyValueDisplay;
class GuiLabel;
class GuiProgressbar;
class SpaceObject;
class GuiRadarView;

class HelmsHeliosScreen : public GuiOverlay
{
private:

    GuiRadarView* radar;
    GuiKeyValueDisplay* energy_display;
    GuiKeyValueDisplay* heading_display;
    GuiKeyValueDisplay* velocity_display;
    GuiLabel* heading_hint;
    GuiProgressbar* impulse_bar;
    GuiProgressbar* warp_bar;
    GuiProgressbar* maneuver_charge_bar;
    GuiLabel* docking_indicator;
public:
    HelmsHeliosScreen(GuiContainer* owner);
    
    virtual void onDraw(sf::RenderTarget& window) override;
    virtual void onHotkey(const HotkeyResult& key) override;
    virtual bool onJoystickAxis(const AxisAction& axisAction) override;
    P<SpaceObject> findDockingTarget();

};

#endif//HELMS_HELIOS_SCREEN_H
