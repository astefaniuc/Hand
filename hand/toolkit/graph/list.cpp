#include "graph/list.h"


List::List(const std::string& name) : Vertex(name)
{
    /*
       Delayed initialization of the public list,
       allows initialization with a customized list
    */

    type(LIST);
}


bool List::add(Vertex* child)
{
    return List::get(PUBLIC)->add(child);
}


bool List::set(Vertex* child)
{
    return List::get(PUBLIC)->set(child);
}


bool List::attach(Vertex* child)
{
    return List::get(PUBLIC)->attach(child);
}


Vertex* List::_get()
{
    return List::get(1);
}


Vertex* List::get(const std::string& s)
{
    if(s == PUBLIC)
    {
        Vertex* child;
        uint i = 0;
        while((child=Vertex::get(++i)) != nullptr)
            if(child->name() == s)
                return child;

        // Avoid endless recursion: Vertex + Attach
        child = new Vertex(s);
        Vertex::attach(child);
        return child;
    }

    return List::get(PUBLIC)->get(s);
}


Vertex* List::get(uint i)
{
    return List::get(PUBLIC)->get(i);
}


Vertex* List::get(const std::string& type, const std::string& name)
{
    return List::get(PUBLIC)->get(type, name);
}


uint List::size()
{
    return List::get(PUBLIC)->size();
}


void List::reset()
{
    Vertex* pl = Vertex::get(ANY, PUBLIC);
    if(pl)
        pl->Vertex::reset();
}


bool List::remove(Vertex* child)
{
    return List::get(PUBLIC)->remove(child);
}


bool List::detach(Vertex* child)
{
    return List::get(PUBLIC)->detach(child);
}
