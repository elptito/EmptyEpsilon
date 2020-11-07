#ifndef DRONE_MASTER_SCREEN_H
#define DRONE_MASTER_SCREEN_H

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
class GuiListbox;
class GuiRotatingModelView;
class GuiOverlay;
class GuiElement;
class Dock;
class DroneMasterScreen : public GuiOverlay
{
  private:

    GuiListbox* docks;
    int index = 0;
    GuiLabel *dockTitle;
    GuiLabel *droneTitle;

    GuiAutoLayout *mainPanel;
    GuiAutoLayout *topPanel;
    GuiAutoLayout *bottomPanel;
    GuiAutoLayout *dronePanel;

    GuiAutoLayout* action_move;
    GuiAutoLayout* action_launch;
    GuiAutoLayout* action_energy;
    GuiAutoLayout* action_weapons;

    GuiAutoLayout* cargoInfo;
    GuiAutoLayout* shipCargoInfo;
    std::vector<GuiKeyValueDisplay*> cargoInfoItems;
    std::vector<GuiKeyValueDisplay*> shipCargoInfoItems;
    GuiRotatingModelView* model;

    GuiElement* energyControl;
    GuiProgressbar *energy_bar;
    GuiSlider *energy_slider;

    GuiAutoLayout *table_weapons;
    GuiAutoLayout *weapons_layout_label;

    GuiAutoLayout *weapons_layout_p1;
    std::vector<GuiAutoLayout*> weapons_layout;
    std::vector<GuiLabel*> weapons_stock_ship;
    std::vector<GuiLabel*> weapons_stock_cargo;
    std::vector<GuiButton*> weapons_stock_p1;
    std::vector<GuiButton*> weapons_stock_m1;

    GuiAutoLayout* dronePanel_col1;
    GuiAutoLayout* dronePanel_col2;
    GuiElement* dronePanel_col3;

    GuiButton* action_launch_button;
    GuiButton* action_move_button;
    GuiElement* action_move_dest;

    GuiSelector* action_move_selector;
    GuiOverlay* overlay;
    GuiLabel *overlay_label;
    GuiProgressbar *distance_bar;
    GuiButton *cancel_move_button;

  public:
    DroneMasterScreen(GuiContainer *owner);

    void onDraw(sf::RenderTarget &window) override;
  private:
    void selectDock(int index);
  private:
    void displayDroneDetails(Dock &dockData);
};
#endif //DRONE_MASTER_SCREEN_H
