#include "tractorBeamScreen.h"

#include "playerInfo.h"
#include "spaceObjects/shipTemplateBasedObject.h"
#include "spaceObjects/playerSpaceship.h"
#include "screenComponents/customShipFunctions.h"

#include "gui/gui2_listbox.h"
#include "gui/gui2_autolayout.h"
#include "gui/gui2_element.h"
#include "gui/gui2_panel.h"
#include "gui/gui2_label.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_slider.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_button.h"
#include "gui/gui2_selector.h"
#include "gui/gui2_togglebutton.h"
#include "screenComponents/powerDamageIndicator.h"
#include "screenComponents/rotatingModelView.h"
#include "screenComponents/radarView.h"
#include "gui/gui2_rotationdial.h"

#include "spaceObjects/playerSpaceship.h"

#include "screenComponents/radarView.h"
#include "screenComponents/alertOverlay.h"
#include "screenComponents/customShipFunctions.h"
#include "screenComponents/systemStatus.h"

#include "gui/gui2_label.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_togglebutton.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_autolayout.h"

#define MAX_ARC 90.f
#define MAX_RANGE 2000.f
#define MODES_HIGHT 60
#define BEAM_PERCISION 50
TractorBeamScreen::TractorBeamScreen(GuiContainer* owner)
: GuiOverlay(owner, "TRACTOR_BEAM_SCREEN_SCREEN", colorConfig.background)
{
    
    GuiElement* main_layout = new GuiElement(this, "MAIN");
    main_layout->setPosition(0, MODES_HIGHT, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    // Render the radar shadow and background decorations.
    GuiOverlay* background_gradient = new GuiOverlay(main_layout, "BACKGROUND_GRADIENT", sf::Color::White);
    background_gradient->setTextureCenter("gui/BackgroundGradient");

    GuiOverlay* background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));

    radar = new GuiRadarView(main_layout, "RADAR", MAX_RANGE + 1000, nullptr, my_spaceship);
    radar->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMatchHeight, 750);
    radar->setRangeIndicatorStepSize(1000.0)->shortRange()->enableGhostDots()->enableCallsigns()->setStyle(GuiRadarView::Circular);
    radar->setFogOfWarStyle(GuiRadarView::RadarRangeAndLineOfSight)->setAutoOrient(true)->setShowSectors(false);
    
    direction_dial = new GuiRotationDial(main_layout, "DIRECTION_DIAL", 0, 360, 0, nullptr);
    direction_dial->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMatchHeight, 800);

    GuiAutoLayout* modesLayout = new GuiAutoLayout(this, "MODES", GuiAutoLayout::LayoutVerticalColumns);
    modesLayout->setPosition(0, 0, ATopCenter)->setSize(100* TBM_Count, MODES_HIGHT);

    for (int i = 0; i < TBM_Count; i++){
        mode_display[i] = new GuiLabel(modesLayout, "BEAM_MODE_" + string(i, 0), getTractorBeamModeName(ETractorBeamMode(i)), 20);
        mode_display[i]->addBackground()->setMargins(0, 0);
    }

    arc_bar = new GuiProgressbar(main_layout, "ARC_BAR", 0.0, MAX_ARC, 0.0);
    arc_bar->setPosition(20, 20, ABottomLeft)->setSize(100, GuiElement::GuiSizeMax);

    range_bar = new GuiProgressbar(main_layout, "RANGE_BAR", 0.0, MAX_RANGE, 0.0);
    range_bar->setPosition(-20, 20, ABottomRight)->setSize(100, GuiElement::GuiSizeMax);

    (new GuiSystemStatus(this, "DOCKS_STATUS", SYS_Docks, my_spaceship))->setPosition(-20, 20, ATopRight)->setSize(300, 40);

    (new GuiCustomShipFunctions(this, tractorBeamHeliosScreen, "", my_spaceship))->setPosition(-20, 120, ATopRight)->setSize(250, GuiElement::GuiSizeMax);
}

void TractorBeamScreen::onDraw(sf::RenderTarget& window)
{
    if (my_spaceship) {
        for (int i = 0; i < TBM_Count; i++){
            sf::Color color = my_spaceship->tractor_beam.getMode() == i? sf::Color::Yellow : sf::Color(96, 96, 96);;
            mode_display[i]->setColor(color)->setTextColor(color);
        }
        arc_bar->setValue(my_spaceship->tractor_beam.getArc());
        direction_dial->setValue(my_spaceship->tractor_beam.getDirection());
        range_bar->setValue(my_spaceship->tractor_beam.getRange());

        if (sf::length(commandPosition) > MAX_RANGE){
            commandPosition = sf::normalize(commandPosition) * MAX_RANGE;
        }        
        sf::Vector2f position =  sf::vector2FromAngle(my_spaceship->tractor_beam.getDirection() - 90) * my_spaceship->tractor_beam.getRange();
        if (sf::length(commandPosition - position) > BEAM_PERCISION){
             // vector2ToAngle returns nan on {0,0} input
            float angle = commandPosition == sf::Vector2f(0,0)? 0 : sf::vector2ToAngle(commandPosition) + 90;
            float range = sf::length(commandPosition);
            my_spaceship->commandSetTractorBeamDirection(angle);
            my_spaceship->commandSetTractorBeamRange(range); 
            float maxArch = std::min(commandArch, my_spaceship->tractor_beam.getMaxArc(range));
            if (std::abs(maxArch - my_spaceship->tractor_beam.getArc()) > 1){
                my_spaceship->commandSetTractorBeamArc(maxArch);
            }
        }  
    }
    GuiOverlay::onDraw(window);
}
void TractorBeamScreen::onHotkey(const HotkeyResult& key){
    if (key.category == "TRACTOR_BEAM" && my_spaceship)
    {
        if (key.hotkey == "MODE_OFF")
            my_spaceship->commandSetTractorBeamMode(ETractorBeamMode::TBM_Off);
        else if (key.hotkey == "MODE_PULL")
            my_spaceship->commandSetTractorBeamMode(ETractorBeamMode::TBM_Pull);
        else if (key.hotkey == "MODE_PUSH")
            my_spaceship->commandSetTractorBeamMode(ETractorBeamMode::TBM_Push);
        else if (key.hotkey == "MODE_HOLD")
            my_spaceship->commandSetTractorBeamMode(ETractorBeamMode::TBM_Hold);
        else if (key.hotkey == "MODE")
            my_spaceship->commandSetTractorBeamMode(ETractorBeamMode((my_spaceship->tractor_beam.getMode() + 1) % ETractorBeamMode::TBM_Count));
    }
}

bool TractorBeamScreen::onJoystickAxis(const AxisAction& axisAction){
    if(my_spaceship){
        if (axisAction.category == "TRACTOR_BEAM"){
            if (axisAction.action == "ARC"){
                commandArch = (axisAction.value + 1) * 90.0 / 2.0;
                my_spaceship->commandSetTractorBeamArc(commandArch);
                return true;            
            } 
            if (axisAction.action == "X"){
                commandPosition.x = axisAction.value * MAX_RANGE;
                return true;
            } 
            if (axisAction.action == "Y"){
                commandPosition.y = axisAction.value * MAX_RANGE;
                return true;
            } 
        }
    }
    return false;
}