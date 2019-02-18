#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include "engine.h"
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

class ScreenMainScreen : public GuiCanvas, public Updatable
{
    P<ThreatLevelEstimate> threat_estimate;
private:
    GuiViewport3D* viewport;
    GuiRadarView* tactical_radar;
    GuiRadarView* long_range_radar;
    GuiRadarView* global_range_radar;
    GuiPanel* dock_comms;
    GuiLabel* label_dock;
    DamageControlScreen*ship_state;
    ProbeScreen*probe_view;
    bool first_person;
    float angle;
    float rotatetime;
    P<ScanProbe> probe;
    GuiCommsOverlay* onscreen_comms;
    int impulse_sound = -1;
    int warp_sound = -1;
public:
    ScreenMainScreen();

    virtual void update(float delta) override;

    virtual void onClick(sf::Vector2f mouse_position) override;
    virtual void onKey(sf::Event::KeyEvent key, int unicode) override;
};

#endif//MAIN_SCREEN_H
