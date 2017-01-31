// Faster Than Lithium is a opensource game which looks like Faster
// Than Light.
// Copyright (C) 2016 FTL's Team (see AUTHORS for more details)

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>

#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/Log.h>
#include <gf/RenderWindow.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "local/Messages.h"
#include "local/Params.h"
#include "local/HeadUpDisplay.h"
#include "local/Ship.h"
#include "local/Singletons.h"
#include "ResourceManager.h"

#include "config.h"

int main() {
    gf::Log::setLevel(gf::Log::Level::Debug);

    gf::Window window("Faster Than Lithium", { INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT });
    gf::RenderWindow renderer(window);

    gf::SingletonStorage<gf::MessageManager> storageForMessageManager(gMessageManager);
    gf::SingletonStorage<ResourceManager> storageForResourceManager(gResourceManager);
    gf::SingletonStorage<gf::Random> storageForRandom(gRandom);

    gResourceManager().addSearchDir(FTL_DATA_DIR);

    gf::ViewContainer views;

    gf::ExtendView shipView;
    shipView.setSize({ (float)GAME_WIDTH, (float)GAME_HEIGHT });
    shipView.setCenter({ GAME_WIDTH/2.0f, GAME_HEIGHT/2.0f });
    views.addView(shipView);

    views.onScreenResize({ INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT });

    gf::ActionContainer actions;

    gf::Action closeWindowAction("Close window");
    closeWindowAction.addCloseControl();
    closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
    actions.addAction(closeWindowAction);

    gf::Action muteMusic("Mute music");
    muteMusic.addKeycodeKeyControl(gf::Keycode::M);
    actions.addAction(muteMusic);

    gf::Action volumeUpMusic("Volume up music");
    volumeUpMusic.addKeycodeKeyControl(gf::Keycode::NumpadPlus);
    actions.addAction(volumeUpMusic);

    gf::Action volumeDownMusic("Volume down music");
    volumeDownMusic.addKeycodeKeyControl(gf::Keycode::NumpadMinus);
    actions.addAction(volumeDownMusic);

    gf::Action fullscreenAction("Fullscreen");
    fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
    actions.addAction(fullscreenAction);

    gf::Action resetAction("Reset");
    resetAction.addKeycodeKeyControl(gf::Keycode::R);
    actions.addAction(resetAction);

    gf::EntityContainer mainEntities;
    Ship ship;
    mainEntities.addEntity(ship);

    HeadUpDisplay hud;
    mainEntities.addEntity(hud);

    sf::Music* backgroundMusic = &gResourceManager().getMusic("music/ObservingTheStar-Redit.ogg");
    backgroundMusic->play();
    backgroundMusic->setLoop(true);

    gf::Clock clock;

    window.setFullscreen(true);
    while (window.isOpen()) {
        // input
        gf::Event event;

        while (window.pollEvent(event)) {
            actions.processEvent(event);
            views.processEvent(event);
            switch(event.type) {
                case gf::EventType::MouseButtonReleased:
                    if (event.mouseButton.button == gf::MouseButton::Left) {
                        LeftClicMouse message;
                        message.position = renderer.mapPixelToCoords(event.mouseButton.coords, shipView);
                        gMessageManager().sendMessage(&message);
                    }
                    else if (event.mouseButton.button == gf::MouseButton::Right) {
                        RightClicMouse message;
                        gMessageManager().sendMessage(&message);
                    }
                    break;

                default:
                    break;
                }
        }

        if (fullscreenAction.isActive()) {
            window.toggleFullscreen();
        }

        if (closeWindowAction.isActive()) {
            window.close();
        }

        if (muteMusic.isActive()) {
            backgroundMusic->getStatus() == sf::Music::Playing ? backgroundMusic->pause() : backgroundMusic->play();
        }

        if (volumeUpMusic.isActive()) {
            backgroundMusic->setVolume(backgroundMusic->getVolume() + 10.0f);
        }

        if (volumeDownMusic.isActive()) {
            backgroundMusic->setVolume(backgroundMusic->getVolume() - 10.0f);
        }

        if (resetAction.isActive()) {
            ResetGame message;
            gMessageManager().sendMessage(&message);
        }

        // update
        auto dt = clock.restart().asSeconds();
        if (!hud.isGameOver()) {
            mainEntities.update(dt);
        }

        // render
        renderer.clear();
        renderer.setView(shipView);
        if (!hud.isGameOver()) {
            mainEntities.render(renderer);
        } else {
            hud.render(renderer);
        }
        renderer.display();

        actions.reset();
    }

    return 0;
}
