#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "scienceHeliosScreen.h"
#include "scienceDatabase.h"
#include "spaceObjects/nebula.h"

#include "screenComponents/radarView.h"
#include "screenComponents/rawScannerDataRadarOverlay.h"
#include "screenComponents/frequencyCurve.h"
#include "screenComponents/databaseView.h"
#include "screenComponents/alertOverlay.h"
#include "screenComponents/customShipFunctions.h"
#include "screenComponents/systemStatus.h"

#include "gui/gui2_autolayout.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_togglebutton.h"
#include "gui/gui2_selector.h"
#include "gui/gui2_text.h"
#include "gui/gui2_listbox.h"
#include "gui/gui2_label.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_image.h"

static sf::Color grey(96, 96, 96);

ScienceHeliosScreen::ScienceHeliosScreen(GuiContainer* owner, ECrewPosition crew_position)
: GuiOverlay(owner, "SCIENCE_SCREEN", colorConfig.background), radar_pov(my_spaceship), probe_view(false), target_waypoints(false), zoom(0.5)
{
    targets.setAllowWaypointSelection();

    GuiOverlay* background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");
    
    GuiAutoLayout *root_layout = new GuiAutoLayout(this, "ROOT", GuiAutoLayout::LayoutHorizontalRightToLeft);
    root_layout->setMargins(0, 20, 0, 20)->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    // Target scan data sidebar.
    GuiAutoLayout *right_column = new GuiAutoLayout(root_layout, "RIGHT_COLUMN", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_column->setMargins(10, 0, 0, 0)->setSize(250, GuiElement::GuiSizeMax);

    // Target scan data sidebar.
    GuiAutoLayout *middle_column = new GuiAutoLayout(root_layout, "MIDDLE_COLUMN", GuiAutoLayout::LayoutVerticalBottomToTop);
    middle_column->setMargins(0, 0, 10, 0)->setSize(250, GuiElement::GuiSizeMax);

    GuiAutoLayout *middle_column_down = new GuiAutoLayout(middle_column, "MIDDLE_COLUMN_DOWN", GuiAutoLayout::LayoutVerticalBottomToTop);
    middle_column_down->setMargins(0, 0, 10, 0)->setSize(GuiElement::GuiSizeMax, 50 + 30 * PlayerSpaceship::max_science_tasks);

    GuiAutoLayout *middle_column_up = new GuiAutoLayout(middle_column, "MIDDLE_COLUMN_UP", GuiAutoLayout::LayoutVerticalTopToBottom);
    middle_column_up->setMargins(0, 0, 10, 0)->setSize(250, GuiElement::GuiSizeMax);

    GuiAutoLayout *left_column = new GuiAutoLayout(root_layout, "LEFT_COLUMN", GuiAutoLayout::LayoutVerticalBottomToTop);
    left_column->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    GuiAutoLayout *under_radar = new GuiAutoLayout(left_column, "UNDER_RADAR", GuiAutoLayout::LayoutVerticalColumns);
    under_radar->setSize(GuiElement::GuiSizeMax, 150);

    GuiAutoLayout *radar_source = new GuiAutoLayout(under_radar, "RADAR_TARGETS", GuiAutoLayout::LayoutVerticalBottomToTop);
    radar_source->setSize(250, GuiElement::GuiSizeMax);
    GuiAutoLayout *radar_controls = new GuiAutoLayout(under_radar, "RADAR_CONTROLS", GuiAutoLayout::LayoutVerticalBottomToTop);
    radar_controls->setSize(250, GuiElement::GuiSizeMax);
    GuiAutoLayout *target_actions = new GuiAutoLayout(under_radar, "TARGET_ACTIONS", GuiAutoLayout::LayoutVerticalBottomToTop);
    target_actions->setSize(250, GuiElement::GuiSizeMax);

    GuiElement* radar_view = new GuiElement(left_column, "RADAR_VIEW");
    radar_view->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    
    // Render the radar shadow and background decorations.
    // GuiOverlay* background_gradient = new GuiOverlay(radar_view, "BACKGROUND_GRADIENT", sf::Color::White);
    GuiImage* background_gradient = new GuiImage(radar_view, "BACKGROUND_GRADIENT", "gui/BackgroundGradientNoMargin");
    background_gradient->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    // Draw the radar.
    // Draw the science radar.
    radar = new GuiRadarView(radar_view, "SCIENCE_RADAR", gameGlobalInfo->long_range_radar_range, &targets, my_spaceship);
    radar->setMargins(25)->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    radar->setRangeIndicatorStepSize(5000.0)->longRange()->enableWaypoints()->enableCallsigns()->enableHeadingIndicators()->setStyle(GuiRadarView::Circular)->setFogOfWarStyle(GuiRadarView::RadarRangeAndLineOfSight);

    new RawScannerDataRadarOverlay(radar, "");

    // Simple scan data.
    info_callsign = new GuiLabel(middle_column_up, "SCIENCE_CALLSIGN", "", 30);
    info_callsign->addBackground()->setSize(GuiAutoLayout::GuiSizeMax, 40);
    info_distance = new GuiKeyValueDisplay(middle_column_up, "SCIENCE_DISTANCE", 0.4, "Distance", "");
    info_distance->setSize(GuiElement::GuiSizeMax, 30);
    info_heading = new GuiKeyValueDisplay(middle_column_up, "SCIENCE_HEADING", 0.4, "Heading", "");
    info_heading->setSize(GuiElement::GuiSizeMax, 30);
    info_relspeed = new GuiKeyValueDisplay(middle_column_up, "SCIENCE_REL_SPEED", 0.4, "Rel. Speed", "");
    info_relspeed->setSize(GuiElement::GuiSizeMax, 30);
    info_faction = new GuiKeyValueDisplay(middle_column_up, "SCIENCE_FACTION", 0.4, "Faction", "");
    info_faction->setSize(GuiElement::GuiSizeMax, 30);
    info_type = new GuiKeyValueDisplay(middle_column_up, "SCIENCE_TYPE", 0.4, "Type", "");
    info_type->setSize(GuiElement::GuiSizeMax, 30);

    info_shields = new GuiKeyValueDisplay(middle_column_up, "SCIENCE_SHIELDS", 0.4, "Shields", "");
    info_shields->setSize(GuiElement::GuiSizeMax, 30);
    info_hull = new GuiKeyValueDisplay(middle_column_up, "SCIENCE_HULL", 0.4, "Hull", "");
    info_hull->setSize(GuiElement::GuiSizeMax, 30);


    // Prep the description text area.
    info_description = new GuiText(middle_column_up, "SCIENCE_DESC", "");
    info_description->setTextSize(28)->setMargins(0, 20, 0, 0)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    // Prep the frequency graphs.
    info_shield_frequency = new GuiFrequencyCurve(right_column, "SCIENCE_SHIELD_FREQUENCY", false, true);
    info_shield_frequency->setSize(GuiElement::GuiSizeMax, 150);
    info_beam_frequency = new GuiFrequencyCurve(right_column, "SCIENCE_BEAM_FREQUENCY", true, false);
    info_beam_frequency->setSize(GuiElement::GuiSizeMax, 150);

    // Show shield and beam frequencies only if enabled by the server.
    if (!gameGlobalInfo->use_beam_shield_frequencies)
    {
        info_shield_frequency->hide();
        info_beam_frequency->hide();
    }

    // List each system's status.
    for(int n = 0; n < SYS_COUNT; n++) {
        info_system[n] = new GuiKeyValueDisplay(right_column, "SCIENCE_SYSTEM_" + string(n), 0.75, getSystemName(ESystem(n)), "-");
        info_system[n]->setSize(GuiElement::GuiSizeMax, 30);
    }

    // Probe view action label
    probe_view_display = new GuiLabel(radar_source, "PROBE_VIEW", "Probe View", 30);
    probe_view_display->addBackground()->setMargins(10, 0)->setSize(GuiElement::GuiSizeMax, 50);
    main_view_display = new GuiLabel(radar_source, "PROBE_VIEW", "Main View", 30);
    main_view_display->addBackground()->setMargins(10, 0)->setSize(GuiElement::GuiSizeMax, 50);

    for(int n = PlayerSpaceship::max_science_tasks - 1; n >= 0; n--){
        tasks_queue[n] = new GuiLabel(middle_column_down, "TASK_"+ string(n, 0), "-", 25);
        tasks_queue[n]->addBackground()->setMargins(10, 2)->setSize(250, 30)->hide();
    }

    tasks_queue_title = new GuiLabel(middle_column_down, "TASKS_TITLE", "0/0 Tasks", 30);
    tasks_queue_title->addBackground()->setMargins(10, 0)->setSize(250, 50);

    // Scan action label.
    scan_status = new GuiLabel(target_actions, "SCAN_STATUS", "Scan Target", 30);
    scan_status->addBackground()->setMargins(10, 0)->setSize(GuiElement::GuiSizeMax, 50);
    
    hack_action = new GuiLabel(target_actions, "HACK_ACTION", "Hack Target", 30);
    hack_action->addBackground()->setMargins(10, 0)->setSize(GuiElement::GuiSizeMax, 50);
    
    // DB info action label.
    query_action = new GuiLabel(target_actions, "QUERY_ACTION", "Query ship Type", 30);
    query_action->addBackground()->setMargins(10, 0)->setSize(GuiElement::GuiSizeMax, 50);
    
    // Draw the zoom slider.
    zoom_bar = new GuiProgressbar(radar_controls, "ZOOM_BAR", 1, 0, -1); // force sync in onDraw()
    zoom_bar->setSize(GuiElement::GuiSizeMax, 50);

    target_mode_waypoints_display = new GuiLabel(radar_controls, "TARGET_WAYPOINTS", "Waypoints", 30);
    target_mode_waypoints_display->addBackground()->setMargins(10, 0)->setSize(GuiAutoLayout::GuiSizeMax, 50);

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));
}

