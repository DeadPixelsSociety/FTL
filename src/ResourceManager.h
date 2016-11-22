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

#ifndef SOUNDRESOURCEMANAGER_H
#define SOUNDRESOURCEMANAGER_H

#include <gf/ResourceManager.h>
#include <SFML/Audio.hpp>

namespace {
    
    template<typename T>
    class ResourceLoader {
    public:
        std::unique_ptr<T> operator()(const gf::Path& filename) {
            std::unique_ptr<T> ptr(new T);
            bool loaded = ptr->loadFromFile(filename.string());
            return loaded ? std::move(ptr) : nullptr;
        }
    };
    
}

class ResourceManager : public gf::ResourceManager {
public:
    
    ResourceManager() : gf::ResourceManager(), m_sounds(ResourceLoader<sf::SoundBuffer>()) {}
    
    sf::SoundBuffer& getSound(const gf::Path &path) {
        return m_sounds.getResource(*this, path);
    }

private:
    gf::ResourceCache<sf::SoundBuffer> m_sounds;
};

#endif // SOUNDRESOURCEMANAGER_H
