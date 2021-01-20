#include "view/layer.h"
#include "view/layers/list.h"
#include "view/theme.h"


Layer::~Layer()
{
    // if (m_Data && m_DataCb)
    // {
    //     m_Data->RemoveDataChangedClient(m_DataCb);
    //     std::cout << "del: " << m_DataCb << std::endl;
    // }

//    delete m_DataCb;
}


void Layer::Quit(Hmi::Item*)
{
    SetParentField(nullptr);
    delete m_Drawer;
    m_Drawer = nullptr;
    Clear();
}


void Layer::Draw(SDL_Surface* buffer)
{
    GetDrawer()->Draw(buffer);
    m_IsModified = false;
}


void Layer::SetContent(Hmi::Item* data)
{
    if (m_Data)
    {
        m_Data->RemoveDataChangedClient(m_DataCb);
        std::cout << "rem: " << m_DataCb << std::endl;
    }
    Clear();

    if (!m_DataCb)
        m_DataCb = new CCallback<Layer>(this, &Layer::OnNotifyChanged);

    data->AddDataChangedClient(m_DataCb);
    SetModifiedContent();

    m_Data = data;
}


void Layer::SetTheme(Theme* theme)
{
    m_Theme = theme;
    if (m_Theme && m_Drawer)
        SetDrawer(CreatetDrawer());

    if (GetLayout())
        GetLayout()->SetTheme(theme);
}


Theme* Layer::GetTheme()
{
    if (m_Theme)
        return m_Theme;
    return GetParentLayer()->GetTheme();
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
    if (!m_IsModified)
        return;

    if (m_ModifiedContent && m_Data)
    {
        ClearCallback(nullptr);
        Rebuild();
        m_ModifiedContent = false;
    }

    if (m_Hand && m_UpdateFocus)
        m_UpdateFocus = UpdateFocus();
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
    m_UpdateFocus = UpdateFocus();
    SetModified();
}


void Layer::ReleaseFocus(Hand* hand)
{
    if (m_Hand && !m_UpdateFocus)
        ClearFocus();

    m_UpdateFocus = false;
    m_Hand = nullptr;
}
