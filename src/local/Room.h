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

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

class Room : public gf::Entity {
public:
    Room(gf::Vector2f size, gf::Vector2f position, const gf::Path &path);

    bool isHit(gf::Vector2f point) const;
    bool hasCrew() const;
    bool isFailure() const;
    bool isInFire() const;

    void addLinkedRoom(Room* room, gf::Vector2f transitionPos, double cost);

    inline gf::Vector2f getPos() { return m_position; }
    inline std::map<Room*, std::pair<double, gf::Vector2f>> getLinkedRoom() { return m_linkedRoom; }
    inline void crewEnter() { m_nbCrew++; }
    void crewOut();
    inline void repare() { m_isRepairing = true; }
    gf::Vector2f getTransPos(Room* withRoom);
    gf::Vector2f getRoomCenter();

    void crewMoveTo(Room &room);
    void failure();
    void fire();

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget &target) override;

private:
    gf::Vector2f m_size;
    gf::Vector2f m_position;
    gf::Texture &m_texture;

    bool m_failure;
    bool m_red;

    unsigned m_nbCrew;
    bool m_isRepairing;

    float m_temperature;
    float m_timeBlink;
    float m_timeRepair;
    float m_timeFailure;

    std::map<Room*, std::pair<double, gf::Vector2f>> m_linkedRoom;
    gf::Animation m_fireAnimation;
};

#endif // LOCAL_ROOM_H
