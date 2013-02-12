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
#include "view/layer/layermanager.h"
#include "graph/method.h"
#include "graph/data.h"


using namespace std;


ListLayer::ListLayer(string name) : Layer(name)
{
    type(LIST);
    BufferType = COLLECTOR;
}


void ListLayer::Init()
{
    Vertex* focus = new Method<ListLayer>("SetFocus", this, &ListLayer::SetFocusControls);
    focus->get("Parameter")->set(Vertex::get(LAYERMANAGER)->get()->get(ANY, COMMANDS));
    get(EXECUTE)->set(focus);
}


void ListLayer::SetContent(Vertex* data)
{
    // Connect list and layer
    Layer::SetContent(data);

    uint nr_of_childs = data->size();
    nr_of_childs--;
    Data<uint>* max_c = dynamic_cast<Data<uint>*>(get(LAYOUT, ANY)->get("MaxSize"));
    if(max_c)
    {
        uint max_size = max_c->get();
        if(max_size < nr_of_childs)
            nr_of_childs = max_size;
    }

    Vertex* child;
    uint i = 0;
    while((child=data->get(++i)) != NULL)
        // Create the sub-objects
        Insert(child, ELEMENT);
}


bool ListLayer::SetFocusControls(Vertex* cmd)
{
    Vertex* children = get(LINK, CHILDREN);
    if(!children)
        return false;

    // Destroy the previous focus
    Vertex* foc_cmds = cmd->Vertex::get(DEVICE, ANY)->Vertex::get(FOCUS);
    uint i = 0;
    {
        Vertex* fc;
        while((fc=foc_cmds->get(++i)) != NULL)
        {
            fc->Vertex::detach(fc->Vertex::get(METHOD, ANY));
            delete(fc->get(VIEW)->Vertex::get(LAYER, CONTROLID));
        }
        foc_cmds->reset();
    }

    {
        Vertex* back = get(BACK)->get();
        if(back)
        {
            (dynamic_cast<LayerManager*>(Vertex::get(LAYERMANAGER)->get()))->GetCommand(back, 2);
            foc_cmds->attach(back->Vertex::get(COMMAND)->get());
        }
    }

    Layer* child;
    i = 0;
    Vertex* curr_cmd;
    while((child=dynamic_cast<Layer*>(children->get(++i))) != NULL)
    {
        curr_cmd = cmd->get(i);
        if(child->Layer::SetCommand(curr_cmd))
        {
            // Store current focus
            foc_cmds->attach(curr_cmd);
            // Add a back reference
            child->get(BACK)->set(get(EXECUTE)->get());
        }
    }
    return true;
}


Vertex* ListLayer::GetLayout(Vertex* data)
{
    // The LIST needs two different COORDINATES rects: one for the blit
    // on the parent surface and one to calculate the children
    Vertex* layout = get(THEME)->get(THEME, ANY)->get(LAYOUT)->get();
    layout->name("Buffer");
    layout->get(COORDINATES)->Vertex::get(REQUEST)->get(RECT)->get(FULL);
    layout->add(Layer::GetLayout(data));

    return layout;
}
