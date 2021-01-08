#include "view/layers/list.h"
#include "input/hand.h"


namespace Layers {


void List::Quit(Hmi::Item*)
{
    Layer* sub = GetFirstChild();
    while (sub)
    {
        // Don't break iterators:
        sub->SetParent(nullptr);

        sub->Exit();
        sub = GetNextChild();
    }
    Layer::Quit(nullptr);
}


bool List::Update()
{
    if (m_ModifiedContent && m_Data)
        Rebuild();

    Layer* sub = GetFirstChild();
    while (sub)
    {
        m_IsModified |= sub->Update();
        sub = GetNextChild();
    }
    return m_IsModified;
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
        m_Layout = CreateLayout();
     return m_Layout;
}


void List::SetLayout(Layout* a_layout)
{
    delete m_Layout;
    m_Layout = a_layout;
    m_ModifiedContent = true;
}


void List::SetFocus(Hand* hand)
{
    Layer* sub = GetFirstChild();
    while (sub)
    {
        sub->SetControl(hand->BindChord(sub));
        sub = GetNextChild();
    }
}

}
