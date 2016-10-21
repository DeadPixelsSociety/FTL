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

#ifndef LOCAL_CREW_H
#define LOCAL_CREW_H

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

class Crew : public gf::Entity {
public:
    Crew(const gf::Path &path);
    
    void setPosition(gf::Vector2f position);

    virtual void render(gf::RenderTarget &target) override;

private:
    gf::Vector2f m_position;
    gf::Texture &m_texture;
};

#endif // LOCAL_CREW_H
