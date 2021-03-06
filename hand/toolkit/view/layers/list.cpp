#include "view/layers/list.h"
#include "input/hand.h"


namespace Layers {


List::~List()
{
    delete m_Layout;
    delete m_LayerCommands;
}


void List::Insert(const std::string& name, Layer* child)
{
    if (!child)
        return;

    Field* field = GetLayout()->GetField(name, false);
    if (!field)
        return;

    field->SetItem(child);
}


void List::Insert(Layer* a_child)
{
    GetLayout()->GetField(a_child->GetData()->GetName())->SetItem(a_child);
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
    if (m_Layout)
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


bool List::IsExpanding(Orientation direction)
{
    return GetLayout()->IsExpanding(direction);
}


Hmi::List* List::GetLayerControls()
{
    if (!m_LayerCommands)
        m_LayerCommands = new Hmi::Vector(LAYER_CONTROLS, "");
    return m_LayerCommands;
}

}
