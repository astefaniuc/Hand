#include "view/layout.h"
#include "view/layer/layer.h"
#include "view/layer/listlayer.h"


namespace Layout {


Node* AssureNode(Node* in) { return in; }
Node* AssureNode(const std::string& in) { return new Field(in); }


void AddH(const SDL_Rect& in, SDL_Rect& out)
{
    out.h += in.h;
    if (in.w > out.w)
        out.w = in.w;
}

void AddV(const SDL_Rect& in, SDL_Rect& out)
{
    out.w += in.w;
    if (in.h > out.h)
        out.h = in.h;
}



SDL_Rect Field::GetSize(Layer* a_tgt, SDL_Rect offset)
{
    LayerMap* tgt = static_cast<LayerMap*>(a_tgt);
    Layer* sub = tgt->GetField(m_Name);
    if (sub)
        return sub->UpdateSize(offset);
    return { 0, 0, 0, 0 };
}


Field* Field::GetField(const std::string& a_name) const
{
    if (a_name == m_Name)
        return (Field*)this;
    return nullptr;
}


SDL_Rect Separator::GetSize(Layer* tgt, SDL_Rect offset)
{
    SDL_Rect size = Node::GetSize(tgt, offset);
    //  Remove the first childs offset from the total size:
    AddH(m_Field1->GetSize(tgt, offset), size);

    if (m_Orientation == Horizontal)
    {
        offset.y += size.h;
        AddH(m_Field2->GetSize(tgt, offset), size);
    }
    else
    {
        offset.x += size.w;
        AddV(m_Field2->GetSize(tgt, offset), size);
    }

    return size;
}


Field* Separator::GetField(const std::string& a_name) const
{
    Field* ret = m_Field1->GetField(a_name);
    if (ret)
        return ret;
    return m_Field2->GetField(a_name);
}


SDL_Rect List::GetSize(Layer* a_tgt, SDL_Rect offset)
{
    SDL_Rect size = Node::GetSize(a_tgt, offset);
    ListLayer* tgt = static_cast<ListLayer*>(a_tgt);
    if (GetOrientation() != Horizontal)
        for (Layer* sub : tgt->GetSubLayers())
        {
            SDL_Rect subSize = sub->UpdateSize(offset);
            AddH(subSize, size);
            offset.y += subSize.h;
        }
    else
        for (Layer* sub : tgt->GetSubLayers())
        {
            SDL_Rect subSize = sub->UpdateSize(offset);
            AddV(subSize, size);
            offset.x += subSize.w;
        }

    return size;
}


Node* CreateButton()
{
    return
        SplitV(
            SplitH(VIEW, TITLE),
            SplitH(DESCRIPTION, CONTROL));
}


Node* CreateData()
{
    return
        SplitV(
            SplitH(TITLE, VIEW),
            SplitH(DESCRIPTION, CONTROL));
}


Node* CreateView()
{
    return
        SplitV(
            TITLE,
            SplitV(
                SplitH(VIEW, DESCRIPTION),
                CONTROL));
}

}
