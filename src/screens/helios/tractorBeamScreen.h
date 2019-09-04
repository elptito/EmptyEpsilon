#ifndef TRACTOR_BEAM_SCREEN_H
#define TRACTOR_BEAM_SCREEN_H

#include "gui/gui2_overlay.h"
#include "spaceObjects/spaceshipParts/tractorBeam.h"

class GuiKeyValueDisplay;
class GuiLabel;
class GuiProgressbar;
class SpaceObject;
class GuiRadarView;
class GuiRotationDial;

class TractorBeamScreen : public GuiOverlay
{
private:
    GuiRadarView* radar;
    GuiLabel* mode_display[ETractorBeamMode::TBM_Count];
    GuiRotationDial* direction_dial;
    GuiProgressbar* arc_bar;
    GuiProgressbar* range_bar;

    sf::Vector2f commandPosition;
    float commandArch;
public:
    TractorBeamScreen(GuiContainer* owner);
    
    virtual void onDraw(sf::RenderTarget& window) override;
    virtual void onHotkey(const HotkeyResult& key) override;
    virtual bool onJoystickAxis(const AxisAction& axisAction) override;
};

#endif //TRACTOR_BEAM_SCREEN_H