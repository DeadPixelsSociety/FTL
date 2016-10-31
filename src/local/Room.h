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

#ifndef LOCAL_ROOM_H
#define LOCAL_ROOM_H

#include <memory>

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

#include "Crew.h"

class Crew;

class Room : public gf::Entity {
public:
    Room(int id, gf::Vector2f size, gf::Vector2f position, const gf::Path &path, bool hasCrew = false);

    bool isHit(gf::Vector2f point) const;
    bool hasCrew() const;
    bool isFailure() const;

    void addLinkedRoom(Room* room);

    inline int getId() { return m_id; }
    inline float getDist() { return m_size.x * m_size.y; }
    inline gf::Vector2f getPos() { return m_position; }
    inline gf::Vector2f getSize() { return m_size; }
    inline std::vector<Room*> getLinkedRoom() { return m_linkedRoom; }
    inline void crewEnter() { m_hasCrew = true; }
    inline void crewOut() { m_hasCrew = false; }
    inline void repare() { m_isRepairing = true; }
    inline bool repareDone() { return !m_isRepairing; }
    
    void crewMoveTo(Room &room);
    void failure();

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget &target) override;

private:
    int m_id;
    
    gf::Vector2f m_size;
    gf::Vector2f m_position;
    bool m_hasCrew;
    bool m_isRepairing;
    gf::Texture &m_texture;
    
    bool m_failure;
    bool m_red;
    
    float m_timeBlink;
    float m_timeRepair;
    float m_timeFailure;
    
    std::vector<Room *> m_linkedRoom;
};

#endif // LOCAL_ROOM_H
