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

#ifndef LOCAL_SHIP_H
#define LOCAL_SHIP_H

#include <vector>

#include <gf/Entity.h>
#include <gf/EntityContainer.h>
#include <gf/Message.h>
#include <gf/Vector.h>
#include <queue>

#include "Crew.h"
#include "Room.h"

template<typename T, typename priority_t>
struct PriorityQueue {
    typedef std::pair<priority_t, T> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>,
    std::greater<PQElement>> elements;
    
    inline bool empty() const { return elements.empty(); }
    
    inline void put(T item, priority_t priority) {
        elements.emplace(priority, item);
    }
    
    inline T get() {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};

class Ship : public gf::Entity {
public:
    Ship();

    void addRoom(gf::Vector2f size, gf::Vector2f position, const gf::Path &path);
    void addCrew(const gf::Path &path, Room* isInRoom);
    
    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget &target) override;

    gf::MessageStatus onLeftClicMouse(gf::Id type, gf::Message *msg);
    gf::MessageStatus onRightClicMouse(gf::Id type, gf::Message *msg);
    gf::MessageStatus onRoomFailure(gf::Id type, gf::Message *msg);
    gf::MessageStatus onGameOver(gf::Id type, gf::Message *msg);

private:
    void generateLevel();
    
    std::vector<Room*> findPath(Room* startRoom, Room* endRoom);

private:
    float m_timeElapsed;
    
    std::vector<Room> m_rooms;
    std::vector<Crew> m_crew;
    
    Crew *m_crewToMove;
};

#endif // LOCAL_SHIP_H
