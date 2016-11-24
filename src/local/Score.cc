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

#include "Score.h"

#include <gf/RenderTarget.h>
#include <gf/Text.h>

#include "Singletons.h"
#include "Messages.h"
#include "Params.h"

Score::Score()
: m_score(0.0f)
, m_font(gResourceManager().getFont("jupiter.ttf")) {
    gMessageManager().registerHandler<GameOver>(&Score::onGameOver, this);
}

void Score::update(float dt) {
    m_score += dt;
}

void Score::render(gf::RenderTarget &target) {
    gf::Text text("Score: " + std::to_string(static_cast<unsigned>(std::round(m_score))), m_font);
    text.setPosition({0.0f, 0.0f});
    text.setColor(gf::Color::White);
    target.draw(text);
}

gf::MessageStatus Score::onGameOver(gf::Id type, gf::Message *msg) {
    UNUSED(msg);
    assert(type == GameOver::type);
    
    m_score = 0.0;
    
    return gf::MessageStatus::Keep;
}
