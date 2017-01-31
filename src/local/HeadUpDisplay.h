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

#ifndef LOCAL_HEAD_UP_DISPLAY_H
#define LOCAL_HEAD_UP_DISPLAY_H

#include <list>
#include <utility>

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/Message.h>
#include <gf/MessageManager.h>

class HeadUpDisplay: public gf::Entity {
public:
    HeadUpDisplay();

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget &target) override;

    gf::MessageStatus onGameOver(gf::Id type, gf::Message *msg);
    gf::MessageStatus onResetGame(gf::Id type, gf::Message *msg);
    gf::MessageStatus onAlertThrow(gf::Id type, gf::Message *msg);

    bool isGameOver() { return m_isGameOver; }

private:
    double m_score;
    bool m_isGameOver;
    gf::Font &m_font;
    std::list< std::pair<std::string, float> > m_alerts;
};

#endif // LOCAL_HEAD_UP_DISPLAY_H
