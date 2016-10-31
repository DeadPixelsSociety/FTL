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

#ifndef LOCAL_ROOM_MANAGER_H
#define LOCAL_ROOM_MANAGER_H

#include <vector>

#include <gf/Entity.h>
#include <gf/EntityContainer.h>
#include <gf/Message.h>
#include <gf/Vector.h>

#include "Crew.h"
#include "Room.h"

typedef struct _roomForGraph {
    int id;
    float dist;
    struct _roomForGraph *next;
}GraphRoom;

class RoomManager : public gf::Entity {
public:
    RoomManager();

    void addRoom(int id, gf::Vector2f size, gf::Vector2f position, const gf::Path &path, Crew *crew = nullptr);
    void addCrew(const gf::Path &path, Room* isInRoom);
    
    inline Room* getRoom(int id) { return &m_rooms.at(id); }
    inline Crew getCrew(int id) { return m_crew.at(id); }

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget &target) override;

    gf::MessageStatus onLeftClicMouse(gf::Id type, gf::Message *msg);
    gf::MessageStatus onRightClicMouse(gf::Id type, gf::Message *msg);
    gf::MessageStatus onRoomFailure(gf::Id type, gf::Message *msg);
    gf::MessageStatus onGameOver(gf::Id type, gf::Message *msg);

private:
    void generateLevel();
    
    // Path finding stuff
    float min(float a, float b);
    std::vector<Room*> findPath(Room* startRoom, Room* endRoom);
    GraphRoom* roomToGraphRoom(Room* room);
    GraphRoom* addRoomToGraph(GraphRoom* anchor, GraphRoom* newRoom);
    void loadGraph();
    void iterDijkstra(float* dist, Room** prev, int current, int* dijkstra, int nbDone);
    // End pathfinding stuff

private:
    std::vector<Room> m_rooms;
    std::vector<Crew> m_crew;
    std::vector<GraphRoom*> m_graph;
    
    Room *m_roomStartMove;
    Crew *m_crewToMove;
};

#endif // LOCAL_ROOM_MANAGER_H
