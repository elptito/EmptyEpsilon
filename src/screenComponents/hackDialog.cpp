#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "spaceObjects/spaceship.h"
#include "hackDialog.h"
#include "gui/gui2_panel.h"
#include "gui/gui2_progressbar.h"
#include "gui/gui2_button.h"
#include "gui/gui2_label.h"
#include "gui/gui2_scrolltext.h"
#include "gui/gui2_listbox.h"
#include "gui/gui2_textentry.h"
#include "gui/gui2_element.h"
#include "gui/gui2_togglebutton.h"

#include "onScreenKeyboard.h"

GuiHackDialog::GuiHackDialog(GuiContainer* owner, string id)
: GuiOverlay(owner, id, sf::Color(64,0,0,64))
{

    setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    hide();

    // Panel de titre
    hack_title = new GuiPanel(this, "HACK_TITLE_BOX");
    hack_title->setSize(600, 50)->setPosition(0, -720, ABottomCenter);

    hack_label = new GuiLabel(hack_title, "", "", 30);
    hack_label->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    hack_label->setText("Invite de commande");

    // Button to close chat comms.
    hack_close_button = new GuiButton(hack_title, "CLOSE_BUTTON", "Fin", [this]() {
        hack_step = 0;
        if (my_spaceship)
            my_spaceship -> hack_protect = false;
        hide();
    });
    hack_close_button->setTextSize(20)->setPosition(-10, 10, ATopRight)->setSize(60, 30);

    hack_minimize_button = new GuiToggleButton(hack_title, "", "_", [this](bool value)
    {
        minimize(value);
    });
    hack_minimize_button->setPosition(-70, 10, ATopRight)->setSize(60, 30);

    minimized = false;

    // Panel for chat communications with GMs and other player ships.
    hack_box = new GuiPanel(this, "HACK_CHAT_BOX");
    hack_box->setSize(800, 600)->setPosition(0, -120, ABottomCenter);

    // Message entry field for chat.
    hack_message_entry = new GuiTextEntry(hack_box, "HACK_CHAT_MESSAGE_ENTRY", "");
    hack_message_entry ->setPosition(20, -20, ABottomLeft)->setSize(640, 50);
    hack_message_entry ->enterCallback([this](string text){
        hack_delay = random(1,5);
        if (my_spaceship)
            my_spaceship->hack_time = 0.001;
    });

    // Progress bar
    progress = new GuiProgressbar(hack_box, "HACK_PROGRESS", 0, hack_delay, 0.0);
    progress->setPosition(20, 30, ATopLeft)->setSize(GuiElement::GuiSizeMax, 30);
    progress->setColor(colorConfig.log_receive_enemy)->setDrawBackground(false)->setText("Acces en cours");

    // Text of incoming chat messages.
    hack_comms_text = new GuiScrollText(hack_box, "HACK_CHAT_TEXT", "");
    hack_comms_text->enableAutoScrollDown()->setPosition(20, 50, ATopLeft)->setSize(760, 500);

    // Button to send a message.
    hack_send_button = new GuiButton(hack_box, "SEND_BUTTON", "Envoyer", [this]() {
        hack_delay = random(1,5);
        if (my_spaceship)
            my_spaceship->hack_time = 0.001;
    });
    hack_send_button->setPosition(-20, -20, ABottomRight)->setSize(120, 50);

    if (!engine->getObject("mouseRenderer")) //If we are a touch screen, add a on screen keyboard.
    {
        OnScreenKeyboardControl* keyboard = new OnScreenKeyboardControl(hack_box, hack_message_entry);
        keyboard->setPosition(20, -20, ABottomLeft)->setSize(760, 200);
        hack_message_entry ->setPosition(20, -220, ABottomLeft);
        hack_send_button->setPosition(-20, -220, ABottomRight);
        hack_comms_text->setSize(hack_comms_text->getSize().x, hack_comms_text->getSize().y - 200);
    }

}

void GuiHackDialog::minimize(bool minimize)
{
    hack_minimize_button->setValue(minimize);
    if (minimize != minimized)
    {
        if (minimize)
        {
            hack_box->hide();
            original_height = getSize().y;
            setSize(getSize().x, original_height);
        }else{
            hack_box->show();
            setSize(getSize().x, original_height);
        }
    }
    minimized = minimize;
}

void GuiHackDialog::open()
{
    //this->target = target;
    hack_step = 0;
    hack_message = "\n> Lancement de l'invite de commande";
    hack_message += "\n> Choisissez l'action a mener";
    hack_message += "\n> Intrusion ou protection";
    show();
}