void ScienceHeliosScreen::onDraw(sf::RenderTarget& window)
{
    if (!my_spaceship)
        return;
    GuiOverlay::onDraw(window);

    // Keep the zoom bar in sync.
    zoom_bar->setValue(zoom);
    zoom_bar->setText("Zoom: " + string(gameGlobalInfo->long_range_radar_range / radar->getDistance(), 1) + "x");
            
    P<ScanProbe> probe = getObjectById(my_spaceship->linked_science_probe_id);

    // de-select items that are deleted / outside radar
    if (targets.get() && (radar->getViewPosition() - targets.get()->getPosition()) > radar->getDistance())
        targets.clear();

    sf::Color color = target_waypoints? sf::Color::Yellow : sf::Color::White;
    target_mode_waypoints_display->setColor(color)->setTextColor(color);

    main_view_display->setText("View Main (" + my_spaceship->getCallSign() + ")");
    if (probe){
        probe_view_display->setText("View Probe (" + probe->getCallSign() + ")");
    } else {
        probe_view = false;
        probe_view_display->setText("No Probe Link");
    }
    if (probe_view){
        radar_pov = probe;
        probe_view_display->setColor(sf::Color::Yellow)->setTextColor(sf::Color::Yellow);
        main_view_display->setColor(sf::Color::White)->setTextColor(sf::Color::White);
    } else {
        radar_pov = my_spaceship;
        sf::Color color = probe? sf::Color::White : grey;
        probe_view_display->setColor(color)->setTextColor(color);
        main_view_display->setColor(sf::Color::Yellow)->setTextColor(sf::Color::Yellow);
    }

    radar->setViewPosition(radar_pov->getPosition())->setAutoCentering(false);
    radar->setDistance(Tween<float>::linear(zoom, 0.f, 1.f, 5000, radar_pov->getRadarRange()));

    showNoInfo();
    P<SpaceObject> target = targets.get();
    P<SpaceShip> ship = target;
    if (target) {
        showTargetInfo();
    } else if (targets.getWaypointIndex() >= 0) {
        showWaypointInfo();
    }
    int tasksCount = ScienceTask::countTasks(my_spaceship->scienceTasks, PlayerSpaceship::max_science_tasks);
    if (tasksCount < PlayerSpaceship::max_science_tasks){
        if (target){
            if (target->getScannedStateFor(my_spaceship) == SS_FullScan){
                scan_status->setText("Scanned")->setColor(grey)->setTextColor(grey);
            } else if(target->canBeScannedBy(my_spaceship)){
                scan_status->setText("Scan '" + ship->getTypeName() + "'")->setColor(sf::Color::White)->setTextColor(sf::Color::White);
            } else {
                scan_status->setText("Can not scan '" + ship->getTypeName() + "'")->setColor(grey)->setTextColor(grey);
            }
            if (target->canBeHackedBy(my_spaceship)){
                hack_action->setText("Hack '" + ship->getTypeName() + "'")->setColor(sf::Color::White)->setTextColor(sf::Color::White);
            } else {
                hack_action->setText("Can't hack " + target->getCallSign())->setColor(grey)->setTextColor(grey);
            }
        } else {
            scan_status->setText("nothing to scan")->setColor(grey)->setTextColor(grey);
            hack_action->setText("nothing to hack")->setColor(grey)->setTextColor(grey);
        }
    } else {
        scan_status->setText("Tasks queue full")->setColor(grey)->setTextColor(grey);
        hack_action->setText("Tasks queue full")->setColor(grey)->setTextColor(grey);
    }
    if (ship && ship->getScannedStateFor(my_spaceship) >= SS_SimpleScan){
        query_action->setText("Query '" + ship->getTypeName() + "'")->setColor(sf::Color::White)->setTextColor(sf::Color::White);
    } else {
        query_action->setText("No query target")->setColor(grey)->setTextColor(grey);
    }

    tasks_queue_title->setText(string(tasksCount, 0) + "/" + string(PlayerSpaceship::max_science_tasks, 0) + " Tasks");
    for(int n = 0; n < PlayerSpaceship::max_science_tasks; n++){
        if (my_spaceship->scienceTasks[n].type != STT_Empty){
            tasks_queue[n]->setText(my_spaceship->scienceTasks[n].getDescription())->show();
        } else {
            tasks_queue[n]->setText("-")->hide();
        }
    }
}

