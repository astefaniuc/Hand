#include "view/layer.h"
#include "view/theme.h"
#include <assert.h>


 Layer::~Layer()
 {
    delete m_Drawer;
    delete m_Layout;
}


void Layer::Exit(HmiItem*)
{
    if (m_Parent)
        m_Parent->Remove(this);
     delete m_Drawer;
     m_Drawer = nullptr;
}


bool Layer::Update()
{
    if (m_ModifiedContent)
        Rebuild();

    Layer* sub = GetFirstChild();
    while (sub)
    {
        m_IsModified |= sub->Update();
        sub = GetNextChild();
    }
    return m_IsModified;
}


void Layer::Draw(SDL_Surface* buffer)
{
    GetDrawer()->Draw(buffer);

    m_ModifiedContent = false;
    m_IsModified = false;
}


void Layer::SetContent(HmiItem* data)
{
    // Layer-Data is a lifelong 1:1 relation.
    assert(!m_Data);
    m_Data = data;
    m_ModifiedContent = true;
}


Layout* Layer::GetLayout()
{
    if (!m_Layout)
        m_Layout = CreateLayout();
     return m_Layout;
}


void Layer::SetLayout(Layout* a_layout)
{
    delete m_Layout;
    m_Layout = a_layout;
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


void Layer::Collapse()
{
    IsExpanded = false;
    m_ModifiedContent = true;
}


SDL_Rect Layer::UpdateSize(const SDL_Rect& outer)
{
    SDL_Rect content = GetDrawer()->GetContentSize(outer);
    m_Size = GetLayout()->GetSize(this, content);
    return GetDrawer()->CalculateSize(m_Size);
}
