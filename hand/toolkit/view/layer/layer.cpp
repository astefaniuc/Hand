#include "view/layer/layer.h"
#include "view/datatypes/rect.h"
#include "view/datatypes/layout.h"


bool Layer::Exit()
{
    // Check if the default location is currently active
    return ParentLayer->Exit();
}


bool Layer::Update(bool forced)
{
    if(Changed || forced)
        Draw(forced);

    return Changed;
}


void Layer::SetContent(HmiItem* data)
{
    // ReleaseContent
    m_Data = data;
    Changed = true;
}


void Layer::Collapse()
{
    HmiItem* children = get(LINK, CHILDREN);
    if(!children)
        return;

    HmiItem* child;
    while((child=children->get(1)) != nullptr)
        // Recursively deletes sub-layers
        delete child;

    IsExpanded = false;
}


bool Layer::Request(HmiItem* req)
{
    return ParentLayer->Request(req);
}


Layer* Layer::Insert(HmiItem* data, const std::string& position)
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


void Layer::Insert(HmiItem* data)
{
    SetContent(data);
}


void Layer::Draw(bool forced)
{
/*  if(BufferType == NONE)
    {
        // TODO: this crashes in CUser during "write through"
        Parent->MapSurface(_Layout->Coordinates,
                            CoordinatesOnBuffer,
                            GetBuffer());
        Updated = true; // ?
    }*/
    // Call the Theme function for drawing with the current settings
    HmiItem* layout = get(LAYOUT, ANY);
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
    HmiItem* children = get(LINK, CHILDREN);
    if(!children)
        return;

    Layer* layer;
    HmiItem* child;
    unsigned i = 0;
    while((child=children->get(++i)) != nullptr)
    {
        layer = dynamic_cast<Layer*>(child);
        if(!layer)
            continue;
        layer->SetSize(GetSize());
        layer->Update(forced);
    }
}
