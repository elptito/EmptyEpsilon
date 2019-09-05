#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "scienceHeliosScreen.h"
#include "scienceDatabase.h"
#include "spaceObjects/nebula.h"

#include "screenComponents/radarView.h"
#include "screenComponents/rawScannerDataRadarOverlay.h"
#include "screenComponents/scanTargetButton.h"
#include "screenComponents/frequencyCurve.h"
#include "screenComponents/scanningDialog.h"
#include "screenComponents/databaseView.h"
#include "screenComponents/alertOverlay.h"
#include "screenComponents/customShipFunctions.h"

#include "gui/gui2_autolayout.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_togglebutton.h"
#include "gui/gui2_selector.h"
#include "gui/gui2_scrolltext.h"
#include "gui/gui2_listbox.h"
#include "gui/gui2_label.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_image.h"

ScienceHeliosScreen::ScienceHeliosScreen(GuiContainer* owner, ECrewPosition crew_position)
: GuiOverlay(owner, "SCIENCE_SCREEN", colorConfig.background)
{
    targets.setAllowWaypointSelection(); // WHY?

    GuiOverlay* background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");
    
    GuiAutoLayout *root_layout = new GuiAutoLayout(this, "ROOT", GuiAutoLayout::LayoutHorizontalRightToLeft);
    root_layout->setMargins(0, 20, 0, 20)->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    // Target scan data sidebar.
    GuiAutoLayout *right_column = new GuiAutoLayout(root_layout, "RIGHT_COLUMN", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_column->setMargins(10, 0, 0, 0)->setSize(250, GuiElement::GuiSizeMax);

    // Target scan data sidebar.
    GuiAutoLayout *middle_column = new GuiAutoLayout(root_layout, "MIDDLE_COLUMN", GuiAutoLayout::LayoutVerticalTopToBottom);
    middle_column->setMargins(0, 0, 10, 0)->setSize(250, GuiElement::GuiSizeMax);

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
    science_radar = new GuiRadarView(radar_view, "SCIENCE_RADAR", gameGlobalInfo->long_range_radar_range, &targets, my_spaceship);
    science_radar->setMargins(25)->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    science_radar->setRangeIndicatorStepSize(5000.0)->longRange()->enableWaypoints()->enableCallsigns()->enableHeadingIndicators()->setStyle(GuiRadarView::Circular)->setFogOfWarStyle(GuiRadarView::RadarRangeAndLineOfSight);
    science_radar->setCallbacks(
        [this](sf::Vector2f position) {
            if (!my_spaceship || my_spaceship->scanning_delay > 0.0)
                return;

            targets.setToClosestTo(position, 1000, TargetsContainer::Selectable);
        }, nullptr, nullptr
    );
    new RawScannerDataRadarOverlay(science_radar, "", gameGlobalInfo->long_range_radar_range);

    // Draw and hide the probe radar.
    probe_radar = new GuiRadarView(radar_view, "PROBE_RADAR", 5000, &targets, my_spaceship);
    probe_radar->setPosition(-270, 0, ACenterRight)->setMargins(40)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax)->hide();
    probe_radar->setAutoCentering(false)->longRange()->enableWaypoints()->enableCallsigns()->enableHeadingIndicators()->setStyle(GuiRadarView::Circular)->setFogOfWarStyle(GuiRadarView::NoFogOfWar);
    probe_radar->setCallbacks(
        [this](sf::Vector2f position) {
            if (!my_spaceship || my_spaceship->scanning_delay > 0.0)
                return;

            targets.setToClosestTo(position, 1000, TargetsContainer::Selectable);
        }, nullptr, nullptr
    );
    new RawScannerDataRadarOverlay(probe_radar, "", 5000);

    // Scan button.
    scan_button = new GuiScanTargetButton(target_actions, "SCAN_BUTTON", &targets);
    scan_button->setSize(GuiElement::GuiSizeMax, 50);
    info_type_button = new GuiButton(target_actions, "TYPE_INFO_BUTTON", "DB", [this]() {
        // P<SpaceShip> ship = targets.get();
        // if (ship && database_view->findAndDisplayEntry(ship->getTypeName())) {
        //     database_view->show();
        // }
    });
    info_type_button->setSize(GuiElement::GuiSizeMax, 50);
    // Simple scan data.
    info_callsign = new GuiKeyValueDisplay(middle_column, "SCIENCE_CALLSIGN", 0.4, "Callsign", "");
    info_callsign->setSize(GuiElement::GuiSizeMax, 30);
    info_distance = new GuiKeyValueDisplay(middle_column, "SCIENCE_DISTANCE", 0.4, "Distance", "");
    info_distance->setSize(GuiElement::GuiSizeMax, 30);
    info_heading = new GuiKeyValueDisplay(middle_column, "SCIENCE_HEADING", 0.4, "Heading", "");
    info_heading->setSize(GuiElement::GuiSizeMax, 30);
    info_relspeed = new GuiKeyValueDisplay(middle_column, "SCIENCE_REL_SPEED", 0.4, "Rel. Speed", "");
    info_relspeed->setSize(GuiElement::GuiSizeMax, 30);
    info_faction = new GuiKeyValueDisplay(middle_column, "SCIENCE_FACTION", 0.4, "Faction", "");
    info_faction->setSize(GuiElement::GuiSizeMax, 30);
    info_type = new GuiKeyValueDisplay(middle_column, "SCIENCE_TYPE", 0.4, "Type", "");
    info_type->setSize(GuiElement::GuiSizeMax, 30);

    info_shields = new GuiKeyValueDisplay(middle_column, "SCIENCE_SHIELDS", 0.4, "Shields", "");
    info_shields->setSize(GuiElement::GuiSizeMax, 30);
    info_hull = new GuiKeyValueDisplay(middle_column, "SCIENCE_HULL", 0.4, "Hull", "");
    info_hull->setSize(GuiElement::GuiSizeMax, 30);


    // Prep the description text area.
    info_description = new GuiScrollText(middle_column, "SCIENCE_DESC", "");
    info_description->setTextSize(28)->setMargins(0, 20, 0, 0)->setSize(GuiElement::GuiSizeMax, 400);

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

    // Prep the database view.
    // database_view = new DatabaseViewComponent(this);
    // database_view->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    // Probe view button
    probe_view_button = new GuiToggleButton(radar_source, "PROBE_VIEW", "Probe View", [this](bool value){
        P<ScanProbe> probe;

        if (game_server)
            probe = game_server->getObjectById(my_spaceship->linked_science_probe_id);
        else
            probe = game_client->getObjectById(my_spaceship->linked_science_probe_id);
        
        if (value && probe)
        {
            sf::Vector2f probe_position = probe->getPosition();
            science_radar->hide();
            probe_radar->show();
            probe_radar->setViewPosition(probe_position)->show();
        } else {
            probe_view_button->setValue(false);
            science_radar->show();
            probe_radar->hide();
        }
    });
    probe_view_button->setSize(GuiElement::GuiSizeMax, 50)->disable();

    // Draw the zoom slider.
    zoom_bar = new GuiProgressbar(radar_controls, "ZOOM_BAR", gameGlobalInfo->long_range_radar_range, 5000.f, -1); // force sync in onDraw()
    zoom_bar->setSize(GuiElement::GuiSizeMax, 50);

    // Scanning dialog.
    new GuiScanningDialog(this, "SCANNING_DIALOG");
    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));
}

