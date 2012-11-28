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
#include "input/node.h"
#include "view/datatypes/rect.h"
#include "view/datatypes/layout.h"


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
    Vertex* child_layout;
    Vertex* field = curr_layout->get(FIELD, position);
    if(!field)
        return NULL;

    FactoryMap* layer_factories = dynamic_cast<FactoryMap*>(get(FACTORYMAP, LAYER_FACTORIES));
    Factory* factory = NULL;


    // Get the Layer and basic Layout type
    Vertex* req = field->Vertex::get(REQUEST)->get();
    Vertex* repo = get(THEME)->get()->get(ANY, LAYOUT);
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
        req = req->get();
        factory = layer_factories->GetFactory(req->name());
        if(!factory)
            return NULL;
    }


    // Get the detailed Layout type
    // Check if there is a layout or layout request attached to the data
    Vertex* data_layout = data->Vertex::get(ANY, LAYOUT);
    if(data_layout)
    {
        data_layout = data_layout->get(ANY, req->name());
        if(data_layout)
        {
            data_layout = data_layout->get();
        }
    }

    repo = repo->get(factory->get(OUTPUTTYPE)->get()->name());

    // TODO: Vertex::get(Vertex* path)
    Vertex* tmp_repo;
    while((req=req->get()) != NULL)
    {
        if(data_layout)
            data_layout = data_layout->get(ANY, req->name());
        tmp_repo = repo->get(ANY, req->name());
        if(!tmp_repo)
            break;
        repo = tmp_repo;
    }
    if(data_layout)
        repo = data_layout;

    if(repo->is(FACTORY))
        child_layout = repo->get();
    else
        child_layout = repo;
    child_layout->get(TARGET)->set(data);


    if(factory->get(OUTPUTTYPE)->get()->name() == LIST)
    {
        // The LIST needs two different SIZEANDPOSITION rects: one for the blit
        // on the parent surface and one to calculate the children
        Vertex* buffer_layout = new Layout("Buffer");
        buffer_layout->get(SIZEANDPOSITION)->Vertex::get(REQUEST)->get(RECT)->get(FULL);
        buffer_layout->add(child_layout);
        child_layout = buffer_layout;
    }

    // Create the Layer
    factory->execute(data);
    Layer* sub_layer = dynamic_cast<Layer*>(data->Vertex::get(LAYER, ANY));
    if(!sub_layer)
        return NULL;

    sub_layer->set(get(THEME));
    sub_layer->SetLayout(child_layout);
    get(CHILDREN)->add(sub_layer);
    sub_layer->SetParent(this);
    sub_layer->set(layer_factories);
    sub_layer->SetContent(data);

    // Add to the update tree
    Vertex* parent_layout = field->get(PARENT)->get();
    parent_layout->get(TOUPDATE)->attach(child_layout);
    field = parent_layout;
    // Bridge also any intermediate lists
    while((parent_layout=field->get(PARENT)->get()) != NULL)
    {
        if(!parent_layout->get(TOUPDATE)->attach(field))
            break;
        field = parent_layout;
    }

    return sub_layer;
}


void Layer::SetLayout(Vertex* layout)
{
    set(layout);
    add(layout);
    get(THEME)->get()->execute(layout);
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
