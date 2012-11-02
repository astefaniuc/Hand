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


#define TARGET "target"
#define TOUPDATE "to update"

#define VIRTUALSURFACE "Virtualsurface"
// Layout and layers
#define LAYER "Layer"
#define LAYERMANAGER "LayerManager"
#define LAYOUT "Layout"
#define VIEW "View"
#define BUTTON "Button"
#define TEXT "Text"
#define FOCUS "Focus"

#define LAYER_FACTORIES "Layer Factories"


#define FRAMEDLIST "Framed list"
#define CONTROLID "Control ID"
#define FRAME "Frame"
#define BACKGROUND "Background"
#define FONT "Font"

// Name of the Vertex containing the Theme object and the drawing func
#define DRAWER "Drawer"
#define COLOR "Color"

// Positions
#define SIZEANDPOSITION "SizeAndPosition"
#define ALIGNMENT "alignment"
#define VERTICAL "vertical"
#define HORIZONTAL "horizontal"
#define SCALED "scaled"
#define SCALEDHORIZONTAL "scaled horizontal"
#define FULL "full"
// Font
#define DIM_FONT_PREFERRED "DIM_FONT_PREFERRED"
#define DIM_FONT_MAX "DIM_FONT_MAX"


// Special Content (Layout) fields for lists and buttons
#define NAME "Name"
#define ICON "ICON"
#define PREVIEW "Preview"


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
