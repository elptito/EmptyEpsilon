#ifndef MISSILE_TUBE_CONTROLS_H
#define MISSILE_TUBE_CONTROLS_H

#include "gui/gui2_autolayout.h"
#include "missileWeaponData.h"
#include "missileAim.h"
#include "spaceObjects/playerSpaceship.h"

class GuiButton;
class GuiProgressbar;
class GuiLabel;
class GuiToggleButton;
class GuiPowerDamageIndicator;

class GuiMissileTubeControls : public GuiAutoLayout, public MissileAim
{
private:
    struct TubeRow {
        GuiAutoLayout* layout;
        GuiButton* load_button;
        GuiButton* fire_button;
        GuiProgressbar* loading_bar;
        GuiLabel* loading_label;
    };
    std::vector<TubeRow> rows;
    class TypeRow {
    public:
        GuiAutoLayout* layout;
        GuiToggleButton* button;
    };
    TypeRow load_type_rows[MW_Count];
    EMissileWeapons load_type;
    bool manual_aim;
    float missile_target_angle;
    P<PlayerSpaceship> target_spaceship;
    GuiPowerDamageIndicator* pdi;
public:
    GuiMissileTubeControls(GuiContainer* owner, string id, P<PlayerSpaceship> targetSpaceship);
    
    virtual void onDraw(sf::RenderTarget& window) override;
    virtual void onHotkey(const HotkeyResult& key) override;
    
    void setMissileTargetAngle(float angle);
    virtual float getMissileTargetAngle() override;
    
    void setManualAim(bool manual);
    virtual bool getManualAim() override;
    void setTargetSpaceship(P<PlayerSpaceship> targetSpaceship);

private:
    void selectMissileWeapon(EMissileWeapons type);
};

#endif//MISSILE_TUBE_CONTROLS_H
