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

#ifndef LOCAL_MESSAGES_H
#define LOCAL_MESSAGES_H

#include <gf/Message.h>


struct LeftClicMouse : public gf::Message {
  static const gf::Id type = "LeftClicMouse"_id; // compile-time definition
  gf::Vector2f position;
};

struct RightClicMouse : public gf::Message {
  static const gf::Id type = "RightClicMouse"_id; // compile-time definition
};

struct RoomFailure : public gf::Message {
  static const gf::Id type = "RoomFailure"_id; // compile-time definition
};

#endif // LOCAL_MESSAGES_H
