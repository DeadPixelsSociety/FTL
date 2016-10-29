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

#include "Ship.h"

#include <cassert>

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#include "Crew.h"
#include "Messages.h"
#include "Params.h"
#include "Singletons.h"

Ship::Ship()
: m_roomStartMove(nullptr), m_timeElapsed(0.0f) {
    gMessageManager().registerHandler<LeftClicMouse>(&Ship::onLeftClicMouse, this);
    gMessageManager().registerHandler<RightClicMouse>(&Ship::onRightClicMouse, this);
    gMessageManager().registerHandler<RoomFailure>(&Ship::onRoomFailure, this);
    gMessageManager().registerHandler<GameOver>(&Ship::onGameOver, this);
    generateLevel();
}

void Ship::addRoom(gf::Vector2f size, gf::Vector2f position, const gf::Path &path, Crew *crew) {
    m_rooms.push_back(Room(size, position, path, crew));
}

void Ship::update(float dt) {
    for(Room &room: m_rooms) {
        room.update(dt);
    }
    m_timeElapsed += dt;

    // If a failure must be launched
    if (m_timeElapsed >= COOLDOWN_FAILURE) {
      m_timeElapsed -= COOLDOWN_FAILURE;

      RoomFailure message;
      gMessageManager().sendMessage(&message);
    }
}

void Ship::render(gf::RenderTarget &target) {
    for(Room &room: m_rooms) {
        room.render(target);
    }
}

gf::MessageStatus Ship::onLeftClicMouse(gf::Id type, gf::Message *msg){
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

gf::MessageStatus Ship::onRightClicMouse(gf::Id type, gf::Message *msg){
    UNUSED(msg);
    assert(type == RightClicMouse::type);

    m_roomStartMove = nullptr;
    gf::Log::debug(gf::Log::General, "Room Released\n");

    return gf::MessageStatus::Keep;
}

gf::MessageStatus Ship::onRoomFailure(gf::Id type, gf::Message *msg){
    UNUSED(msg);
    assert(type == RoomFailure::type);

    // Check if a room is ok
    bool isOk = false;
    for (Room &room: m_rooms) {
        if (!room.isFailure() && !room.hasCrew()) {
            isOk = true;
            break;
        }
    }
    if (!isOk) {
        return gf::MessageStatus::Keep;
    }

    // Select random room
    unsigned random = gRandom().computeUniformInteger<unsigned>(0, m_rooms.size() - 1);
    while (m_rooms[random].hasCrew() || m_rooms[random].isFailure()) {
        random = gRandom().computeUniformInteger<unsigned>(0, m_rooms.size() - 1);
    }

    gf::Log::debug(gf::Log::General, "Room failure %d\n", random);
    m_rooms[random].failure();

    return gf::MessageStatus::Keep;
}

gf::MessageStatus Ship::onGameOver(gf::Id type, gf::Message *msg){
    UNUSED(msg);
    assert(type == GameOver::type);

    m_rooms.clear();
    generateLevel();

    return gf::MessageStatus::Keep;
}

void Ship::generateLevel() {
    // Add all ship's rooms
    addRoom({04, 4}, {15.0f, 3.0f}, "cockpit.png", new Crew("pirategirl2.png"));
    addRoom({13, 1}, {03.0f, 1.5f}, "corridor_top.png");
    addRoom({02, 2}, {00.5f, 1.5f}, "engine_top.png");
    addRoom({01, 2}, {01.5f, 4.0f}, "corridor_left.png", new Crew("pirate_m2.png"));
    addRoom({02, 2}, {00.5f, 6.5f}, "engine_bottom.png");
    addRoom({13, 1}, {03.0f, 7.5f}, "corridor_bottom.png");
    addRoom({02, 2}, {05.0f, 3.5f}, "oxygen_room.png");
}
