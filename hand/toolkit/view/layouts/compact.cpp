#include "view/layouts/compact.h"
#include "view/layers/list.h"


namespace Layouts { namespace Compact {


Field* AssureNode(Layout* in)
{
    Field* ret = new Field("");
    ret->SetLayout(in);
    return ret;
}
Field* AssureNode(const std::string& in) { return new Field(in); }


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



SDL_Rect Map::GetSize(SDL_Rect& outer)
{
    SDL_Rect size = Layout::GetSize(outer);
    SDL_Rect tmp = outer;
    AddH(m_Fields[0]->GetSize(tmp), size);

    if (m_Orientation == Horizontal)
    {
        outer.y += size.h;
        AddH(m_Fields[1]->GetSize(outer), size);
    }
    else
    {
        outer.x += size.w;
        AddV(m_Fields[1]->GetSize(outer), size);
    }

    return size;
}


SDL_Rect List::GetSize(SDL_Rect& outer)
{
//    wir brauchen hier nur ein subset von tgt => Mit Field rechnen...
    SDL_Rect size = Layout::GetSize(outer);
    std::vector<Layouts::Field*> fields;
    GetValidFields(fields);

    if (GetOrientation() != Horizontal)
    {
        for (auto field : fields)
        {
            SDL_Rect subSize = field->GetSize(outer);
            AddH(subSize, size);
            outer.y += subSize.h;
        }
    }
    else
    {
        for (auto field : fields)
        {
            SDL_Rect subSize = field->GetSize(outer);
            AddV(subSize, size);
            outer.x += subSize.w;
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
