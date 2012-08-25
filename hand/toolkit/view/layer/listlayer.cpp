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

#include "view/layer/listlayer.h"
#include "graph/data.h"


using namespace std;


ListLayer::ListLayer() : Layer()
{
    BufferType = OVERLAY;
    SetType(LISTLAYER);
}


ListLayer::~ListLayer()
{
}


void ListLayer::SetContent(Vertex* data)
{
    // Connect list and layer
    Layer::SetContent(data);

    uint nr_of_childs = data->GetSize();
    nr_of_childs--;
    Data<uint>* max_c = dynamic_cast<Data<uint>*>(Get("Layout")->Get(1)->Get("MaxSize"));
    if(max_c)
    {
        uint max_size = max_c->Get();
        if(max_size < nr_of_childs)
            nr_of_childs = max_size;
    }

    Vertex* child;
    uint i = 0;
    uint j = 0;
    // Now create layout
    while((child=data->Get(++i)) != NULL)
    {
        // Create the sub-objects
        // Check if the Layer supports insertion at position
//        if(Insert(child, GetFieldName(j)))
        if(Insert(child, "ListElement"))
            j++;
    }
}


string ListLayer::GetFieldName(uint position)
{
    // Don't do any error checking here
    stringstream s;
    s << LST_FIELD_NR << position;
    return s.str();
}


void ListLayer::Configure(Vertex* sub_layout)
{
    // Inherit parent settings
/*    GetSubRect(sub_layout->Position, sub_layout->SizeAndPosition);
    FlipAlignment(sub_layout->Alignment);*/
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


bool ListLayerFactory::IsValidInput(Vertex* input)
{
    if(input->IsType(LIST))
        return true;
    return false;
}


Vertex* ListLayerFactory::Produce(Vertex* descriptor)
{
    Layer* ret = new ListLayer();
    return ret;
}


void ListLayerFactory::TakeBack(Vertex* product)
{
    // TODO: shouldn't delete objects of derived classes
    if(dynamic_cast<ListLayer*>(product))
        delete(product);
}
