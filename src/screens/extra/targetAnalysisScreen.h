#ifndef TARGET_ANALYSIS_SCREEN_H
#define TARGET_ANALYSIS_SCREEN_H

#include "gui/gui2_overlay.h"
#include "spaceObjects/shipTemplateBasedObject.h"

class GuiPanel;
class GuiSlider;
class GuiButton;
class GuiLabel;
class GuiAutoLayout;
class GuiEntryList;
class GuiSelector;
class GuiProgressbar;
class GuiKeyValueDisplay;
class GuiScrollText;
class GuiListbox;
class GuiRotatingModelView;
class GuiSignalQualityIndicator;
class GuiFrequencyCurve;
class GuiOverlay;
class GuiElement;
class GuiImage;
class ScienceDatabase;

class TargetAnalysisScreen : public GuiOverlay
{
  public:

    GuiOverlay* indicator_overlay;
    GuiOverlay* analysis_overlay;
    
    GuiLabel* indicator_label;
    
    GuiKeyValueDisplay* info_callsign;
    GuiKeyValueDisplay* info_distance;
    GuiKeyValueDisplay* info_heading;
    GuiKeyValueDisplay* info_relspeed;

    GuiKeyValueDisplay* info_faction;
    GuiKeyValueDisplay* info_type;
    GuiKeyValueDisplay* info_shields;
    GuiKeyValueDisplay* info_hull;
    GuiScrollText* info_description;
    GuiSignalQualityIndicator* info_electrical_signal_band;
    GuiLabel* info_electrical_signal_label;
    GuiSignalQualityIndicator* info_gravity_signal_band;
    GuiLabel* info_gravity_signal_label;
    GuiSignalQualityIndicator* info_biological_signal_band;
    GuiLabel* info_biological_signal_label;
    GuiFrequencyCurve* info_shield_frequency;
    GuiFrequencyCurve* info_beam_frequency;
    GuiKeyValueDisplay* info_system[SYS_COUNT];
    GuiKeyValueDisplay* info_other[10];
    GuiKeyValueDisplay* info_template[30];

    GuiRotatingModelView* model;
    P<ScienceDatabase> selected_entry;

  public:
    TargetAnalysisScreen(GuiContainer *owner);
    void onDraw(sf::RenderTarget &window) override;
    P<ScienceDatabase> findDatabaseEntry(string name);
};
#endif //TARGET_ANALYSIS_SCREEN_H
