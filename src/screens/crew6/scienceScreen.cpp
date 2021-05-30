#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "scienceScreen.h"
#include "scienceDatabase.h"
#include "spaceObjects/nebula.h"
#include "preferenceManager.h"
#include "shipTemplate.h"

#include "screenComponents/radarView.h"
#include "screenComponents/rawScannerDataRadarOverlay.h"
#include "screenComponents/scanTargetButton.h"
#include "screenComponents/frequencyCurve.h"
#include "screenComponents/scanningDialog.h"
#include "screenComponents/databaseView.h"
#include "screenComponents/alertOverlay.h"
#include "screenComponents/customShipFunctions.h"
#include "screenComponents/commsOverlay.h"
#include "screenComponents/shipsLogControl.h"

#include "gui/gui2_autolayout.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_togglebutton.h"
#include "gui/gui2_selector.h"
#include "gui/gui2_scrolltext.h"
#include "gui/gui2_listbox.h"
#include "gui/gui2_slider.h"

ScienceScreen::ScienceScreen(GuiContainer* owner, ECrewPosition crew_position)
: GuiOverlay(owner, "SCIENCE_SCREEN", colorConfig.background)
{
    targets.setAllowWaypointSelection();

    // Render the radar shadow and background decorations.
    background_gradient = new GuiOverlay(this, "BACKGROUND_GRADIENT", sf::Color::White);
    background_gradient->setTextureCenter("gui/BackgroundGradientOffset");

    background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");

    // Render the alert level color overlay.
    (new AlertLevelOverlay(this));

    // Draw the radar.
    radar_view = new GuiElement(this, "RADAR_VIEW");
    radar_view->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    // Draw the science radar.
    science_radar = new GuiRadarView(radar_view, "SCIENCE_RADAR", my_spaceship ? my_spaceship->getLongRangeRadarRange() : 30000.0, &targets, my_spaceship);
    science_radar->setPosition(-270, 0, ACenterRight)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    science_radar->setRangeIndicatorStepSize(5000.0)->longRange()->enableWaypoints()->enableCallsigns()->enableHeadingIndicators()->setStyle(GuiRadarView::Circular)->setFogOfWarStyle(GuiRadarView::NebulaFogOfWar);
    science_radar->setCallbacks(
        [this](sf::Vector2f position) {
            if (!my_spaceship || my_spaceship->scanning_delay > 0.0)
                return;

            targets.setToClosestTo(position, 1000, TargetsContainer::Selectable);
        }, nullptr, nullptr
    );
    science_radar->setAutoRotating(PreferencesManager::get("science_radar_lock","0")=="1");
    new RawScannerDataRadarOverlay(science_radar, "", my_spaceship ? my_spaceship->getLongRangeRadarRange() : 30000.0f);

    // Draw and hide the probe radar.
    probe_radar = new GuiRadarView(radar_view, "PROBE_RADAR", 5000, &targets, my_spaceship);
    probe_radar->setPosition(-270, 0, ACenterRight)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax)->hide();
    probe_radar->setAutoCentering(false)->longRange()->enableWaypoints()->enableCallsigns()->enableHeadingIndicators()->setStyle(GuiRadarView::Circular)->setFogOfWarStyle(GuiRadarView::NoFogOfWar);
    probe_radar->setCallbacks(
        [this](sf::Vector2f position) {
            if (!my_spaceship || my_spaceship->scanning_delay > 0.0)
                return;

            targets.setToClosestTo(position, 1000, TargetsContainer::Selectable);
        }, nullptr, nullptr
    );
    new RawScannerDataRadarOverlay(probe_radar, "", 5000);

    sidebar_selector = new GuiSelector(radar_view, "", [this](int index, string value)
    {
        info_sidebar->setVisible(index == 0);
        custom_function_sidebar->setVisible(index == 1);
    });
    sidebar_selector->setOptions({"Scanning", "Other"});
    sidebar_selector->setSelectionIndex(0);
    sidebar_selector->setPosition(-20, 120, ATopRight)->setSize(250, 50);

    // Target scan data sidebar.
    info_sidebar = new GuiAutoLayout(radar_view, "SIDEBAR", GuiAutoLayout::LayoutVerticalTopToBottom);
    info_sidebar->setPosition(-20, 170, ATopRight)->setSize(250, GuiElement::GuiSizeMax);

    custom_function_sidebar = new GuiCustomShipFunctions(radar_view, crew_position, "", my_spaceship);
    custom_function_sidebar->setPosition(-20, 170, ATopRight)->setSize(250, GuiElement::GuiSizeMax)->hide();

    // Scan button.
    scan_button = new GuiScanTargetButton(info_sidebar, "SCAN_BUTTON", &targets);
    scan_button->setSize(GuiElement::GuiSizeMax, 50)->setVisible(my_spaceship && my_spaceship->getCanScan());

    // Simple scan data.
    info_callsign = new GuiKeyValueDisplay(info_sidebar, "SCIENCE_CALLSIGN", 0.4, tr("Callsign"), "");
    info_callsign->setSize(GuiElement::GuiSizeMax, 30);
    info_distance = new GuiKeyValueDisplay(info_sidebar, "SCIENCE_DISTANCE", 0.4, tr("science","Distance"), "");
    info_distance->setSize(GuiElement::GuiSizeMax, 30);
    info_heading = new GuiKeyValueDisplay(info_sidebar, "SCIENCE_HEADING", 0.4, tr("Bearing"), "");
    info_heading->setSize(GuiElement::GuiSizeMax, 30);
    info_relspeed = new GuiKeyValueDisplay(info_sidebar, "SCIENCE_REL_SPEED", 0.4, tr("Rel. Speed"), "");
    info_relspeed->setSize(GuiElement::GuiSizeMax, 30);
    info_faction = new GuiKeyValueDisplay(info_sidebar, "SCIENCE_FACTION", 0.4, tr("Faction"), "");
    info_faction->setSize(GuiElement::GuiSizeMax, 30);
    info_type = new GuiKeyValueDisplay(info_sidebar, "SCIENCE_TYPE", 0.4, tr("science","Type"), "");
    info_type->setSize(GuiElement::GuiSizeMax, 30);
    info_type_button = new GuiButton(info_type, "SCIENCE_TYPE_BUTTON", tr("database", "DB"), [this]() {
        //P<SpaceShip> ship = targets.get();
        P<ShipTemplateBasedObject> shipTemplate = targets.get();
        if (shipTemplate)
        {
            if (database_view->findAndDisplayEntry(shipTemplate->getTypeName()))
            {
                view_mode_selection->setSelectionIndex(1);
                radar_view->hide();
                background_gradient->hide();
                database_view->show();
            }
        }
    });
    info_type_button->setTextSize(20)->setPosition(0, 1, ATopLeft)->setSize(50, 28);
    info_shields = new GuiKeyValueDisplay(info_sidebar, "SCIENCE_SHIELDS", 0.4, tr("science", "Shields"), "");
    info_shields->setSize(GuiElement::GuiSizeMax, 30);
    info_hull = new GuiKeyValueDisplay(info_sidebar, "SCIENCE_HULL", 0.4, tr("science", "Hull"), "");
    info_hull->setSize(GuiElement::GuiSizeMax, 30);

    // Full scan data

    // Draw and hide the sidebar pager.
    sidebar_pager = new GuiSelector(info_sidebar, "SIDEBAR_PAGER", [this](int index, string value) {});
    sidebar_pager->setSize(GuiElement::GuiSizeMax, 50)->hide();
    
    // Add sidebar page for signature
    sidebar_pager->addEntry("Signals", "Signals");

    // If the server uses frequencies, add the Tactical sidebar page.
    if (gameGlobalInfo->use_beam_shield_frequencies)
    {
        sidebar_pager->addEntry("Tactical", "Tactical");
    }

    // Add sidebar page for systems.
    sidebar_pager->addEntry("Systems", "Systems");

    // Add sidebar page for a description.
    sidebar_pager->addEntry("Description", "Description");
    
    // Add sidebar page for structured infos.
    sidebar_pager->addEntry("Informations", "Informations");

    // Default the pager to the first item.
    sidebar_pager->setSelectionIndex(0);

    // Prep and hide the frequency graphs.
    info_shield_frequency = new GuiFrequencyCurve(info_sidebar, "SCIENCE_SHIELD_FREQUENCY", false, true);
    info_shield_frequency->setSize(GuiElement::GuiSizeMax, 150);
    info_beam_frequency = new GuiFrequencyCurve(info_sidebar, "SCIENCE_BEAM_FREQUENCY", true, false);
    info_beam_frequency->setSize(GuiElement::GuiSizeMax, 150);

    // Show shield and beam frequencies only if enabled by the server.
    if (!gameGlobalInfo->use_beam_shield_frequencies)
    {
        info_shield_frequency->hide();
        info_beam_frequency->hide();
    }

    // List each system's status.
    for(int n = 0; n < SYS_COUNT; n++)
    {
        info_system[n] = new GuiKeyValueDisplay(info_sidebar, "SCIENCE_SYSTEM_" + string(n), 0.75, getLocaleSystemName(ESystem(n)), "-");
        info_system[n]->setSize(GuiElement::GuiSizeMax, 30);
        info_system[n]->hide();
    }

    // Prep and hide the description text area.
    info_description = new GuiScrollText(info_sidebar, "SCIENCE_DESC", "");
    info_description->setTextSize(28)->setMargins(20, 20, 0, 0)->setSize(GuiElement::GuiSizeMax, 400)->hide();

    // Prep and hide the other structured informations
    for(int n = 0; n < 10; n++)
    {
        info_other[n] = new GuiKeyValueDisplay(info_sidebar, "SCIENCE_INFOS_" + string(n), 0.6, "-", "-");
        info_other[n]->setSize(GuiElement::GuiSizeMax, 30);
        info_other[n]->hide();
    }
    
    // Prep and hide the detailed signal bands.
    info_electrical_signal_band = new GuiSignalQualityIndicator(info_sidebar, "ELECTRICAL_SIGNAL");
    info_electrical_signal_band->showGreen(false)->showBlue(false)->setSize(GuiElement::GuiSizeMax, 80)->hide();
    info_electrical_signal_label = new GuiLabel(info_electrical_signal_band, "", "Electrical", 30);
    info_electrical_signal_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    info_gravity_signal_band = new GuiSignalQualityIndicator(info_sidebar, "GRAVITY_SIGNAL");
    info_gravity_signal_band->showRed(false)->showGreen(false)->setSize(GuiElement::GuiSizeMax, 80)->hide();
    info_gravity_signal_label = new GuiLabel(info_gravity_signal_band, "", "Gravitational", 30);
    info_gravity_signal_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    info_biological_signal_band = new GuiSignalQualityIndicator(info_sidebar, "BIOLOGICAL_SIGNAL");
    info_biological_signal_band->showRed(false)->showBlue(false)->setSize(GuiElement::GuiSizeMax, 80)->hide();
    info_biological_signal_label = new GuiLabel(info_biological_signal_band, "", "Biological", 30);
    info_biological_signal_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    // Prep and hide the database view.
    database_view = new DatabaseViewComponent(this);
    database_view->hide()->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    // Probe view button
    probe_view_button = new GuiToggleButton(radar_view, "PROBE_VIEW", tr("button", "Probe View"), [this](bool value){
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
        }else{
            probe_view_button->setValue(false);
            science_radar->show();
            probe_radar->hide();
        }
    });
    probe_view_button->setPosition(20, -120, ABottomLeft)->setSize(200, 50)->disable();

    // Link target to analysis screen.
    link_to_analysis_button = new GuiToggleButton(radar_view, "LINK_TO_ANALYSIS", tr("Link to Analysis"), [this](bool value){
        if (value)
            my_spaceship->commandSetAnalysisLink(targets.get()->getMultiplayerId());
        else
            my_spaceship->commandSetAnalysisLink(-1);
    });
    link_to_analysis_button->setPosition(-20, -70, ABottomRight)->setSize(250, 50);

    // Draw the zoom slider.
    zoom_slider = new GuiSlider(radar_view, "", my_spaceship ? my_spaceship->getLongRangeRadarRange() : 30000.0f, my_spaceship ? my_spaceship->getShortRangeRadarRange() : 5000.0f, my_spaceship ? my_spaceship->getLongRangeRadarRange() : 30000.0f, [this](float value)
    {
        if (my_spaceship)
            zoom_label->setText(tr("Zoom: {zoom}x").format({{"zoom", string(my_spaceship->getLongRangeRadarRange() / value, 1)}}));
        science_radar->setDistance(value);
    });
    zoom_slider->setPosition(-20, -20, ABottomRight)->setSize(250, 50);
    zoom_label = new GuiLabel(zoom_slider, "", "Zoom: 1.0x", 30);
    zoom_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    // Radar/database view toggle.
    view_mode_selection = new GuiListbox(this, "VIEW_SELECTION", [this](int index, string value) {
        radar_view->setVisible(index == 0);
        background_gradient->setVisible(index == 0);
        database_view->setVisible(index == 1);
    });
    view_mode_selection->setOptions({tr("button", "Radar"), tr("button", "Database")})->setSelectionIndex(0)->setPosition(20, -20, ABottomLeft)->setSize(200, 100);

    // Scanning dialog.
    new GuiScanningDialog(this, "SCANNING_DIALOG");

    new ShipsLog(this, crew_position);
}

