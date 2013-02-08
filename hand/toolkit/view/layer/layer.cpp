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
#include "view/datatypes/rect.h"
#include "view/datatypes/layout.h"
#include "base/factory.h"


using namespace std;


Layer::Layer(string name) : VirtualSurface(name)
{
    ParentLayer = NULL;
    Updated = false;
    IsVisible = true;
    IsExpanded = false;
    type(LAYER);
}


Layer::~Layer()
{
    Collapse();
}


bool Layer::Exit()
{
    // Check if the default location is currently active
    return ParentLayer->Exit();
}


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
    get(CHILDREN)->reset();
    get(CONTENT)->set(data);
    Changed = true;
}


Vertex* Layer::GetContent()
{
    return get(CONTENT)->get();
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


bool Layer::SetCommand(Vertex* cmd)
{
    if(!cmd)
        return false;
    Vertex* target = get(EXECUTE)->get();
    if(!target)
        return false;
    if(cmd->Vertex::get(METHOD)->set(target))
        target->get(COMMAND)->set(cmd);
    return true;
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


void Layer::NotifyChanged()
{
    Changed = true;
}


bool Layer::Request(Vertex* req)
{
    return ParentLayer->Request(req);
}


Layer* Layer::Insert(Vertex* data, string position)
{
    if(!data)
        return NULL;

    Layer* sub_layer = GetLayer(data, position);
    if(!sub_layer)
        return NULL;

    get(CHILDREN)->add(sub_layer);
    sub_layer->SetParent(this);

    sub_layer->Insert(data);

    return sub_layer;
}


void Layer::Insert(Vertex* data)
{
    SetLayout(GetLayout(data));

    SetContent(data);
}


Layer* Layer::GetLayer(Vertex* data, string position)
{
    if(!data)
        return NULL;

    Vertex* field = get(LAYOUT, ANY)->get(FIELD, position);
    if(!field)
        return NULL;
    Vertex* req = field->Vertex::get(REQUEST)->get();

    FactoryMap* layer_factories = dynamic_cast<FactoryMap*>(get(FACTORYMAP, LAYER_FACTORIES));
    Factory* factory = NULL;

    // Get the Layer and basic Layout type
    if(req->get()->name() == ANY)
    {
        factory = layer_factories->GetFactory(data);
        if(!factory)
            factory = dynamic_cast<Factory*>(layer_factories->get(FACTORY, ANY));
        req = req->get(factory->get(OUTPUTTYPE)->get()->name());
    }
    else if(req->size() > 1)
    {
        Vertex* tmp_req;
        uint i = 0;
        while((tmp_req=req->get(++i)) != NULL)
        {
            factory = layer_factories->GetFactory(tmp_req->name());
            if(factory && factory->IsValidInput(data))
            {
                req = tmp_req;
                break;
            }
            else
                factory = NULL;
        }
    }

    if(!factory)
    {
        factory = layer_factories->GetFactory(req->get()->name());
        if(!factory)
            return NULL;
    }

    // Create the Layer
    factory->execute(data);
    Vertex* layer = data->Vertex::get(LAYER, ANY);
    // The position is needed further for layout handling
    layer->name(position);
    return dynamic_cast<Layer*>(layer);
}


Vertex* Layer::GetLayout(Vertex* data)
{
    // Get the detailed Layout type
    // Check if there is a layout or layout request attached to the data
    Vertex* layout_data = data->Vertex::get(ANY, LAYOUT);
    string layer_type = type();
    if(layout_data)
    {
        layout_data = layout_data->get(ANY, layer_type);
        if(layout_data)
        {
            layout_data = layout_data->get();
        }
    }

    Vertex* layout = get(THEME)->get(THEME, ANY)->get(ANY, LAYOUT)->get(layer_type);
    Vertex* field = Parent->get(LAYOUT, ANY)->get(FIELD, name());

    // TODO: Vertex::get(Vertex* path)
    Vertex* tmp_repo;
    Vertex* req = field->Vertex::get(REQUEST)->get(layer_type);
    while((req=req->get()) != NULL)
    {
        if(layout_data)
            layout_data = layout_data->get(ANY, req->name());
        tmp_repo = layout->get(ANY, req->name());
        if(!tmp_repo)
            break;
        layout = tmp_repo;
    }
    if(layout_data)
        layout = layout_data;

    if(layout->is(FACTORY))
        layout = layout->get();
    layout->get(TARGET)->set(data);

    return layout;
}


void Layer::SetLayout(Vertex* layout)
{
    // TODO: custom layouts shouldn't be deleted with the layer
    add(layout);
    get(THEME)->get(THEME, ANY)->execute(layout);

    // Add to the update tree
    layout->set(Parent->get(LAYOUT, ANY)->get(FIELD, name())->get(PARENT));
    Vertex* parent_layout;
    // Bridge also any intermediate lists
    while((parent_layout=layout->get(PARENT)->get()) != NULL)
    {
        if(!parent_layout->get(TOUPDATE)->attach(layout))
            break;
        layout = parent_layout;
    }
}


void Layer::Draw(bool forced)
{
/*  if(BufferType == NONE)
    {
        // TODO: this crashes in LayerManager during "write through"
        Parent->MapSurface(_Layout->Coordinates,
                            CoordinatesOnBuffer,
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
    Rel_Rect* sap = GetRect(COORDINATES, layout);
    if(sap)
        tmp = *sap;

    Show(&CoordinatesOnBuffer, &tmp);

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
