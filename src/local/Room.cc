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
#include <gf/Shapes.h>

#include "Messages.h"
#include "Params.h"
#include "Singletons.h"

static constexpr float MIN_TEMPERATURE = 18.0f; // Celsuis
static constexpr float FLOOR_TEMPERATURE = 50.0f; // Celsuis
static constexpr float STEP_TEMPERATURE = 2.0f; // Celsuis
static constexpr float FIRE_FACTOR = 4.0f;
static constexpr float FIRE_FACTOR_BASE = 2.0f;

Room::Room(gf::Vector2f size, gf::Vector2f position, const gf::Path &path)
: m_size(size)
, m_position(position)
, m_texture(gResourceManager().getTexture(path))
, m_failure(false)
, m_red(false)
, m_nbCrew(0)
, m_isRepairing(false)
, m_temperature(MIN_TEMPERATURE)
, m_timeBlink(0.0f)
, m_timeRepair(0.0f)
, m_timeFailure(0.0f) {
}

bool Room::isHit(gf::Vector2f point) const {
    // If a crew is present
    gf::RectF room(m_position * TILE_SIZE, m_size * TILE_SIZE);
    return room.contains(point);
}

bool Room::hasCrew() const {
    return m_nbCrew > 0;
}

bool Room::isFailure() const {
    return m_failure;
}

bool Room::isInFire() const {
    return m_temperature >= FLOOR_TEMPERATURE;
}

void Room::crewMoveTo(Room &room) {
    crewOut();
    room.crewEnter();
}

void Room::failure() {
    m_failure = true;
}

void Room::fire() {
    m_temperature = FLOOR_TEMPERATURE;
}

void Room::addLinkedRoom(Room* room, gf::Vector2f transitionPos, double cost) {
    m_linkedRoom.emplace(room, std::make_pair(cost, transitionPos));
}

void Room::crewOut() {
    m_nbCrew--;
    if(m_nbCrew==0)
    {
        m_isRepairing=false;
    }
}

gf::Vector2f Room::getTransPos(Room* withRoom){
    gf::Vector2f realSize = (m_size + 1.0f) * TILE_SPRITE_SIZE;
    gf::Vector2f worldSize = (m_size + 1.0f) * TILE_SIZE;
    gf::Vector2f scale = worldSize / realSize;

    auto it = m_linkedRoom.find(withRoom);
    if(it != m_linkedRoom.end()) {
        return m_position * TILE_SIZE - (0.5 * TILE_SIZE) + it->second.second * TILE_SIZE * scale;
    } else {
        return {-1.0f, -1.0f};
    }
}

gf::Vector2f Room::getRoomCenter() {
    return (m_position * TILE_SIZE) + (m_size * TILE_SIZE / 2);
}

void Room::update(float dt) {
    if(m_failure) {
        m_timeBlink += dt;
        m_timeFailure += dt;
        if (m_timeBlink >= BLINK_TIME) {
            m_timeBlink -= BLINK_TIME;
            m_red = !m_red;
        }
    }

    if(m_isRepairing && m_failure) {
        m_timeRepair += dt;
        if (m_timeRepair >= COOLDOWN_REPAIR) {
            m_timeRepair = 0.0f;
            m_failure = false;
            m_isRepairing = false;
        }
    }
    else if (!m_isRepairing && m_timeRepair > 0.0f) {
        m_timeRepair = 0.0f;
    }

    // Check the temperature statut
    float factor = 0.0f;
    if (isInFire()) {
        factor = FIRE_FACTOR_BASE;
    }
    else {
        factor = -FIRE_FACTOR_BASE;
    }
    for (auto it = m_linkedRoom.begin(); it != m_linkedRoom.end(); ++it) {
        Room *currentRoom = it->first;
        if (currentRoom->isInFire()) {
            factor += FIRE_FACTOR;
        }
    }
    m_temperature += STEP_TEMPERATURE * factor * dt;

    if (m_temperature < MIN_TEMPERATURE) {
        m_temperature = MIN_TEMPERATURE;
    }

    if (m_timeFailure >= COOLDOWN_DESTROY) {
        GameOver message;
        gMessageManager().sendMessage(&message);
        gf::Log::debug(gf::Log::General, "Destroy them all\n");
    }
}

void Room::render(gf::RenderTarget &target) {
    gf::Sprite sprite;

    gf::Vector2f realSize = (m_size + 1.0f) * TILE_SPRITE_SIZE;
    gf::Vector2f worldSize = (m_size + 1.0f) * TILE_SIZE;
    sprite.setTexture(m_texture);
    sprite.setScale(worldSize / realSize);
    if (m_failure) {
        if(m_red) {
            sprite.setColor({1.0f, 0.5f, 0.5f, 1.0f});
        }
        else {
            sprite.setColor({1.0f, 1.0f, 1.0f, 1.0f});
        }
    }
    sprite.setPosition(m_position * TILE_SIZE - (0.5 * TILE_SIZE));

    target.draw(sprite);

    if (isInFire()) {
        gf::Log::debug(gf::Log::General, "Room on fire!\n");
    }

    if (hasCrew()) {
        gf::Vector2f position(m_position * TILE_SIZE);
        position = position + (m_size * TILE_SIZE / 2);

        // Drawing repair time as a loading bar upper the crew
        if (m_timeRepair > 0.0f) {
            gf::RectangleShape rect1({100, 10});
            position.y -= TILE_SIZE - 10;
            position.x -= 50;
            rect1.setPosition(position);
            rect1.setColor(gf::Color::White);

            gf::RectangleShape rect2({m_timeRepair / COOLDOWN_REPAIR * 100, 10});
            rect2.setPosition(position);
            rect2.setColor(gf::Color::Green);

            target.draw(rect1);
            target.draw(rect2);
        }
    }
}
