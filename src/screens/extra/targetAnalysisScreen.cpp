#include "targetAnalysisScreen.h"

#include "playerInfo.h"
#include "shipTemplate.h"
#include "gameGlobalInfo.h"
#include "scienceDatabase.h"
#include "spaceObjects/shipTemplateBasedObject.h"
#include "spaceObjects/asteroid.h"
#include "spaceObjects/artifact.h"
#include "spaceObjects/planet.h"
#include "spaceObjects/playerSpaceship.h"

#include "screenComponents/customShipFunctions.h"
#include "screenComponents/tractorBeamControl.h"
#include "screenComponents/radarView.h"
#include "screenComponents/powerDamageIndicator.h"
#include "screenComponents/frequencyCurve.h"
#include "screenComponents/rotatingModelView.h"
#include "screenComponents/rawScannerDataRadarOverlay.h"
#include "screenComponents/signalQualityIndicator.h"

#include "gui/gui2_listbox.h"
#include "gui/gui2_autolayout.h"
#include "gui/gui2_element.h"
#include "gui/gui2_panel.h"
#include "gui/gui2_label.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_scrolltext.h"
#include "gui/gui2_slider.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_button.h"
#include "gui/gui2_selector.h"
#include "gui/gui2_image.h"

const int ROW_SIZE = 4;
const int ROW_HEIGHT = 200;
const int BOX_WIDTH = 290;
const int COLUMN_WIDTH = 500;