void ScienceHeliosScreen::showNoInfo(){
    info_callsign->setText("-");
    info_distance->setValue("-");
    info_heading->setValue("-");
    info_relspeed->setValue("-");
    info_faction->setValue("-");
    info_type->setValue("-");
    info_shields->setValue("-");
    info_hull->setValue("-");
    info_shield_frequency->setFrequency(-1);
    info_beam_frequency->setFrequency(-1);
    info_description->setText("");

    for(int n = 0; n < SYS_COUNT; n++) {
        info_system[n]->setValue("-")->setColor(sf::Color::White)->setContentColor(sf::Color::White);
    }
}

void ScienceHeliosScreen::showWaypointInfo(){
    // If the target is a waypoint, show its heading and distance, and our
    // velocity toward it.
        // sidebar_pager->hide();
        sf::Vector2f position_diff = my_spaceship->waypoints[targets.getWaypointIndex()] - my_spaceship->getPosition();
        info_callsign->setText("Waypoint " + string(targets.getWaypointIndex() + 1, 0));
        float distance = sf::length(position_diff);
        float heading = sf::vector2ToAngle(position_diff) - 270;

        while(heading < 0) heading += 360;

        float rel_velocity = -dot(my_spaceship->getVelocity(), position_diff / distance);

        if (fabs(rel_velocity) < 0.01)
            rel_velocity = 0.0;

        info_distance->setValue(string(distance / 1000.0f, 1) + DISTANCE_UNIT_1K);
        info_heading->setValue(string(int(heading)));
        info_relspeed->setValue(string(rel_velocity / 1000.0f * 60.0f, 1) + DISTANCE_UNIT_1K + "/min");
}

