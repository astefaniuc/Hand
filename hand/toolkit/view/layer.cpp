#include "view/layer.h"
#include "view/layers/list.h"
#include "view/theme.h"


Layer::~Layer()
{
    SetParentField(nullptr);
    if (m_Data)
        RemoveData();
    ExitListeners.Notify(this);
}


void Layer::Quit(Layers::Item*)
{
    delete this;
}


void Layer::Draw(SDL_Surface* buffer)
{
    GetDrawer()->Draw(buffer);
    m_IsModified = false;
}


void Layer::SetData(Hmi::Item* data)
{
    if (m_Data)
        RemoveData();

    data->DataListeners.Add(this, &Layer::OnDataChanged);
    data->ExitListeners.Add(this, &Layer::OnDataExit);
    SetModifiedContent();

    m_Data = data;
}


void Layer::RemoveData()
{
    m_Data->DataListeners.Remove(this);
    m_Data->ExitListeners.Remove(this);
    m_Data = nullptr;
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
    {
        m_Drawer->Exit(this);
        ExitListeners.Remove(m_Drawer);
    }
    m_Drawer = a_drawer;
    ExitListeners.Add(m_Drawer, &Drawer::Exit);
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

    m_ModifiedContent = false;
}


void Layer::Clear()
{
    delete this;
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


void Layer::OnDataExit(Hmi::Item*)
{
    RemoveData();
    delete this;
}
