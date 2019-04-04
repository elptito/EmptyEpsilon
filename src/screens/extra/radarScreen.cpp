#include "radarScreen.h"
#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "screenComponents/indicatorOverlays.h"
#include "screenComponents/selfDestructIndicator.h"
#include "screenComponents/globalMessage.h"
#include "screenComponents/jumpIndicator.h"
#include "screenComponents/commsOverlay.h"
#include "screenComponents/shipDestroyedPopup.h"
#include "screenComponents/radarView.h"
#include "screenComponents/alertOverlay.h"
#include "gui/gui2_overlay.h"
RadarScreen::RadarScreen(GuiContainer* owner,string type)
: GuiOverlay(owner, "RADAR_SCREEN", colorConfig.background)
{
     if (type == "tactical" && my_spaceship){
      tactical_radar = new GuiRadarView(this, "TACTICAL", 5000.0f, nullptr, my_spaceship);
      tactical_radar->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
      tactical_radar->setRangeIndicatorStepSize(1000.0f)->shortRange()->enableCallsigns()->show();
    }

    if (type == "science" && my_spaceship){
      science_radar = new GuiRadarView(this, "SCIENCE", gameGlobalInfo->long_range_radar_range, nullptr, my_spaceship);
      science_radar->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
      science_radar->setRangeIndicatorStepSize(5000.0f)->longRange()->enableCallsigns()->show();
      science_radar->setFogOfWarStyle(GuiRadarView::NebulaFogOfWar)->setStyle(GuiRadarView::Circular);
   }

    if (type == "relay" && my_spaceship){
      relay_radar = new GuiRadarView(this, "RELAY", 50000.0f, nullptr, my_spaceship);
      relay_radar->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
      relay_radar->setAutoCentering(true);
      relay_radar->longRange()->enableWaypoints()->enableCallsigns()->setStyle(GuiRadarView::Rectangular)->setFogOfWarStyle(GuiRadarView::FriendlysShortRangeFogOfWar);
      relay_radar->show();
    }

    new GuiJumpIndicator(this);
    new GuiSelfDestructIndicator(this);
    new GuiGlobalMessage(this);
    new GuiIndicatorOverlays(this);
}

void RadarScreen::update(float delta)
{
    // Sonar
    if (this->type == "science" && gameGlobalInfo->scanning_system == SS_Circular)
    {
        sonar_step_time += delta;
        if (sonar_step_time > 10.0)
        {
            if (!sonar_sound_played)
            {
//                soundManager->playSound("explosion.wav", getCenterPoint(), 0, 1.0, 1.0f,100,true);
//                soundManager->playSound("explosion.wav", getCenterPoint(), 200.0, 1.0, 1.0f + random(-0.2f, 0.2f));
                sonar_sound_played = true;
            }

            science_radar->setSonarParameter(sonar_parameter);
            sonar_parameter += 100.0;
            if (sonar_parameter > gameGlobalInfo->long_range_radar_range*2.0)
            {
                science_radar->setSonarParameter(0.0);
                sonar_step_time = 0.0;
                sonar_parameter = 5000.0;
                sonar_sound_played = false;
            }
        }
    }
    if (this->type == "science" && gameGlobalInfo->scanning_system == SS_Line)
    {
        sonar_parameter += delta*10.0;
        if (sonar_parameter > 360)
            sonar_parameter = 0;
        science_radar->setSonarParameter(sonar_parameter);

        if (sonar_step_time > 0)
            sonar_step_time -= delta;

        if (science_radar->getSonarTest() && sonar_step_time <= 0.0){
            sonar_step_time = 5.0;
//            soundManager->playSound("button.wav");
        }
    }
}
