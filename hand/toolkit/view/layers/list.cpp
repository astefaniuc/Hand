#include "view/layers/list.h"
#include "input/hand.h"


namespace Layers {


List::~List()
{
    delete m_Layout;
    delete m_LayerCommands;
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


void List::UpdateFocus()
{
    if (m_InteractionGroup)
        m_InteractionGroup->Update();
}


void List::ClearFocus()
{
    if (m_InteractionGroup)
        m_InteractionGroup->Clear();
}


void List::ClearContent()
{
    Layer::ClearContent();
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


bool List::SetCommand(Interaction::Command* ctrl)
{
    m_Command = ctrl;
    SetModifiedContent();
    return true;
}


void List::ReleaseCommand()
{
    if (!m_Command)
        return;
    m_Command = nullptr;
    SetModifiedContent();
}


void List::SetFocus()
{
    m_HasFocus = true;
    m_InteractionGroup->GetControl()->Add(
        new Interaction::Group(GetLayerControls()->GetExpandedView()->GetListLayer()));
}


void List::RemoveFocus()
{
    m_HasFocus = false;
}

}
