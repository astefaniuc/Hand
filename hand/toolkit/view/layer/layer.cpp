#include "view/layer/layer.h"
#include "view/theme.h"


 Layer::~Layer()
 {
     Collapse();
     delete m_Drawer;
     delete m_Layout;
}


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


void Layer::SetSize(const SDL_Rect& a_size)
{
    if ((a_size.w != m_Coordinates.w) || (a_size.h != m_Coordinates.h))
        // needs redrawing
        Changed = true;

    m_Coordinates = a_size;
}


void Layer::SetSubSize(Layer* sub, const Rel_Rect& fieldSize)
{
    // Add the parent positioning, the returned rect is relative to the current rect
    SDL_Rect parent = GetDrawer()->GetContentSize();
    SDL_Rect child = Multiply(fieldSize, parent);
    child.x += parent.x;
    child.y += parent.y;
    sub->SetSize(child);
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
