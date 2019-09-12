#include "scienceTaskHeliosScreen.h"
#include "spaceObjects/playerSpaceship.h"
#include "gui/gui2_autolayout.h"

#include "playerInfo.h"
#include "gui/gui2_panel.h"
#include "gui/gui2_label.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_button.h"
#include "gui/gui2_keyvaluedisplay.h"

static sf::Color grey(96, 96, 96);

ScienceTaskHeliosScreen::ScienceTaskHeliosScreen(GuiContainer* owner, ECrewPosition crew_position)
: GuiOverlay(owner, "SCAN_SCREEN", colorConfig.background) {
    
    task_id = -1;

    locked = false;
    lock_start_time = 0;
    scan_depth = 0;
    scanning = false;

    GuiOverlay* background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");
    
    current_task_title = new GuiLabel(this, "CURRENT_TASK_TITLE", "", 30);
    current_task_title->addBackground()->setPosition(0, 20, ATopCenter)->setSize(450, 50);

    GuiAutoLayout *tasks_container = new GuiAutoLayout(this, "TASKS_CONTAINER", GuiAutoLayout::LayoutVerticalTopToBottom);
    tasks_container->setPosition(-20, 60, ATopRight)->setSize(250, GuiElement::GuiSizeMax);

    tasks_queue_title = new GuiLabel(tasks_container, "TASKS_TITLE", "0/0 Tasks", 30);
    tasks_queue_title->addBackground()->setMargins(10, 0)->setSize(250, 50);

    for(int n = 0; n < my_spaceship->max_science_tasks; n++){
        tasks_queue[n] = new GuiKeyValueDisplay(tasks_container, "TASK_"+ string(n, 0), 0.2, "["+string(n+1,0)+"] ", "");
        tasks_queue[n]->setTextSize(25)->setColor(grey)->setContentColor(grey)->setMargins(10, 2)->setSize(250, 30);
    }

    initScanner();
}

void ScienceTaskHeliosScreen::initScanner(){
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
    
    for(int n=0; n<max_bars; n++) {
        bars[n] = new GuiProgressbar(box, "SLIDER_" + string(n), 0.0, 1.0, 0.5);
        bars[n]->setBiDirectional(true)->setPosition(0, 200 + n * 70, ATopCenter)->setSize(450, 50);
    }

    setupParametersScanner();
}

void ScienceTaskHeliosScreen::onDrawScanner(sf::RenderTarget& window){
    
    updateSignalScanner();
    if (getTarget() && getTaskType() == STT_Scan) {
        float scanning_complexity = getTarget()->scanningComplexity(my_spaceship);
        float scanning_depth = getTarget()->scanningChannelDepth(my_spaceship);
        if (scanning_complexity && scanning_depth) {
            no_target_label->hide();
            if (!scanning)
            {
                scanning = true;
                scan_depth = 0;
                setupParametersScanner();
            }
            
            if (locked && engine->getElapsedTime() - lock_start_time > lock_delay) {
                scan_depth += 1;
                if (scan_depth >= scanning_depth) {
                    my_spaceship->commandCompleteScienceTask(task_id);
                    lock_start_time = engine->getElapsedTime() - 1.0f;
                } else {
                    setupParametersScanner();
                }
            }
            
            if (locked && engine->getElapsedTime() - lock_start_time > lock_delay / 2.0f) {
                locked_label->show();
            } else {
                locked_label->hide();
            }
        } else {
            resetScanner();
        }
    } else {
        resetScanner();
    }
}

