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


Layout::Layout(string name) : List(name)
{
    type(LAYOUT);
    add(new FieldsContainer(this));
}


bool Layout::add(Vertex* child)
{
    if(!child->is(LAYOUT))
        return List::add(child);

    get(FIELDS)->add(child);
    return child->Vertex::get(PARENT)->set(this);
}


Vertex* Layout::get(string type, string name)
{
    Vertex* ret = List::get(type, name);
    if(ret)
        return ret;

    if(type != FIELD)
        return NULL;

    // "GetField" mode
    // Skip layouts from different layers (fields)
    Vertex* fields = get(FIELDS)->Vertex::get(PUBLIC);

    ret = fields->get(ANY, name);
    if(ret)
        return ret;

    Vertex* child;
    uint i = 0;
    while((child=fields->get(++i)) != NULL)
    {
        ret = child->get(type, name);
        if(ret)
            return ret;
    }

    return NULL;
}


bool Layout::execute(Vertex* vs)
{
    // Set the surface in the layout and use the layout
    // as parameter for the drawer
    set(vs);
    Vertex* drawer = get(DRAWER)->get();
    if(drawer)
        // Execute drawer on current layout
        drawer->execute(this);

    // Skip layouts from different layers (fields)
    Vertex* fields = get(FIELDS)->Vertex::get(PUBLIC);
    Vertex* layout;
    uint i = 0;
    while((layout=fields->get(++i)) != NULL)
        // Ignored parameter
        layout->execute(vs);

    return true;
}


void Layout::reset()
{
    Vertex* coords = get(COORDINATES)->get();
    if(coords)
        coords->reset();

    Vertex* fields = get(FIELDS);
    Vertex* layout;
    uint i = 0;
    while((layout=fields->get(++i)) != NULL)
        layout->reset();
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


Vertex* FieldsContainer::get(uint i)
{
    Vertex* sub;
    uint found = 0;
    uint j = 0;
    while((sub=List::get(++j)) != NULL)
    {
        if(sub->is(LAYOUT))
        {
            found++;
            if(found == i)
                return sub;
        }
        else
        {
            if((found + sub->size()) >= i)
                return sub->get(i - found);
            found += sub->size();
        }
    }
    return NULL;
}


Vertex* FieldsContainer::get(string name)
{
    Vertex* ret = get(ANY, name);
    if(!ret)
    {
        // Creates a new vertex
        ret = List::get(name);
        ret->Vertex::set(Vertex::get(PARENT));
    }
    return ret;
}


uint FieldsContainer::size()
{
    Vertex* sub;
    uint found = 0;
    uint j = 0;
    while((sub=List::get(++j)) != NULL)
        if(sub->is(LAYOUT))
            found++;
        else
            found += sub->size();

    return found;
}
