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

class RoomManager : public gf::Entity {
public:
    RoomManager();

    void addRoom(gf::Vector2f size, gf::Vector2f position, const gf::Path &path, Crew *crew = nullptr);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget &target) override;

    gf::MessageStatus onLeftClicMouse(gf::Id type, gf::Message *msg);
    gf::MessageStatus onRightClicMouse(gf::Id type, gf::Message *msg);
    gf::MessageStatus onRoomFailure(gf::Id type, gf::Message *msg);
    gf::MessageStatus onGameOver(gf::Id type, gf::Message *msg);

private:
    void generateLevel();

private:
    std::vector<Room> m_rooms;
    Room *m_roomStartMove;
};

#endif // LOCAL_ROOM_MANAGER_H
