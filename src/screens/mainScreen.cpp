#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "mainScreen.h"
#include "main.h"
#include "epsilonServer.h"
#include "preferenceManager.h"

#include "screenComponents/indicatorOverlays.h"
#include "screenComponents/selfDestructIndicator.h"
#include "screenComponents/globalMessage.h"
#include "screenComponents/jumpIndicator.h"
#include "screenComponents/commsOverlay.h"
#include "screenComponents/viewport3d.h"
#include "screenComponents/radarView.h"
#include "screenComponents/shipDestroyedPopup.h"
#include "screens/extra/damcon.h"
#include "screens/extra/probeScreen.h"

#include "gui/gui2_panel.h"
#include "gui/gui2_overlay.h"
#include "gui/gui2_panel.h"
#include "gui/gui2_label.h"

ScreenMainScreen::ScreenMainScreen()
{
    new GuiOverlay(this, "", sf::Color::Black);

    viewport = new GuiViewport3D(this, "VIEWPORT");
    //viewport->showCallsigns()->showHeadings()->showSpacedust();
    viewport->showSpacedust();
    viewport->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);

    //TODO : voir pourquoi c'est commente
    //(new GuiRadarView(viewport, "VIEWPORT_RADAR", my_spaceship->getShortRangeRadarRange(), nullptr, my_spaceship))->setStyle(GuiRadarView::CircularMasked)->setSize(200, 200)->setPosition(-20, 20, ATopRight);
    
    tactical_radar = new GuiRadarView(this, "TACTICAL", nullptr, my_spaceship);
    tactical_radar->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    tactical_radar->setRangeIndicatorStepSize(1000.0f)->shortRange()->enableCallsigns()->hide();
    if (my_spaceship)
    {
        long_range_radar = new GuiRadarView(this, "TACTICAL", nullptr, my_spaceship);
        long_range_radar->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
        long_range_radar->setRangeIndicatorStepSize(5000.0f)->longRange()->enableCallsigns()->hide();
        long_range_radar->setFogOfWarStyle(GuiRadarView::NebulaFogOfWar);
    }
    global_range_radar = new GuiRadarView(this, "GLOBAL", 50000.0f, nullptr, my_spaceship);
    global_range_radar->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    global_range_radar->setAutoCentering(true);
    global_range_radar->longRange()->enableWaypoints()->enableCallsigns()->setStyle(GuiRadarView::Rectangular)->setFogOfWarStyle(GuiRadarView::FriendlysShortRangeFogOfWar);
    global_range_radar->hide();
    ship_state = new DamageControlScreen(this);
    ship_state->setPosition(0, 0, ATopLeft)->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
    ship_state->hide();

    onscreen_comms = new GuiCommsOverlay(this);
    onscreen_comms->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax)->setVisible(false);

    dock_comms = new GuiPanel(this,"DOCK_BOX");
    dock_comms->setSize(800, 100)->setPosition(0, 250, ATopCenter);
    label_dock = new GuiLabel(dock_comms, "DOCK_LABEL", "PROCEDURE DE DOCK", 40);
    label_dock->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax)->setPosition(0, 0, ACenter);
    dock_comms -> hide();

    new GuiShipDestroyedPopup(this);

    new GuiJumpIndicator(this);
    new GuiSelfDestructIndicator(this);
    new GuiGlobalMessage(this);
    new GuiIndicatorOverlays(this);

    keyboard_help = new GuiHelpOverlay(this, "Keyboard Shortcuts");

    for (std::pair<string, string> shortcut : hotkeys.listHotkeysByCategory("Main Screen"))
        keyboard_general += shortcut.second + ":\t" + shortcut.first + "\n";

    keyboard_help->setText(keyboard_general);

    if (PreferencesManager::get("music_enabled") != "0")
    {
        threat_estimate = new ThreatLevelEstimate();
        threat_estimate->setCallbacks([](){
            LOG(INFO) << "Switching to ambient music";
            soundManager->playMusicSet(findResources("music/ambient/*.ogg"));
        }, []() {
            LOG(INFO) << "Switching to combat music";
            soundManager->playMusicSet(findResources("music/combat/*.ogg"));
        });
    }

    rotatetime = 0.0007;
    angle = 0.0f;
    first_person = PreferencesManager::get("first_person") == "1";
}

