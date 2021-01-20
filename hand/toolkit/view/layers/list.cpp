#include "view/layers/list.h"
#include "input/hand.h"


namespace Layers {


SDL_Rect List::ComputeSize(const SDL_Rect& outer)
{
    GetLayout();
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
    if (m_Layout)
        Layer::Update();
}


bool List::UpdateFocus()
{
    // Layer* sub = GetFirstChild();
    // while (sub)
    // {
    //     sub->SetControl(m_Hand->AddControl(sub));
    //     sub = GetNextChild();
    // }
    return false;
}


void List::ClearFocus()
{
    // Layer* sub = GetFirstChild();
    // while (sub)
    // {
    //     sub->RemoveControl();
    //     m_Hand->RemoveControl(sub);
    //     sub = GetNextChild();
    // }
}


void List::Clear()
{
    if (m_Hand && !m_UpdateFocus)
    {
        ClearFocus();
        m_UpdateFocus = true;
    }

    GetLayout()->Clear();
    SetModifiedContent();
}

}
