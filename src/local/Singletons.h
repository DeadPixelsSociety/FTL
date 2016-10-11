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

#ifndef LOCAL_SINGLETONS_H
#define LOCAL_SINGLETONS_H

#include <gf/MessageManager.h>
#include <gf/ResourceManager.h>
#include <gf/Singleton.h>

extern gf::Singleton<gf::MessageManager> gMessageManager;
extern gf::Singleton<gf::ResourceManager> gResourceManager;

#endif // LOCAL_SINGLETONS_H
