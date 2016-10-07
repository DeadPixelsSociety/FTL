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

#include "Room.h"

#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#include "Params.h"

Room::Room(gf::Vector2f size, gf::Vector2f position, Crew *crew)
: m_size(size)
, m_position(position)
, m_crew(crew) {

}

void Room::render(gf::RenderTarget &target) {
    gf::RectangleShape sprite(m_size * TILE_SIZE);
    sprite.setColor(gf::Color::Blue);
    sprite.setOutlineColor(gf::Color::Black);
    sprite.setOutlineThickness(1.0f);
    sprite.setPosition(m_position * TILE_SIZE);

    target.draw(sprite);

    // Render the crew if present
    if (m_crew) {
        // Place the crew at room center
        gf::Vector2f position(m_position * TILE_SIZE);
        position = position + (m_size * TILE_SIZE / 2);
        m_crew->setPosition(position);
        m_crew->render(target);
    }
}
