#include "view/layer/layer.h"
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

    UpdateSubContent();
    return m_IsModified;
}


void Layer::Draw(SDL_Surface* buffer, bool a_forced)
{
    GetDrawer()->Draw(buffer, a_forced);

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


void Layer::SetPreferredSize(const SDL_Rect& a_size)
{
    if ((a_size.w != m_PreferredSize.w) || (a_size.h != m_PreferredSize.h))
        // needs redrawing
        m_IsModified = true;

    m_PreferredSize = a_size;
}


void Layer::SetFieldSize(const SDL_Rect& outer)
{
    m_FieldSize = m_ContentSize;
    m_FieldSize.x += m_PreferredSize.x + outer.x;
    m_FieldSize.y += m_PreferredSize.y + outer.y;
    SetLayoutSize(m_FieldSize);
}


Layout::Node* Layer::GetLayout()
{
    if (!m_Layout)
        m_Layout = CreateLayout();
     return m_Layout;
}


void Layer::SetLayout(Layout::Node* a_layout)
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


SDL_Rect Layer::UpdateSize(const SDL_Rect& externalOffset)
{
    SDL_Rect total = { 0, 0, 0, 0 };
    SDL_Rect ret = GetDrawer()->CalculateSize(m_ContentSize, total);

    total.x += externalOffset.x;
    total.y += externalOffset.y;
    SetPreferredSize(total);
    return ret;
}



LayerMap::~LayerMap()
{
    for (auto entry : m_Sublayers)
        entry.second->SetParent(nullptr);
}


void LayerMap::DrawChildren(SDL_Surface* buffer, bool forced)
{
    for (auto entry : m_Sublayers)
        GetDrawer()->DrawChild(buffer, entry.second, forced);
}


Layer* LayerMap::Insert(const std::string& field, Layer* child)
{
    m_Sublayers[field] = child;
    child->SetParent(this);
    m_IsModified = true;
    return child;
}


void LayerMap::Remove(Layer* child)
{
    for (auto it = m_Sublayers.begin(); it != m_Sublayers.end(); ++it)
    {
        if (it->second == child)
        {
            m_Sublayers.erase(it);
            m_IsModified = true;
            return;
        }
    }
}


Layer* LayerMap::GetField(const std::string& name)
{
    auto it = m_Sublayers.find(name);
    if (it != m_Sublayers.end())
        return it->second;
    return nullptr;
}


void LayerMap::UpdateSubContent()
{
    for (auto sub : m_Sublayers)
        m_IsModified |= sub.second->Update();
}


void LayerMap::SetLayoutSize(const SDL_Rect& outer)
{
    for (auto sub : m_Sublayers)
        sub.second->SetFieldSize(m_FieldSize);
}
