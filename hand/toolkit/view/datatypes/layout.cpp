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
    Type(LAYOUT);
    if(type != "")
        Vertex::Get(REQUEST)->Get(type);
    // Colours.Set(r[ed], g[reen], b[lue])
/*    Color_Frame.Set(0, 3, 200);
    Color_Background.Set(0, 0, 0);
    Color_Alt_Frame.Set(0, 3, 232);
    Color_Alt_Background.Set(128, 128, 128);*/

//    SizeAndPosition.Set(0.0, 0.0, 1.0, 1.0);

//    Background = SOLID_COLOR;
}


bool Layout::Add(Vertex* child)
{
    if(!child->Is(LAYOUT))
        return List::Add(child);

    Get(CHILDREN)->Add(child);
    return child->Get(PARENT)->Set(this);
}


Vertex* Layout::Get(string type, string name)
{
    Vertex* ret = List::Get(type, name);
    if(ret || (type!=LAYOUT))
        return ret;

    // Check if the requested layout is a field and has to be created
    Vertex* f = Get("Fields")->Get(LIST, name);
    if(f)
    {
        // Creator mode
        ret = new Layout("Created", "");
        Vertex* reqs = ret->Vertex::Get(REQUEST);
        uint i = 0;
        Vertex* sub_type;
        while((sub_type=f->Get(++i)) != 0)
            reqs->Attach(sub_type);
        Get("Update")->Attach(ret);
        return ret;
    }

    Vertex* children = Get(LINK, CHILDREN);
    if(!children)
        return NULL;

    // "GetField" mode
    ret = children->Get(type, name);
    if(ret)
    {
        Get("Update")->Attach(ret);
        return ret;
    }

    Vertex* child;
    uint i = 0;
    while((child=children->Get(++i)) != NULL)
    {
        ret = child->Get(type, name);
        if(ret)
        {
            Get("Update")->Attach(child);
            return ret;
        }
    }

    return NULL;
}


bool Layout::Execute(Vertex* vs)
{
    // Set the surface in the layout and use the layout
    // as parameter for the drawer
    Get("Surface")->Set(vs);
    Vertex* f = Get("Methods")->Get("DrawFunc")->Get();
    if(f)
        // Execute drawer on current layout
        f->Execute(this);
    f = Get(LINK, CHILDREN);
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
    Vertex* children = Get(LINK, CHILDREN);
    if(!children)
        return;
    Vertex* layout;
    uint i = 0;
    while((layout=children->Get(++i)) != NULL)
        layout->Reset();
}
