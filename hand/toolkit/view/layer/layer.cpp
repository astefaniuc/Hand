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

#include "view/layer/layer.h"
#include "view/theme/theme.h"
#include "input/node.h"
#include "view/datatypes/rect.h"
#include "view/theme/drawer.h"
#include "view/datatypes/layout.h"
#include "view/layer/layermanager.h"


using namespace std;


Layer::Layer(string name) : VirtualSurface(name)
{
    ParentLayer = NULL;
    Updated = false;
    IsVisible = true;
    IsExpanded = false;
    _Node = NULL;
    type(LAYER);
}


Layer::~Layer()
{
    ReleaseCommand();
    Collapse();
}


bool Layer::Exit()
{
    // Check if the default location is currently active
    return ParentLayer->Exit();
}

/*
bool Layer::Update(bool forced)
{
    if(!IsVisible)
    {
        return false;
    }
    if(_Layout->Update(forced))
    {
        forced = true;
    }
    return forced;
}
*/

bool Layer::Update(bool forced)
{
    if(Animations.Update(this))
        Changed = true;

    if(Changed || forced)
        Draw(forced);

    return Changed;
}


void Layer::SetContent(Vertex* data)
{
    // ReleaseContent
    get(CONTENT)->set(data);
    Changed = true;
}


Vertex* Layer::GetContent()
{
    return get(CONTENT)->get();
}


void Layer::SetCommand(Node* cmd)
{
    ReleaseCommand();
    if(cmd != NULL)
    {
        _Node = cmd;
        _Node->SetLayer(this);
    }
}


void Layer::ReleaseCommand()
{
    if(_Node != NULL)
    {
        //_Node_->ReleaseList();
        _Node->SetLayer(NULL);
        _Node = NULL;
    }
}


void Layer::Collapse()
{
    Vertex* children = get(LINK, CHILDREN);
    if(!children)
        return;

    Vertex* child;
    while((child=children->get(1)) != NULL)
        // Recursively deletes sub-layers
        delete child;

    IsExpanded = false;
}


// --- Getter and setter for external objects ---
void Layer::SetParentLayer(Layer* l)
{
    ParentLayer = l;
}

// TODO: any use for this?
Layer* Layer::GetParentLayer()
{
    return ParentLayer;
}


void Layer::PressGui()
{
    // TODO: modify layout (IsPressed = true)
    //Draw();
}

void Layer::ReleaseGui()
{
    // TODO: reset layout (IsPressed = false)
    //Draw();
}


void Layer::NotifyChanged()
{
    Changed = true;
}


uint Layer::GetLevel()
{
    if(ParentLayer != NULL)
        return (1 + ParentLayer->GetLevel());
    return 0;
}


bool Layer::Request(Vertex* req)
{
    return ParentLayer->Request(req);
}


Layer* Layer::Insert(Vertex* data, string position)
{
    Vertex* curr_layout = get(LAYOUT, ANY);
    // Get only a rump layout with size relative to the parent and the
    // list of supported layer types
    // This already connects all involved layouts and connects to the Theme
    Vertex* child_layout = curr_layout->get(LAYOUT, position);
    child_layout->name(data->name());
    Vertex* layer_factories = curr_layout->get(FACTORYMAP, LAYER_FACTORIES);

    // For use further down the spiral
    child_layout->set(layer_factories);
    child_layout->set(curr_layout->get("Theme"));

    data->Vertex::set(child_layout);
    child_layout->get(TARGET)->set(data);

    // Create the Layer
    layer_factories->execute(child_layout);
    Layer* sub_layer = dynamic_cast<Layer*>(data->Vertex::get(LAYER, ANY));
    if(!sub_layer)
        return NULL;

    sub_layer->SetLayout(child_layout);

    // Connect the components
    // on Layer/VS level
    get(CHILDREN)->add(sub_layer);
    sub_layer->SetParent(this);
    sub_layer->SetContent(data);

    return sub_layer;
}


void Layer::SetLayout(Vertex* layout)
{
    set(layout);
    add(layout);
    layout->get("Theme")->get()->execute(layout);
}


void Layer::Draw(bool forced)
{
/*  if(BufferType == NONE)
    {
        // TODO: this crashes in LayerManager during "write through"
        Parent->MapSurface(_Layout->SizeAndPosition,
                            SizeAndPositionOnBuffer,
                            GetBuffer());
        Updated = true; // ?
    }*/
    // Call the Theme function for drawing with the current settings
    Vertex* layout = get(LAYOUT, ANY);
    if(!layout)
        return;

    layout->execute(this);

    if(BufferType == COLLECTOR)
        // Draw first the child on the buffer
        DrawChilds(forced);

    Rel_Rect tmp;
    Rel_Rect* sap = GetRect(SIZEANDPOSITION, layout);
    if(sap)
        tmp = *sap;
        //tmp.Init(sap->x, sap->y, sap->w, sap->h);

    Show(&SizeAndPositionOnBuffer, &tmp);

    if(BufferType == OVERLAY)
    {
        // Draw childs afterwards (not buffered)
        DrawChilds(forced);
//        Updated = true;
    }
    // Clean-up temporary size and position values
    layout->reset();
}


void Layer::DrawChilds(bool forced)
{
    Vertex* children = get(LINK, CHILDREN);
    if(!children)
        return;

    Layer* layer;
    Vertex* child;
    uint i = 0;
    while((child=children->get(++i)) != NULL)
    {
        layer = dynamic_cast<Layer*>(child);
        if(!layer)
            continue;
        layer->SetSize(GetSize());
        layer->Update(forced);
    }
}