void ScienceTaskHeliosScreen::onDraw(sf::RenderTarget& window)
{
    if (my_spaceship)
    {
        int tasksCount = ScienceTask::countTasks(my_spaceship->scienceTasks, my_spaceship->max_science_tasks);
        tasks_queue_title->setText(string(tasksCount, 0) + "/" + string(my_spaceship->max_science_tasks, 0) + " Tasks");
        for(int n = 0; n < my_spaceship->max_science_tasks; n++){
            if (my_spaceship->scienceTasks[n].type != STT_Empty){
                tasks_queue[n]->setValue(my_spaceship->scienceTasks[n].getDescription());
                tasks_queue[n]->setColor(sf::Color::White)->setContentColor(sf::Color::White);
            } else {
                tasks_queue[n]->setValue("")->setColor(grey)->setContentColor(grey);
            }
        }
        if (getTaskType() == STT_Empty){
            current_task_title->setText("No task selected");
        } else {
            current_task_title->setText("["+string(task_id+1,0)+"] " + my_spaceship->scienceTasks[task_id].getDescription());
        }
    }
    onDrawScanner(window);
}

void ScienceTaskHeliosScreen::resetScanner() {
    scanning = false;
    no_target_label->show();
    locked_label->hide();
    box->setSize(500, 500);
    signal_label->setText("Electric signature");
    for(int n=0; n<max_bars; n++) {
        bars[n]->show(); //->setValue(0.5f)
        bars_target[n] = random(0.0, 1.0);
    }
}

void ScienceTaskHeliosScreen::setupParametersScanner() {
    if (!my_spaceship)
        return;
    if (getTarget()) {
        float scanning_complexity = getTarget()->scanningComplexity(my_spaceship);
        for(int n=0; n<max_bars; n++) {
            if (n < scanning_complexity)
                bars[n]->show();
            else
                bars[n]->hide();
        }
        box->setSize(500, 215 + 70 * scanning_complexity);

        for(int n=0; n<max_bars; n++) {
            bars_target[n] = random(0.0, 1.0);
            while(fabsf(bars_target[n]) < 0.2f)
                bars_target[n] = random(0.0, 1.0);
        }
        updateSignalScanner();

        string label = "[" + string(scan_depth + 1) + "/" + string(getTarget()->scanningChannelDepth(my_spaceship), 0) + "] ";
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
    }
}

void ScienceTaskHeliosScreen::updateSignalScanner() {
    float noise = 0.0;
    float period = 0.0;
    float phase = 0.0;

    for(int n=0; n<max_bars; n++)
    {
        if (bars[n]->isVisible())
        {
            noise += fabsf(bars_target[n] - bars[n]->getValue());
            period += fabsf(bars_target[n] - bars[n]->getValue());
            phase += fabsf(bars_target[n] - bars[n]->getValue());
        }
    }
    if (noise < 0.04f && period < 0.04f && phase < 0.04f)
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

P<SpaceShip> ScienceTaskHeliosScreen::getTarget() {
    if (!my_spaceship || task_id < 0 || task_id >= my_spaceship->max_science_tasks){
        return nullptr;
    }
    return getObjectById(my_spaceship->scienceTasks[task_id].target_id);
}

EScienceTaskType ScienceTaskHeliosScreen::getTaskType() {
    if (!my_spaceship || task_id < 0 || task_id >= my_spaceship->max_science_tasks){
        return STT_Empty;
    }
    return my_spaceship->scienceTasks[task_id].type;
}

bool ScienceTaskHeliosScreen::onJoystickAxis(const AxisAction& axisAction){
    if(my_spaceship){
        if (axisAction.category == "SCAN"){
            for(int n=0; n<max_bars; n++) {
                if (axisAction.action == std::string("SCAN_PARAM_") + string(n+1)){
                    bars[n]->setValue((axisAction.value + 1) / 2.0);
                    updateSignalScanner();
                    return true;
                }
            }   
        }
    }
    return false;
}

void ScienceTaskHeliosScreen::onHotkey(const HotkeyResult& key){
    if (my_spaceship) {
        for(int n = 0; n < my_spaceship->max_science_tasks; n++){
            if (key.hotkey == "DO_TASK_" + string(n, 0)) {
                task_id = n;
            }
        }
    }
}
