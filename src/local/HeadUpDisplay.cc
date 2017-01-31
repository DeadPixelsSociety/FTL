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

#include "HeadUpDisplay.h"

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

#include "Singletons.h"
#include "Messages.h"
#include "Params.h"

static constexpr float COOLDOWN_ALERT = 2.0f;

HeadUpDisplay::HeadUpDisplay()
: m_score(0.0f)
, m_isGameOver(false)
, m_font(gResourceManager().getFont("jupiter.ttf")) {
    gMessageManager().registerHandler<GameOver>(&HeadUpDisplay::onGameOver, this);
    gMessageManager().registerHandler<ResetGame>(&HeadUpDisplay::onResetGame, this);
    gMessageManager().registerHandler<AlertThrow>(&HeadUpDisplay::onAlertThrow, this);
}

void HeadUpDisplay::update(float dt) {
    m_score += dt;
    for (auto &pair: m_alerts) {
        pair.second += dt;
    }
}

void HeadUpDisplay::render(gf::RenderTarget &target) {
    gf::Text text("Score: " + std::to_string(static_cast<unsigned>(std::round(m_score))), m_font);
    text.setColor(gf::Color::White);

    if (m_isGameOver) {
        text.setString("Score: " + std::to_string(static_cast<unsigned>(std::round(m_score))) + "\nPress [R] to restart.");
        text.setPosition({GAME_WIDTH / 2, GAME_HEIGHT / 2});
        text.setScale(2.5f);
        text.setAnchor(gf::Anchor::Center);
        target.draw(text);
        return;
    } else {
        text.setPosition({GAME_WIDTH * 0.01f, GAME_HEIGHT * 0.01f});
        text.setAnchor(gf::Anchor::CenterLeft);
    }

    target.draw(text);

    // Manage the alert
    float heightOffset = 0.0f;
    for (auto &pair: m_alerts) {
        if (pair.second < COOLDOWN_ALERT) {
            text.setString(pair.first);
            text.setPosition({GAME_WIDTH * 0.99f, GAME_HEIGHT * 0.01f + heightOffset});
            text.setAnchor(gf::Anchor::CenterRight);
            target.draw(text);

            heightOffset += text.getLocalBounds().height + 5.0f;
        }
    }
}

gf::MessageStatus HeadUpDisplay::onGameOver(gf::Id type, gf::Message *msg) {
    UNUSED(msg);
    assert(type == GameOver::type);

    m_isGameOver = true;

    return gf::MessageStatus::Keep;
}

gf::MessageStatus HeadUpDisplay::onResetGame(gf::Id type, gf::Message *msg) {
    UNUSED(msg);
    assert(type == ResetGame::type);

    m_score = 0.0;
    m_isGameOver = false;

    return gf::MessageStatus::Keep;
}

gf::MessageStatus HeadUpDisplay::onAlertThrow(gf::Id type, gf::Message *msg) {
    assert(type == AlertThrow::type);
    AlertThrow *alert = static_cast<AlertThrow*>(msg);

    m_alerts.push_back(std::pair<std::string, float>(alert->message, 0.0f));

    return gf::MessageStatus::Keep;
}
