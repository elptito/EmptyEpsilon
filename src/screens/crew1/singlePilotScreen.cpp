#include "main.h"
#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "singlePilotScreen.h"
#include "singlePilotView.h"
#include "gui/gui2_element.h"
#include "gui/gui2_togglebutton.h"
#include "preferenceManager.h"

#include "screenComponents/viewport3d.h"

#include "screenComponents/alertOverlay.h"

SinglePilotScreen::SinglePilotScreen(GuiContainer* owner, P<PlayerSpaceship> targetSpaceship)
: GuiOverlay(owner, "SINGLEPILOT_SCREEN", colorConfig.background)
{
    init(owner, targetSpaceship);
}

SinglePilotScreen::SinglePilotScreen(GuiContainer* owner)
: GuiOverlay(owner, "SINGLEPILOT_SCREEN", colorConfig.background)
{
    init(owner, my_spaceship);
}

void SinglePilotScreen::init(GuiContainer* owner, P<PlayerSpaceship> targetSpaceship)
{
    current_spaceship = targetSpaceship;
    // Create a 3D viewport behind everything, to serve as the right-side panel
    viewport = new GuiViewport3D(this, "3D_VIEW");
    viewport->setPosition(1000, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    viewport->hide();

    first_person = PreferencesManager::get("first_person") == "1";

    // Create left panel for controls.
    left_panel = new GuiElement(this, "LEFT_PANEL");
    left_panel->setPosition(0, 0, ATopLeft)->setSize(1000, GuiElement::GuiSizeMax);

    // single pilot
    single_pilot_view = new SinglePilotView(left_panel, targetSpaceship);
    single_pilot_view->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    background_crosses = new GuiOverlay(left_panel, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));

    show_3d_button = new GuiToggleButton(this, "SHOW_3D_BUTTON", "Vue exterieure", [this](bool value)
        {
          if(true == value)
          {
              viewport->show();
              left_panel->setSize(1000, GuiElement::GuiSizeMax);
          }
          else
          {
              viewport->hide();
              left_panel->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
          }
        });
    show_3d_button->setPosition(-20, 100, ATopRight)->setSize(250, 50);
    left_panel->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    show_3d_button->setValue(false);
}

void SinglePilotScreen::onDraw(sf::RenderTarget& window)
{
    GuiOverlay::onDraw(window);

    // Responsively show/hide the 3D viewport.
//    if (viewport->getRect().width < viewport->getRect().height / 3.0f)
//    {
//        viewport->hide();
//        left_panel->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
//    }
//    else
//    {
//        viewport->show();
//        left_panel->setSize(1000, GuiElement::GuiSizeMax);
//    }

    if (current_spaceship)
    {
        float target_camera_yaw = current_spaceship->getRotation();
        camera_pitch = 30.0f;

        const float camera_ship_distance = 420.0f;
        const float camera_ship_height = 420.0f;
        if (first_person)
        {
            camera_ship_distance = -(current_spaceship->getRadius() * 1.5);
            camera_ship_height = current_spaceship->getRadius() / 10.f;
            camera_pitch = 0;
        }

        sf::Vector2f cameraPosition2D = current_spaceship->getPosition() + sf::vector2FromAngle(target_camera_yaw) * -camera_ship_distance;
        sf::Vector3f targetCameraPosition(cameraPosition2D.x, cameraPosition2D.y, camera_ship_height);
#ifdef DEBUG
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            targetCameraPosition.x = current_spaceship->getPosition().x;
            targetCameraPosition.y = current_spaceship->getPosition().y;
            targetCameraPosition.z = 3000.0;
            camera_pitch = 90.0f;
        }
#endif
        if (first_person)
        {
            camera_position = targetCameraPosition;
            camera_yaw = target_camera_yaw;
        } else {
            camera_position = camera_position * 0.9f + targetCameraPosition * 0.1f;
            camera_yaw += sf::angleDifference(camera_yaw, target_camera_yaw) * 0.1f;
        }
    }
}

void SinglePilotScreen::setTargetSpaceship(P<PlayerSpaceship> targetSpaceship)
{
    single_pilot_view->setTargetSpaceship(targetSpaceship);
    current_spaceship = targetSpaceship;
}
