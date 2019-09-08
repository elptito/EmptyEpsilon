#include "databaseHeliosScreen.h"
#include "screenComponents/customShipFunctions.h"
#include "gui/gui2_element.h"
#include "scienceDatabase.h"
#include "gui/gui2_listbox.h"
#include "gui/gui2_autolayout.h"
#include "gui/gui2_keyvaluedisplay.h"
#include "gui/gui2_scrolltext.h"
#include "gui/gui2_image.h"
#include "screenComponents/rotatingModelView.h"
#include "screenComponents/passwordEntry.h"

#define MARGIN 20
DatabaseHeliosScreen::DatabaseHeliosScreen(GuiContainer* owner, bool linkedToScience)
: GuiOverlay(owner, "DATABASE_SCREEN", colorConfig.background), linkedToScience(linkedToScience)
{
    database_entry = nullptr;

    item_list = new GuiListbox(this, "DATABASE_ITEM_LIST", [this](int index, string value) {
        P<ScienceDatabase> entry;
        if (selected_entry) {
            if (index == 0) {
                selected_entry = selected_entry->parent;
                fillListBox();
            } else {
                entry = selected_entry->items[index - 1];
            }
        } else {
            entry = ScienceDatabase::science_databases[index];
        }
        if (entry && entry->password.length() > 0){
            item_list->setSelectionIndex(-1);
            password->challange("Enter code to read " + entry->getName(), entry->password, [this, entry, index](){
                item_list->setSelectionIndex(index);
                display(entry);
            });
        } else {
            display(entry);
        }
    });
    item_list->setPosition(0, 0, ATopLeft)->setMargins(20, 20, 20, 130)->setSize(400, GuiElement::GuiSizeMax);
    fillListBox();

    password = new PasswordEntry(this);
    (new GuiCustomShipFunctions(this, databaseView, "", my_spaceship))->setPosition(-20, 120, ATopRight)->setSize(250, GuiElement::GuiSizeMax);
}

void DatabaseHeliosScreen::onDraw(sf::RenderTarget& window){
    if (linkedToScience 
        && my_spaceship 
        && my_spaceship->science_query_to_bridge_db != ""
        && (!displayed_entry || displayed_entry->getName() != my_spaceship->science_query_to_bridge_db)){
        my_spaceship->commandSendScienceQueryToBridgeDB("");
        findAndDisplayEntry(my_spaceship->science_query_to_bridge_db);
    }
}

bool DatabaseHeliosScreen::findAndDisplayEntry(string name)
{
    foreach(ScienceDatabase, sd, ScienceDatabase::science_databases)
    {
        if (findAndDisplayEntry(name, sd))
            return true;
    }
    return false;
}

bool DatabaseHeliosScreen::findAndDisplayEntry(string name, P<ScienceDatabase> parent)
{
    foreach(ScienceDatabase, sd, parent->items)
    {
        if (sd->getName() == name)
        {
            if (sd->password.length() > 0){
                password->challange("Enter code to read " + sd->getName(), sd->password, [this, parent, sd, name](){
                    selected_entry = parent;
                    fillListBox();
                    display(sd);
                    item_list->setSelectionIndex(item_list->indexByValue(name));
                });
            } else {
                selected_entry = parent;
                fillListBox();
                display(sd);
                item_list->setSelectionIndex(item_list->indexByValue(name));
            }
            return true;
        }
        if (findAndDisplayEntry(name, sd))
            return true;
    }
    return false;
}

void DatabaseHeliosScreen::fillListBox()
{
    item_list->setOptions({});
    item_list->setSelectionIndex(-1);
    if (selected_entry) {
        item_list->addEntry("Back", "");
        foreach(ScienceDatabase, sd, selected_entry->items) {
            item_list->addEntry(sd->getName(), sd->getName());
        }
    } else {
        foreach(ScienceDatabase, sd, ScienceDatabase::science_databases) {
            item_list->addEntry(sd->getName(), sd->getName());
        }
    }
}

void DatabaseHeliosScreen::display(P<ScienceDatabase> entry)
{
    displayed_entry = entry;
    if (database_entry)
        database_entry->destroy();
    
    database_entry = new GuiAutoLayout(this, "DATABASE_ENTRY", GuiAutoLayout::LayoutVerticalColumns);
    database_entry->setPosition(400, 20, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    
    if (!entry)
        return;

    if (entry->keyValuePairs.size() > 0) {
        GuiAutoLayout* vLayout = new GuiAutoLayout(database_entry, "DATABASE_ENTRY_V_LAYOUT", GuiAutoLayout::LayoutVerticalTopToBottom);
        vLayout->setMargins(MARGIN);

        for(unsigned int n=0; n<entry->keyValuePairs.size(); n++) {
            (new GuiKeyValueDisplay(vLayout, "", 0.37, entry->keyValuePairs[n].key, entry->keyValuePairs[n].value))->setSize(GuiElement::GuiSizeMax, 40);
        }
    }
    if (entry->model_data || entry->image_name.length() > 0 || entry->longDescription.length() > 0){
        GuiAutoLayout* hLayout = new GuiAutoLayout(database_entry, "DATABASE_ENTRY_H_LAYOUT", GuiAutoLayout::LayoutHorizontalRows);
        if (entry->image_name.length() > 0) {
            (new GuiImage(hLayout, "DATABASE_IMAGE", entry->image_name));
        }
        if (entry->model_data) {
            (new GuiRotatingModelView(hLayout, "DATABASE_MODEL_VIEW", entry->model_data))->setMargins(MARGIN);
        }
        if (entry->longDescription.length() > 0) {
            (new GuiScrollText(hLayout, "DATABASE_LONG_DESCRIPTION", entry->longDescription))->setTextSize(24)->setMargins(MARGIN);
        }
    }
    if (entry->items.size() > 0) {
        selected_entry = entry;
        fillListBox();
    }
}
