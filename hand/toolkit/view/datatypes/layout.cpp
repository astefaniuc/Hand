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


Layout::Layout(string name, string stype) : List(name)
{
    type(LAYOUT);
    if(stype != "")
        Vertex::get(REQUEST)->get(stype);
}


bool Layout::add(Vertex* child)
{
    if(!child->is(LAYOUT))
        return List::add(child);

    get(CHILDREN)->add(child);
    return child->get(PARENT)->set(this);
}


Vertex* Layout::get(string type, string name)
{
    Vertex* ret = List::get(type, name);
    if(ret)
        return ret;

    if(type == LAYOUT)
    {
        ret = new Layout(name, name);
        add(ret);
        return ret;
    }

    if(type == FIELD)
    {
        // "GetField" mode
        Vertex* f = get(FIELDS)->get(ANY, name);
        if(f)
        {
            if(f->is(LAYOUT))
                ret = f;
            else
            {
                // Creator mode
                ret = new Layout(name, "");
                ret->get(PARENT)->set(this);
                Vertex* reqs = ret->Vertex::get(REQUEST);
                uint i = 0;
                Vertex* sub_type;
                while((sub_type=f->get(++i)) != NULL)
                    reqs->attach(sub_type);
            }
            return ret;
        }

        Vertex* children = get(LINK, CHILDREN);
        if(!children)
            return NULL;

        ret = children->get(type, name);
        if(ret)
            return ret;

        Vertex* child;
        uint i = 0;
        while((child=children->get(++i)) != NULL)
        {
            ret = child->get(type, name);
            if(ret)
                return ret;
        }
    }

    return NULL;
}


bool Layout::execute(Vertex* vs)
{
    // Set the surface in the layout and use the layout
    // as parameter for the drawer
    set(vs);
    Vertex* f = get(DRAWER)->get();
    if(f)
        // Execute drawer on current layout
        f->execute(this);
    f = get(LINK, CHILDREN);
    if(f)
    {
        Vertex* layout;
        uint i = 0;
        while((layout=f->get(++i)) != NULL)
            // Ignored parameter
            layout->execute(vs);
    }
    return true;
}


void Layout::reset()
{
    Vertex* sub = get(SIZEANDPOSITION)->get();
    if(sub)
        sub->reset();
    Vertex* children = get(LINK, CHILDREN);
    if(!children)
        return;
    Vertex* layout;
    uint i = 0;
    while((layout=children->get(++i)) != NULL)
        layout->reset();
}
