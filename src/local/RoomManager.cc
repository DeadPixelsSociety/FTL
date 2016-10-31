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
#include <cfloat>

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#include "Crew.h"
#include "Messages.h"
#include "Params.h"
#include "Singletons.h"

RoomManager::RoomManager()
: m_graph()
, m_crewToMove(nullptr) {
    gMessageManager().registerHandler<LeftClicMouse>(&RoomManager::onLeftClicMouse, this);
    gMessageManager().registerHandler<RightClicMouse>(&RoomManager::onRightClicMouse, this);
    gMessageManager().registerHandler<RoomFailure>(&RoomManager::onRoomFailure, this);
    gMessageManager().registerHandler<GameOver>(&RoomManager::onGameOver, this);
    generateLevel();
}

void RoomManager::addRoom(int id, gf::Vector2f size, gf::Vector2f position, const gf::Path &path) {
    m_rooms.push_back(Room(id, size, position, path));
}

void RoomManager::addCrew(const gf::Path &path, Room* isInRoom) {
    m_crew.push_back(Crew(path, isInRoom));
}

void RoomManager::update(float dt) {
    for(Room &room: m_rooms) {
        room.update(dt);
    }
    
    for(Crew &crew: m_crew) {
        crew.update(dt);
    }
}

void RoomManager::render(gf::RenderTarget &target) {
    for(Room &room: m_rooms) {
        room.render(target);
    }
    
    for(Crew &crew: m_crew) {
        crew.render(target);
    }
}

gf::MessageStatus RoomManager::onLeftClicMouse(gf::Id type, gf::Message *msg){
    assert(type == LeftClicMouse::type);
    LeftClicMouse* leftClic = static_cast<LeftClicMouse*>(msg);
    
    if (m_crewToMove == nullptr) {
        for(Crew &crew: m_crew) {
            if(crew.getCurrentRoom()->isHit(leftClic->position) && crew.getCurrentRoom()->repareDone()) {
                gf::Log::debug(gf::Log::General, "Room hited 1\n");
                m_crewToMove = &crew;
            }
        }
    } else {
        for(Room &room: m_rooms) {
            if (room.isHit(leftClic->position) && !room.hasCrew()) {
                gf::Log::debug(gf::Log::General, "Room hited 2\n");
                std::vector<Room*> roomPath = findPath(m_crewToMove->getCurrentRoom(), &room);
                m_crewToMove->setPathToRoom(roomPath);
                m_crewToMove = nullptr;
            }
        }
    }

    return gf::MessageStatus::Keep;
}

gf::MessageStatus RoomManager::onRightClicMouse(gf::Id type, gf::Message *msg){
    UNUSED(msg);
    assert(type == RightClicMouse::type);

    m_crewToMove = nullptr;
    gf::Log::debug(gf::Log::General, "Room Released\n");

    return gf::MessageStatus::Keep;
}