void ScienceHeliosScreen::showTargetInfo(){
        P<SpaceObject> obj = targets.get();
        P<SpaceShip> ship = obj;
        P<SpaceStation> station = obj;

        sf::Vector2f position_diff = obj->getPosition() - my_spaceship->getPosition();
        float distance = sf::length(position_diff);
        float heading = sf::vector2ToAngle(position_diff) - 270;

        while(heading < 0) heading += 360;

        float rel_velocity = dot(obj->getVelocity(), position_diff / distance) - dot(my_spaceship->getVelocity(), position_diff / distance);

        if (fabs(rel_velocity) < 0.01)
            rel_velocity = 0.0;

        string duration = "";
        if (fabs(rel_velocity) > 0.01) {
            if (fabs(my_spaceship->getHeading()-heading) < 10) {
                int seconds = fabs(distance/rel_velocity);
                int minutes = seconds / 60;
                int hours = minutes / 60;
                int days = hours / 24;
                duration += " (" + string(days) + ":";
                if (hours < 10)
                    duration += "0";
                duration += string(hours%24) + ":";
                if (minutes < 10)
                    duration += "0";
                duration += string(minutes%60) + ":";
                if (seconds%60 < 10)
                    duration += "0";
                duration += string(seconds%60) + ")";
            }
        }

        info_callsign->setText(obj->getCallSign());
        info_distance->setValue(string(distance / 1000.0f, 1) + DISTANCE_UNIT_1K);
        info_heading->setValue(string(int(heading)));
        info_relspeed->setValue(string(rel_velocity / 1000.0f * 60.0f, 1) + DISTANCE_UNIT_1K + "/min" + duration);

        string description = obj->getDescriptionFor(my_spaceship);

        // If the target is a ship, show information about the ship based on how
        // deeply we've scanned it.
        if (ship) {
            // On a simple scan or deeper, show the faction, ship type, shields,
            // hull integrity, and database reference button.
            if (ship->getScannedStateFor(my_spaceship) >= SS_SimpleScan) {
                info_faction->setValue(factionInfo[obj->getFactionId()]->getName());
                info_type->setValue(ship->getTypeName());
                // info_type_button->show();
                info_shields->setValue(ship->getShieldDataString());
                info_hull->setValue(int(ship->getHull()));
            }
            if (ship->getScannedStateFor(my_spaceship) >= SS_FullScan) {
                // If beam and shield frequencies are enabled on the server,
                // populate their graphs.
                if (gameGlobalInfo->use_beam_shield_frequencies) {
                    info_shield_frequency->setFrequency(ship->shield_frequency);
                    info_beam_frequency->setFrequency(ship->beam_frequency);
                }
                
                // Show the status of each subsystem.
                for(int n = 0; n < SYS_COUNT; n++) {
                    if (ship->hasSystem(ESystem(n))){
                        float effectiveness = ship->getSystemEffectiveness(ESystem(n));
                        sf::Color statusColor = GuiSystemStatus::getSystemEffectivenessColor(effectiveness);
                        sf::Color actionColor = ship->canBeHackedBy(my_spaceship) ? sf::Color::Yellow : sf::Color::White;
                        info_system[n]->setValue(string(int(effectiveness * 100.0f)) + "%")->setColor(actionColor)->setKeyColor(actionColor)->setValueColor(statusColor);
                    } else {
                        info_system[n]->setValue("N/A")->setContentColor(sf::Color::White);
                    }
                }
            }
            // }
        } else {
        // If the target isn't a ship, show basic info.
            // sidebar_pager->hide();
            info_faction->setValue(factionInfo[obj->getFactionId()]->getName());

            // If the target is a station, show basic tactical info.
            if (station) {
                info_type->setValue(station->template_name);
                info_shields->setValue(station->getShieldDataString());
                info_hull->setValue(int(station->getHull()));
            }
        }
}
// TODO waypoints
void ScienceHeliosScreen::iterateTagrets(bool forward){
    if (target_waypoints){
        targets.nextWaypoint(forward);
    } else {
        PVector<SpaceObject> potentialTargetsUnfiltered = GuiRadarView::getVisibleObjects(
            radar_pov->getPosition(), 
            my_spaceship->getFactionId(), 
            GuiRadarView::RadarRangeAndLineOfSight, 
            radar->getDistance());
        PVector<SpaceObject> potentialTargets;
        foreach(SpaceObject, obj, potentialTargetsUnfiltered) {
            if(obj != my_spaceship && obj->canBeSelectedBy(my_spaceship)) {
                potentialTargets.push_back(obj);
            }
        }
        std::sort(potentialTargets.begin(), potentialTargets.end(), [this](P<SpaceObject> o1, P<SpaceObject> o2) { 
            return (o1->getPosition() - radar_pov->getPosition()) < (o2->getPosition() - radar_pov->getPosition());
        }); 
        targets.next(potentialTargets, forward);
    }
}