TargetAnalysisScreen::TargetAnalysisScreen(GuiContainer *owner)
    : GuiOverlay(owner, "DOCK_MASTER_SCREEN", colorConfig.background)
{

    selected_entry = nullptr;

    GuiOverlay *background_crosses = new GuiOverlay(this, "BACKGROUND_CROSSES", sf::Color::White);
    background_crosses->setTextureTiled("gui/BackgroundCrosses");
    
    indicator_overlay = new GuiOverlay(this, "", sf::Color(0, 0, 0, 128));
    (new GuiPanel(indicator_overlay, "PAUSE_BOX"))->setPosition(0, 0, ACenter)->setSize(600, 100);
    indicator_label = new GuiLabel(indicator_overlay, "PAUSE_LABEL", "Power cut", 70);
    indicator_label->setPosition(0, 0, ACenter)->setSize(500, 100);
    
    analysis_overlay = new GuiOverlay(this, "", sf::Color(0, 0, 0, 128));
    
    model = new GuiRotatingModelView(analysis_overlay, "MODEL_VIEW", nullptr);
    model->setSize(500, 500);
    model->setPosition(0, 0, ACenter);

    GuiAutoLayout* left_col = new GuiAutoLayout(analysis_overlay, "LEFT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    left_col->setPosition(25, 50, ATopLeft)->setSize(400, GuiElement::GuiSizeMax);
    left_col->setMargins(20, 20, 20, 20);
    
    GuiAutoLayout* center_col = new GuiAutoLayout(analysis_overlay, "CENTER_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    center_col->setPosition(0, 50, ATopCenter)->setSize(400, GuiElement::GuiSizeMax);
    center_col->setMargins(20, 20, 20, 20);

    GuiAutoLayout* right_col = new GuiAutoLayout(analysis_overlay, "RIGHT_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
    right_col->setPosition(-25, 50, ATopRight)->setSize(400, GuiElement::GuiSizeMax);
    right_col->setMargins(20, 20, 20, 20);
    
    (new GuiLabel(left_col, "TITLE", "Basic Informations", 30))
        ->addBackground()
        ->setAlignment(ACenter)
        ->setPosition(0, 0, ABottomCenter)
        ->setSize(GuiElement::GuiSizeMax, 50);
    
    info_callsign = new GuiKeyValueDisplay(left_col, "SCIENCE_CALLSIGN", 0.4, tr("Callsign"), "");
    info_callsign->setSize(GuiElement::GuiSizeMax, 30);
    info_distance = new GuiKeyValueDisplay(left_col, "SCIENCE_DISTANCE", 0.4, tr("science","Distance"), "");
    info_distance->setSize(GuiElement::GuiSizeMax, 30);
    info_heading = new GuiKeyValueDisplay(left_col, "SCIENCE_HEADING", 0.4, tr("Bearing"), "");
    info_heading->setSize(GuiElement::GuiSizeMax, 30);
    info_relspeed = new GuiKeyValueDisplay(left_col, "SCIENCE_REL_SPEED", 0.4, tr("Rel. Speed"), "");
    info_relspeed->setSize(GuiElement::GuiSizeMax, 30);
    info_faction = new GuiKeyValueDisplay(left_col, "SCIENCE_FACTION", 0.4, tr("Faction"), "");
    info_faction->setSize(GuiElement::GuiSizeMax, 30);
    info_type = new GuiKeyValueDisplay(left_col, "SCIENCE_TYPE", 0.4, tr("science","Type"), "");
    info_type->setSize(GuiElement::GuiSizeMax, 30);
    info_shields = new GuiKeyValueDisplay(left_col, "SCIENCE_SHIELDS", 0.4, tr("science", "Shields"), "");
    info_shields->setSize(GuiElement::GuiSizeMax, 30);
    info_hull = new GuiKeyValueDisplay(left_col, "SCIENCE_HULL", 0.4, tr("science", "Hull"), "");
    info_hull->setSize(GuiElement::GuiSizeMax, 30);

    (new GuiLabel(left_col, "TITLE", "Description", 30))
        ->addBackground()
        ->setAlignment(ACenter)
        ->setPosition(0, 0, ABottomCenter)
        ->setSize(GuiElement::GuiSizeMax, 50);
        
    info_description = new GuiScrollText(left_col, "SCIENCE_DESC", "");
    info_description->setTextSize(28)->setMargins(20, 20, 0, 0)->setSize(GuiElement::GuiSizeMax, 150);

    (new GuiLabel(left_col, "TITLE", "Additionnal Informations", 30))
        ->addBackground()
        ->setAlignment(ACenter)
        ->setPosition(0, 0, ABottomCenter)
        ->setSize(GuiElement::GuiSizeMax, 50);

    for(int n = 0; n < 10; n++)
    {
        info_other[n] = new GuiKeyValueDisplay(left_col, "SCIENCE_INFOS_" + string(n), 0.6, "", "");
        info_other[n]->setSize(GuiElement::GuiSizeMax, 30);
        info_other[n]->hide();
    }
    
    (new GuiLabel(center_col, "TITLE", "Template Informations", 30))
        ->addBackground()
        ->setAlignment(ACenter)
        ->setPosition(0, 0, ABottomCenter)
        ->setSize(GuiElement::GuiSizeMax, 50);

    for(int n = 0; n < 30; n++)
    {
        info_template[n] = new GuiKeyValueDisplay(center_col, "TARGET_TEMPLATE_" + string(n), 0.37, "-", "-");
        info_template[n]->setSize(GuiElement::GuiSizeMax, 30);
        info_template[n]->hide();
    }

    (new GuiLabel(center_col, "TITLE", "Systems", 30))
        ->addBackground()
        ->setAlignment(ACenter)
        ->setPosition(0, 0, ABottomCenter)
        ->setSize(GuiElement::GuiSizeMax, 50);

    for(int n = 0; n < SYS_COUNT; n++)
    {
        info_system[n] = new GuiKeyValueDisplay(center_col, "TARGET_SYSTEM_" + string(n), 0.75, getLocaleSystemName(ESystem(n)), "-");
        info_system[n]->setSize(GuiElement::GuiSizeMax, 30);
        info_system[n]->hide();
    }
    
    // Advanced scan data.
    (new GuiLabel(right_col, "TITLE", "Frequencies", 30))
        ->addBackground()
        ->setAlignment(ACenter)
        ->setPosition(0, 0, ABottomCenter)
        ->setSize(GuiElement::GuiSizeMax, 50);

    // Frequency
    info_shield_frequency = new GuiFrequencyCurve(right_col, "SCIENCE_SHIELD_FREQUENCY", false, true);
    info_shield_frequency->setSize(GuiElement::GuiSizeMax, 150);
    info_beam_frequency = new GuiFrequencyCurve(right_col, "SCIENCE_BEAM_FREQUENCY", true, false);
    info_beam_frequency->setSize(GuiElement::GuiSizeMax, 150);

    if (!gameGlobalInfo->use_beam_shield_frequencies)
    {
        info_shield_frequency->hide();
        info_beam_frequency->hide();
    }

    (new GuiLabel(right_col, "TITLE", "Signatures", 30))
        ->addBackground()
        ->setAlignment(ACenter)
        ->setPosition(0, 0, ABottomCenter)
        ->setSize(GuiElement::GuiSizeMax, 50);

    info_electrical_signal_band = new GuiSignalQualityIndicator(right_col, "ELECTRICAL_SIGNAL");
    info_electrical_signal_band->showGreen(false)->showBlue(false)->setSize(GuiElement::GuiSizeMax, 80);
    info_electrical_signal_label = new GuiLabel(info_electrical_signal_band, "", "Electrical", 30);
    info_electrical_signal_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    info_gravity_signal_band = new GuiSignalQualityIndicator(right_col, "GRAVITY_SIGNAL");
    info_gravity_signal_band->showRed(false)->showBlue(false)->setSize(GuiElement::GuiSizeMax, 80);
    info_gravity_signal_label = new GuiLabel(info_gravity_signal_band, "", "Gravitational", 30);
    info_gravity_signal_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    info_biological_signal_band = new GuiSignalQualityIndicator(right_col, "BIOLOGICAL_SIGNAL");
    info_biological_signal_band->showRed(false)->showGreen(false)->setSize(GuiElement::GuiSizeMax, 80);
    info_biological_signal_label = new GuiLabel(info_biological_signal_band, "", "Biological", 30);
    info_biological_signal_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
}

void TargetAnalysisScreen::onDraw(sf::RenderTarget &window)
{
    GuiOverlay::onDraw(window);
    if (my_spaceship)
    {
        P<SpaceObject> obj;
        if (game_server)
            obj = game_server->getObjectById(my_spaceship->linked_analysis_object_id);
        else
            obj = game_client->getObjectById(my_spaceship->linked_analysis_object_id);
        
        if (!obj)
        {
            indicator_label->setText("No object targeted");
            indicator_overlay->show();
            analysis_overlay->hide();
        }
        else if (obj->getScannedStateFor(my_spaceship) < SS_FullScan)
        {
            indicator_label->setText("Object not fully scanned");
            indicator_overlay->show();
            analysis_overlay->hide();
        }
        else
        {
            indicator_label->setText("Object not fully scanned");
            indicator_overlay->hide();
            analysis_overlay->show();
            
            P<SpaceShip> ship = obj;
            P<ShipTemplateBasedObject> shipTemplate = obj;
            P<SpaceStation> station = obj;
            P<Asteroid> asteroid = obj;
            P<Artifact> artifact = obj;
            P<Planet> planet = obj;
            
            string description = obj->getDescriptionFor(my_spaceship);
            info_description->setText(description);
            
            if (asteroid)
            {
                info_type->setValue(tr("Asteroid"));
                model->setModel(ModelData::getModel("Astroid_" + string(asteroid->model_number)));
            }
            
            if (artifact)
            {
                info_type->setValue(tr("Unknown"));
                model->setModel(ModelData::getModel(artifact->current_model_data_name));
            }
            
            if (planet)
            {
                info_type->setValue(tr("Planet"));
                model->setModel(ModelData::getModel(planet->getPlanetSurfaceTexture()));
            }
            
            if (shipTemplate)
            {
                selected_entry = findDatabaseEntry(shipTemplate->getTypeName());
                model->setModel(selected_entry->getModelData());

                for(int n = 0; n < 30; n++)
                    info_template[n]->hide();
                for(unsigned int n = 0; n < selected_entry->keyValuePairs.size(); n++)
                {
                    if (n > 30)
                        break;
                    if (selected_entry->keyValuePairs[n].key != "")
                    {
                        info_template[n]->show();
                        info_template[n]->setKey(selected_entry->keyValuePairs[n].key);
                        info_template[n]->setValue(selected_entry->keyValuePairs[n].value);
                    }
                }
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
            
            info_faction->setValue(factionInfo[obj->getFactionId()]->getLocaleName());
            info_hull->setValue(int(ceil(obj->hull)));
            if (shipTemplate)
            {
                info_hull->setValue(int(ceil(shipTemplate->getHull())));
                info_type->setValue(shipTemplate->getTypeName());
                info_shields->setValue(shipTemplate->getShieldDataString());
            }
            
            if (ship)
            {
                info_shield_frequency->setFrequency(ship->shield_frequency);
                info_beam_frequency->setFrequency(ship->beam_frequency);
            
                for(int n = 0; n < SYS_COUNT; n++)
                    info_system[n]->hide();
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
            
            for(int n = 0; n < 10; n++)
            {
                if (obj->infos_label[n] == "")
                    continue;
                info_other[n]->show();
                info_other[n]->setKey(obj->infos_label[n]);
                info_other[n]->setValue(obj->infos_value[n]);
            }
            
            // Calculate signal noise for unscanned objects more than SRRR away.
            float distance_variance = 0.0f;
            float signal = 0.0f;

            if (distance > my_spaceship->getShortRangeRadarRange() && !obj->isScannedBy(my_spaceship))
                distance_variance = (random(0.01f, (distance - my_spaceship->getShortRangeRadarRange())) / (my_spaceship->getLongRangeRadarRange() - my_spaceship->getShortRangeRadarRange())) / 10;
                
            RawRadarSignatureInfo info;

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
    }
}

P<ScienceDatabase> TargetAnalysisScreen::findDatabaseEntry(string name)
{
    for(auto sd : ScienceDatabase::science_databases)
    {
        if (!sd) continue;
        if (sd->getName() == name)
        {
            return sd;
        }
    }
    return nullptr;
}

