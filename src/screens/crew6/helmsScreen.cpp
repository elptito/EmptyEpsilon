#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "helmsScreen.h"

#include "screenComponents/radarView.h"
#include "screenComponents/impulseControls.h"
#include "screenComponents/warpControls.h"
#include "screenComponents/jumpControls.h"
#include "screenComponents/dockingButton.h"
#include "screenComponents/alertOverlay.h"
#include "screenComponents/customShipFunctions.h"

#include "gui/gui2_label.h"
#include "gui/gui2_togglebutton.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_autolayout.h"

HelmsScreen::HelmsScreen(GuiContainer* owner)
: GuiOverlay(owner, "HELMS_SCREEN", colorConfig.background)
{
    // Render the radar shadow and background decorations.
    background_gradient = new GuiOverlay(this, "BACKGROUND_GRADIENT", sf::Color::White);
    background_gradient->setTextureCenter("gui/BackgroundGradient");

    background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));

    GuiRadarView* radar = new GuiRadarView(this, "HELMS_RADAR", my_spaceship->tactical_radar_range, nullptr, my_spaceship);

    combat_maneuver = new GuiCombatManeuver(this, "COMBAT_MANEUVER", my_spaceship);
    combat_maneuver->setPosition(-20, -20, ABottomRight)->setSize(280, 215);

    radar->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMatchHeight, 800);
    radar->setRangeIndicatorStepSize(1000.0)->shortRange()->enableGhostDots()->enableWaypoints()->enableCallsigns()->enableHeadingIndicators()->setStyle(GuiRadarView::Circular);
    radar->enableMissileTubeIndicators();
    radar->setCallbacks(
        [this](sf::Vector2f position) {
            if (my_spaceship)
            {
                float angle = sf::vector2ToAngle(position - my_spaceship->getPosition());
                heading_hint->setText(string(fmodf(angle + 90.f + 360.f, 360.f), 1))->setPosition(InputHandler::getMousePos() - sf::Vector2f(0, 50))->show();
                my_spaceship->commandTargetRotation(angle);
            }
        },
        [this](sf::Vector2f position) {
            if (my_spaceship)
            {
                float angle = sf::vector2ToAngle(position - my_spaceship->getPosition());
                heading_hint->setText(string(fmodf(angle + 90.f + 360.f, 360.f), 1))->setPosition(InputHandler::getMousePos() - sf::Vector2f(0, 50))->show();
                my_spaceship->commandTargetRotation(angle);
            }
        },
        [this](sf::Vector2f position) {
            if (my_spaceship)
                my_spaceship->commandTargetRotation(sf::vector2ToAngle(position - my_spaceship->getPosition()));
            heading_hint->hide();
        }
    );

    radar->setJoystickCallbacks(
        [this](float x_position) {
            if (my_spaceship)
            {
                my_spaceship->commandTurnSpeed(x_position / 100);
            }
        },
        [this](float y_position) {
//            if (my_spaceship)
//            {
//                if (my_spaceship->combat_maneuver_boost_speed > 0.0)
//                {
//                    // Add some more hysteresis, since y-axis can be hard to keep at 0
//                    float value;
//                    if (y_position > 0)
//                        value = (y_position-20)*1.25/100;
//                    else
//                        value = (y_position+20)*1.25/100;
//
//                    my_spaceship->commandCombatManeuverBoost(-value);
//                    combat_maneuver->setBoostValue(fabs(value));
//                }
//                else
//                {
//                    my_spaceship->commandCombatManeuverBoost(0.0);
//                combat_maneuver->setBoostValue(0.0);
//                }
//            }
        },
        [this](float z_position) {
            if (my_spaceship)
                my_spaceship->commandImpulse(-(z_position / 100));
        },
        [this](float r_position) {
//            if (my_spaceship)
//            {
//                if (my_spaceship->combat_maneuver_strafe_speed > 0.0)
//                {
//                    my_spaceship->commandCombatManeuverStrafe(r_position/100);
//                    combat_maneuver->setStrafeValue(r_position/100);
//                }
//            }
        }
        );
    heading_hint = new GuiLabel(this, "HEADING_HINT", "", 30);
    heading_hint->setAlignment(ACenter)->setSize(0, 0);

    energy_display = new GuiKeyValueDisplay(this, "ENERGY_DISPLAY", 0.45, "Energie", "");
    energy_display->setIcon("gui/icons/energy")->setTextSize(20)->setPosition(20, 100, ATopLeft)->setSize(240, 40);
    heading_display = new GuiKeyValueDisplay(this, "HEADING_DISPLAY", 0.45, "Direction", "");
    heading_display->setIcon("gui/icons/heading")->setTextSize(20)->setPosition(20, 140, ATopLeft)->setSize(240, 40);
    velocity_display = new GuiKeyValueDisplay(this, "VELOCITY_DISPLAY", 0.45, "Vitesse", "");
    velocity_display->setIcon("gui/icons/speed")->setTextSize(20)->setPosition(20, 180, ATopLeft)->setSize(240, 40);

    GuiAutoLayout* engine_layout = new GuiAutoLayout(this, "ENGINE_LAYOUT", GuiAutoLayout::LayoutHorizontalLeftToRight);
    engine_layout->setPosition(20, -100, ABottomLeft)->setSize(GuiElement::GuiSizeMax, 300);
    (new GuiImpulseControls(engine_layout, "IMPULSE", my_spaceship))->setSize(100, GuiElement::GuiSizeMax);
    warp_controls = (new GuiWarpControls(engine_layout, "WARP", my_spaceship))->setSize(100, GuiElement::GuiSizeMax);
    jump_controls = (new GuiJumpControls(engine_layout, "JUMP", my_spaceship))->setSize(100, GuiElement::GuiSizeMax);

    (new GuiDockingButton(this, "DOCKING", my_spaceship))->setPosition(20, -20, ABottomLeft)->setSize(280, 50);

    (new GuiCustomShipFunctions(this, helmsOfficer, "", my_spaceship))->setPosition(-20, 120, ATopRight)->setSize(250, GuiElement::GuiSizeMax);
}

void HelmsScreen::onDraw(sf::RenderTarget& window)
{
    if (my_spaceship)
    {
        energy_display->setValue(string(int(my_spaceship->energy_level/my_spaceship->max_energy_level * 100)) + "%");
        if (my_spaceship->energy_level < 100)
            energy_display->setColor(sf::Color::Red);
        else
            energy_display->setColor(sf::Color::White);

        heading_display->setValue(string(my_spaceship->getHeading(), 1) + "�");
        float velocity = sf::length(my_spaceship->getVelocity()) / 1000 * 60;
        velocity_display->setValue(string(velocity, 1) + DISTANCE_UNIT_1K + "/min");

        warp_controls->setVisible(my_spaceship->has_warp_drive);
        jump_controls->setVisible(my_spaceship->has_jump_drive);
    }
    GuiOverlay::onDraw(window);
}

void HelmsScreen::onHotkey(const HotkeyResult& key)
{
    if (key.category == "HELMS" && my_spaceship)
    {
        if (key.hotkey == "TURN_LEFT")
            my_spaceship->commandTargetRotation(my_spaceship->getRotation() - 5.0f);
        else if (key.hotkey == "TURN_RIGHT")
            my_spaceship->commandTargetRotation(my_spaceship->getRotation() + 5.0f);
    }
}
