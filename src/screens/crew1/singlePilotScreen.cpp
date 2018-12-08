#include "main.h"
#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "singlePilotScreen.h"
#include "singlePilotView.h"
#include "gui/gui2_element.h"

#include "screenComponents/viewport3d.h"

#include "screenComponents/alertOverlay.h"

SinglePilotScreen::SinglePilotScreen(GuiContainer* owner)
: GuiOverlay(owner, "SINGLEPILOT_SCREEN", colorConfig.background)
{
    // Create a 3D viewport behind everything, to serve as the right-side panel
    viewport = new GuiViewport3D(this, "3D_VIEW");
    viewport->setPosition(1000, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    // Create left panel for controls.
    left_panel = new GuiElement(this, "LEFT_PANEL");
    left_panel->setPosition(0, 0, ATopLeft)->setSize(1000, GuiElement::GuiSizeMax);

    // single pilot
    single_pilot_view = new SinglePilotView(left_panel, my_spaceship);
    single_pilot_view->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    background_crosses = new GuiOverlay(left_panel, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));

    // 5U tactical radar with piloting features.
    radar = new GuiRadarView(left_panel, "TACTICAL_RADAR", 5000.0, &targets);
    radar->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMatchHeight, 650);
    radar->setRangeIndicatorStepSize(1000.0)->shortRange()->enableGhostDots()->enableWaypoints()->enableCallsigns()->enableHeadingIndicators()->setStyle(GuiRadarView::Circular);
    radar->setCallbacks(
        [this](sf::Vector2f position) {
            targets.setToClosestTo(position, 250, TargetsContainer::Targetable);
            if (my_spaceship && targets.get())
                my_spaceship->commandSetTarget(targets.get());
            else if (my_spaceship)
                my_spaceship->commandTargetRotation(sf::vector2ToAngle(position - my_spaceship->getPosition()));
        },
        [this](sf::Vector2f position) {
            if (my_spaceship)
                my_spaceship->commandTargetRotation(sf::vector2ToAngle(position - my_spaceship->getPosition()));
        },
        [this](sf::Vector2f position) {
            if (my_spaceship)
                my_spaceship->commandTargetRotation(sf::vector2ToAngle(position - my_spaceship->getPosition()));
        }
    );

    // Joystick controls.
    radar->setJoystickCallbacks(
        [this](float x_position) {
            if (my_spaceship)
            {
                float angle = my_spaceship->getRotation() + x_position;
                my_spaceship->commandTargetRotation(angle);
            }
        },
        [this](float y_position) {
            if (my_spaceship && (fabs(y_position) > 20))
            {
                // Add some more hysteresis, since y-axis can be hard to keep at 0
                float value;
                if (y_position > 0)
                    value = (y_position-20) * 1.25 / 100;
                else
                    value = (y_position+20) * 1.25 / 100;

                my_spaceship->commandCombatManeuverBoost(-value);
            }
            else if (my_spaceship)
            {
                my_spaceship->commandCombatManeuverBoost(0.0);
            }
        },
        [this](float z_position) {
            if (my_spaceship)
                my_spaceship->commandImpulse(-(z_position / 100));
        },
        [this](float r_position) {
            if (my_spaceship)
                my_spaceship->commandCombatManeuverStrafe(r_position / 100);
        }
    );

    // Ship stats and combat maneuver at bottom right corner of left panel.
    (new GuiCombatManeuver(left_panel, "COMBAT_MANEUVER"))->setPosition(-20, -180, ABottomRight)->setSize(200, 150);

    energy_display = new GuiKeyValueDisplay(left_panel, "ENERGY_DISPLAY", 0.45, "Energie", "");
    energy_display->setIcon("gui/icons/energy")->setTextSize(20)->setPosition(-20, -140, ABottomRight)->setSize(240, 40);
    heading_display = new GuiKeyValueDisplay(left_panel, "HEADING_DISPLAY", 0.45, "Direction", "");
    heading_display->setIcon("gui/icons/heading")->setTextSize(20)->setPosition(-20, -100, ABottomRight)->setSize(240, 40);
    velocity_display = new GuiKeyValueDisplay(left_panel, "VELOCITY_DISPLAY", 0.45, "Vitesse", "");
    velocity_display->setIcon("gui/icons/speed")->setTextSize(20)->setPosition(-20, -60, ABottomRight)->setSize(240, 40);
    shields_display = new GuiKeyValueDisplay(left_panel, "SHIELDS_DISPLAY", 0.45, "Boucliers", "");
    shields_display->setIcon("gui/icons/shields")->setTextSize(20)->setPosition(-20, -20, ABottomRight)->setSize(240, 40);

    // Unlocked missile aim dial and lock controls.
    missile_aim = new GuiRotationDial(left_panel, "MISSILE_AIM", -90, 360 - 90, 0, [this](float value){
        tube_controls->setMissileTargetAngle(value);
    });
    missile_aim->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMatchHeight, 700);

    // Weapon tube controls.
    tube_controls = new GuiMissileTubeControls(left_panel, "MISSILE_TUBES");
    tube_controls->setPosition(20, -20, ABottomLeft);
    radar->enableTargetProjections(tube_controls);

    // Engine layout in top left corner of left panel.
    GuiAutoLayout* engine_layout = new GuiAutoLayout(left_panel, "ENGINE_LAYOUT", GuiAutoLayout::LayoutHorizontalLeftToRight);
    engine_layout->setPosition(20, 80, ATopLeft)->setSize(GuiElement::GuiSizeMax, 250);
    (new GuiImpulseControls(engine_layout, "IMPULSE"))->setSize(100, GuiElement::GuiSizeMax);
    warp_controls = (new GuiWarpControls(engine_layout, "WARP"))->setSize(100, GuiElement::GuiSizeMax);
    jump_controls = (new GuiJumpControls(engine_layout, "JUMP"))->setSize(100, GuiElement::GuiSizeMax);

    // Docking, comms, and shields buttons across top.
    (new GuiDockingButton(left_panel, "DOCKING"))->setPosition(20, 20, ATopLeft)->setSize(250, 50);
    (new GuiOpenCommsButton(left_panel, "OPEN_COMMS_BUTTON", &targets))->setPosition(270, 20, ATopLeft)->setSize(250, 50);
    (new GuiCommsOverlay(this))->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    (new GuiShieldsEnableButton(left_panel, "SHIELDS_ENABLE"))->setPosition(520, 20, ATopLeft)->setSize(250, 50);

    // Missile lock button near top right of left panel.
    lock_aim = new AimLockButton(left_panel, "LOCK_AIM", tube_controls, missile_aim);
    lock_aim->setPosition(250, 70, ATopCenter)->setSize(130, 50);
}

