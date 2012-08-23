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


TextLayer::TextLayer() : Layer()
{
    BufferType = OVERLAY;
    SetType(TEXTLAYER);
}


TextLayer::~TextLayer()
{
}


void TextLayer::Configure(Functoid* sub_layout)
{
    // Never called
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


bool TextLayerFactory::IsValidInput(Functoid* input)
{
    if(input)
        return true;
    return false;
}


Functoid* TextLayerFactory::Produce(Functoid* descriptor)
{
    Layer* ret = new TextLayer();
    return ret;
}


void TextLayerFactory::TakeBack(Functoid* product)
{
    // TODO: shouldn't delete objects of derived classes
    if(dynamic_cast<TextLayer*>(product))
        delete(product);
}
