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

#ifndef VIEW_DEFINES_H
#define VIEW_DEFINES_H


// TODO: font engine
#define FONT_FILE "/usr/share/fonts/truetype/freefont/FreeSans.ttf"

// Themes strings
#define THEMES_DIRECTORY "./apps/themes/"
#define DEFAULT_THEME "default.so"
// Menu names
#define THEMES_MENU "Themes"



#define VIRTUALSURFACE "VIRTUALSURFACE"
// Layout and layers
#define LAYER "Layer"
#define LAYERMANAGER "LayerManager"
#define LAYOUT "Layout"
#define VIEW "View"
#define BUTTON "Button"
#define TEXT "Text"
#define FOCUS "Focus"

#define FRAMEDLIST "Framed list"
#define CONTROLID "Control ID"
#define FRAME "Frame"
#define BACKGROUND "Background"

#define LAYER_FACTORIES "Layer Factories"

// Drawer types
#define GUI_DRAWER_LIST "GUI_DRAWER_LIST"
#define GUI_DRAWER_BUTTON "GUI_DRAWER_BUTTON"
#define GUI_DRAWER_FRAME "GUI_DRAWER_FRAME"
#define GUI_DRAWER_BACKGROUND "GUI_DRAWER_BACKGROUND"
#define GUI_DRAWER_TEXT "GUI_DRAWER_TEXT"

// Data types
// Colors
#define GUI_COLOR_FRAME "GUI_COLOR_FRAME"
#define GUI_COLOR_BACKGR_LIST "GUI_COLOR_BACKGR_LIST"
#define GUI_COLOR_BACKGR_BUTTON "GUI_COLOR_BACKGR_BUTTON"
#define GUI_COLOR_FONT "GUI_COLOR_FONT"
// Positions
#define SIZEANDPOSITION "SizeAndPosition"
#define ALIGNMENT "ALIGNMENT"
#define GUI_RECT_FULL "GUI_RECT_FULL"
// Font
#define DIM_FONT_PREFERRED "DIM_FONT_PREFERRED"
#define DIM_FONT_MAX "DIM_FONT_MAX"

// TODO
#define BGRD_SOLID_COLOR "BGRD_SOLID_COLOR"
#define BGRD_IMAGE "BGRD_IMAGE"


// Special Content (Layout) fields for lists and buttons
#define LST_FIELD_NR "LST_FIELD_"
#define BTN_FIELD_NAME "BTN_FIELD_NAME"
#define BTN_FIELD_ICON "BTN_FIELD_ICON"
#define BTN_FIELD_CONTROL "BTN_FIELD_CONTROL"
#define BTN_FIELD_PREVIEW "BTN_FIELD_PREVIEW"
#define BTN_FIELD_DESCRIPTION "BTN_FIELD_DESCRIPTION"


// Name of the Vertex containing the Theme object and the drawing func
#define DRAWOBJ_DRAWER "Drawer"



enum button_string
{
    BTN_NAME = 1,
    BTN_DESCRIPTION = 2,
    BTN_ICON = 4,
    BTN_CONTROL = 8,
    BTN_PREVIEW = 16
};


enum buffer_type
{
    NONE,
    OVERLAY,
    COLLECTOR
};


enum maximum_number_of_buttons
{
    MAX_NUMBER_OF_BUTTONS = 5
};

#endif /* VIEW_DEFINES_H */
