#include "actionItemOverlay.h"

#include "gameGlobalInfo.h"

#include "gui/gui2_panel.h"
#include "gui/gui2_label.h"
#include "gui/gui2_selector.h"
#include "gui/gui2_slider.h"
#include "gui/gui2_textentry.h"
#include "gui/gui2_togglebutton.h"
#include "gui/gui2_advancedscrolltext.h"

#include "GMActions.h"

ActionItemOverlay::ActionItemOverlay(GuiContainer *owner)
    : GuiOverlay(owner, "ACTION_ITEM_OVERLAY", sf::Color::Black - sf::Color(0, 0, 0, 192)), index(-1)
{
    GuiPanel* action_item_box = new GuiPanel(this, "ACTION_ITEM_BOX");
    action_item_box->setPosition(0, 100, ATopCenter)->setSize(800, 600);
    close_button = new GuiButton(action_item_box, "CLOSE_BUTTON", "X", [this]() {
        setIndex(-1);
    });    
    close_button->setPosition(0, 0, ATopRight)->setSize(40, 40);
    GuiElement* action_item_box_inner = new GuiElement(action_item_box, "ACTION_ITEM_BOX_INNER");
    action_item_box_inner->setPosition(0, 0, ACenter)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax)->setMargins(20);
    title_label = new GuiLabel(action_item_box_inner, "TITLE_LABEL", "", 30);
    title_label->setPosition(0, 40, ATopCenter)->setSize(GuiElement::GuiSizeMax, 40);
    query_label = new GuiLabel(action_item_box_inner, "QUERY_LABEL", "", 30);
    query_label->setPosition(0, 80, ATopCenter)->setSize(GuiElement::GuiSizeMax, 120);
    response_text = new GuiTextEntry(action_item_box_inner, "RESPONSE_TEXT", "");
    response_text->setPosition(0, 200, ATopCenter)->setSize(GuiElement::GuiSizeMax, 40);
    accept_button = new GuiButton(action_item_box_inner, "ACCEPT_BUTTON", "Accept", [this]() {
        gameMasterActions->commandResolveActionItem(this->index, true, response_text->getText());
        setIndex(-1);
    });
    accept_button->setPosition(0, 0, ABottomLeft)->setSize(150, 50);
    decline_button = new GuiButton(action_item_box_inner, "DECLINE_BUTTON", "Decline", [this]() {
        gameMasterActions->commandResolveActionItem(this->index, false, response_text->getText());
        setIndex(-1);
    });
    decline_button->setPosition(0, 0, ABottomRight)->setSize(150, 50);
    this->setBlocking(true);
    this->hide();
}

void ActionItemOverlay::setIndex(int index){
    this->index = index;
    // TODO move to onDraw:
    if (index == -1){
        title_label->setText("");
        query_label->setText("");
        response_text->setText("");
        this->hide();
    } else {
        P<ActionItem> ai = ActionItem::actionItems.at(index);
        if (ai){
            title_label->setText(ai->title);
            query_label->setText(ai->query);
            response_text->setText(ai->response);
            this->show();
        } else {
            setIndex(-1);
        }
    }
}