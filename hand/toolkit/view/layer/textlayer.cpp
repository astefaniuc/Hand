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
    type(TEXT);
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


bool TextLayerFactory::execute(Vertex* tree)
{
    Vertex* types = tree->Vertex::get(REQUEST);
    types->reset();
    types->get(TEXT);
    return tree->get(TARGET)->get()->Vertex::set(new TextLayer(TEXT));
}


bool TextLayerFactory::IsValidInput(Vertex* input)
{
    Vertex* tgt = input->get(TARGET)->get();
    if(tgt && tgt->is(NOTE) && input->Vertex::get(REQUEST)->get(ANY, TEXT))
        return true;
    return false;
}