void GuiHackDialog::onDraw(sf::RenderTarget& window)
{
//    if (!target)
//    {
//        hide();
//        return;
//    }
    GuiOverlay::onDraw(window);

    if (my_spaceship)
    {
        hack_time = my_spaceship->hack_time;

        if (my_spaceship->hack_time > hack_delay)
        {
            my_spaceship->hack_time = 0.0;
            commandHack();
        }
    }

    if (hack_target && hack_target -> hack_protect)
    {
        hack_message  = "\n> Intruision detectee par " + hack_target -> getCallSign();
        hack_message  = "\n> Exclusion du systeme";
        hack_step = 0;
    }

    if (hack_time > 0.0)
    {
        progress->setValue(hack_time);
        hack_message_entry->disable();
        hack_message_entry->hide();
        progress->show();
    }
    else
    {
        progress->setValue(0.0);
        hack_message_entry->enable();
        hack_message_entry->show();
        progress->hide();
    }

    // If we're on a ship, show comms activity on draw.
    if (my_spaceship)
    {
        hack_comms_text->setText(hack_message);
    }

    // Si fini
    if (hack_step == 9 && my_spaceship && hack_target)
    {
        hack_message  += "\n> Hack reussi vers " + hack_target -> getCallSign();
        hack_step = 99;

        if (target_system == "reacteur")
            my_spaceship->commandHackingFinished(hack_target, "Reacteur","");
        else if (target_system == "laser")
            my_spaceship->commandHackingFinished(hack_target, "Faisceau laser","");
        else if (target_system == "missile")
            my_spaceship->commandHackingFinished(hack_target, "Systeme de missiles","");
        else if (target_system == "manoeuvre")
            my_spaceship->commandHackingFinished(hack_target, "Manoeuvres","");
        else if (target_system == "impulsion")
            my_spaceship->commandHackingFinished(hack_target, "Moteur subluminique","");
        else if (target_system == "warp")
            my_spaceship->commandHackingFinished(hack_target, "Moteur WARP","");
        else if (target_system == "jump")
            my_spaceship->commandHackingFinished(hack_target, "Moteur JUMP","");
        else if (target_system == "boulierAv")
            my_spaceship->commandHackingFinished(hack_target, "Generateur Bouclier Av.","");
        else if (target_system == "boulierAr")
            my_spaceship->commandHackingFinished(hack_target, "Generateur Bouclier Ar.","");
        else
            hack_message  += "\n> Systeme cible inconnu";
    }
    // Si fini (federation)
    if (hack_step == 91 && my_spaceship && hack_target)
    {
        hack_step = 99;

        my_spaceship->commandHackingFinished(hack_target, "",target_system);

        P<PlayerSpaceship> player_target = hack_target;
        if (player_target)
        {
            player_target -> addToShipLog("Tentative de Piratage de la signature de faction",colorConfig.log_receive_enemy,"generic");
            player_target -> addToShipLog("Tentative de Piratage de la signature de faction",colorConfig.log_receive_enemy,"intern");
        }

        hack_message  += "\n> Hack reussi vers " + hack_target -> getCallSign();
        hack_message  += "\n> Tentative de transformation vers " + target_system;
    }
}

void GuiHackDialog::clearElements()
{
    // Force all panels to hide, in case hiding the overlay doesn't hide its
    // contents on draw.
    hack_box->hide();
}

