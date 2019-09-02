#ifndef PASSWORD_ENTRY_H
#define PASSWORD_ENTRY_H

#include "gui/gui2_overlay.h"

class GuiLabel;
class GuiPanel;
class GuiTextEntry;
class GuiButton;
class PasswordEntry : public GuiOverlay
{
public:
    typedef std::function<void()> func_t;
private:
    GuiLabel* password_label;
    GuiPanel* password_entry_box;
    GuiTextEntry* password_entry;
    GuiButton* password_entry_ok;
    GuiButton* password_cancel;
    GuiButton* password_confirmation;

    string prompt;
    string control_code;
    func_t onSuccess;
public:
    PasswordEntry(GuiContainer* owner);

    void challange(string prompt, string control_code, func_t onSuccess);
};

#endif //PASSWORD_ENTRY_H