void ScreenMainScreen::update(float delta)
{
    if (game_client && game_client->getStatus() == GameClient::Disconnected)
    {
        soundManager->stopMusic();
        soundManager->stopSound(impulse_sound);
        soundManager->stopSound(warp_sound);
        destroy();
        disconnectFromServer();
        returnToMainMenu();
        return;
    }

    if (my_spaceship)
    {
        if (my_spaceship->id_dock != PreferencesManager::get("id_dock"))
        {
            destroy();
            soundManager->stopMusic();
            soundManager->stopSound(impulse_sound);
            soundManager->stopSound(warp_sound);
            disconnectFromServer();
            returnToMainMenu();
            return;
        }
        P<SpaceObject> target_ship = my_spaceship->getTarget();
        float target_camera_yaw = my_spaceship->getRotation();
        switch(my_spaceship->main_screen_setting)
        {
        case MSS_Back: target_camera_yaw += 180; break;
        case MSS_Left: target_camera_yaw -= 90; break;
        case MSS_Right: target_camera_yaw += 90; break;
        case MSS_Target:
            if (target_ship)
            {
                sf::Vector2f target_camera_diff = my_spaceship->getPosition() - target_ship->getPosition();
                target_camera_yaw = sf::vector2ToAngle(target_camera_diff) + 180;
            }
            break;
        default: break;
        }
        camera_pitch = 30.0f;

//      Probe view
        if (game_server)
                probe = game_server->getObjectById(my_spaceship->linked_probe_3D_id);
            else
                probe = game_client->getObjectById(my_spaceship->linked_probe_3D_id);

        if (my_spaceship->main_screen_setting == MSS_ProbeView && probe){
            rotatetime -= delta;
            if (rotatetime <= 0.0)
            {
//                rotatetime = 0.0007;
                rotatetime = 0.1;
                angle += 0.1f;
//                angle += 5.0f;
            }

            camera_yaw = angle;
            camera_pitch = 0.0f;

            sf::Vector2f position = probe->getPosition() + sf::rotateVector(sf::Vector2f(probe->getRadius(), 0), camera_yaw);
            camera_position.x = position.x;
            camera_position.y = position.y;
            camera_position.z = 0.0;
        }else{
            float camera_ship_distance = 420.0f;
    //        float camera_ship_height = 420.0f;
            float camera_ship_height = my_spaceship->getRadius();
            if (first_person)
            {
                camera_ship_distance = -my_spaceship->getRadius();
//                camera_ship_height = my_spaceship->getRadius() / 10.f;
                camera_ship_height = my_spaceship->getTranslateZ();
                camera_pitch = 0;
            }
            sf::Vector2f cameraPosition2D = my_spaceship->getPosition() + sf::vector2FromAngle(target_camera_yaw) * -camera_ship_distance;
            sf::Vector3f targetCameraPosition(cameraPosition2D.x, cameraPosition2D.y, camera_ship_height);
            if (first_person)
            {
                camera_position = targetCameraPosition;
                camera_yaw = target_camera_yaw;
            }
            else
            {
                camera_position = camera_position * 0.9f + targetCameraPosition * 0.1f;
                camera_yaw += sf::angleDifference(camera_yaw, target_camera_yaw) * 0.1f;
            }

        }

#ifdef DEBUG
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
//        {
//            targetCameraPosition.x = my_spaceship->getPosition().x;
//            targetCameraPosition.y = my_spaceship->getPosition().y;
//            targetCameraPosition.z = 3000.0;
//            camera_pitch = 90.0f;
//        }
#endif

        switch(my_spaceship->main_screen_setting)
        {
        case MSS_Front:
        case MSS_Back:
        case MSS_Left:
        case MSS_Right:
        case MSS_Target:
            viewport->show();
            tactical_radar->hide();
            long_range_radar->hide();
            global_range_radar->hide();
            ship_state->hide();
            break;
        case MSS_Tactical:
            viewport->hide();
            tactical_radar->show();
            long_range_radar->hide();
            global_range_radar->hide();
            ship_state->hide();
            break;
        case MSS_LongRange:
            viewport->hide();
            tactical_radar->hide();
            long_range_radar->show();
            global_range_radar->hide();
            ship_state->hide();
            break;
        case MSS_GlobalRange:
            viewport->hide();
            tactical_radar->hide();
            long_range_radar->hide();
            global_range_radar->show();
            ship_state->hide();
            break;
        case MSS_ShipState:
            viewport->hide();
            tactical_radar->hide();
            long_range_radar->hide();
            global_range_radar->hide();
            ship_state->show();
            break;
        }

        // if (my_spaceship->docking_state != DS_NotDocking)
        // {
            // viewport->hide();
            // tactical_radar->hide();
            // long_range_radar->hide();
            // global_range_radar->hide();
            // ship_state->show();
            // dock_comms->show();
        // }else{
            // dock_comms->hide();
        // }

        switch(my_spaceship->main_screen_overlay)
        {
        case MSO_ShowComms:
            onscreen_comms->clearElements();
            onscreen_comms->show();
            viewport->hide();
            break;
        case MSO_HideComms:
            onscreen_comms->clearElements();
            onscreen_comms->hide();
            viewport->show();
            break;
        }

        // If we have an impulse power, loop the engine sound.
        float impulse_ability = std::max(0.0f, std::min(my_spaceship->getSystemEffectiveness(SYS_Impulse), my_spaceship->getSystemPower(SYS_Impulse)));
        string impulse_sound_file = my_spaceship->impulse_sound_file;
        if (impulse_ability > 0 && impulse_sound_file.length() > 0)
        {
            if (impulse_sound > -1)
            {
                soundManager->setSoundVolume(impulse_sound, std::max(10.0f * impulse_ability, fabsf(my_spaceship->current_impulse) * 30.0f * 100.0f * std::max(0.1f, impulse_ability)));
                soundManager->setSoundPitch(impulse_sound, std::max(0.7f * impulse_ability, fabsf(my_spaceship->current_impulse) + 0.2f * std::max(0.1f, impulse_ability)));
            }
            else
            {
                impulse_sound = soundManager->playSound(impulse_sound_file, std::max(0.7f * impulse_ability, fabsf(my_spaceship->current_impulse) + 0.2f * impulse_ability), std::max(30.0f, fabsf(my_spaceship->current_impulse) * 100.0f * impulse_ability), true);
            }
        } else if (impulse_sound > -1) 
        {
            // If we don't have impulse available, stop the engine sound.
            soundManager->stopSound(impulse_sound);
            impulse_sound = -1;
            // TODO: Play an engine failure sound.
        }

        // If we have warp power, loop the warp engine sound.
        float warp_ability = std::max(0.0f, std::min(my_spaceship->getSystemEffectiveness(SYS_Warp), my_spaceship->getSystemPower(SYS_Warp)));
        string warp_sound_file = my_spaceship->warp_sound_file;
        if (warp_ability > 0 && warp_sound_file.length() > 0 && my_spaceship->current_warp > 0)
        {
            if (warp_sound > -1)
            {
                soundManager->setSoundVolume(warp_sound, std::max(0.0f * warp_ability, fabsf(my_spaceship->current_warp) * 10.0f * 100.0f * std::max(0.1f, warp_ability)));
                soundManager->setSoundPitch(warp_sound, std::max(0.7f * warp_ability, fabsf(my_spaceship->current_warp) + 0.2f * std::max(0.1f, warp_ability)));
            }
            else
            {
                warp_sound = soundManager->playSound(warp_sound_file, std::max(0.7f * warp_ability, fabsf(my_spaceship->current_warp) + 0.2f * warp_ability), std::max(30.0f, fabsf(my_spaceship->current_warp) * 100.0f * warp_ability), true);
            }
        }
        // If we don't have impulse available, stop the engine sound.
        else if (warp_sound > -1 || my_spaceship->current_warp == 0)
        {
            soundManager->stopSound(warp_sound);
            warp_sound = -1;
            // TODO: Play an engine failure sound.
        }

    } else 
    {
        // If we don't have ship available, stop engine sounds.
        soundManager->stopSound(impulse_sound);
        impulse_sound = -1;
        soundManager->stopSound(warp_sound);
        warp_sound = -1;
    }
}

