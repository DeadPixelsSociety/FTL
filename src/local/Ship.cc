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

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <unordered_map>

#include "Messages.h"
#include "Params.h"
#include "Singletons.h"

float heuristic(gf::Vector2f a, gf::Vector2f b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

Ship::Ship()
: m_timeElapsed(0.0f)
, m_crewToMove(nullptr) {
    gMessageManager().registerHandler<LeftClicMouse>(&Ship::onLeftClicMouse, this);
    gMessageManager().registerHandler<RightClicMouse>(&Ship::onRightClicMouse, this);
    gMessageManager().registerHandler<RoomFailure>(&Ship::onRoomFailure, this);
    gMessageManager().registerHandler<GameOver>(&Ship::onGameOver, this);
    gMessageManager().registerHandler<Fire>(&Ship::onFire, this);
    generateLevel();
}

void Ship::addRoom(gf::Vector2f size, gf::Vector2f position, const gf::Path &path) {
    m_rooms.push_back(Room(size, position, path));
}

void Ship::addCrew(const gf::Path &path, Room* isInRoom) {
    m_crew.push_back(Crew(path, isInRoom));
}

void Ship::update(float dt) {
    for(Room &room: m_rooms) {
        room.update(dt);
    }

    for(Crew &crew: m_crew) {
        crew.update(dt);
    }

    m_timeElapsed += dt;

    // If a failure must be launched
    if (m_timeElapsed >= COOLDOWN_FAILURE) {
        m_timeElapsed -= COOLDOWN_FAILURE;


        float rand = gRandom().computeUniformFloat(0.0f, 100.0f);
        if (rand <= 200.0f) {
            Fire message;
            gMessageManager().sendMessage(&message);
        }
        else {
            RoomFailure message;
            gMessageManager().sendMessage(&message);
        }
    }
}

void Ship::render(gf::RenderTarget &target) {
    for(Room &room: m_rooms) {
        room.render(target);
    }

    for(Crew &crew: m_crew) {
        crew.render(target);
    }
}

gf::MessageStatus Ship::onLeftClicMouse(gf::Id type, gf::Message *msg){
    assert(type == LeftClicMouse::type);
    LeftClicMouse* leftClic = static_cast<LeftClicMouse*>(msg);

    if (m_crewToMove == nullptr) {
        for(Crew &crew: m_crew) {
            if(crew.getCurrentRoom()->isHit(leftClic->position)) {
                gf::Log::debug(gf::Log::General, "Room hited 1\n");
                m_crewToMove = &crew;
            }
        }
    } else {
        for(Room &room: m_rooms) {
            if (room.isHit(leftClic->position)
                    && !m_crewToMove->getCurrentRoom()->isHit(leftClic->position)
                    ) {
                gf::Log::debug(gf::Log::General, "Room hited 2\n");
                std::vector<Room*> roomPath = findPath(m_crewToMove->getCurrentRoom(), &room);
                m_crewToMove->setPathToRoom(roomPath);
                m_crewToMove = nullptr;
            }
        }
    }

    return gf::MessageStatus::Keep;
}

gf::MessageStatus Ship::onRightClicMouse(gf::Id type, gf::Message *msg){
    UNUSED(msg);
    assert(type == RightClicMouse::type);

    m_crewToMove = nullptr;
    gf::Log::debug(gf::Log::General, "PathFinding Released\n");

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

gf::MessageStatus Ship::onGameOver(gf::Id type, gf::Message *msg) {
    UNUSED(msg);
    assert(type == GameOver::type);

    m_rooms.clear();
    m_crew.clear();
    m_crewToMove = nullptr;
    generateLevel();
    m_timeElapsed = 0.0f;

    return gf::MessageStatus::Keep;
}

gf::MessageStatus Ship::onFire(gf::Id type, gf::Message *msg) {
    UNUSED(msg);
    assert(type == Fire::type);

    // Check if a room is ok
    bool isOk = false;
    for (Room &room: m_rooms) {
        if (!room.isInFire() && !room.hasCrew()) {
            isOk = true;
            break;
        }
    }
    if (!isOk) {
        return gf::MessageStatus::Keep;
    }

    // Select random room
    unsigned random = gRandom().computeUniformInteger<unsigned>(0, m_rooms.size() - 1);
    while (m_rooms[random].hasCrew() || m_rooms[random].isInFire()) {
        random = gRandom().computeUniformInteger<unsigned>(0, m_rooms.size() - 1);
    }

    gf::Log::debug(gf::Log::General, "Room fire %d\n", random);
    m_rooms[random].fire();

    return gf::MessageStatus::Keep;
}


void Ship::generateLevel() {
    // Add all ship's rooms
    addRoom({04, 4}, {15.0f, 3.0f}, "cockpit.png");
    addRoom({13, 1}, {03.0f, 1.5f}, "corridor_top.png");
    addRoom({02, 2}, {00.5f, 1.5f}, "engine_top.png");
    addRoom({01, 2}, {01.5f, 4.0f}, "corridor_left.png");
    addRoom({02, 2}, {00.5f, 6.5f}, "engine_bottom.png");
    addRoom({13, 1}, {03.0f, 7.5f}, "corridor_bottom.png");
    addRoom({02, 2}, {05.0f, 3.5f}, "oxygen_room.png");

    // addLinkedRoom(ptrLRoom, pos, cost);
    // Add connection between rooms. Parameters are:
    // ptrLRoom : A pointer to the linked room
    // pos : position in room where the "checkpoint" will be place to the next room.
    // cost : numeric value between the room and the linked room cost
    //          (higher value tells to A* Search that going from this room to the other cost more energy.
    //          It will so prefer a path or another.)
    // TODO: review the cost value!
    m_rooms[0].addLinkedRoom(&m_rooms[1], {2.0f, 2.0f}, 1);
    m_rooms[0].addLinkedRoom(&m_rooms[5], {2.0f, 8.0f}, 1);
    m_rooms[1].addLinkedRoom(&m_rooms[0], {26.0f, 2.0f}, 1);
    m_rooms[1].addLinkedRoom(&m_rooms[2], {2.0f, 2.0f}, 1);
    m_rooms[1].addLinkedRoom(&m_rooms[6], {8.0f, 2.0f}, 1);
    m_rooms[2].addLinkedRoom(&m_rooms[1], {4.0f, 2.0f}, 1);
    m_rooms[2].addLinkedRoom(&m_rooms[3], {4.0f, 4.0f}, 1);
    m_rooms[3].addLinkedRoom(&m_rooms[2], {2.0f, 2.0f}, 1);
    m_rooms[3].addLinkedRoom(&m_rooms[4], {2.0f, 4.0f}, 1);
    m_rooms[4].addLinkedRoom(&m_rooms[3], {4.0f, 2.0f}, 1);
    m_rooms[4].addLinkedRoom(&m_rooms[5], {4.0f, 4.0f}, 1);
    m_rooms[5].addLinkedRoom(&m_rooms[0], {26.0f, 2.0f}, 1);
    m_rooms[5].addLinkedRoom(&m_rooms[4], {2.0f, 2.0f}, 1);
    m_rooms[6].addLinkedRoom(&m_rooms[1], {4.0f, 2.0f}, 1);

    addCrew("pirategirl2.png", &m_rooms[0]);
    addCrew("pirate_m2.png", &m_rooms[3]);
}

std::vector<Room *> Ship::findPath(Room* startRoom, Room* endRoom) {
    //////////////////
    // A* Algorithm //
    //////////////////
    std::unordered_map<Room*, Room*> cameFrom;
    std::unordered_map<Room*, double> costSoFar;
    PriorityQueue<Room*, double> frontier;
    frontier.put(startRoom, 0);
    cameFrom[startRoom] = startRoom;
    costSoFar[startRoom] = 0;

    while(!frontier.empty()) {
        Room* current = frontier.get();
        if (current == endRoom) {
            break;
        }
        for (auto next : current->getLinkedRoom()) {
            double newCost = costSoFar[current] + next.second.first;
            if (!costSoFar.count(next.first) || newCost < costSoFar[next.first]) {
                costSoFar[next.first] = newCost;
                double priority = newCost + heuristic(next.first->getPos(), endRoom->getPos());
                frontier.put(next.first, priority);
                cameFrom[next.first] = current;
            }
        }
    }
    /////////////////////////
    // End of A* algorithm //
    /////////////////////////

    //////////////////////////////////
    // Rebuild the path
    // Convert the path for crew.
    //////////////////////////////////
    std::vector<Room*> roomPath;
    Room* current = endRoom;

    roomPath.push_back(current);
    while (current != startRoom) {
        current = cameFrom[current];
        roomPath.push_back(current);
    }
    roomPath.pop_back();

    return roomPath;
}
