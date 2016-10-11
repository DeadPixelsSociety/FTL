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

RoomManager::RoomManager()
: m_roomStartMove(nullptr){
    // Add all ship's rooms
    addRoom({04, 4}, {15, 3}, {100, 100}, "cockpit.png", new Crew());
    addRoom({13, 1}, {03, 2}, {0, 100}, "corridor_top.png");
    addRoom({02, 2}, {01, 2}, {100, 100}, "upper_engine.png");
    addRoom({01, 2}, {02, 4}, {100, 0}, "left_corridor.png", new Crew());
    addRoom({02, 2}, {01, 6}, {100, 100}, "lower_engine.png");
    addRoom({13, 1}, {03, 7}, {0, 100}, "corridor_bottom.png");
    addRoom({02, 2}, {05, 4}, {100, 100}, "oxygen_room.png");
    addRoom({01, 1}, {06, 3}, {100, 0}, "small_corridor_up.png");

    gMessageManager().registerHandler<LeftClicMouse>(&RoomManager::onLeftClicMouse, this);
    gMessageManager().registerHandler<RightClicMouse>(&RoomManager::onRightClicMouse, this);
}

void RoomManager::addRoom(gf::Vector2f size, gf::Vector2f position, gf::Vector2u origin, const gf::Path &path, Crew *crew) {
    m_rooms.push_back(Room(size, position, origin, path, crew));
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

    for(Room &room: m_rooms) {
        if (room.isHit(leftClic->position)) {
            // If is the first clic
            if (m_roomStartMove == nullptr && room.hasCrew()) {
                gf::Log::debug(gf::Log::General, "Room hited 1\n");
                m_roomStartMove = &room;
            }
            // Else we do the move
            else if (m_roomStartMove != nullptr && !room.hasCrew()) {
                gf::Log::debug(gf::Log::General, "Room hited 2\n");
                m_roomStartMove->crewMoveTo(room);
                m_roomStartMove = nullptr;
            }
        }
    }
    return gf::MessageStatus::Keep;
}

gf::MessageStatus RoomManager::onRightClicMouse(gf::Id type, gf::Message *msg){
    UNUSED(msg);
    assert(type == RightClicMouse::type);

    m_roomStartMove = nullptr;
    gf::Log::debug(gf::Log::General, "Room Released\n");

    return gf::MessageStatus::Keep;
}