gf::MessageStatus RoomManager::onRoomFailure(gf::Id type, gf::Message *msg){
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

gf::MessageStatus RoomManager::onGameOver(gf::Id type, gf::Message *msg){
    UNUSED(msg);
    assert(type == GameOver::type);

    m_rooms.clear();
    m_crew.clear();
    m_crewToMove = nullptr;
    generateLevel();

    return gf::MessageStatus::Keep;
}

void RoomManager::generateLevel() {
    // Add all ship's rooms
    addRoom(m_rooms.size(), {04, 4}, {15.0f, 3.0f}, "cockpit.png");
    addRoom(m_rooms.size(), {13, 1}, {03.0f, 1.5f}, "corridor_top.png");
    addRoom(m_rooms.size(), {02, 2}, {00.5f, 1.5f}, "engine_top.png");
    addRoom(m_rooms.size(), {01, 2}, {01.5f, 4.0f}, "corridor_left.png");
    addRoom(m_rooms.size(), {02, 2}, {00.5f, 6.5f}, "engine_bottom.png");
    addRoom(m_rooms.size(), {13, 1}, {03.0f, 7.5f}, "corridor_bottom.png");
    addRoom(m_rooms.size(), {02, 2}, {05.0f, 3.5f}, "oxygen_room.png");
    
    m_rooms.at(0).addLinkedRoom(&m_rooms.at(1), {m_rooms.at(1).getId(), {2.0f, 2.0f}});
    m_rooms.at(0).addLinkedRoom(&m_rooms.at(5), {m_rooms.at(5).getId(), {2.0f, 8.0f}});
    m_rooms.at(1).addLinkedRoom(&m_rooms.at(0), {m_rooms.at(0).getId(), {26.0f, 2.0f}});
    m_rooms.at(1).addLinkedRoom(&m_rooms.at(2), {m_rooms.at(2).getId(), {2.0f, 2.0f}});
    m_rooms.at(1).addLinkedRoom(&m_rooms.at(6), {m_rooms.at(6).getId(), {8.0f, 2.0f}});
    m_rooms.at(2).addLinkedRoom(&m_rooms.at(1), {m_rooms.at(1).getId(), {4.0f, 2.0f}});
    m_rooms.at(2).addLinkedRoom(&m_rooms.at(3), {m_rooms.at(3).getId(), {4.0f, 4.0f}});
    m_rooms.at(3).addLinkedRoom(&m_rooms.at(2), {m_rooms.at(2).getId(), {2.0f, 2.0f}});
    m_rooms.at(3).addLinkedRoom(&m_rooms.at(4), {m_rooms.at(4).getId(), {2.0f, 4.0f}});
    m_rooms.at(4).addLinkedRoom(&m_rooms.at(3), {m_rooms.at(3).getId(), {4.0f, 2.0f}});
    m_rooms.at(4).addLinkedRoom(&m_rooms.at(5), {m_rooms.at(5).getId(), {4.0f, 4.0f}});
    m_rooms.at(5).addLinkedRoom(&m_rooms.at(0), {m_rooms.at(0).getId(), {26.0f, 2.0f}});
    m_rooms.at(5).addLinkedRoom(&m_rooms.at(4), {m_rooms.at(4).getId(), {2.0f, 2.0f}});
    m_rooms.at(6).addLinkedRoom(&m_rooms.at(1), {m_rooms.at(1).getId(), {4.0f, 2.0f}});
    
    addCrew("pirategirl2.png", &m_rooms.at(0));
    addCrew("pirate_m2.png", &m_rooms.at(3));
    
    loadGraph();
}

////////////////////////
// DIJKSTRA FUNCTIONS //
////////////////////////
void printMinWay(float* dist, Room** prev, Room* startRoom, Room* endRoom) {
    Room* way = endRoom;

    gf::Log::debug(gf::Log::General, "Way :\n");
    if(dist[endRoom->getId()] == FLT_MAX) {
        gf::Log::debug(gf::Log::General, "There is no way :'(\n");
    } else {
        while(way->getId() != startRoom->getId()) {
            gf::Log::debug(gf::Log::General, "Room : [%d]\n", way->getId(), dist[way->getId()]);
            way = prev[way->getId()];
        }
        gf::Log::debug(gf::Log::General, " Start : %d\n", startRoom->getId());
    }
}

float RoomManager::min(float a, float b){
    return a<b ? a : b;
}

std::vector<Room *> RoomManager::findPath(Room* startRoom, Room* endRoom) {
    float* dist;
    Room** prev;
    int* dijkstra;

    dist        = (float*)malloc(m_rooms.size()*sizeof(float));
    prev        = (Room**)malloc(m_rooms.size()*sizeof(Room*));
    dijkstra    = (int*)malloc(m_rooms.size()*sizeof(int));

    for(unsigned i=0; i<m_rooms.size(); i++)
    {
        dist[i] = FLT_MAX;
        prev[i] = nullptr;
        dijkstra[i] = 0;
    }

    dist[startRoom->getId()] = 0;
    dijkstra[startRoom->getId()] = 1;

    iterDijkstra(dist, prev, startRoom->getId(), dijkstra, 1);

    // DEBUG IN CONSOLE :
    // printMinWay(dist, prev, startRoom, endRoom);

    std::vector<Room*> roomPath;
    Room* way = endRoom;

    if(dist[endRoom->getId()] == FLT_MAX) {
        gf::Log::debug(gf::Log::General, "There is no way :'(");
    } else {
        while(way->getId() != startRoom->getId()) {
            roomPath.push_back(way);
            way = prev[way->getId()];
        }
    }

    return roomPath;
}

GraphRoom* RoomManager::roomToGraphRoom(Room* room){
    GraphRoom* gRoom = (GraphRoom*)malloc(sizeof(GraphRoom));
    gRoom->id = room->getId();
    gRoom->dist = room->getDist();

    return gRoom;
}

GraphRoom* RoomManager::addRoomToGraph(GraphRoom* anchor, GraphRoom* newRoom){
    if(anchor == nullptr) {
        anchor = newRoom;
        anchor->next = nullptr;
    } else {
        newRoom->next = anchor;
        anchor = newRoom;
    }
    return anchor;
}

void RoomManager::loadGraph(){
    GraphRoom* tmp = nullptr;

    for(unsigned i=0; i<m_rooms.size(); i++) {
        m_graph.push_back(new GraphRoom());
        m_graph[i] = nullptr;

        int nbNext = m_rooms[i].getLinkedRoom().size();
        if(nbNext != 0) {
            for(int j=0; j < nbNext; j++) {
                tmp = new GraphRoom();
                tmp = roomToGraphRoom(m_rooms[i].getLinkedRoom()[j]);
                m_graph[i] = addRoomToGraph(m_graph[i], tmp);
            }
        }
    }
}

void RoomManager::iterDijkstra(float* dist, Room** prev, int current, int* dijkstra, int nbDone){
    int newWay = -1;
    GraphRoom* copy=nullptr;
    float minWay=FLT_MAX;
    float tmpDist=0;

    copy = new GraphRoom();
    copy = m_graph[current];

    while(copy!=nullptr) {
        tmpDist =  dist[copy->id];
        dist[copy->id] = min(dist[copy->id], dist[current] + copy->dist);
        if(tmpDist != dist[copy->id]) {
            prev[copy->id] = &m_rooms[current];
        }
        copy = copy->next;
    }

    for(unsigned i=0; i<m_rooms.size(); i++)
    {
        if(dijkstra[i] == 0)
        {
            minWay = min(minWay, dist[i]);
            if(minWay == dist[i]) {
                newWay = i;
            }
        }
    }    

    dijkstra[newWay] = 1;
    nbDone++;

    if(nbDone != (int)m_rooms.size()) {
        iterDijkstra(dist, prev, newWay, dijkstra, nbDone);
    }
}
