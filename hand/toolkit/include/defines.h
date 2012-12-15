/*
 *  Copyright 2012 Alex Stefaniuc
 *
 *  This file is part of Hand.
 *
 *  Hand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  Hand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with Hand. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_DEFINES_H
#define INCLUDE_DEFINES_H

#include "base/defines.h"
#include "graph/defines.h"
#include "view/defines.h"
#include "input/defines.h"


// Paths and file names
#define FONT_FILE "/usr/share/fonts/truetype/freefont/FreeSans.ttf"
#define LIBRARY_FILE_EXTENSION ".so"
#define THEMES_DIRECTORY "./apps/themes/"


enum default_number_of_items
{
    NUMBER_OF_BUTTONS = 5,
    MAX_NUMBER_OF_BUTTONS = 10
};


enum key_pointer
{
    ROOT,
    PRESSED,
    RELEASED
};


enum key_position
{
    THUMB,
    FOREFINGER,
    MIDDLEFINGER,
    RINGFINGER,
    LITTLEFINGER
};

#endif /* INCLUDE_DEFINES_H */
