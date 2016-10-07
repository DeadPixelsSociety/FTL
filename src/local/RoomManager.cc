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

#include "RoomManager.h"

#include <cassert>

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#include "Crew.h"
#include "Messages.h"
#include "Params.h"
#include "Singletons.h"

RoomManager::RoomManager() {
    // Add all ship's rooms
    addRoom({04, 4}, {15, 3}, new Crew());
    addRoom({01, 1}, {15, 2});
    addRoom({12, 1}, {03, 2});
    addRoom({02, 2}, {01, 2});
    addRoom({01, 2}, {02, 4});
    addRoom({02, 2}, {01, 6});
    addRoom({01, 1}, {15, 7});
    addRoom({12, 1}, {03, 7});
    addRoom({02, 2}, {05, 4});
    addRoom({01, 1}, {06, 3});

    gMessageManager().registerHandler<LeftClicMouse>(&RoomManager::onLeftClicMouse, this);
}

void RoomManager::addRoom(gf::Vector2f size, gf::Vector2f position, Crew *crew) {
    m_rooms.push_back(Room(size, position, crew));
}

void RoomManager::update(float dt) {
    for(Room &room: m_rooms) {
        room.update(dt);
    }
}

void RoomManager::render(gf::RenderTarget &target) {
    for(Room &room: m_rooms) {
        room.render(target);
    }
}

gf::MessageStatus RoomManager::onLeftClicMouse(gf::Id type, gf::Message *msg){
    assert(type == LeftClicMouse::type);

    LeftClicMouse* leftClic = static_cast<LeftClicMouse*>(msg);
    gf::Log::debug(gf::Log::General, "Event receive\n");
    gf::Log::debug(gf::Log::General, "mouse coords = [%f, %f]\n", leftClic->position.x, leftClic->position.y);
    return gf::MessageStatus::Keep;
}