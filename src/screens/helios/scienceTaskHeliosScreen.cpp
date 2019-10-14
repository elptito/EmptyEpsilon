#include "scienceTaskHeliosScreen.h"
#include "spaceObjects/playerSpaceship.h"
#include "gui/gui2_autolayout.h"

#include "playerInfo.h"
#include "gui/gui2_panel.h"
#include "gui/gui2_label.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_button.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_textentry.h"
#include "gui/gui2_advancedscrolltext.h"
#include "gui/gui2_autolayout.h"

static sf::Color grey(96, 96, 96);

ScienceTaskHeliosScreen::ScienceTaskHeliosScreen(GuiContainer* owner, ECrewPosition crew_position)
: GuiOverlay(owner, "SCAN_SCREEN", colorConfig.background) {
    
    task_id = -1;
    simulation = STT_Empty;
    avg_line_type_time = 10;

    locked = false;
    lock_start_time = 0;
    scan_depth = 0;
    scanning = false;

    hacker_next_line = 0;

    GuiOverlay* background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");
    
    current_task_title = new GuiLabel(this, "CURRENT_TASK_TITLE", "", 30);
    current_task_title->addBackground()->setPosition(0, 20, ATopCenter)->setSize(450, 50);

    GuiAutoLayout *tasks_container = new GuiAutoLayout(this, "TASKS_CONTAINER", GuiAutoLayout::LayoutVerticalTopToBottom);
    tasks_container->setPosition(-20, 60, ATopRight)->setSize(250, GuiElement::GuiSizeMax);

    tasks_queue_title = new GuiLabel(tasks_container, "TASKS_TITLE", "0/0 Tasks", 30);
    tasks_queue_title->addBackground()->setMargins(10, 0)->setSize(250, 50);

    for(int n = 0; n < PlayerSpaceship::max_science_tasks; n++){
        tasks_queue[n] = new GuiKeyValueDisplay(tasks_container, "TASK_"+ string(n, 0), 0.2, "["+string(n+1,0)+"] ", "");
        tasks_queue[n]->setTextSize(25)->setColor(grey)->setContentColor(grey)->setMargins(10, 2)->setSize(250, 30);
    }

    initScanner();
    initHacker();
}

void ScienceTaskHeliosScreen::initHacker(){
    hacker = new GuiPanel(this, "HACKER");
    hacker->setSize(700, 700)->setPosition(0, 0, ACenter);
    GuiElement *hacker_container = new GuiElement(hacker, "");
    hacker_container->setSize(660, 660)->setPosition(0, 0, ACenter);
    GuiAutoLayout *hackerLayout = new GuiAutoLayout(hacker_container, "HACKER_LAYOUT", GuiAutoLayout::LayoutVerticalColumns);
    hackerLayout->setPosition(0, 0, EGuiAlign::ATopCenter)->setMargins(0,0,0,50)->setSize(660, 660);
    hacker_target = new GuiAdvancedScrollText(hackerLayout, "HACKER_TARGET");
    hacker_current = new GuiAdvancedScrollText(hackerLayout, "HACKER_CURRENT");
    hacker_current->enableAutoScrollDown();

    hacker_input = new GuiTextEntry(hacker_container, "", "");
    hacker_input->setPosition(0, 0, EGuiAlign::ABottomCenter)->setSize(GuiElement::GuiSizeMax, 50);
    hacker_input->enterCallback([this](string text){
        if (text.lower() == hacker_target->getEntryText(hacker_next_line).lower()){
            hacker_current->addEntry("", text, sf::Color::Green);
            hacker_input->setText("");
            hacker_next_line++;
            avg_line_type_time = 0.9 * avg_line_type_time + 0.1 * (engine->getElapsedTime() - last_line_start);
            last_line_start = engine->getElapsedTime();
        }
    });

    hacked_label = new GuiLabel(hacker_container, "HACKED_LABEL", "executing attack", 50);
    hacked_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    setupParametersHacker();
}

void ScienceTaskHeliosScreen::initScanner(){
    scanner = new GuiPanel(this, "SCANNER");
    scanner->setSize(500, 500)->setPosition(0, 0, ACenter);
    
    signal_label = new GuiLabel(scanner, "LABEL", "Electric signature", 30);
    signal_label->addBackground()->setPosition(0, 20, ATopCenter)->setSize(450, 50);
    
    signal_quality = new GuiSignalQualityIndicator(scanner, "SIGNAL");
    signal_quality->setPosition(0, 80, ATopCenter)->setSize(450, 100);
    
    locked_label = new GuiLabel(signal_quality, "LOCK_LABEL", "LOCKED", 50);
    locked_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    
    no_target_label = new GuiLabel(signal_quality, "NO_TARGET_LABEL", "No Scan Target", 50);
    no_target_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    
    for(int n=0; n<max_bars; n++) {
        scanner_bars[n] = new GuiProgressbar(scanner, "SLIDER_" + string(n), 0.0, 1.0, 0.5);
        scanner_bars[n]->setBiDirectional(true)->setPosition(0, 200 + n * 70, ATopCenter)->setSize(450, 50);
    }

    setupParametersScanner();
}

