#include "view/layer.h"
#include "view/layers/list.h"
#include "view/theme.h"
#include "input/interaction.h"


Layer::~Layer()
{
    Exit();
}


void Layer::Quit(Layers::Item*)
{
    SetModified();
    SetDrawer(nullptr);
    Clear();
}


void Layer::Draw(SDL_Surface* buffer)
{
    GetDrawer()->Draw(buffer);
    m_IsModified = false;
}


void Layer::SetData(Hmi::Item* data)
{
    if (m_Data)
        m_Data->DataListeners.Remove(this);
    ClearContent();

    data->DataListeners.Add(this, &Layer::OnNotifyChanged);
    SetModifiedContent();

    m_Data = data;
}


void Layer::SetTheme(Theme* theme)
{
    m_Theme = theme;
    if (m_Theme && m_Drawer)
        GetDrawerFromTheme();
}


Theme* Layer::GetTheme()
{
    if (m_Theme)
        return m_Theme;
    return GetParentLayer()->GetTheme();
}


void Layer::SetDrawer(Drawer* a_drawer)
{
    if (m_Drawer)
        m_Drawer->Exit();
    m_Drawer = a_drawer;
}


Drawer* Layer::GetDrawer()
{
    if (m_Drawer)
        return m_Drawer;

    return GetDrawerFromTheme();
}


Layers::Interface* Layer::GetInterface()
{
    Layers::List* parent = GetParentLayer();
    if (parent)
        return parent->GetInterface();
    return nullptr;
}


void Layer::SetModified()
{
    Layers::List* parent = GetParentLayer();
    if (parent && !parent->IsModified())
        parent->SetModified();
    m_IsModified = true;
}


void Layer::SetModifiedContent()
{
    if (m_ModifiedContent)
        return;

    SetModified();
    m_ModifiedContent = true;
}


void Layer::Update()
{
    if (!m_ModifiedContent || !CanUpdate())
        return;

    ClearContent();
    Rebuild();
    UpdateInteractionGroup();

    m_ModifiedContent = false;
}


void Layer::Clear()
{
    SetParentField(nullptr);
    ClearContent();
}


void Layer::ClearContent()
{
    ClearInteractionGroup();
    SetModifiedContent();
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
