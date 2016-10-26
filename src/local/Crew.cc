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

#include "Crew.h"

#include <gf/AnimatedSprite.h>
#include <gf/RenderTarget.h>

#include "Params.h"
#include "Singletons.h"

static constexpr float FrameTime = 0.05f;

static constexpr float Speed = 180.0f;

static void loadSingleFrameAnimation(gf::Animation& animation, const gf::Path& path) {
    gf::Texture& texture = gResourceManager().getTexture(path);
    texture.setSmooth();
    animation.addFrame(texture, {0.0f, 0.0f, 33.0f / 132.0f, 48.0f / 192.0f}, 1.0f);
}

static void loadMultiFrameAnimation(gf::Animation& animation, const gf::Path& path, gf::Direction direction) {
    gf::Texture& texture = gResourceManager().getTexture(path);
    texture.setSmooth();
    
    float directionOffset;
    
    // This switch is based on the sprite, and as values aren't equal between anim.sprite and direction, it have to get this switch converter
    switch(direction) {
        case gf::Direction::Up: directionOffset = 3.0f; break;
        case gf::Direction::Down: directionOffset = 0.0f; break;
        case gf::Direction::Left: directionOffset = 1.0f; break;
        case gf::Direction::Right: directionOffset = 2.0f; break;
    }

    for (unsigned i = 0; i < 4; ++i) {
        animation.addFrame(texture, { (i % 4) / 4.0f, directionOffset / 4.0f, 33.0f / 132.0f, 48.0f / 192.0f }, FrameTime);
    }
}

Crew::Crew(const gf::Path &path)
: gf::Entity(2)
, m_position(0.0, 0.0)
, m_direction(gf::Direction::Down)
, m_isWalking(false)
, m_currentAnimation(&m_static) {
    
    // load animation

    loadSingleFrameAnimation(m_static, path);

    loadMultiFrameAnimation(m_running[static_cast<int>(gf::Direction::Up)], path, gf::Direction::Up);
    loadMultiFrameAnimation(m_running[static_cast<int>(gf::Direction::Down)], path, gf::Direction::Down);
    loadMultiFrameAnimation(m_running[static_cast<int>(gf::Direction::Left)], path, gf::Direction::Left);
    loadMultiFrameAnimation(m_running[static_cast<int>(gf::Direction::Right)], path, gf::Direction::Right);
    
    gResourceManager().getTexture(path);
}

void Crew::goRight() {
    m_isWalking = true;
    m_direction = gf::Direction::Right;
}

void Crew::goLeft() {
    m_isWalking = true;
    m_direction = gf::Direction::Left;
}

void Crew::goUp() {
    m_isWalking = true;
    m_direction = gf::Direction::Up;
}

void Crew::goDown() {
    m_isWalking = true;
    m_direction = gf::Direction::Down;
}

void Crew::stop() {
    m_isWalking = false;
}

void Crew::setPosition(gf::Vector2f position) {
    m_position = position;
}

void Crew::update(float dt) {
    // update position
    // if (m_isWalking) {
    //   m_position += gf::displacement(m_direction) * Speed * dt;
    // }
    
    // update animation
    if (m_isWalking) {
      m_currentAnimation = &m_running[static_cast<int>(m_direction)];
    } else {
      m_currentAnimation = &m_static;
    }
    
    m_currentAnimation->update(dt);
}

void Crew::render(gf::RenderTarget &target) {
    gf::AnimatedSprite sprite;

    sprite.setAnimation(*m_currentAnimation);
    sprite.setPosition(m_position);
    sprite.setAnchor(gf::Anchor::Center);

    target.draw(sprite);
}
