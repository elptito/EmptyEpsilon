#include "scanHeliosScreen.h"
#include "spaceObjects/playerSpaceship.h"
#include "playerInfo.h"
#include "gui/gui2_panel.h"
#include "gui/gui2_label.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_button.h"

ScanHeliosScreen::ScanHeliosScreen(GuiContainer* owner, ECrewPosition crew_position)
: GuiOverlay(owner, "SCAN_SCREEN", colorConfig.background) {
    
    locked = false;
    lock_start_time = 0;
    scan_depth = 0;
    
    GuiOverlay* background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");
    
    box = new GuiPanel(this, "BOX");
    box->setSize(500, 500)->setPosition(0, 0, ACenter);
    
    signal_label = new GuiLabel(box, "LABEL", "Electric signature", 30);
    signal_label->addBackground()->setPosition(0, 20, ATopCenter)->setSize(450, 50);
    
    signal_quality = new GuiSignalQualityIndicator(box, "SIGNAL");
    signal_quality->setPosition(0, 80, ATopCenter)->setSize(450, 100);
    
    locked_label = new GuiLabel(signal_quality, "LOCK_LABEL", "LOCKED", 50);
    locked_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    
    no_target_label = new GuiLabel(signal_quality, "NO_TARGET_LABEL", "No Scan Target", 50);
    no_target_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    
    for(int n=0; n<max_bars; n++)
    {
        bars[n] = new GuiProgressbar(box, "SLIDER_" + string(n), 0.0, 1.0, 0.0);
        bars[n]->setPosition(0, 200 + n * 70, ATopCenter)->setSize(450, 50);
    }

    setupParameters();
}

void ScanHeliosScreen::onDraw(sf::RenderTarget& window)
{
    updateSignal();
    
    if (my_spaceship)
    {
        P<SpaceObject> scanning_target = my_spaceship->getScanTarget();
        if (scanning_target){
            float scanning_complexity = scanning_target->scanningComplexity(my_spaceship);
            float scanning_depth = scanning_target->scanningChannelDepth(my_spaceship);
            if (scanning_complexity && scanning_depth){
                if (!box->isVisible())
                {
                    box->show();
                    scan_depth = 0;
                    setupParameters();
                }
                
                if (locked && engine->getElapsedTime() - lock_start_time > lock_delay)
                {
                    scan_depth += 1;
                    if (scan_depth >= scanning_depth)
                    {
                        my_spaceship->commandScanDone();
                        lock_start_time = engine->getElapsedTime() - 1.0f;
                    }else{
                        setupParameters();
                    }
                }
                
                if (locked && engine->getElapsedTime() - lock_start_time > lock_delay / 2.0f)
                {
                    locked_label->show();
                }else{
                    locked_label->hide();
                }
                no_target_label->hide();
            }else{
                no_target_label->show();
                locked_label->hide();
                // box->hide();
            }
        }else{
            no_target_label->show();
            locked_label->hide();
            // box->hide();
        }
    }
}
bool ScanHeliosScreen::onJoystickAxis(const AxisAction& axisAction){
    if(my_spaceship){
        if (axisAction.category == "SCIENCE"){
            for(int n=0; n<max_bars; n++) {
                if (axisAction.action == std::string("SCAN_PARAM_") + string(n+1)){
                    bars[n]->setValue((axisAction.value + 1) / 2.0);
                    updateSignal();
                    return true;
                }
            }   
        }
    }
    return false;
}

void ScanHeliosScreen::setupParameters()
{
    if (!my_spaceship)
        return;
    P<SpaceObject> scanning_target = my_spaceship->getScanTarget();
    if (scanning_target){
        float scanning_complexity = scanning_target->scanningComplexity(my_spaceship);
        float scanning_depth = scanning_target->scanningChannelDepth(my_spaceship);

        for(int n=0; n<max_bars; n++)
        {
            if (n < scanning_complexity)
                bars[n]->show();
            else
                bars[n]->hide();
        }
        box->setSize(500, 215 + 70 * scanning_complexity);

        for(int n=0; n<max_bars; n++)
        {
            target[n] = random(0.0, 1.0);
            bars[n]->setValue(random(0.0, 1.0));
            while(fabsf(target[n] - bars[n]->getValue()) < 0.2)
                bars[n]->setValue(random(0.0, 1.0));
        }
        updateSignal();
        
        string label = "[" + string(scan_depth + 1) + "/" + string(scanning_depth) + "] ";
        switch(irandom(0, 10))
        {
        default:
        case 0: label += "Electric signature"; break;
        case 1: label += "Biomass frequency"; break;
        case 2: label += "Gravity well signature"; break;
        case 3: label += "Radiation halftime"; break;
        case 4: label += "Radio profile"; break;
        case 5: label += "Ionic phase shift"; break;
        case 6: label += "Infra-red color shift"; break;
        case 7: label += "Doppler stability"; break;
        case 8: label += "Raspberry jam prevention"; break;
        case 9: label += "Infinity impropability"; break;
        case 10: label += "Zerospace audio frequency"; break;
        }
        signal_label->setText(label);
    } else {

    }
}

void ScanHeliosScreen::updateSignal()
{
    float noise = 0.0;
    float period = 0.0;
    float phase = 0.0;

    for(int n=0; n<max_bars; n++)
    {
        if (bars[n]->isVisible())
        {
            noise += fabsf(target[n] - bars[n]->getValue());
            period += fabsf(target[n] - bars[n]->getValue());
            phase += fabsf(target[n] - bars[n]->getValue());
        }
    }
    if (noise < 0.05f && period < 0.05f && phase < 0.05f)
    {
        if (!locked)
        {
            lock_start_time = engine->getElapsedTime();
            locked = true;
        }
        if (engine->getElapsedTime() - lock_start_time > lock_delay / 2.0f)
        {
            noise = period = phase = 0.0f;
        }else{
            float f = 1.0f - (engine->getElapsedTime() - lock_start_time) / (lock_delay / 2.0f);
            noise *= f;
            period *= f;
            phase *= f;
        }
    }else{
        locked = false;
    }
    
    signal_quality->setNoiseError(noise);
    signal_quality->setPeriodError(period);
    signal_quality->setPhaseError(phase);
}
