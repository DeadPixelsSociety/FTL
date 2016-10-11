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
#include <gf/Sprite.h>
#include <gf/Log.h>

#include "Params.h"
#include "Singletons.h"

Room::Room(gf::Vector2f size, gf::Vector2f position, const gf::Path &path, Crew *crew)
: m_size(size)
, m_position(position)
, m_crew(crew)
, m_texture(gResourceManager().getTexture(path)) {

}

bool Room::isHit(gf::Vector2f point) const {
    // If a crew is present
    gf::Vector2f cornerTopLeft = m_position * TILE_SIZE;
    gf::Vector2f cornerBottomRight = cornerTopLeft + m_size * TILE_SIZE;
    if (cornerTopLeft.x < point.x && cornerBottomRight.x > point.x &&
        cornerTopLeft.y < point.y && cornerBottomRight.y > point.y) {
        return true;
    }

    return false;
}

bool Room::hasCrew() const {
    return static_cast<bool>(m_crew);
}

void Room::crewMoveTo(Room &room) {
    room.m_crew = std::move(m_crew);
}

void Room::render(gf::RenderTarget &target) {
    gf::Sprite sprite;
    gf::Vector2f realSize = m_size * TILE_SIZE * 10.0f;
    gf::Log::debug(gf::Log::General, "m_size = {%f, %f}\n", m_size.x, m_size.y);
    sprite.setTexture(m_texture);
    //sprite.setTextureRect({ 1.0f, 1.0f, 1.0f, 1.0f });
    sprite.setScale((m_size * TILE_SIZE) / realSize);
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