void ScienceHeliosScreen::onDraw(sf::RenderTarget& window)
{
    GuiOverlay::onDraw(window);
    P<ScanProbe> probe;

    // Handle mouse wheel
    float mouse_wheel_delta = InputHandler::getMouseWheelDelta();
    if (mouse_wheel_delta != 0.0)
    {
        float view_distance = science_radar->getDistance() * (1.0 - (mouse_wheel_delta * 0.1f));
        if (view_distance > gameGlobalInfo->long_range_radar_range)
            view_distance = gameGlobalInfo->long_range_radar_range;
        if (view_distance < 5000.0f)
            view_distance = 5000.0f;
        science_radar->setDistance(view_distance);
    }

    // Keep the zoom slider in sync.
    if (zoom_bar->getValue() != science_radar->getDistance()){
        zoom_bar->setValue(science_radar->getDistance());
        zoom_bar->setText("Zoom: " + string(gameGlobalInfo->long_range_radar_range / science_radar->getDistance(), 1) + "x");
    }

    if (!my_spaceship)
        return;

    if (game_server)
        probe = game_server->getObjectById(my_spaceship->linked_science_probe_id);
    else
        probe = game_client->getObjectById(my_spaceship->linked_science_probe_id);

    if (probe_view_button->getValue() && probe)
    {
        if (targets.get() && (probe->getPosition() - targets.get()->getPosition()) > 5000.0f)
            targets.clear();
    }else{
        if (targets.get() && Nebula::blockedByNebula(my_spaceship->getPosition(), targets.get()->getPosition()))
            targets.clear();
    }
    
    // sidebar_selector->setVisible(sidebar_selector->getSelectionIndex() > 0 || custom_function_sidebar->hasEntries());

    info_callsign->setValue("-");
    info_distance->setValue("-");
    info_heading->setValue("-");
    info_relspeed->setValue("-");
    info_faction->setValue("-");
    info_type->setValue("-");
    info_shields->setValue("-");
    info_hull->setValue("-");
    info_shield_frequency->setFrequency(-1);
    info_beam_frequency->setFrequency(-1);
    info_description->setText("-");
    // info_type_button->hide();
    // sidebar_pager->hide();

    for(int n = 0; n < SYS_COUNT; n++) {
        info_system[n]->setValue("-");
    }

    if (probe) {
        probe_view_button->enable();
        probe_radar->setViewPosition(probe->getPosition());
    } else {
        probe_view_button->disable();
        probe_view_button->setValue(false);
        science_radar->show();
        probe_radar->hide();
    }

    if (targets.get()) {
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

                duration += " (";
                duration += string(days);
                duration += ":";
                if (hours < 10)
                    duration += "0";
                duration += string(hours%24);
                duration += ":";
                if (minutes < 10)
                    duration += "0";
                duration += string(minutes%60);
                duration += ":";
                if (seconds%60 < 10)
                    duration += "0";
                duration += string(seconds%60);
                duration += ")";
            }
        }

        info_callsign->setValue(obj->getCallSign());
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

                // If beam and shield frequencies are enabled on the server,
                // populate their graphs.
                if (gameGlobalInfo->use_beam_shield_frequencies) {
                    info_shield_frequency->setFrequency(ship->shield_frequency);
                    info_beam_frequency->setFrequency(ship->beam_frequency);
                }
                
                // Show the status of each subsystem.
                for(int n = 0; n < SYS_COUNT; n++) {
                    float system_health = ship->systems[n].health;
                    info_system[n]->setValue(string(int(system_health * 100.0f)) + "%")->setColor(sf::Color(255, 127.5 * (system_health + 1), 127.5 * (system_health + 1), 255));
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
    } else if (targets.getWaypointIndex() >= 0) {
    // If the target is a waypoint, show its heading and distance, and our
    // velocity toward it.
        // sidebar_pager->hide();
        sf::Vector2f position_diff = my_spaceship->waypoints[targets.getWaypointIndex()] - my_spaceship->getPosition();
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
}
