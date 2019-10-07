#include "passwordEntry.h"

#include "gui/gui2_panel.h"
#include "gui/gui2_label.h"
#include "gui/gui2_selector.h"
#include "gui/gui2_slider.h"
#include "gui/gui2_textentry.h"
#include "gui/gui2_togglebutton.h"

PasswordEntry::PasswordEntry(GuiContainer *owner) 
    : GuiOverlay(owner, "PASSWORD_OVERLAY", sf::Color::Black - sf::Color(0, 0, 0, 192))
{
    password_entry_box = new GuiPanel(this, "PASSWORD_ENTRY_BOX");
    password_entry_box->setPosition(0, 350, ATopCenter)->setSize(600, 200);
    password_label = new GuiLabel(password_entry_box, "PASSWORD_LABEL", prompt, 30);
    password_label->setPosition(0, 40, ATopCenter);
    password_entry = new GuiTextEntry(password_entry_box, "PASSWORD_ENTRY", "");
    password_entry->setPosition(20, 0, ACenterLeft)->setSize(400, 50);
    password_cancel = new GuiButton(password_entry_box, "PASSWORD_CANCEL_BUTTON", "Cancel", [this]() {
        // Reset the dialog.
        password_label->setText(prompt);
        password_entry->setText("");
        this->hide();
    });
    password_cancel->setPosition(0, -20, ABottomCenter)->setSize(300, 50);

    // Control code entry button.
    password_entry_ok = new GuiButton(password_entry_box, "PASSWORD_ENTRY_OK", "Ok", [this]() {
        // Get the password.
        string password = password_entry->getText();

        if (password != control_code){
            // Notify the player.
            password_label->setText("Incorrect control code. " + prompt);
            // Reset the dialog.
            password_entry->setText("");
        } else {
            // Notify the player.
            password_label->setText("Control code accepted.");
            // Reset and hide the password field.
            password_entry->setText("");
            password_entry->hide();
            password_cancel->hide();
            password_entry_ok->hide();
            // Show a confirmation button.
            password_confirmation->show();
        }
    });
    password_entry_ok->setPosition(420, 0, ACenterLeft)->setSize(160, 50);

    // Control code confirmation button
    password_confirmation = new GuiButton(password_entry_box, "PASSWORD_CONFIRMATION_BUTTON", "OK", [this]() {
        // Reset the dialog.
        password_entry->show();
        password_cancel->show();
        password_entry_ok->show();
        password_label->setText(prompt)->setPosition(0, 40, ATopCenter);
        password_confirmation->hide();
        // Hide the dialog.
        this->hide();
        if (onSuccess)
            onSuccess();
    });
    password_confirmation->setPosition(0, -20, ABottomCenter)->setSize(250, 50)->hide();
    this->setBlocking(true);
    this->hide();
}

void PasswordEntry::challange(string prompt, string control_code, func_t onSuccess){
    this->prompt = prompt;
    password_label->setText(prompt);
    this->control_code = control_code.lower();
    this->onSuccess = onSuccess;
    this->show();
    this->moveToFront();
    this->password_entry->setFocus();
}