#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include "engine.h"
#include "screenComponents/helpOverlay.h"
#include "gui/gui2_canvas.h"
#include "threatLevelEstimate.h"
#include "spaceObjects/scanProbe.h"

class GuiViewport3D;
class GuiRadarView;
class GuiCommsOverlay;
class GuiPanel;
class GuiLabel;
class DamageControlScreen;
class ProbeScreen;
class RelayScreen;
class GuiHelpOverlay;
class ImpulseSound;

class ScreenMainScreen : public GuiCanvas, public Updatable
{
    P<ThreatLevelEstimate> threat_estimate;
private:
    GuiViewport3D* viewport;
    GuiHelpOverlay* keyboard_help;
    string keyboard_general = "";
    GuiRadarView* tactical_radar;
    GuiRadarView* long_range_radar;
    GuiRadarView* global_range_radar;
    GuiPanel* dock_comms;
    GuiLabel* label_dock;
    DamageControlScreen*ship_state;
    ProbeScreen*probe_view;
    float angle;
    float rotatetime;
    P<ScanProbe> probe;
    GuiCommsOverlay* onscreen_comms;
    ImpulseSound* impulse_sound;
public:
    ScreenMainScreen();

    virtual void update(float delta) override;

    virtual void onClick(sf::Vector2f mouse_position) override;
    virtual void onHotkey(const HotkeyResult& key) override;
    virtual void onKey(sf::Event::KeyEvent key, int unicode) override;
};

#endif//MAIN_SCREEN_H
