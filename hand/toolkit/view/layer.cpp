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
    m_IsValid = false;
    SetModified();
}


void Layer::Prune()
{
    if (!m_IsValid)
        delete this;
}


void Layer::Draw(SDL_Surface* buffer)
{
    GetDrawer()->Draw(buffer);
    SetModified(false);
}


void Layer::SetData(Data::Item* data)
{
    if (m_Data)
        RemoveData();

    m_Data = data;
    data->DataListeners.Add(this, &Layer::OnDataChanged);
    data->ExitListeners.Add(this, &Layer::OnDataExit);

    Update();
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


SDL_Rect Layer::ComputeSize(const SDL_Rect& outer)
{
    return GetDrawer()->ComputeSize(outer);
}


void Layer::UpdatePositions(const SDL_Rect& outer)
{
    m_Size = GetDrawer()->GetContentSize(outer);
}


void Layer::Update()
{
    ClearContent();
    Rebuild();
    DataListeners.Notify(this);
}


void Layer::OnDataExit(Data::Item*)
{
    m_Data = nullptr;
    Exit();
}
