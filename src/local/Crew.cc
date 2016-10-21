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

#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>

#include "Params.h"
#include "Singletons.h"

Crew::Crew(const gf::Path &path)
: m_texture(gResourceManager().getTexture(path)) {
}

void Crew::setPosition(gf::Vector2f position) {
    m_position = position;
}

void Crew::render(gf::RenderTarget &target) {
    gf::Sprite sprite;
    
    sprite.setTexture(m_texture);
    sprite.setTextureRect({0.0f, 0.0f, 33.0f / 132.0f, 48.0f / 192.0f});
    m_position.x -= 33.0f / 2.0f;
    m_position.y -= 48.0f / 2.0f;
    sprite.setPosition(m_position);
    
    target.draw(sprite);
}
