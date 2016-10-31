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

#ifndef LOCAL_CREW_H
#define LOCAL_CREW_H

#include <gf/Animation.h>
#include <gf/Direction.h>
#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

#include "Room.h"

class Crew : public gf::Entity {
public:
    Crew(const gf::Path &path, Room* isInRoom);
    
    void setPathToRoom(std::vector<Room*> &pathRooms);
    
    inline Room* getCurrentRoom() { return m_isInRoom; }
    
    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget &target) override;

private:
    void walkToCenterRoom();
    void walkToTransitionRoom();
        
private:
    gf::Vector2f m_position;
    
    gf::Direction m_direction;
    bool m_isWalking;
    bool m_arrivedToCurrTransPos;
    bool m_arrivedToTranPos;

    gf::Animation m_static;
    gf::Animation m_running[4];
    gf::Animation *m_currentAnimation;
    
    Room* m_isInRoom;
    std::vector<Room*> m_pathToRoom;
    gf::Vector2f m_walkToPos;
};

#endif // LOCAL_CREW_H
