#include "view/layer/layer.h"
#include "view/datatypes/rect.h"
#include "view/datatypes/layout.h"
#include "base/factory.h"


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
//    get(CHILDREN)->reset();
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
    while((child=children->get(1)) != nullptr)
        // Recursively deletes sub-layers
        delete child;

    IsExpanded = false;
}


bool Layer::SetCommand(Vertex* cmd)
{
    if(!cmd)
        return false;

    Vertex* target = get(EXECUTE)->get();
    if(!cmd->set(target))
        return false;
    target->Vertex::get(COMMAND)->set(cmd);

    Insert(cmd->get(VIEW), CONTROLID);

    return true;
}


bool Layer::Request(Vertex* req)
{
    return ParentLayer->Request(req);
}


Layer* Layer::Insert(Vertex* data, const std::string& position)
{
    if(!data)
        return nullptr;

    Layer* sub_layer = GetLayer(data, position);
    if(!sub_layer)
        return nullptr;

    get(CHILDREN)->add(sub_layer);
    sub_layer->SetParent(this);

    sub_layer->Insert(data);
    sub_layer->Init();

    return sub_layer;
}


void Layer::Insert(Vertex* data)
{
    SetLayout(GetLayout(data));

    SetContent(data);
}


Layer* Layer::GetLayer(Vertex* data, const std::string& position)
{
    if(!data)
        return nullptr;

    Vertex* field = get(LAYOUT, ANY)->get(FIELD, position);
    if(!field)
        return nullptr;
    Vertex* req = field->Vertex::get(REQUEST)->get();

    FactoryMap* layer_factories = dynamic_cast<FactoryMap*>(get(FACTORYMAP, LAYER_FACTORIES));
    Factory* factory = nullptr;

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
        while((tmp_req=req->get(++i)) != nullptr)
        {
            factory = layer_factories->GetFactory(tmp_req->name());
            if(factory && factory->IsValidInput(data))
            {
                req = tmp_req;
                break;
            }
            else
                factory = nullptr;
        }
    }

    if(!factory)
    {
        factory = layer_factories->GetFactory(req->get()->name());
        if(!factory)
            return nullptr;
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
    std::string layer_type = type();
    if(layout_data)
    {
        layout_data = layout_data->get(ANY, layer_type);
        if(layout_data)
            layout_data = layout_data->get();
    }

    Vertex* layout = get(THEME)->get(THEME, ANY)->get(ANY, LAYOUT)->get(layer_type);
    Vertex* field = Parent->get(LAYOUT, ANY)->get(FIELD, name());

    // TODO: Vertex::get(Vertex* path)
    Vertex* tmp_repo;
    Vertex* req = field->Vertex::get(REQUEST)->get(layer_type);
    while((req=req->get()) != nullptr)
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
    add(layout);
    get(THEME)->get(THEME, ANY)->execute(layout);

    // Add to the update tree
    Vertex* field = Parent->get(LAYOUT, ANY)->get(FIELD, name());
    field->attach(layout);
    layout->Vertex::set(field->Vertex::get(PARENT));
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
    while((child=children->get(++i)) != nullptr)
    {
        layer = dynamic_cast<Layer*>(child);
        if(!layer)
            continue;
        layer->SetSize(GetSize());
        layer->Update(forced);
    }
}
