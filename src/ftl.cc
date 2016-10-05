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

#include "local/Params.h"
#include "local/RoomManager.h"
#include "local/Ship.h"

int main() {
    gf::Log::setLevel(gf::Log::Level::Debug);

    gf::Window window("Faster Than Lithium", { 720, 405 });
    gf::RenderWindow renderer(window);

    gf::ViewContainer views;

    gf::FitView mainView;
    gf::ExtendView shipView;
    shipView.setSize({ GAME_WIDTH, GAME_HEIGHT });
    shipView.setCenter({ GAME_WIDTH/2.0f, GAME_HEIGHT/2.0f });
    views.addView(shipView);

    gf::ActionContainer actions;

    gf::Action closeWindowAction("Close window");
    closeWindowAction.addCloseControl();
    closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
    actions.addAction(closeWindowAction);

    gf::EntityContainer mainEntities;
    Ship ship;
    mainEntities.addEntity(ship);

    RoomManager rooms;
    mainEntities.addEntity(rooms);

    gf::Clock clock;
    while (window.isOpen()) {
        // input
        gf::Event event;

        while (window.pollEvent(event)) {
            actions.update(event);
            views.update(event);
        }

        if (closeWindowAction.isActive()) {
            window.close();
        }

        // update
        auto dt = clock.restart().asSeconds();
        mainEntities.update(dt);

        // render
        renderer.clear();
        renderer.setView(shipView);
        mainEntities.render(renderer);
        renderer.display();

        actions.reset();
    }

    return 0;
}
