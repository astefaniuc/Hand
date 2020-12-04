#include "view/layouts/aligned.h"
#include "view/layouts/compact.h"
#include "view/layers/list.h"


namespace Layouts { namespace Aligned {


SDL_Rect Map::GetSize(const SDL_Rect& outer)
{
    std::vector<Layouts::Field*> fields;
    GetValidFields(fields);

    SDL_Rect rest = outer;
    for (auto field : fields)
    {
        SDL_Rect subSize = field->GetAlignedSize(rest);
        EAlignment alignment = field->GetAlignment().Parent;
        if (alignment == Top)
        {
            rest.h -= subSize.h;
            rest.y += subSize.h;
        }
        else if (alignment == Bottom)
            rest.h -= subSize.h;
        else if (alignment == Left)
        {
            rest.w -= subSize.w;
            rest.x += subSize.w;
        }
        else if (alignment == Right)
            rest.w -= subSize.w;

    }
    return outer;
}


Map* CreateView()
{
    Map* ret = new Map();
    ret->SetField(TITLE, { Top, Center });
    ret->SetField(DESCRIPTION, { Right, Center });
    ret->SetField(MANIPULATORS, { Left, Center });

    Compact::List* aggregateLayout = new Compact::List();
    aggregateLayout->GetField(CONTROL, true);
    aggregateLayout->GetField(LAYER_CONTROLS, true);

    Field* agg = ret->GetField("Aggregate");
    agg->SetLayout(aggregateLayout);
    agg->SetAlignment({ Bottom, Center });

    // List* aggregateLayout = new List();
    // aggregateLayout->SetField(CONTROL, { Left, Center });
    // aggregateLayout->SetField(LAYER_CONTROLS, { Right, Center });
    // ret->GetField("Aggregate")->SetLayout(aggregateLayout);

    ret->SetField(VIEW, { Center, Center });
    return ret;
}



SDL_Rect List::GetSize(const SDL_Rect& outer)
{
    std::vector<Layouts::Field*> fields;
    if (!GetValidFields(fields))
        return Layout::GetSize(outer);

    SDL_Rect size = outer;
    SDL_Rect fieldSize = outer;

    // Calculate first the fixed size. The list expands only in one direction.
    if (GetOrientation() == Vertical)
    {
        fieldSize.h /= fields.size();
        SDL_Rect available = fieldSize;
        fieldSize.w = 0;
        for (auto field : fields)
        {
            SDL_Rect subSize = field->GetSize(available);
            if (subSize.w > fieldSize.w)
                fieldSize.w = subSize.w;
        }

        size.w = fieldSize.w;
    }
    else
    {
        fieldSize.w /= fields.size();
        SDL_Rect available = fieldSize;
        fieldSize.h = 0;
        for (auto field : fields)
        {
            SDL_Rect subSize = field->GetSize(available);
            if (subSize.h > fieldSize.h)
                fieldSize.h = subSize.h;
        }

        size.h = fieldSize.h;
    }

    // Align each sub-layer into its field.
    for (auto field : fields)
    {
        SDL_Rect subSize = field->GetSize(fieldSize);
        Align(m_Alignment, fieldSize, subSize);
        field->GetSize(subSize);

        if (GetOrientation() == Vertical)
            fieldSize.y += fieldSize.h;
        else
            fieldSize.x += fieldSize.w;
    }

    return size;
}

}}
