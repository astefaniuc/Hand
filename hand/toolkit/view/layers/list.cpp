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


SDL_Rect List::UpdateSize(const SDL_Rect& outer)
{
    SDL_Rect content = GetDrawer()->GetContentSize(outer);
    m_Size = GetLayout()->GetSize(content);
    return GetDrawer()->CalculateSize(m_Size);
}


void List::UpdatePositions(const SDL_Rect& outer)
{
    SDL_Rect content = GetDrawer()->GetContentPosition(outer);
    m_Size.x = content.x;
    m_Size.y = content.y;
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