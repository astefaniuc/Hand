#include "view/layers/list.h"


namespace Layers {


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
    return GetDrawer()->ComputeSize(
                GetLayout()->ComputeSize(
                    GetDrawer()->GetContentSize(outer)));
}


void List::UpdatePositions(const SDL_Rect& outer)
{
    m_Size = GetDrawer()->GetContentSize(outer);
    if (m_Layout)
        m_Layout->UpdatePositions(m_Size);
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

}