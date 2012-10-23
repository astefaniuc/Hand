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

#include "view/layer/textlayer.h"


using namespace std;


TextLayer::TextLayer(string name) : Layer(name)
{
    BufferType = OVERLAY;
    Type(TEXTLAYER);
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


bool TextLayerFactory::Execute(Vertex* tree)
{
    Vertex* layout = tree->Vertex::Get(LAYOUT, ANY);
    layout->Vertex::Delete(layout->Vertex::Get(REQUEST));
    layout->Vertex::Get(REQUEST)->Get(LAYOUT_TEXT);
    return tree->Vertex::Set(new TextLayer(TEXTLAYER));
}


bool TextLayerFactory::IsValidInput(Vertex* input)
{
    if(input->Is(NOTE) &&
            input->Vertex::Get(LAYOUT, ANY)->Vertex::Get(REQUEST)->Get(ANY, LAYOUT_TEXT))
        return true;
    return false;
}
