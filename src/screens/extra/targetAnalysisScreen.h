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

class TargetAnalysisScreen : public GuiOverlay
{
  public:
    GuiAutoLayout* basic_info_layout;
    GuiAutoLayout* advanced_info_layout;
    GuiAutoLayout* texts_panel;
    GuiAutoLayout* description_panel;
    GuiAutoLayout* informations_panel;
    GuiAutoLayout* frequency_panel;
    GuiAutoLayout* others_panel;
    GuiAutoLayout* signals_panel;
    GuiAutoLayout* systems_panel;
    
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

    GuiRotatingModelView* model;

  public:
    TargetAnalysisScreen(GuiContainer *owner);
    void onDraw(sf::RenderTarget &window) override;
};
#endif //TARGET_ANALYSIS_SCREEN_H
