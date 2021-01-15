#include "view/layer.h"
#include "view/layers/list.h"
#include "view/theme.h"


void Layer::Quit(Hmi::Item*)
{
    SetParentField(nullptr);

    if (m_Parent)
    {
        m_Parent->Remove(this);
        m_Parent = nullptr;
    }
    delete m_Drawer;
    m_Drawer = nullptr;
    Clear(nullptr);
}


void Layer::Draw(SDL_Surface* buffer)
{
    GetDrawer()->Draw(buffer);
    m_IsModified = false;
}


void Layer::SetContent(Hmi::Item* data)
{
    SetModifiedContent();
    m_Data = data;
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


void Layer::SetModified()
{
    if (m_Parent && !m_Parent->IsModified())
        m_Parent->SetModified();
    m_IsModified = true;
}


void Layer::SetModifiedContent()
{
    if (m_ModifiedContent)
        return;

    if (m_Hand)
        ClearFocus();
    SetModified();
    m_ModifiedContent = true;
}


void Layer::Update()
{
    if (!m_IsModified)
        return;

    if (m_ModifiedContent && m_Data)
    {
        Rebuild();
        m_ModifiedContent = false;
    }

    if (m_Hand)
        UpdateFocus();
}


SDL_Rect Layer::ComputeSize(const SDL_Rect& outer)
{
    Update();
    return GetDrawer()->ComputeSize(outer);
}


void Layer::UpdatePositions(const SDL_Rect& outer)
{
    m_Size = GetDrawer()->GetContentSize(outer);
}


void Layer::SetFocus(Hand* hand)
{
    m_Hand = hand;
    SetModified();
}


void Layer::ReleaseFocus(Hand* hand)
{
    if (m_Hand)
    {
        ClearFocus();
        m_Hand = nullptr;
    }
}
