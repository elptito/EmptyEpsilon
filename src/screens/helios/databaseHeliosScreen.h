#ifndef DATABASE_HELIOS_SCREEN_H
#define DATABASE_HELIOS_SCREEN_H

#include "gui/gui2_overlay.h"
#include "shipTemplate.h"

class ScienceDatabase;
class GuiListbox;
class GuiElement;
class PasswordEntry;

class DatabaseHeliosScreen : public GuiOverlay
{
    P<ScienceDatabase> selected_entry;
    GuiListbox* item_list;
    GuiElement* database_entry;
    PasswordEntry* password;
    const bool linkedToScience;
    P<ScienceDatabase> displayed_entry;
public:
    DatabaseHeliosScreen(GuiContainer* owner, bool linkedToScience);
    virtual void onDraw(sf::RenderTarget& window) override;
    bool findAndDisplayEntry(string name);
private:

    bool findAndDisplayEntry(string name, P<ScienceDatabase> parent);
    //Fill the selection listbox with options from the selected_entry, or the main database list if selected_entry is nullptr
    void fillListBox();
    void display(P<ScienceDatabase> entry);

};

#endif//DATABASE_HELIOS_SCREEN_H