void GuiHackDialog::commandHack()
{
    if (!my_spaceship)
        return;

    string hack_text = hack_message_entry -> getText();
    string hack_test = "";
    hack_text = hack_text.lower();
    hack_message_entry ->setText("");
//    if (target)
//    {
        // Connect to ship
        if (hack_step == 0)
        {
            if (hack_text == "intrusion")
            {
                hack_message  = "\n> Module d'intrusion lance";
                hack_message  = "\n> Intrusion possible dans un rayon de 50U";
                hack_message  += "\n> Indiquez l'identifiant de la cible : ";
                hack_step = 1;
            }
            else if (hack_text == "protection")
            {
                hack_message  = "\n> Module de protection lance";
                hack_message  += "\n> Confirmez le lancement de la protection : OUI";
                hack_step = 10;
            }
            else
            {
                hack_message = "\n> Lancement de l'invite de commande";
                hack_message += "\n> Choisissez l'action a mener";
                hack_message += "\n> Intrusion ou protection";
                hack_step = 0;
            }
        }
        // Connect to ship
        else if (hack_step == 1)
        {
            foreach(SpaceObject, obj, space_object_list)
            {
                P<SpaceShip> ship = obj;

                if (!ship)
                    continue;

                if (sf::length(obj->getPosition() - my_spaceship->getPosition()) > 50000.0)
                    continue;

                hack_test = obj -> getCallSign();
                hack_test = hack_test.lower();

                //if (hack_text.startswith(hack_test))
                if (hack_text == hack_test)
                {
                    hack_message  = "\n> Connexion vers " + obj -> getCallSign() + " reussie";

                    hack_target = obj;

                    hack_message += "\n> Code d'acces du systeme informatique :";
                    hack_message += "\n> (Le code est en " + string(hack_target -> getHackDiff()) + " bits)";

                    defineHackCode(hack_target -> getHackDiff());

                    P<PlayerSpaceship> player_target = obj;
                    if (player_target)
                        player_target -> addToShipLog("Intrusion informatique en cours",colorConfig.log_receive_enemy,"intern");
                    hack_step = 2;
                    break;
                }
            }
            if (hack_step != 2)
            {
                hack_message  += "\n> Aucune cible atteinte";
                hack_message  += "\n> Hack rate";
                hack_step = 99;
            }
        }
        // Give ship hack code
        else if (hack_step == 2)
        {
            if (hack_text == hack_code)
            {
                hack_message  = "\n> Acces complet a l'interface";
                hack_message  += "\n> Choisissez le systeme a hacker";
                hack_message  += "\n> reacteur | laser | missile | manoeuvre | impulsion | warp | jump | boulierAv | boulierAr";
                hack_step = 5;
            }
            else if (hack_text.length() != hack_target -> getHackDiff())
            {
                hack_message  += "\n> Nombre de bits incorrect";
            }
            else
            {
                hack_message  += "\n> Code " + hack_text + " incorrect";
                hack_message  += "\n> " + string(verifHackCode(hack_target -> getHackDiff(), hack_text, hack_code)) + "bits corrects";
            }
        }
        else if (hack_step == 5)
        {
            if (hack_text == "spay")
            {
                hack_message  = "\n> Acces au service S-PAY";
                hack_message += "\n> Indiquez le Code d'acces du S-PAY :";
                hack_message += "\n> (Le code est en 6 bits)";
                defineHackCode(6);
                hack_step = 6;
            }else if (hack_text == "signature")
            {
                hack_message  = "\n> Acces au service de perturbation de signature";
                hack_message += "\n> Indiquez le Code d'acces pour modifier la signature :";
                hack_message += "\n> (Le code est en 6 bits)";
                defineHackCode(6);
                hack_step = 7;
            }else
            {
                target_system = hack_text;
                hack_step = 9;
            }
        }
        else if (hack_step == 6)
        {
            if (hack_text == hack_code)
            {
                hack_message  = "\n> Acces complet au S-PAY";
                hack_message  += "\n> Retrait de 200 Q obtenu";
                hack_message  += "\n> Somme verse sur votre compte S-PAY";
                my_spaceship -> addToShipLog("Obtention de 200Q par hack du S-PAY",colorConfig.log_receive_enemy,"generic");
                P<PlayerSpaceship> player_target = hack_target;
                if (player_target)
                    player_target -> addToShipLog("Perte de 200Q par hack du S-PAY",colorConfig.log_receive_enemy,"intern");
                hack_step = 99;
            }
            else if (hack_text.length() != 6)
            {
                hack_message  += "\n> Nombre de bits incorrect";
            }
            else
            {
                hack_message  += "\n> Code " + hack_text + " incorrect";
                hack_message  += "\n> " + string(verifHackCode(6, hack_text, hack_code)) + "bits corrects";
            }
        }
        else if (hack_step == 7)
        {
            if (hack_text == hack_code)
            {
                hack_message  = "\n> Acces complet au piratage signature";
                hack_message  += "\n> quelle est la signature a attribuer";
                hack_message  += "\n> luxa | arroya | trente nebuleuses | gcg | gusm | systemes souverains | ganima";
                hack_step = 8;
            }
            else if (hack_text.length() != 6)
            {
                hack_message  += "\n> Nombre de bits incorrect";
            }
            else
            {
                hack_message  += "\n> Code " + hack_text + " incorrect";
                hack_message  += "\n> " + string(verifHackCode(6, hack_text, hack_code)) + " bits corrects";
            }
        }
        else if (hack_step == 8)
        {
            target_system = hack_text;
            hack_step = 91;
        }
        // Protection
        else if (hack_step == 10)
        {
            if (hack_text == "oui")
            {
                hack_message  = "\n> Protection active, les eventuels intrus ont ete ejectes.";
                my_spaceship -> hack_protect = true;
            }
            else
            {
                hack_message  += "\n> Systeme de protection abandonne";
                hack_step = 99;
                my_spaceship -> hack_protect = false;
            }
        }
        else if (hack_step == 99)
        {
            hack_message = "\n> Lancement de l'invite de commande";
            hack_message += "\n> Choisissez l'action a mener";
            hack_message += "\n> Intrusion ou protection";
            hack_step = 0;
        }
//    }
}

void GuiHackDialog::defineHackCode(int hackDiff)
{
    hack_code = "";
    for(int n=0; n<hackDiff; n++)
        hack_code += string(irandom(0, 1));
}

int GuiHackDialog::verifHackCode(int hackDiff, string hack_text, string hack_code)
{
    int n_correct = 0;
    for(int n=0; n<hackDiff; n++)
    {
        if (hack_text[n] == hack_code[n])
            n_correct += 1;
    }
    return n_correct;
}