void ScienceTaskHeliosScreen::completeTask(){
    if (simulation == STT_Empty){
        my_spaceship->commandCompleteScienceTask(task_id, true);
    } else {
        simulation = STT_Empty;
    }
}

void ScienceTaskHeliosScreen::onDrawHacker(sf::RenderTarget& window){

    if (my_spaceship){
        if (getTarget() && getTaskType() == STT_Hack) {
            if (!hacker->isVisible()){
                setupParametersHacker();    
                hacker->show();
                hacker_input->setFocus();
            }
            if (engine->getElapsedTime() > add_next_line_time){
                addCodeLine();
                float minTime = PreferencesManager::get("hack_minigame_min_line_time_factor", "0.5").toFloat() * avg_line_type_time;
                float maxTime = PreferencesManager::get("hack_minigame_max_line_time_factor", "3.0").toFloat() * avg_line_type_time;
                add_next_line_time = engine->getElapsedTime() + random(minTime, maxTime);
            }
            if (hacker_next_line == hacker_target->getEntryCount() && !hacked_label->isVisible()){
                hacked_label->show();
                hacker_target->hide();
                hacker_current->hide();
                this->setFocus();
                hacker_input->hide();
                hack_time = engine->getElapsedTime();
            }
            if (hacked_label->isVisible() && engine->getElapsedTime() - hack_time > lock_delay){
                completeTask();
            }
            hacker_target->setScrollPosition(hacker_current->getScrollPosition() + 1);
            for (unsigned int i = 0; i < hacker_target->getEntryCount(); i++){
                hacker_target->setEntryColor(i, i == hacker_next_line? sf::Color::White : grey);
            }
        } else {
            hacker->hide();
            this->setFocus();
        }
    }
}