void ScienceHeliosScreen::onHotkey(const HotkeyResult& key)
{
     if (key.category == "TARGET" && my_spaceship){
        if (key.hotkey == "NEXT_TARGET") {
            iterateTagrets(true);
        } else if (key.hotkey == "PREV_TARGET") {
            iterateTagrets(false);
        } else if (key.hotkey == "TARGET_WAYPOINTS_TOGGLE") {
            targets.clear();
            target_waypoints = !target_waypoints;
        } 
    } else if (key.category == "SCIENCE" && my_spaceship){
        int tasksCount = ScienceTask::countTasks(my_spaceship->scienceTasks, PlayerSpaceship::max_science_tasks);
        if (key.hotkey == "POV_SHIP") {
            probe_view = false;
        } else if (key.hotkey == "POV_PROBE") {
            probe_view = true;
        } else if (key.hotkey == "CLEAR_TASKS") {
            my_spaceship->commandClearAllTasks();
        } else if (key.hotkey == "OPEN_TYPE_IN_DB") {
            P<SpaceShip> ship = targets.get();
            if (ship && ship->getScannedStateFor(my_spaceship) >= SS_SimpleScan){
                my_spaceship->commandSendScienceQueryToBridgeDB(ship->getTypeName());
            }
        } else if (tasksCount < PlayerSpaceship::max_science_tasks){
            if (key.hotkey == "SCAN") {
                if (targets.get() && targets.get()->canBeScannedBy(my_spaceship)){
                    my_spaceship->commandAddScanTask(targets.get());
                }
            } else {
                for(int n=0; n<SYS_COUNT; n++) {
                    ESystem system = ESystem(n);
                    string systemName = getSystemName(system);
                    if (key.hotkey == "HACK_" + systemName) {
                        P<SpaceShip> ship = targets.get();
                        if (ship && ship->canBeHackedBy(my_spaceship) && ship->hasSystem(system)){
                            my_spaceship->commandAddHackTask(ship, system);
                        }
                    }
                }
            }
        }
    }
}

bool ScienceHeliosScreen::onJoystickAxis(const AxisAction& axisAction){
    if (axisAction.category == "SCIENCE" && my_spaceship){
        if (axisAction.action == "ZOOM"){
            zoom = (axisAction.value + 1) / 2.0;
            return true;
        } 
    }
    return false;
}
