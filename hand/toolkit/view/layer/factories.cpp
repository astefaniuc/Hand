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


#include "view/layer/factories.h"
#include "view/layer/buttonlayer.h"
#include "view/layer/listlayer.h"
#include "view/layer/textlayer.h"


using namespace std;


bool ButtonLayerFactory::execute(Vertex* input)
{
    return input->Vertex::set(new ButtonLayer(BUTTON));
}


bool ButtonLayerFactory::IsValidInput(Vertex* input)
{
    if(input && input->is(METHOD))
        return true;
    return false;
}

// ----------------------------------------------------------------

bool ListLayerFactory::execute(Vertex* input)
{
    return input->Vertex::set(new ListLayer(LIST));
}


bool ListLayerFactory::IsValidInput(Vertex* input)
{
    if(input && input->is(LIST))
        return true;
    return false;
}

// ----------------------------------------------------------------

bool TextLayerFactory::execute(Vertex* input)
{
    return input->Vertex::set(new TextLayer(TEXT));
}


bool TextLayerFactory::IsValidInput(Vertex* input)
{
    if(input && input->is(NOTE))
        return true;
    return false;
}