void ScienceScreen::onDraw(sf::RenderTarget& window)
{
    GuiOverlay::onDraw(window);
    P<ScanProbe> probe;

    if (!my_spaceship)
        return;

    float view_distance = science_radar->getDistance();
    float mouse_wheel_delta=InputHandler::getMouseWheelDelta();
    if (mouse_wheel_delta!=0)
    {
        view_distance *= (1.0 - (mouse_wheel_delta * 0.1f));
    }
    view_distance = std::min(view_distance,my_spaceship->getLongRangeRadarRange());
    view_distance = std::max(view_distance,my_spaceship->getShortRangeRadarRange());
    if (view_distance!=science_radar->getDistance() || previous_long_range_radar != my_spaceship->getLongRangeRadarRange() || previous_short_range_radar != my_spaceship->getShortRangeRadarRange())
    {
        previous_short_range_radar=my_spaceship->getShortRangeRadarRange();
        previous_long_range_radar=my_spaceship->getLongRangeRadarRange();
        science_radar->setDistance(view_distance);
        // Keep the zoom slider in sync.
        zoom_slider->setValue(view_distance)->setRange(my_spaceship->getLongRangeRadarRange(),my_spaceship->getShortRangeRadarRange());
        zoom_label->setText(tr("Zoom: {zoom}x").format({{"zoom", string(my_spaceship->getLongRangeRadarRange() / view_distance, 1)}}));
    }

    if (game_server)
        probe = game_server->getObjectById(my_spaceship->linked_science_probe_id);
    else
        probe = game_client->getObjectById(my_spaceship->linked_science_probe_id);

    float radar_range = 5000.0;
    if (my_spaceship && my_spaceship->hasSystem(SYS_Scanner))
        radar_range = radar_range * my_spaceship->getSystemEffectiveness(SYS_Scanner);

    if (probe_view_button->getValue() && probe)
    {
        if (targets.get() && (probe->getPosition() - targets.get()->getPosition()) > radar_range + targets.get()->getRadius())
            targets.clear();
    }else{
        if (targets.get() && Nebula::blockedByNebula(my_spaceship->getPosition(), targets.get()->getPosition()))
            targets.clear();
    }

    sidebar_selector->setVisible(sidebar_selector->getSelectionIndex() > 0 || custom_function_sidebar->hasEntries());

    info_callsign->setValue("-");
    info_distance->setValue("-");
    info_heading->setValue("-");
    info_relspeed->setValue("-");
    info_faction->setValue("-");
    info_type->setValue("-");
    info_shields->setValue("-");
    info_hull->setValue("-");
    info_shield_frequency->setFrequency(-1)->hide();
    info_beam_frequency->setFrequency(-1)->hide();
    info_description->hide();
    for(int n = 0; n < 10; n++)
    {
        info_other[n]->setValue("-")->hide();
        info_other[n]->setKey("-")->hide();
    }
    info_electrical_signal_band->hide();
    info_gravity_signal_band->hide();
    info_biological_signal_band->hide();
    info_type_button->hide();
    sidebar_pager->hide();
    link_to_analysis_button->disable();

    for(int n = 0; n < SYS_COUNT; n++)
        info_system[n]->setValue("-")->hide();

    if (probe)
    {
        probe_view_button->enable();
        probe_radar->setViewPosition(probe->getPosition());
        probe_radar->setDistance(radar_range);
    }
    else
    {
        probe_view_button->disable();
        probe_view_button->setValue(false);
        science_radar->show();
        probe_radar->hide();
    }

    if (targets.get())
    {
        P<SpaceObject> obj = targets.get();
        P<SpaceShip> ship = obj;
        P<ShipTemplateBasedObject> shipTemplate = obj;
        P<SpaceStation> station = obj;
        
        if (my_spaceship and obj->getScannedStateFor(my_spaceship) >= SS_FullScan)
        {
            link_to_analysis_button->setValue(my_spaceship->linked_analysis_object_id == obj->getMultiplayerId());
            link_to_analysis_button->enable();
        }
        else
        {
            link_to_analysis_button->setValue(false);
            link_to_analysis_button->disable();
        }

        sf::Vector2f position_diff = obj->getPosition() - my_spaceship->getPosition();
        float distance = sf::length(position_diff);
        float heading = sf::vector2ToAngle(position_diff) - 270;

        while(heading < 0) heading += 360;

        float rel_velocity = dot(obj->getVelocity(), position_diff / distance) - dot(my_spaceship->getVelocity(), position_diff / distance);

        if (fabs(rel_velocity) < 0.01)
            rel_velocity = 0.0;

        info_callsign->setValue(obj->getCallSign());
        info_distance->setValue(string(distance / 1000.0f, 1) + DISTANCE_UNIT_1K);
        info_heading->setValue(string(int(heading)));
        info_relspeed->setValue(string(rel_velocity / 1000.0f * 60.0f, 1) + DISTANCE_UNIT_1K + "/min");

        string description = obj->getDescriptionFor(my_spaceship);
        info_description->setText(description)->setVisible(description != "");
        string sidebar_pager_selection = sidebar_pager->getSelectionValue();

        sidebar_pager->setVisible(false);            
        
        // Check each Scan level
        switch(obj->getScannedStateFor(my_spaceship))
        {
        case SS_NotScanned:
            break;
        case SS_FriendOrFoeIdentified:
        case SS_SimpleScan:
            info_faction->setValue(factionInfo[obj->getFactionId()]->getLocaleName());
            info_hull->setValue(int(ceil(obj->hull)));
            if (shipTemplate)
            {
                info_hull->setValue(int(ceil(shipTemplate->getHull())));
                info_type->setValue(shipTemplate->getTypeName());
                info_type_button->show();
                info_shields->setValue(shipTemplate->getShieldDataString());
            }
            break;
        case SS_FullScan:
            info_faction->setValue(factionInfo[obj->getFactionId()]->getLocaleName());
            info_hull->setValue(int(ceil(obj->hull)));
            if (shipTemplate)
            {
                info_hull->setValue(int(ceil(shipTemplate->getHull())));
                info_type->setValue(shipTemplate->getTypeName());
                info_type_button->show();
                info_shields->setValue(shipTemplate->getShieldDataString());
            }
            sidebar_pager->setVisible(true);

            // Check sidebar pager state.
            if (sidebar_pager_selection == "Tactical")
            {
                info_shield_frequency->show();
                info_beam_frequency->show();
				
				// Show on graph information that target has no beams instad of frequencies. 
				bool has_beams = false;
				for(int n = 0; n < max_beam_weapons; n++)
				{
					if (ship->beam_weapons[n].getRange() > 0.0) {
						has_beams = true;
						break;
					}
				}
				info_beam_frequency->setEnemyHasEquipment(has_beams);
				
				info_shield_frequency->setFrequency(ship->shield_frequency);
                info_beam_frequency->setFrequency(ship->beam_frequency);

				// Show on graph information that target has no shields instead of frequencies. 
				info_shield_frequency->setEnemyHasEquipment(ship->getShieldCount() > 0);
                
                for(int n = 0; n < SYS_COUNT; n++)
                {
                    info_system[n]->hide();
                }

                info_description->hide();
                for(int n = 0; n < 10; n++)
                    info_other[n]->hide();
                info_electrical_signal_band->hide();
                info_gravity_signal_band->hide();
                info_biological_signal_band->hide();
                
                if (ship && gameGlobalInfo->use_beam_shield_frequencies)
                {
                    info_shield_frequency->show();
                    info_shield_frequency->setFrequency(ship->shield_frequency);
                    info_beam_frequency->show();
                    info_beam_frequency->setFrequency(ship->beam_frequency);
                }
            }
            else if (sidebar_pager_selection == "Systems")
            {
                info_shield_frequency->hide();
                info_beam_frequency->hide();

                for(int n = 0; n < SYS_COUNT; n++)
                {
                    info_system[n]->hide();
                }

                info_description->hide();
                for(int n = 0; n < 10; n++)
                    info_other[n]->hide();
                info_electrical_signal_band->hide();
                info_gravity_signal_band->hide();
                info_biological_signal_band->hide();
                
                // Show the status of each subsystem.
                if (ship)
                {
                    for(int n = 0; n < SYS_COUNT; n++)
                    {
                        if (ship->hasSystem(ESystem(n)))
                        {
                            info_system[n]->show();
                            float system_health = ship->systems[n].health;
                            info_system[n]->setValue(string(int(system_health * 100.0f)) + "%")->setColor(sf::Color(255, 127.5 * (system_health + 1), 127.5 * (system_health + 1), 255));
                        }
                    }
                }
            }
            else if (sidebar_pager_selection == "Description")
            {
                info_shield_frequency->hide();
                info_beam_frequency->hide();

                for(int n = 0; n < SYS_COUNT; n++)
                {
                    info_system[n]->hide();
                }

                info_description->show();
                for(int n = 0; n < 10; n++)
                    info_other[n]->hide();
                info_electrical_signal_band->hide();
                info_gravity_signal_band->hide();
                info_biological_signal_band->hide();
            }
            else if (sidebar_pager_selection == "Informations")
            {
                info_shield_frequency->hide();
                info_beam_frequency->hide();

                for(int n = 0; n < SYS_COUNT; n++)
                {
                    info_system[n]->hide();
                }

                info_description->hide();
                
                for(int n = 0; n < 10; n++)
                {
                    if (obj->infos_label[n] == "")
                        continue;
                    info_other[n]->show();
                    info_other[n]->setKey(obj->infos_label[n]);
                    info_other[n]->setValue(obj->infos_value[n]);
                }

                info_electrical_signal_band->hide();
                info_gravity_signal_band->hide();
                info_biological_signal_band->hide();
            }
            else if (sidebar_pager_selection == "Signals")
            {
                info_shield_frequency->hide();
                info_beam_frequency->hide();

                for(int n = 0; n < SYS_COUNT; n++)
                {
                    info_system[n]->hide();
                }

                info_description->hide();
                for(int n = 0; n < 10; n++)
                    info_other[n]->hide();
                info_electrical_signal_band->show();
                info_gravity_signal_band->show();
                info_biological_signal_band->show();
                
                // Calculate signal noise for unscanned objects more than SRRR away.
                float distance_variance = 0.0f;
                float signal = 0.0f;

                if (distance > my_spaceship->getShortRangeRadarRange() && !obj->isScannedBy(my_spaceship))
                    distance_variance = (random(0.01f, (distance - my_spaceship->getShortRangeRadarRange())) / (my_spaceship->getLongRangeRadarRange() - my_spaceship->getShortRangeRadarRange())) / 10;
                    
                RawRadarSignatureInfo info;
                P<SpaceShip> ship = obj;

                if (ship)
                {
                    // Use dynamic signatures for ships.
                    info = ship->getDynamicRadarSignatureInfo();
                } else {
                    // Otherwise, use the baseline only.
                    info = obj->getRadarSignatureInfo();
                }

                // Calculate their waveforms.
                signal = std::max(0.0f, info.electrical - distance_variance);
                info_electrical_signal_band->setMaxAmp(signal);
                info_electrical_signal_band->setNoiseError(std::max(0.0f, (signal - 1.0f) / 10));
                info_electrical_signal_label->setText("Electrical: " + string(signal) + " MJ");

                signal = std::max(0.0f, info.gravity - distance_variance);
                info_gravity_signal_band->setMaxAmp(signal);
                info_gravity_signal_band->setPeriodError(std::max(0.0f, (signal - 1.0f) / 10));
                info_gravity_signal_label->setText("Gravitational: " + string(signal) + " dN");

                signal = std::max(0.0f, info.biological - distance_variance);
                info_biological_signal_band->setMaxAmp(signal);
                info_biological_signal_band->setPhaseError(std::max(0.0f, (signal - 1.0f) / 10));
                info_biological_signal_label->setText("Biological: " + string(signal) + " um");
            }
            else
            {
                LOG(WARNING) << "Invalid pager state: " << sidebar_pager_selection;
            }
            break;
        }
    }

    // If the target is a waypoint, show its heading and distance, and our
    // velocity toward it.
    else if (targets.getWaypointIndex() >= 0)
    {
        sidebar_pager->hide();
        sf::Vector2f position_diff = my_spaceship->waypoints[targets.getRouteIndex()][targets.getWaypointIndex()] - my_spaceship->getPosition();
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

void ScienceScreen::onHotkey(const HotkeyResult& key)
{
    if (key.category == "SCIENCE" && my_spaceship)
    {
        // Initiate a scan on scannable objects.
        if (key.hotkey == "SCAN_OBJECT" &&
            my_spaceship->getCanScan() &&
            my_spaceship->scanning_delay == 0.0)
        {
            P<SpaceObject> obj = targets.get();

            // Allow scanning only if the object is scannable, and if the player
            // isn't already scanning something.
            if (obj &&
                obj->canBeScannedBy(my_spaceship))
            {
                my_spaceship->commandScan(obj);
                return;
            }
        }

        // Cycle selection through scannable objects.
        if (key.hotkey == "NEXT_SCANNABLE_OBJECT" &&
            my_spaceship->scanning_delay == 0.0)
        {
            bool current_found = false;
            for (P<SpaceObject> obj : space_object_list)
            {
                // If this object is the current object, flag and skip it.
                if (obj == targets.get())
                {
                    current_found = true;
                    continue;
                }

                // If this object is my ship or not visible due to a Nebula,
                // skip it.
                if (obj == my_spaceship ||
                    Nebula::blockedByNebula(my_spaceship->getPosition(), obj->getPosition()))
                    continue;

                // If this is a scannable object and the currently selected
                // object, and it remains in radar range, continue to set it.
                if (current_found &&
                    sf::length(obj->getPosition() - my_spaceship->getPosition()) < science_radar->getDistance() &&
                    obj->canBeScannedBy(my_spaceship))
                {
                    targets.set(obj);
                    return;
                }
            }

            // Advance to the next object.
            for (P<SpaceObject> obj : space_object_list)
            {
                if (obj == targets.get() ||
                    obj == my_spaceship ||
                    Nebula::blockedByNebula(my_spaceship->getPosition(), obj->getPosition()))
                    continue;

                if (sf::length(obj->getPosition() - my_spaceship->getPosition()) < science_radar->getDistance() &&
                    obj->canBeScannedBy(my_spaceship))
                {
                    targets.set(obj);
                    return;
                }
            }
        }
        if (key.hotkey == "SHOW_PROBE")
        {
            P<ScanProbe> probe;
             if (game_server)
                probe = game_server->getObjectById(my_spaceship->linked_science_probe_id);
            else
                probe = game_client->getObjectById(my_spaceship->linked_science_probe_id);
             if (probe && !probe_view_button->getValue())
            {
                probe_view_button->setValue(true);
                sf::Vector2f probe_position = probe->getPosition();
                science_radar->hide();
                probe_radar->show();
                probe_radar->setViewPosition(probe_position)->show();
            }else{
                probe_view_button->setValue(false);
                science_radar->show();
                probe_radar->hide();
            }
        }
        if (key.hotkey == "SHOW_DATABASE")
        {
            P<SpaceShip> ship = targets.get();
            if (ship)
            {
                if (database_view->findAndDisplayEntry(ship->getTypeName()))
                {
                    view_mode_selection->setSelectionIndex(1);
                    radar_view->hide();
                    background_gradient->hide();
                    database_view->show();
                }
            }
        }
        if (key.hotkey == "SHOW_RADAR")
        {
            view_mode_selection->setSelectionIndex(0);
            radar_view->show();
            background_gradient->show();
            database_view->hide();
        }
        if (key.hotkey == "DEC_ZOOM")
        {
            float view_distance = science_radar->getDistance() - 1500.0f;
            if (view_distance > my_spaceship->getLongRangeRadarRange())
                view_distance = my_spaceship->getLongRangeRadarRange();
            if (view_distance < my_spaceship->getShortRangeRadarRange())
                view_distance = my_spaceship->getShortRangeRadarRange();
            science_radar->setDistance(view_distance);
            // Keep the zoom slider in sync.
            zoom_slider->setValue(view_distance);
            zoom_label->setText(tr("Zoom: {zoom}x").format({{"zoom", string(my_spaceship->getLongRangeRadarRange() / view_distance, 1)}}));
        }
        if (key.hotkey == "INC_ZOOM")
        {
            float view_distance = science_radar->getDistance() + 1500.0f;
            if (view_distance > my_spaceship->getLongRangeRadarRange())
                view_distance = my_spaceship->getLongRangeRadarRange();
            if (view_distance < my_spaceship->getShortRangeRadarRange())
                view_distance = my_spaceship->getShortRangeRadarRange();
            science_radar->setDistance(view_distance);
            // Keep the zoom slider in sync.
            zoom_slider->setValue(view_distance);
            zoom_label->setText(tr("Zoom: {zoom}x").format({{"zoom", string(my_spaceship->getLongRangeRadarRange() / view_distance, 1)}}));
        }
    }
}
