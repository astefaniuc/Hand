#include "view/layer/layer.h"
#include "view/theme.h"


void Layer::Exit(HmiItem*)
{
    // Check if the default location is currently active
    m_Parent->Exit(nullptr);
}


void Layer::Update()
{
    if (Changed)
        Rebuild();

    for (Layer* sub : m_Sublayers)
        sub->Update();
}


bool Layer::Draw(bool a_forced)
{
    if (Changed || a_forced)
        GetDrawer()->Draw(a_forced);

    return Changed;
}


void Layer::SetContent(HmiItem* data)
{
    // ReleaseContent
    m_Data = data;
    Changed = true;
}


void Layer::SetSize(SDL_Rect size)
{
     if ((size.w != CoordinatesOnBuffer.w) || (size.h != CoordinatesOnBuffer.h))
    {
        CoordinatesOnBuffer.w = size.w;
        CoordinatesOnBuffer.h = size.h;
        Changed = true;
    }
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
}


void Layer::Insert(Layer* a_child)
{
    m_Sublayers.push_back(a_child);
    a_child->SetParent(this);
}
