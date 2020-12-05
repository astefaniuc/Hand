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

    List* aggregateLayout = new List();
    aggregateLayout->GetField(CONTROL, true);
    aggregateLayout->GetField(LAYER_CONTROLS, true);

    Field* agg = ret->GetField("Aggregate");
    agg->SetLayout(aggregateLayout);
    agg->SetAlignment({ Bottom, Center });

    // List* aggregateLayout = new List();
    // aggregateLayout->SetField(CONTROL, { Left, Center });
    // aggregateLayout->SetField(LAYER_CONTROLS, { Right, Center });
    // ret->GetField("Aggregate")->SetLayout(aggregateLayout);

    // ret->SetField(LAYER_CONTROLS, { Top, Right });
    // ret->SetField(CONTROL, { Bottom, Center });
    ret->SetField(VIEW, { Center, Center });
    return ret;
}



SDL_Rect List::GetSize(const SDL_Rect& outer)
{
    std::vector<Layouts::Field*> fields;
    if (!GetValidFields(fields))
        return Layout::GetSize(outer);

    SDL_Rect size = outer;
    SDL_Rect fieldSize = { outer.x, outer.y, 0, 0 };

    // Calculate first the fixed size. The list expands only in one direction.
    for (auto field : fields)
    {
        SDL_Rect subSize = field->GetSize(outer);
        if (subSize.w > fieldSize.w)
            fieldSize.w = subSize.w;
        if (subSize.h > fieldSize.h)
            fieldSize.h = subSize.h;
    }

    if (GetOrientation() == Vertical)
        size.w = fieldSize.w;
    else
        size.h = fieldSize.h;

    // Align each sub-layer into its field.
    for (auto field : fields)
    {
        field->GetAlignedSize(fieldSize);
        if (GetOrientation() == Vertical)
            fieldSize.y += fieldSize.h;
        else
            fieldSize.x += fieldSize.w;
    }

    return size;
}

}}
