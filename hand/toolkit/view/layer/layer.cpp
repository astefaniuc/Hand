#include "view/layer/layer.h"


void Layer::Exit(HmiItem*)
{
    // Check if the default location is currently active
    ParentLayer->Exit(nullptr);
}


bool Layer::Update(bool forced)
{
    if(Changed || forced)
        Draw(forced);

    return Changed;
}


void Layer::SetContent(HmiItem* data)
{
    // ReleaseContent
    m_Data = data;
    Changed = true;
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


void Layer::Draw(bool forced)
{
/*  if(BufferType == NONE)
    {
        // TODO: this crashes in MasterLayer during "write through"
        Parent->MapSurface(_Layout->Coordinates,
                            CoordinatesOnBuffer,
                            GetBuffer());
        Updated = true; // ?
    }*/
    // Call the Theme function for drawing with the current settings


    // TODO: derived drawing

    if(BufferType == COLLECTOR)
        // Draw first the child on the buffer
        DrawChildren(forced);

    Rel_Rect tmp = GetCoordinates();
    Show(&CoordinatesOnBuffer, &tmp);

    if(BufferType == OVERLAY)
    {
        // Draw childs afterwards (not buffered)
        DrawChildren(forced);
//        Updated = true;
    }
}


void Layer::DrawFrame()
{
    SDL_Rect total_size = GetSize();
    SDL_Rect content_size = total_size;
    Multiply(GetCoordinates(), content_size);

    // Draw each frame line separately
    SDL_Rect up, down, left, right;
    up.x = total_size.x;
    up.y = total_size.y;
    up.w = total_size.w;
    up.h = content_size.y - total_size.y;

    down.x = total_size.x;
    down.y = content_size.y + content_size.h;
    down.w = total_size.w;
    down.h = total_size.h - content_size.h - up.h;

    left.x = total_size.x;
    left.y = content_size.y;
    left.w = content_size.x - total_size.x;
    left.h = content_size.h;

    right.x = content_size.x + content_size.w;
    right.y = left.y;
    right.w = total_size.w - left.w - content_size.w;
    right.h = left.h;

    SDL_Surface* buffer = GetBuffer();
    SDL_Rect* border[4] = { &up, &down, &left, &right };
    for (unsigned i = 0; i < 4; ++i)
    {
        SDL_SetClipRect(buffer, border[i]);
        FillRect(buffer, border[i], GetFrameColor());
    }
}

void Layer::DrawChildren(bool forced)
{
    for (Layer* layer : m_Sublayers)
    {
        layer->SetSize(GetSize());
        layer->Update(forced);
    }
}
