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


#define VIRTUALSURFACE "VIRTUALSURFACE"
// Layer types
#define LAYERMANAGER "LayerManager"
#define VIEWLAYER "ViewLayer"
#define LISTLAYER "ListLayer"
#define BUTTONLAYER "ButtonLayer"
#define TEXTLAYER "TextLayer"
#define FOCUSLAYER "FocusLayer"

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

// Other
#define TAG_RELATION_CONTENT "TAG_RELATION_CONTENT"

// Special Content (Layout) fields for lists and buttons
#define LST_FIELD_NR "LST_FIELD_"
#define BTN_FIELD_NAME "BTN_FIELD_NAME"
#define BTN_FIELD_ICON "BTN_FIELD_ICON"
#define BTN_FIELD_CONTROL "BTN_FIELD_CONTROL"
#define BTN_FIELD_PREVIEW "BTN_FIELD_PREVIEW"
#define BTN_FIELD_DESCRIPTION "BTN_FIELD_DESCRIPTION"


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


#endif /* VIEW_DEFINES_H */
