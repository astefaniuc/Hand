#include "view/layer.h"
#include "view/layers/list.h"
#include "view/theme.h"
#include <assert.h>


void Layer::Exit(Hmi::Item*)
{
    if (m_Parent)
        m_Parent->Remove(this);
     delete m_Drawer;
     m_Drawer = nullptr;
}


void Layer::Draw(SDL_Surface* buffer)
{
    GetDrawer()->Draw(buffer);

    m_ModifiedContent = false;
    m_IsModified = false;
}


void Layer::SetContent(Hmi::Item* data)
{
    // Layer-Data is a lifelong 1:1 relation.
    assert(!m_Data);
    m_Data = data;
    m_ModifiedContent = true;
}


void Layer::SetTheme(Theme* a_theme)
{
    delete m_Theme;
    m_Theme = a_theme;
}


Theme* Layer::GetTheme()
{
    if (m_Theme)
        return m_Theme;
    return GetParent()->GetTheme();
}


void Layer::SetDrawer(Drawer* a_drawer)
{
    delete m_Drawer;
    m_Drawer = a_drawer;
}


Drawer* Layer::GetDrawer()
{
    if (!m_Drawer)
    {
        SetDrawer(CreatetDrawer());
        m_Drawer->SetLayer(this);
    }
    return m_Drawer;
}


SDL_Rect Layer::UpdateSize(const SDL_Rect& outer)
{
    return GetDrawer()->CalculateSize(m_Size);
}


void Layer::UpdatePositions(const SDL_Rect& outer)
{
    SDL_Rect content = GetDrawer()->GetContentPosition(outer);
    m_Size.x = content.x;
    m_Size.y = content.y;
}