void SinglePilotScreen::onDraw(sf::RenderTarget& window)
{
    GuiOverlay::onDraw(window);

    // Responsively show/hide the 3D viewport.
    if (viewport->getRect().width < viewport->getRect().height / 3.0f)
    {
        viewport->hide();
        left_panel->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    }
    else
    {
        viewport->show();
        left_panel->setSize(1000, GuiElement::GuiSizeMax);
    }

    if (my_spaceship)
    {
        float target_camera_yaw = my_spaceship->getRotation();
        camera_pitch = 30.0f;

        const float camera_ship_distance = 420.0f;
        const float camera_ship_height = 420.0f;
        sf::Vector2f cameraPosition2D = my_spaceship->getPosition() + sf::vector2FromAngle(target_camera_yaw) * -camera_ship_distance;
        sf::Vector3f targetCameraPosition(cameraPosition2D.x, cameraPosition2D.y, camera_ship_height);
#ifdef DEBUG
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            targetCameraPosition.x = my_spaceship->getPosition().x;
            targetCameraPosition.y = my_spaceship->getPosition().y;
            targetCameraPosition.z = 3000.0;
            camera_pitch = 90.0f;
        }
#endif
        camera_position = camera_position * 0.9f + targetCameraPosition * 0.1f;
        camera_yaw += sf::angleDifference(camera_yaw, target_camera_yaw) * 0.1f;
    }
}
