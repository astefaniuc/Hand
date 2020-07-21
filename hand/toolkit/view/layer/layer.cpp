#include "view/layer/layer.h"


void Layer::Exit(HmiItem*)
{
    // Check if the default location is currently active
    m_Parent->Exit(nullptr);
}


bool Layer::Update(bool forced)
{
    if (m_Drawer && (Changed || forced))
        m_Drawer->Draw(forced);

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
    // Store only the size, position from layout
    Multiply(GetLayout()->GetCoordinates(), size);
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
        m_Layout = new Layout();
     return m_Layout;
}


void Layer::SetLayout(Layout* a_layout)
{
    delete m_Layout;
    m_Layout = a_layout;
}


void Layer::SetDrawer(Drawer* a_drawer)
{
    delete m_Drawer;
    m_Drawer = a_drawer;
}


void Layer::Collapse()
{
    IsExpanded = false;
}


Layer* Layer::Insert(Layer* a_child)
{
    m_Sublayers.push_back(a_child);
    a_child->SetParent(this);

    return a_child;
}
