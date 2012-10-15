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
    Type(LAYER);
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
    Get("Content")->Set(data);
    Changed = true;
}


Vertex* Layer::GetContent()
{
    return Get("Content")->Get();
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
    Vertex* children = Get(LINK, CHILDREN);
    if(!children)
        return;

    Vertex* child;
    while((child=children->Get(1)) != NULL)
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
    // TODO: check if there is already a Layer at searched
    // position and if it has the right type
    // TODO: delete / (re)create layer if needed
    Layout* layout = dynamic_cast<Layout*>(Get(LAYOUT)->Get());
    // Is the layer type restricted by the current layout?
    if(!layout)
        return NULL;

    List* field = layout->GetField(position);
    if(!field)
        return NULL;

    data->Vertex::Get(OUTPUTTYPE)->Set(field->Get("Layer Type")->Get());
    // TODO: generic factory
    Layer* sub_layer = dynamic_cast<LayerManager*>(Vertex::Get(LAYERMANAGER)->Get())
            ->CreateLayer(data);
    if(!sub_layer)
        return NULL;

    // Connect the components
    // on Layer/VS level
    Get(CHILDREN)->Add(sub_layer);
    sub_layer->SetParent(this);
    // on Layout level
    Layout* field_layout = dynamic_cast<Layout*>(field->Get(LAYOUT)->Get());
    if(field_layout)
        field_layout->AddForUpdate(sub_layer->Get(LAYOUT)->Get());

    return sub_layer;
}


void Layer::SetTheme(Vertex* theme)
{
    // TODO: generic recursive Set() on a sub-tree defined by a "relation"
    // (possibly using Search() with MultipleFindings)
    SetLayout(theme);
    Vertex* children = Get(LINK, CHILDREN);
    if(!children)
        return;

    Layer* layer;
    Vertex* child;
    uint i = 0;
    while((child=children->Get(++i)) != NULL)
    {
        layer = dynamic_cast<Layer*>(child);
        if(layer)
            layer->SetTheme(theme);
    }
}


void Layer::SetLayout(Vertex* drawer_lib)
{
    // Delete from the layout items owned by the previous theme
    Vertex* layout = Get("Layout")->Get();
    if(!layout)
    {
        layout = new Layout("Layout", Type()+"_Layout");
        Get("Layout")->Set(layout);
    }
    drawer_lib->Execute(layout);
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
    Layout* layout = dynamic_cast<Layout*>(Get("Layout")->Get());
    if(!layout)
        return;

    layout->Execute(this);

    if(BufferType == COLLECTOR)
        // Draw first the child on the buffer
        DrawChilds(forced);

    Rect tmp;
    Rect* sap = GetRect(SIZEANDPOSITION, layout);
    if(sap)
        tmp.Init(sap->x, sap->y, sap->w, sap->h);

    Show(&SizeAndPositionOnBuffer, &tmp);

    if(BufferType == OVERLAY)
    {
        // Draw childs afterwards (not buffered)
        DrawChilds(forced);
//        Updated = true;
    }
    // Clean-up temporary size and position values
    layout->Reset();
}


void Layer::DrawChilds(bool forced)
{
    Vertex* children = Get(LINK, CHILDREN);
    if(!children)
        return;

    Layer* layer;
    Vertex* child;
    uint i = 0;
    while((child=children->Get(++i)) != NULL)
    {
        layer = dynamic_cast<Layer*>(child);
        if(!layer)
            continue;
        layer->SetSize(GetSize());
        layer->Update(forced);
    }
}