void ScreenMainScreen::onClick(sf::Vector2f mouse_position)
{
    if (!my_spaceship)
        return;

    if (InputHandler::mouseIsPressed(sf::Mouse::Left))
    {
        switch(my_spaceship->main_screen_setting)
        {
        case MSS_Front: my_spaceship->commandMainScreenSetting(MSS_Left); break;
        case MSS_Left: my_spaceship->commandMainScreenSetting(MSS_Back); break;
        case MSS_Back: my_spaceship->commandMainScreenSetting(MSS_Right); break;
        case MSS_Right: my_spaceship->commandMainScreenSetting(MSS_Front); break;
        default: my_spaceship->commandMainScreenSetting(MSS_Front); break;
        }
    }
    if (InputHandler::mouseIsPressed(sf::Mouse::Right))
    {
        switch(my_spaceship->main_screen_setting)
        {
        case MSS_Front: my_spaceship->commandMainScreenSetting(MSS_Right); break;
        case MSS_Right: my_spaceship->commandMainScreenSetting(MSS_Back); break;
        case MSS_Back: my_spaceship->commandMainScreenSetting(MSS_Left); break;
        case MSS_Left: my_spaceship->commandMainScreenSetting(MSS_Front); break;
        default: my_spaceship->commandMainScreenSetting(MSS_Front); break;
        }
    }
    if (InputHandler::mouseIsPressed(sf::Mouse::Middle))
    {
        switch(my_spaceship->main_screen_setting)
        {
        case MSS_Tactical:
            if (gameGlobalInfo->allow_main_screen_long_range_radar)
                my_spaceship->commandMainScreenSetting(MSS_LongRange);
            else if (gameGlobalInfo->allow_main_screen_global_range_radar)
                my_spaceship->commandMainScreenSetting(MSS_GlobalRange);
            else if (gameGlobalInfo->allow_main_screen_ship_state)
                my_spaceship->commandMainScreenSetting(MSS_ShipState);
            break;
        case MSS_LongRange:
            if (gameGlobalInfo->allow_main_screen_global_range_radar)
                my_spaceship->commandMainScreenSetting(MSS_GlobalRange);
            else if (gameGlobalInfo->allow_main_screen_ship_state)
                my_spaceship->commandMainScreenSetting(MSS_ShipState);
            else if (gameGlobalInfo->allow_main_screen_tactical_radar)
                my_spaceship->commandMainScreenSetting(MSS_Tactical);
            break;
        case MSS_GlobalRange:
            if (gameGlobalInfo->allow_main_screen_ship_state)
                my_spaceship->commandMainScreenSetting(MSS_ShipState);
            else if (gameGlobalInfo->allow_main_screen_tactical_radar)
                my_spaceship->commandMainScreenSetting(MSS_Tactical);
            else if (gameGlobalInfo->allow_main_screen_long_range_radar)
                my_spaceship->commandMainScreenSetting(MSS_LongRange);
            break;
        case MSS_ShipState:
            if (gameGlobalInfo->allow_main_screen_tactical_radar)
                my_spaceship->commandMainScreenSetting(MSS_Tactical);
            else if (gameGlobalInfo->allow_main_screen_long_range_radar)
                my_spaceship->commandMainScreenSetting(MSS_LongRange);
            else if (gameGlobalInfo->allow_main_screen_global_range_radar)
                my_spaceship->commandMainScreenSetting(MSS_GlobalRange);
            break;
        default:
            if (gameGlobalInfo->allow_main_screen_tactical_radar)
                my_spaceship->commandMainScreenSetting(MSS_Tactical);
            else if (gameGlobalInfo->allow_main_screen_long_range_radar)
                my_spaceship->commandMainScreenSetting(MSS_LongRange);
            else if (gameGlobalInfo->allow_main_screen_global_range_radar)
                my_spaceship->commandMainScreenSetting(MSS_GlobalRange);
            else if (gameGlobalInfo->allow_main_screen_ship_state)
                my_spaceship->commandMainScreenSetting(MSS_ShipState);
            break;
        }
    }
}

