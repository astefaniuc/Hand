#include "view/layouts/compact.h"
#include "view/layers/list.h"


namespace Layouts { namespace Compact {


Field* MakeField(Layout* in)
{
    Field* ret = new Field("");
    ret->SetLayout(in);
    return ret;
}
Field* MakeField(const std::string& in) { return new Field(in); }

Layout* Split(Field* field1, Field* field2, Layout::Orientation orientation)
{
    List* ret = new List();
    ret->SetField(field1);
    ret->SetField(field2);
    ret->SetOrientation(orientation);
    return ret;
}


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



SDL_Rect List::GetSize(SDL_Rect& outer)
{
    SDL_Rect size = Layout::GetSize(outer);
    std::vector<Layouts::Field*> fields;
    GetValidFields(fields);

    if (GetOrientation() == Vertical)
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
