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

#include "view/datatypes/layout.h"
#include "view/datatypes/rect.h"


using namespace std;


Layout::Layout(string name, string type) : List(name)
{
    Type(GUI_LAYOUT);
    Type(type);
    Set(new Rect(SIZEANDPOSITION, 0, 0, 1, 1));
    // Colours.Set(r[ed], g[reen], b[lue])
/*    Color_Frame.Set(0, 3, 200);
    Color_Background.Set(0, 0, 0);
    Color_Alt_Frame.Set(0, 3, 232);
    Color_Alt_Background.Set(128, 128, 128);*/

//    SizeAndPosition.Set(0.0, 0.0, 1.0, 1.0);

//    Background = SOLID_COLOR;
}


List* Layout::AddField(string name, string type)
{
    List* field = new List(name);
    field->Type(type);
    field->Get("Layout")->Set(this);
    Get("Fields")->Add(field);
    return field;
}


List* Layout::GetField(string position)
{
    List* s = dynamic_cast<List*>(
            Get("Fields")->Get(ANY, position));
    if(s)
        return s;
    Vertex* childs = Get(RELATION, CHILDREN);
    if(childs)
    {
        Vertex* layout;
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


void Layout::AddForUpdate(Vertex* sublayout)
{
    Get("Update")->Add(sublayout);

    Layout* parent = dynamic_cast<Layout*>(Get(PARENT)->Get());
    if(parent)
        parent->AddForUpdate(this);
}


bool Layout::Execute(Vertex* vs)
{
    // Set the surface in the layout and use the layout
    // as parameter for the drawer
    Get("Surface")->Set(vs);
    Vertex* f = Get("Methods")->Get(ANY, "DrawFunc");
    if(f)
        // Execute drawer on current layout
        f->Execute(this);
    f = Get(RELATION, CHILDREN);
    if(f)
    {
        Vertex* layout;
        uint i = 0;
        while((layout=f->Get(++i)) != NULL)
            // Ignored parameter
            layout->Execute(vs);
    }
    return true;
}


void Layout::Reset()
{
    Vertex* sub = Get(RECT, SIZEANDPOSITION);
    if(sub)
        sub->Reset();
    Vertex* children = Get(RELATION, CHILDREN);
    if(!children)
        return;
    Vertex* layout;
    uint i = 0;
    while((layout=children->Get(++i)) != NULL)
        layout->Reset();
}
