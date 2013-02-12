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

#include "view/layer/buttonlayer.h"
#include "view/layer/textlayer.h"
#include "graph/method.h"
#include "graph/data.h"


using namespace std;


ButtonLayer::ButtonLayer(string name) : Layer(name)
{
    BufferType = COLLECTOR;
    type(BUTTON);
}


void ButtonLayer::SetContent(Vertex* data)
{
    Layer::SetContent(data);
    get(EXECUTE)->set(data);

    // Create the sub-elements of the button
    // The preview
/*    if(data->is(LIST))
    {
        SetExpandable();
        // Add list as preview
        Insert(data, PREVIEW);
    }*/

    Insert(data, NAME);
    Insert(data->Vertex::get(NOTE, DESCRIPTION), DESCRIPTION);
    Insert(data->Vertex::get(ANY, ICON), ICON);
    Vertex* cmd = data->Vertex::get(COMMAND)->get();
    if(cmd)
        Insert(cmd->get(VIEW), CONTROLID);
}


void ButtonLayer::SetExpandable()
{
    FunctionBox = new Method<ButtonLayer>("ExpandList", this, &ButtonLayer::ExpandList);
}


bool ButtonLayer::ExpandList(Vertex* ignore)
{
    if(ParentLayer->Request(GetContent()))
        return true;
    return false;
}
