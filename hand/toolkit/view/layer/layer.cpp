#include "view/layer/layer.h"
#include "view/theme.h"
#include <assert.h>


 Layer::~Layer()
 {
    Collapse();
    for (Layer* sub : m_Sublayers)
        sub->SetParent(nullptr);

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
    if (m_ChangedContent)
        Rebuild();

    UpdateSubSizes();
    for (Layer* sub : m_Sublayers)
        m_IsChanged |= sub->Update();

    return m_IsChanged;
}


void Layer::Draw(bool a_forced)
{
    if (m_ChangedContent || m_IsChanged || a_forced)
        GetDrawer()->Draw(a_forced);

    m_ChangedContent = false;
    m_IsChanged = false;
}


void Layer::SetContent(HmiItem* data)
{
    // Layer-Data is a lifelong 1:1 relation.
    assert(!m_Data);
    m_Data = data;
    m_ChangedContent = true;
}


void Layer::SetSize(const SDL_Rect& a_size)
{
    if ((a_size.w != m_Coordinates.w) || (a_size.h != m_Coordinates.h))
        // needs redrawing
        m_IsChanged = true;

    m_Coordinates = a_size;
}


void Layer::SetSubSize(Layer* sub, const std::string& field)
{
    if (sub)
        SetSubSize(sub, GetLayout()->GetField(field));
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
    m_ChangedContent = true;
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
    m_ChangedContent = true;
}


Layer* Layer::Insert(Layer* a_child)
{
    m_Sublayers.push_back(a_child);
    a_child->SetParent(this);
    m_IsChanged = true;
    return a_child;
}


void Layer::Remove(Layer* a_child)
{
    for (unsigned i = 0; i < m_Sublayers.size(); ++i)
    {
        if (m_Sublayers[i] == a_child)
        {
            m_Sublayers.erase(m_Sublayers.begin() + i);
            m_IsChanged = true;
            return;
        }
    }
}
