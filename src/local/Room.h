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
#include <gf/Vector.h>

#include "Crew.h"

class Room : public gf::Entity {
public:
    Room(gf::Vector2f size, gf::Vector2f position, Crew *crew = nullptr);

    bool isHit(gf::Vector2f point) const;
    bool hasCrew() const;

    void crewMoveTo(Room &room);

    // virtual void update(float dt) override;
    virtual void render(gf::RenderTarget &target) override;

private:
    gf::Vector2f m_size;
    gf::Vector2f m_position;
    std::unique_ptr<Crew> m_crew; // Crew present in Room
};

#endif // LOCAL_ROOM_H
