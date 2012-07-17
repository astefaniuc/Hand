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

#ifndef HAND_DEFINES_H
#define HAND_DEFINES_H

#include <SDL/SDL.h>


using namespace std;


enum default_number_of_items
{
    NUMBER_OF_BUTTONS = 5
};


enum key_pointer
{
    ROOT,
    PRESSED,
    RELEASED,
    FOLLOWER
};


enum key_position
{
    THUMB,
    FOREFINGER,
    MIDDLEFINGER,
    RINGFINGER,
    LITTLEFINGER
};


enum button_string
{
    BTN_NAME = 1,
    BTN_DESCRIPTION = 2,
    BTN_ICON = 4,
    BTN_CONTROL = 8,
    BTN_PREVIEW = 16
};


struct Size
{
    int w;
    int h;
};


struct Position
{
    int x;
    int y;
};


#define FONT_FILE "/usr/share/fonts/truetype/freefont/FreeSans.ttf"

// Themes strings
#define THEMES_DIRECTORY "file:///home/james/workspace/test_app/toolkit/themes/"
#define DEFAULT_THEME "file://default.so"
#define LIBRARY_FILE_EXTENSION ".so"

// Menu names
#define SETTINGS_MENU "DrawObject"
#define THEMES_MENU "Themes"

// Default URI schemes / types
#define TYPE_URI_DATA "data:"
#define TYPE_URI_ACTION "action:"
#define TYPE_URI_SETTINGS "settings:"


// Basic GUI and Item types
#define BUTTON_ITEM "BUTTON_ITEM"
#define TEXT_ITEM "TEXT_ITEM"
#define DRAW_ITEM "DRAW_ITEM"
#define FOCUS_ITEM_HACK "FOCUS_ITEM_HACK"

// Layer types
#define TYPE_LAYERMANAGER "LayerManager"
#define TYPE_VIEWLAYER "ViewLayer"
#define TYPE_LISTLAYER "ListLayer"
#define TYPE_BUTTONLAYER "ButtonLayer"
#define TYPE_TEXTLAYER "TextLayer"

// Layout types
#define GUI_LAYOUT "Layout"
#define GUI_LAYOUT_LAYERMANAGER "LayerManager_Layout"
#define GUI_LAYOUT_VIEW "ViewLayer_Layout"
#define GUI_LAYOUT_LIST "List_Layout"
#define GUI_LAYOUT_FRAMEDLIST "ListLayer_Layout"
#define GUI_LAYOUT_BUTTON "ButtonLayer_Layout"
#define GUI_LAYOUT_CONTROLID "ControlID_Layout"
#define GUI_LAYOUT_FRAME "Frame_Layout"
#define GUI_LAYOUT_BACKGROUND "Background_Layout"
#define GUI_LAYOUT_TEXT "TextLayer_Layout"

// Drawer types
#define GUI_DRAWER_LIST "GUI_DRAWER_LIST"
#define GUI_DRAWER_BUTTON "GUI_DRAWER_BUTTON"
#define GUI_DRAWER_FRAME "GUI_DRAWER_FRAME"
#define GUI_DRAWER_BACKGROUND "GUI_DRAWER_BACKGROUND"
#define GUI_DRAWER_TEXT "GUI_DRAWER_TEXT"

// Data types
#define GUI_COLOR_FRAME "GUI_COLOR_FRAME"
#define GUI_COLOR_BACKGR_LIST "GUI_COLOR_BACKGR_LIST"
#define GUI_COLOR_BACKGR_BUTTON "GUI_COLOR_BACKGR_BUTTON"
#define GUI_COLOR_FONT "GUI_COLOR_FONT"

#define GUI_RECT_SIZEANDPOS "SizeAndPosition"

// Special Content (Layout) fields for lists and buttons
#define LST_FIELD_NR "LST_FIELD_"
#define BTN_FIELD_NAME "BTN_FIELD_NAME"
#define BTN_FIELD_ICON "BTN_FIELD_ICON"
#define BTN_FIELD_CONTROL "BTN_FIELD_CONTROL"
#define BTN_FIELD_PREVIEW "BTN_FIELD_PREVIEW"
#define BTN_FIELD_DESCRIPTION "BTN_FIELD_DESCRIPTION"


// Name of the Functoid containing the Theme object and the drawing func
#define DRAWOBJ_DRAWER "Drawer"

#endif /* HAND_DEFINES_H */