void ScienceTaskHeliosScreen::onDrawScanner(sf::RenderTarget& window){
    
    updateSignalScanner();
    
    if (getTarget() && getTaskType() == STT_Scan) {
        int scanning_complexity = getTarget()->scanningComplexity(my_spaceship);
        int scanning_depth = getTarget()->scanningChannelDepth(my_spaceship);
        if (scanning_complexity && scanning_depth) {
            scanner->show();
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
                    completeTask();
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
        int tasksCount = ScienceTask::countTasks(my_spaceship->scienceTasks, PlayerSpaceship::max_science_tasks);
        tasks_queue_title->setText(string(tasksCount, 0) + "/" + string(PlayerSpaceship::max_science_tasks, 0) + " Tasks");
        for(int n = 0; n < PlayerSpaceship::max_science_tasks; n++){
            if (my_spaceship->scienceTasks[n].type != STT_Empty){
                tasks_queue[n]->setValue(my_spaceship->scienceTasks[n].getDescription());
                tasks_queue[n]->setColor(sf::Color::White)->setContentColor(sf::Color::White);
            } else {
                tasks_queue[n]->setValue("")->setColor(grey)->setContentColor(grey);
            }
        }
        if (getTaskType() == STT_Empty){
            current_task_title->setText("No task selected");
        } else if (simulation != STT_Empty){
            current_task_title->setText("SIMULATION");
        } else {
            current_task_title->setText("["+string(task_id+1,0)+"] " + my_spaceship->scienceTasks[task_id].getDescription());
        }
    }
    onDrawScanner(window);
    onDrawHacker(window);
}

void ScienceTaskHeliosScreen::resetScanner() {
    scanning = false;
    scanner->hide();
    no_target_label->show();
    locked_label->hide();
    scanner->setSize(500, 500);
    signal_label->setText("Electric signature");
    for(int n=0; n<max_bars; n++) {
        scanner_bars[n]->show();
        scanner_targets[n] = random(0.0, 1.0);
    }
}

static string letters[] = { "a", "b", "c", "d", "e", "f", "g", /*"h","i",*/ 
    "j", "k", "l", "m", "n", "o", /*"p",*/ "q", "r",
    "s", "t", "u", "v", "w", "x", "y", "z" };

#define LETTER letters[irandom(0,sizeof(letters) / sizeof(letters[0]))]
#define REGISTER letters[irandom(0,4)] + (irandom(0,1)? "x" : irandom(0,1)? "l" : "k")

void ScienceTaskHeliosScreen::addCodeLine() {
    string line = "";
    if (random(0, 1) < 0.1){
        line += LETTER + LETTER + LETTER;
    }
    line += LETTER + LETTER + LETTER;
    if (random(0, 1) < 0.2){
        line += LETTER;
    }
    if (random(0, 1) < 0.7){
        line += " " + REGISTER;
        if (random(0, 1) < 0.8){
            line += ", " + REGISTER;
        }
    } else {
        line += " " + LETTER + LETTER + LETTER + LETTER;
        while (random(0, 1) < 0.6 && line.size() < 20){
            line += ", " + LETTER + LETTER + LETTER + LETTER;
        }
    }
    hacker_target->addEntry("", line, sf::Color::White);
}

void ScienceTaskHeliosScreen::setupParametersHacker() {
    hacked_label->hide();
    hacker_target->show();
    hacker_current->show();
    hacker_input->show();
    last_line_start = engine->getElapsedTime();
    add_next_line_time = engine->getElapsedTime() + avg_line_type_time;
    hacker_current->clearEntries();
    hacker_target->clearEntries();
    int minLines = PreferencesManager::get("hack_minigame_min_initial_code_lines", "6").toInt();
    int maxLines = PreferencesManager::get("hack_minigame_max_initial_code_lines", "10").toInt();
    int numOfLines = irandom(minLines, maxLines);
    for (int i = 0; i < numOfLines; i++){
        addCodeLine();
    }
    hacker_input->setText("");
    hacker_next_line = 0;
}

void ScienceTaskHeliosScreen::setupParametersScanner() {
    if (!my_spaceship)
        return;
    if (getTarget()) {
        float scanning_complexity = getTarget()->scanningComplexity(my_spaceship);
        if (simulation != STT_Empty){
            // maximum complexity
            scanning_complexity = (float) gameGlobalInfo->scanning_complexity;
        }
        for(int n=0; n<max_bars; n++) {
            if (n < scanning_complexity)
                scanner_bars[n]->show();
            else
                scanner_bars[n]->hide();
        }
        scanner->setSize(500, 215 + 70 * scanning_complexity);

        for(int n=0; n<max_bars; n++) {
            scanner_targets[n] = random(0.0, 1.0);
            while(fabsf(scanner_targets[n]) < 0.2f)
                scanner_targets[n] = random(0.0, 1.0);
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
        if (scanner_bars[n]->isVisible())
        {
            noise += fabsf(scanner_targets[n] - scanner_bars[n]->getValue());
            period += fabsf(scanner_targets[n] - scanner_bars[n]->getValue());
            phase += fabsf(scanner_targets[n] - scanner_bars[n]->getValue());
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
    if (my_spaceship && simulation != STT_Empty){
        return my_spaceship;
    }
    if (!my_spaceship || task_id < 0 || task_id >= PlayerSpaceship::max_science_tasks){
        return nullptr;
    }
    return getObjectById(my_spaceship->scienceTasks[task_id].target_id);
}

EScienceTaskType ScienceTaskHeliosScreen::getTaskType() {
    if (!my_spaceship || task_id < 0 || task_id >= PlayerSpaceship::max_science_tasks || simulation != STT_Empty){
        return simulation;
    }
    return my_spaceship->scienceTasks[task_id].type;
}

bool ScienceTaskHeliosScreen::onJoystickAxis(const AxisAction& axisAction){
    if(my_spaceship){
        if (axisAction.category == "SCAN"){
            for(int n=0; n<max_bars; n++) {
                if (axisAction.action == std::string("SCAN_PARAM_") + string(n+1)){
                    scanner_bars[n]->setValue((axisAction.value + 1) / 2.0);
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
        if (getTaskType() == STT_Empty){
            if (key.hotkey == "SIMULATE_HACK") {
                simulation = STT_Hack;
            } else if (key.hotkey == "SIMULATE_SCAN") {
                simulation = STT_Scan;
            } 
            for(int n = 0; n < PlayerSpaceship::max_science_tasks; n++){
                if (key.hotkey == "DO_TASK_" + string(n, 0)) {
                    task_id = n;
                }
            }
        } else {
            if (key.hotkey == "ABORT_TASK") {
                if (simulation == STT_Empty)
                    my_spaceship->commandCompleteScienceTask(task_id, false);
                simulation = STT_Empty;
            } 
        }
    }
}