void ScreenMainScreen::onHotkey(const HotkeyResult& key)
{
    if (key.category == "MAIN_SCREEN" && my_spaceship)
    {
        if (key.hotkey == "VIEW_FORWARD")
            my_spaceship->commandMainScreenSetting(MSS_Front);
        else if (key.hotkey == "VIEW_LEFT")
            my_spaceship->commandMainScreenSetting(MSS_Left);
        else if (key.hotkey == "VIEW_RIGHT")
            my_spaceship->commandMainScreenSetting(MSS_Right);
        else if (key.hotkey == "VIEW_BACK")
            my_spaceship->commandMainScreenSetting(MSS_Back);
        else if (key.hotkey == "VIEW_TARGET")
            my_spaceship->commandMainScreenSetting(MSS_Target);
        else if (key.hotkey == "TACTICAL_RADAR")
            my_spaceship->commandMainScreenSetting(MSS_Tactical);
        else if (key.hotkey == "LONG_RANGE_RADAR")
            my_spaceship->commandMainScreenSetting(MSS_LongRange);
        else if (key.hotkey == "FIRST_PERSON")
            first_person = !first_person;
    }
}

void ScreenMainScreen::onKey(sf::Event::KeyEvent key, int unicode)
{
    switch (key.code)
    {
    //TODO: This is more generic code and is duplicated.
    case sf::Keyboard::Escape:
    case sf::Keyboard::Home:
        soundManager->stopMusic();
        soundManager->stopSound(impulse_sound);
        soundManager->stopSound(warp_sound);
        destroy();
        returnToShipSelection();
        break;
    case sf::Keyboard::Slash:
    case sf::Keyboard::F1:
        // Toggle keyboard help.
        keyboard_help->frame->setVisible(!keyboard_help->frame->isVisible());
        break;
    case sf::Keyboard::P:
        if (game_server)
            engine->setGameSpeed(0.0);
        break;
    default:
        break;
    }
}
