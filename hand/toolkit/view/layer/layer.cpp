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
    if (m_ChangedContent)
        Rebuild();

    UpdateSubContent();
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


SDL_Rect Layer::GetFramedSize(SDL_Rect& content, const SDL_Rect& offset)
{
    SDL_Rect total = content;
    total.w += offset.w;
    total.h += offset.h;
    content.x += offset.x;
    content.y += offset.y;
    return total;
}

SDL_Rect Layer::UpdateSize(const SDL_Rect& offset)
{
    SDL_Rect content = GetDrawer()->CalculateSize(offset);
    SDL_Rect total = GetFramedSize(content, GetDrawer()->GetFrameOffset());

    total.x += offset.x;
    total.y += offset.y;
    SetContentSize(content);
    SetSize(total);

    return total;
}



LayerMap::~LayerMap()
{
    for (auto entry : m_Sublayers)
        entry.second->SetParent(nullptr);
}


void LayerMap::DrawChildren(bool forced)
{
    for (auto entry : m_Sublayers)
        GetDrawer()->DrawChild(entry.second, forced);
}


Layer* LayerMap::Insert(const std::string& field, Layer* child)
{
    m_Sublayers[field] = child;
    child->SetParent(this);
    m_IsChanged = true;
    return child;
}


void LayerMap::Remove(Layer* child)
{
    for (auto it = m_Sublayers.begin(); it != m_Sublayers.end(); ++it)
    {
        if (it->second == child)
        {
            m_Sublayers.erase(it);
            m_IsChanged = true;
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
        m_IsChanged |= sub.second->Update();
}
