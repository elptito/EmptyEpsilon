#include "engine.h"
#include "mainMenus.h"
#include "main.h"
#include "preferenceManager.h"
#include "epsilonServer.h"
#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "spaceObjects/spaceship.h"
#include "mouseCalibrator.h"
#include "menus/serverCreationScreen.h"
#include "menus/optionsMenu.h"
#include "menus/tutorialMenu.h"
#include "menus/serverBrowseMenu.h"
#include "screens/gm/gameMasterScreen.h"
#include "screenComponents/rotatingModelView.h"

#include "gui/gui2_image.h"
#include "gui/gui2_label.h"
#include "gui/gui2_button.h"

class DebugAllModelView : public GuiCanvas
{
public:
    DebugAllModelView()
    {
        new GuiOverlay(this, "", colorConfig.background);
        (new GuiOverlay(this, "", sf::Color::White))->setTextureTiled("gui/BackgroundCrosses");

        std::vector<string> names = ModelData::getModelDataNames();
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.startswith("transport_"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.startswith("artifact"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.startswith("SensorBuoyMK"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.startswith("space_station_"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name == "ammo_box"; }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name == "shield_generator"; }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.endswith("Blue"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.endswith("Green"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.endswith("Grey"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.endswith("Red"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.endswith("White"); }), names.end());
        int col_count = sqrtf(names.size()) + 1;
        int row_count = ceil(names.size() / col_count) + 1;
        int x = 0;
        int y = 0;
        float w = 1600 / col_count;
        float h = 900 / row_count;
        for(string name : names)
        {
            (new GuiRotatingModelView(this, "", ModelData::getModel(name)))->setPosition(x * w, y * h, ATopLeft)->setSize(w, h);
            x++;
            if (x == col_count)
            {
                x = 0;
                y++;
            }
        }
    }
};

MainMenu::MainMenu()
{
    constexpr float logo_size = 256;
    constexpr float logo_size_y = 256;
    constexpr float logo_size_x = 1024;
    constexpr float title_y = 160;

    new GuiOverlay(this, "", colorConfig.background);
    (new GuiOverlay(this, "", sf::Color::White))->setTextureTiled("gui/BackgroundCrosses");

    (new GuiImage(this, "LOGO", "logo_full"))->setPosition(0, title_y, ATopCenter)->setSize(logo_size_x, logo_size_y);
    (new GuiLabel(this, "VERSION", "Version: " + string(VERSION_NUMBER), 20))->setPosition(0, title_y + logo_size, ATopCenter)->setSize(0, 20);

    (new GuiButton(this, "START_SERVER", "Lancer serveur", [this]() {
        new EpsilonServer();
        if (game_server)
        {
            new ServerCreationScreen();
            destroy();
        }
    }))->setPosition(sf::Vector2f(50, -230), ABottomLeft)->setSize(300, 50);

    (new GuiButton(this, "START_CLIENT", "Lancer client", [this]() {
        new ServerBrowserMenu(ServerBrowserMenu::Local);
        destroy();
    }))->setPosition(sf::Vector2f(50, -170), ABottomLeft)->setSize(300, 50);

    (new GuiButton(this, "OPEN_OPTIONS", "Options", [this]() {
        new OptionsMenu();
        destroy();
    }))->setPosition(sf::Vector2f(50, -110), ABottomLeft)->setSize(300, 50);

    (new GuiButton(this, "QUIT", "Quitter", [this]() {
        engine->shutdown();
    }))->setPosition(sf::Vector2f(50, -50), ABottomLeft)->setSize(300, 50);

    (new GuiButton(this, "START_TUTORIAL", "Tutorials", [this]() {
        new TutorialMenu();
        destroy();
    }))->setPosition(sf::Vector2f(370, -50), ABottomLeft)->setSize(300, 50);

    if (InputHandler::touch_screen)
    {
        (new GuiButton(this, "TOUCH_CALIB", "Calibrate\nTouchscreen", [this]() {
            destroy();
            new MouseCalibrator("");
        }))->setPosition(sf::Vector2f(-50, -50), ABottomRight)->setSize(300, 100);
    }

    float y = 25;
    (new GuiLabel(this, "CREDITS", "Credits", 25))->setAlignment(ACenterRight)->setPosition(-50, y, ATopRight)->setSize(0, 25); y += 25;
    (new GuiLabel(this, "CREDITS1", "Empty Epsilon", 20))->setAlignment(ACenterRight)->setPosition(-50, y, ATopRight)->setSize(0, 20); y += 20;

    if (PreferencesManager::get("instance_name") != "")
    {
        (new GuiLabel(this, "", PreferencesManager::get("instance_name"), 25))->setAlignment(ACenterLeft)->setPosition(20, 20, ATopLeft)->setSize(0, 18);
    }

#ifdef DEBUG
    (new GuiButton(this, "", "TO DA GM!", [this]() {
        new EpsilonServer();
        if (game_server)
        {
            gameGlobalInfo->startScenario("scenario_10_empty.lua");

            my_player_info->commandSetShipId(-1);
            destroy();
            new GameMasterScreen();
        }
    }))->setPosition(sf::Vector2f(370, -150), ABottomLeft)->setSize(300, 50);
    
    (new GuiButton(this, "", "MODELS!", [this]() {
        destroy();
        new DebugAllModelView();
    }))->setPosition(sf::Vector2f(370, -200), ABottomLeft)->setSize(300, 50);
#endif
}
