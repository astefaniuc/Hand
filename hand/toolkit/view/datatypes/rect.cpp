#include "view/datatypes/rect.h"


Rect::Rect(const std::string& name, Rel_Rect* val) : Data<Rel_Rect*>(name, val)
{
    type(RECT);
    // For now add plain data directly
    set(new Data<double>("x", Value->x));
    set(new Data<double>("y", Value->y));
    set(new Data<double>("w", Value->w));
    set(new Data<double>("h", Value->h));
}


Rect::~Rect()
{
    delete(Value);
}


bool Rect::set(Rel_Rect* val)
{
    Data<Rel_Rect*>::set(val);
    // For now add plain data directly
    ((Data<double>*)get("x"))->set(Value->x);
    ((Data<double>*)get("y"))->set(Value->y);
    ((Data<double>*)get("w"))->set(Value->w);
    ((Data<double>*)get("h"))->set(Value->h);
    return true;
}


void Rect::reset()
{
    if(!Value)
        Value = new Rel_Rect();
    Value->x = ((Data<double>*)get("x"))->get();
    Value->y = ((Data<double>*)get("y"))->get();
    Value->w = ((Data<double>*)get("w"))->get();
    Value->h = ((Data<double>*)get("h"))->get();
}

// ----------------------------------------------------------------

Alternate::Alternate(Rect* alig, Rect* alt_alig)
    : Data<Rel_Rect*>(alig->name(), alig->get())
{
    type(METHOD);
    attach(alig);
    attach(alt_alig);
}


bool Alternate::execute(Vertex* layout)
{
    while((layout=layout->Vertex::get(PARENT)->get()) != nullptr)
    {
        Vertex* alig = layout->get(ALIGNMENT)->get();
        if(!alig)
            continue;
        if(alig->name() == name())
        {
            Vertex* alt = get(2);
            // Change Vertex positions
            set(alt);
            // Change returned data
            set((dynamic_cast<Rect*>(alt))->get());
            // Change also the name
            name(alt->name());
        }
        break;
    }

    return true;
}

// ----------------------------------------------------------------

void Multiply(Rel_Rect* src, Rel_Rect* tgt)
{
    if(!src || !tgt)
        return;
    tgt->x += (src->x * tgt->w);
    tgt->y += (src->y * tgt->h);
    tgt->w *= src->w;
    tgt->h *= src->h;
}


void Multiply(Rel_Rect* src, SDL_Rect* tgt)
{
    if(!src || !tgt)
        return;
    tgt->x += (src->x * tgt->w);
    tgt->y += (src->y * tgt->h);
    tgt->w *= src->w;
    tgt->h *= src->h;
}


Rel_Rect* GetRect(const std::string& name, Vertex* layout)
{
    Vertex* rect = layout->get(ANY, name);
    if(rect && ((rect=rect->get()) != nullptr))
    {
        if(rect->is(METHOD))
            // Allow value changes based on (parent) layout configuration
            rect->execute(layout);

        Data<Rel_Rect*>* container = dynamic_cast<Data<Rel_Rect*>*>(rect);
        if(container)
            return container->get();
    }
    return nullptr;
}
