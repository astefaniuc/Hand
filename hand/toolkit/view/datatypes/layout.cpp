#include "view/datatypes/layout.h"
#include "view/datatypes/rect.h"


Layout::Layout(const std::string& name) : List(name)
{
    type(LAYOUT);
    add(new FieldsContainer(this));
}


bool Layout::add(Vertex* child)
{
    if(!child->is(LAYOUT))
        return List::add(child);

    get(FIELDS)->add(child);
    return child->Vertex::get(PARENT)->set(this);
}


Vertex* Layout::get(const std::string& type, const std::string& name)
{
    Vertex* ret = List::get(type, name);
    if(ret)
        return ret;

    if(type != FIELD)
        return nullptr;

    // "GetField" mode
    // Skip layouts from different layers (fields)
    Vertex* fields = get(FIELDS)->Vertex::get(PUBLIC);

    ret = fields->get(ANY, name);
    if(ret)
        return ret;

    Vertex* child;
    unsigned i = 0;
    while((child=fields->get(++i)) != nullptr)
    {
        ret = child->get(type, name);
        if(ret)
            return ret;
    }

    return nullptr;
}


bool Layout::execute(Vertex* vs)
{
    // Set the surface in the layout and use the layout
    // as parameter for the drawer
    set(vs);
    Vertex* drawer = get(DRAWER)->get();
    if(drawer)
        // Execute drawer on current layout
        drawer->execute(this);

    // Skip layouts from different layers (fields)
    Vertex* fields = get(FIELDS)->Vertex::get(PUBLIC);
    Vertex* layout;
    unsigned i = 0;
    while((layout=fields->get(++i)) != nullptr)
        // Ignored parameter
        layout->execute(vs);

    return true;
}


void Layout::reset()
{
    Vertex* coords = get(COORDINATES)->get();
    if(coords)
        coords->reset();

    Vertex* fields = get(FIELDS);
    Vertex* layout;
    unsigned i = 0;
    while((layout=fields->get(++i)) != nullptr)
        layout->reset();
}

// ----------------------------------------------------------------

Vertex* FieldsContainer::get(unsigned i)
{
    Vertex* sub;
    unsigned found = 0;
    unsigned j = 0;
    while((sub=List::get(++j)) != nullptr)
    {
        if(sub->is(LAYOUT))
        {
            ++found;
            if(found == i)
                return sub;
        }
        else
        {
            if((found + sub->size()) >= i)
                return sub->get(i - found);
            found += sub->size();
        }
    }
    return nullptr;
}


Vertex* FieldsContainer::get(const std::string& name)
{
    Vertex* ret = get(ANY, name);
    if(!ret)
    {
        // Creates a new vertex
        ret = List::get(name);
        ret->Vertex::set(Vertex::get(PARENT));
    }
    return ret;
}


unsigned FieldsContainer::size()
{
    Vertex* sub;
    unsigned found = 0;
    unsigned j = 0;
    while((sub=List::get(++j)) != nullptr)
    {
        if(sub->is(LAYOUT))
            found++;
        else
            found += sub->size();
    }

    return found;
}
