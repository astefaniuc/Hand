#include "view/layouts/compact.h"
#include "view/layer.h"


namespace Layouts { namespace Compact {


Layout* AssureNode(Layout* in) { return in; }
Layout* AssureNode(const std::string& in) { return new Field(in); }


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



SDL_Rect Map::GetSize(Layer* tgt, SDL_Rect outer)
{
    SDL_Rect size = Layout::GetSize(tgt, outer);
    AddH(m_Field1->GetSize(tgt, outer), size);

    if (m_Orientation == Horizontal)
    {
        outer.y += size.h;
        AddH(m_Field2->GetSize(tgt, outer), size);
    }
    else
    {
        outer.x += size.w;
        AddV(m_Field2->GetSize(tgt, outer), size);
    }

    return size;
}


Field* Map::GetField(const std::string& name) const
{
    Field* ret = m_Field1->GetField(name);
    if (ret)
        return ret;
    return m_Field2->GetField(name);
}


SDL_Rect List::GetSize(Layer* tgt, SDL_Rect outer)
{
    SDL_Rect size = Layout::GetSize(tgt, outer);
    Layer* sub = tgt->GetFirstChild();

    if (GetOrientation() != Horizontal)
    {
        while (sub)
        {
            SDL_Rect subSize = sub->UpdateSize(outer);
            AddH(subSize, size);
            outer.y += subSize.h;
            sub = tgt->GetNextChild();
        }
    }
    else
    {
        while (sub)
        {
            SDL_Rect subSize = sub->UpdateSize(outer);
            AddV(subSize, size);
            outer.x += subSize.w;
            sub = tgt->GetNextChild();
        }
    }

    return size;
}


Layout* CreateButton()
{
    return
        SplitV(
            SplitH(VIEW, TITLE),
            SplitH(DESCRIPTION, CONTROL));
}


Layout* CreateData()
{
    return
        SplitV(
            SplitH(TITLE, VIEW),
            SplitH(DESCRIPTION, CONTROL));
}


Layout* CreateView()
{
    return
        SplitV(
            TITLE,
            SplitV(
                SplitH(VIEW, DESCRIPTION),
                CONTROL));
}

}}
