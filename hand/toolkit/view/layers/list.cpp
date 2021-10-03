#include "view/layers/list.h"
#include "input/hand.h"


namespace Layers {


List::~List()
{
    delete m_Layout;
    delete m_LayerCommands;
}


void List::Prune()
{
    if (!m_IsValid)
        delete this;
    else if (m_Layout)
        m_Layout->Prune();
}


SDL_Rect List::ComputeSize(const SDL_Rect& outer)
{
    SDL_Rect ret = GetDrawer()->ComputeSize(
        GetLayout()->ComputeSize(GetDrawer()->GetContentSize(outer)));

    if ((ret.w > outer.w) || (ret.h > outer.h))
    {
        // TODO. The Layer/logic has to deal with space shortage.
    }

    return ret;
}


void List::UpdatePositions(const SDL_Rect& outer)
{
    Layer::UpdatePositions(outer);
    m_Layout->UpdatePositions(GetSize());
}


Layout* List::GetLayout()
{
    if (!m_Layout)
        SetLayout(CreateLayout());
    return m_Layout;
}


void List::SetLayout(Layout* layout)
{
    if (m_Layout && layout)
    {
        std::vector<Layer*> layers;
        m_Layout->GetActiveLayer(layers);
        for (Layer* layer : layers)
            layout->GetField(layer->GetParentField()->GetName())->SetItem(layer);
        delete m_Layout;
    }
    m_Layout = layout;
    if (m_Layout)
        m_Layout->SetLayer(this);
}


Data::List* List::GetLayerControls()
{
    if (!m_LayerCommands)
        m_LayerCommands = new Data::Vector(LAYER_CONTROLS, "");
    return m_LayerCommands;
}


void List::SetExpandChildren(bool expand)
{
    if (m_ExpandChildren != expand)
    {
        m_ExpandChildren = expand;
        if (m_Data)
            Update();
    }
}

}
