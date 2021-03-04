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
    SetModified();
}


void List::Insert(Layer* a_child)
{
    GetLayout()->GetField(a_child->GetData()->GetName())->SetItem(a_child);
    SetModified();
}


SDL_Rect List::ComputeSize(const SDL_Rect& outer)
{
    Update();

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


void List::SetLayout(Layout* a_layout)
{
    delete m_Layout;
    m_Layout = a_layout;
    if (m_Layout)
        m_Layout->SetLayer(this);
    SetModifiedContent();
}


void List::Update()
{
    // Force a layout if yet not set.
    GetLayout();
    Layer::Update();
}


bool List::IsExpanding(Orientation direction)
{
    Update();
    return GetLayout()->IsExpanding(direction);
}


void List::UpdateInteractionGroup()
{
    if (m_InteractionGroup)
        m_InteractionGroup->Update();
}


void List::ClearContent()
{
    SetModifiedContent();
    if (m_Layout)
        m_Layout->Clear();
}


Hmi::List* List::GetLayerControls()
{
    if (!m_LayerCommands)
        m_LayerCommands = new Hmi::Vector(LAYER_CONTROLS, "");
    return m_LayerCommands;
}


bool List::SetInteraction(Interaction::Group* focus)
{
    m_InteractionGroup = focus;
    return true;
}


void List::ReleaseInteractionGroup()
{
    m_InteractionGroup = nullptr;
}

}
