#ifndef HAND_INCLUDE_DEFINES_H
#define HAND_INCLUDE_DEFINES_H

#include "graph/defines.h"
#include "view/defines.h"
#include "input/defines.h"


// Paths and file names
#define FONT_FILE "/usr/share/fonts/truetype/freefont/FreeSans.ttf"
#define LIBRARY_FILE_EXTENSION ".so"


enum default_number_of_items
{
    NUMBER_OF_BUTTONS = 5,
    MAX_NUMBER_OF_BUTTONS = 10
};


enum key_position
{
    THUMB,
    FOREFINGER,
    MIDDLEFINGER,
    RINGFINGER,
    LITTLEFINGER
};

#endif // HAND_INCLUDE_DEFINES_H
