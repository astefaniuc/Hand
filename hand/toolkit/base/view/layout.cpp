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

#include "layout.h"
#include "rect.h"


using namespace std;


Layout::Layout(string name, string type) : FunctoidList(name)
{
    SetType(GUI_LAYOUT);
    SetType(type);
    Set(new Rect(GUI_RECT_SIZEANDPOS, 0, 0, 1, 1));
    // Colours.Set(r[ed], g[reen], b[lue])
/*    Color_Frame.Set(0, 3, 200);
    Color_Background.Set(0, 0, 0);
    Color_Alt_Frame.Set(0, 3, 232);
    Color_Alt_Background.Set(128, 128, 128);*/

//    SizeAndPosition.Set(0.0, 0.0, 1.0, 1.0);

//    Background = SOLID_COLOR;
}


FunctoidList* Layout::AddField(string name, string type)
{
    FunctoidList* field = new FunctoidList(name);
    field->SetType(type);
    field->Get("Layout")->Set(this);
    Get("Fields")->Add(field);
    return field;
}


FunctoidList* Layout::GetField(string position)
{
    FunctoidList* s = dynamic_cast<FunctoidList*>(
            Get("Fields")->Get(IGNORE, position));
    if(s)
        return s;
    Functoid* childs = Get(FUNCTOIDRELATION, TAG_RELATION_CHILD);
    if(childs)
    {
        Functoid* layout;
        uint i = 0;
        while((layout=childs->Get(++i)) != NULL)
        {
            s = ((Layout*)layout)->GetField(position);
            if(s)
                return s;
        }
    }
    return NULL;
}


void Layout::AddForUpdate(Functoid* sublayout)
{
    Get("Update")->Add(sublayout);

    Layout* parent = dynamic_cast<Layout*>(Get(TAG_RELATION_PARENT)->Get(1));
    if(parent)
        parent->AddForUpdate(this);
}


bool Layout::Execute(Functoid* vs)
{
    // Set the surface in the layout and use the layout
    // as parameter for the drawer
    Get("Surface")->Set(vs);
    Functoid* f = Get("Methods")->Get(IGNORE, "DrawFunc");
    if(f)
        // Execute drawer on current layout
        f->Execute(this);
    f = Get(FUNCTOIDRELATION, TAG_RELATION_CHILD);
    if(f)
    {
        Functoid* layout;
        uint i = 0;
        while((layout=f->Get(++i)) != NULL)
            // Ignored parameter
            layout->Execute(vs);
    }
    return true;
}


void Layout::Reset()
{
    Rect* sub = dynamic_cast<Rect*>(Get(GUI_RECT_SIZEANDPOS));
    if(sub)
        // Reset size and position to factory settings
        sub->Reset();
    Functoid* children = Get(FUNCTOIDRELATION, TAG_RELATION_CHILD);
    if(!children)
        return;
    Layout* layout;
    uint i = 0;
    while((layout=dynamic_cast<Layout*>(children->Get(++i))) != NULL)
        if(layout)
            layout->Reset();
}
