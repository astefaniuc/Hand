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


ListLayer::ListLayer(string name) : Layer(name)
{
    BufferType = OVERLAY;
    Type(LISTLAYER);
}


ListLayer::~ListLayer()
{
}


void ListLayer::SetContent(Vertex* data)
{
    // Connect list and layer
    Layer::SetContent(data);

    uint nr_of_childs = data->Size();
    nr_of_childs--;
    Data<uint>* max_c = dynamic_cast<Data<uint>*>(Get("Layout")->Get()->Get("MaxSize"));
    if(max_c)
    {
        uint max_size = max_c->Get();
        if(max_size < nr_of_childs)
            nr_of_childs = max_size;
    }

    Vertex* child;
    uint i = 0;
    // Now create layout
    while((child=data->Get(++i)) != NULL)
        // Create the sub-objects
        // Check if the Layer supports insertion at position
        Insert(child, "ListElement");
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


bool ListLayerFactory::Execute(Vertex* tree)
{
    return tree->Vertex::Add(new ListLayer(LISTLAYER));
}
