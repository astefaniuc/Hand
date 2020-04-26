#include "graph/baselist.h"
#include "graph/vertex.h"
#include "graph/link.h"
#include "graph/search/regularexpression.h"
#include "graph/search/search.h"
#include "base/handserver.h"


Vertex::Vertex(const std::string& name) : Name(name)
{
    Body = new BaseList();
    References = new BaseList();
}


Vertex::~Vertex()
{
    reset();
    delete(Type);
    // Remove references to this object
    for(VIterator curr = References->begin(); curr != References->end(); ++curr)
        (*curr)->Body->erase(this);
    delete(References);
    delete(Body);
}


void Vertex::reset()
{
    VIterator curr = Body->begin();
    while(curr != Body->end())
    {
        // Recursively delete all children
        if(!(*curr)->getOwner() || ((*curr)->getOwner() == this))
            delete((*curr));
        else
        {
            // Detach
            (*curr)->References->erase(this);
            Body->erase(curr);
        }
    }
}


bool Vertex::add(Vertex* child)
{
    if(!child)
        return false;

    if(Body->find(child)==Body->end())
        Body->push_back(child);
    child->setOwner(this);

    return true;
}


bool Vertex::set(Vertex* child)
{
    if(!child)
        return false;

    VIterator curr = Body->find(child);
    if(curr == Body->end())
        // New sub-item
        child->References->push_back(this);
    else if(curr == Body->begin())
        // Already set
        return true;
    else
        // Already here but in wrong position
        Body->erase(curr);

    Body->insert(Body->begin(), child);

    return true;
}


bool Vertex::attach(Vertex* child)
{
    if(!child || (Body->find(child) != Body->end()))
        return false;

    Body->push_back(child);
    child->References->push_back(this);

    return true;
}


Vertex* Vertex::get()
{
    return _get();
}


Vertex* Vertex::_get()
{
    return nullptr;
}


Vertex* Vertex::get(const std::string& s)
{
    if(s == "")
        return nullptr;
    VIterator curr;
    VIterator end = Body->end();
    for(curr = Body->begin(); curr != end; ++curr)
        if((*curr)->name() == s)
            return (*curr);

    Link* r = new Link(s);
    Vertex::add(r);
    return r;
}


Vertex* Vertex::get(const std::string& type, const std::string& name)
{
    VIterator curr = Body->begin();
    VIterator end = Body->end();

    if(type == ANY)
    {
        for(; curr != end; ++curr)
            if((*curr)->name() == name)
                return (*curr);
    }
    else if(name == ANY)
    {
        for(; curr != end; ++curr)
            if((*curr)->is(type))
                return (*curr);
    }
    else
    {
        for(; curr != end; ++curr)
            if(((*curr)->name() == name) && (*curr)->is(type))
                return (*curr);
        if((type == FACTORY) || (type == FACTORYMAP))
            return HandServer::GetInstance()->get(type, name);
    }

    return nullptr;
}


Vertex* Vertex::get(uint i)
{
    if(i == 0)
        return nullptr;

    // 1-based
    --i;

    if(i < Body->size())
        return Body->at(i);

    return nullptr;
}


bool Vertex::remove(Vertex* child)
{
    if(!child->getOwner() || (child->getOwner() == this))
    {
        delete(child);
        return true;
    }
    return Vertex::detach(child);
}


bool Vertex::detach(Vertex* child)
{
    if(!child)
        return false;

    child->References->erase(this);
    return Body->erase(child);
}


uint Vertex::size()
{
    return Body->size();
}


void Vertex::type(const std::string& type)
{
    if(type.empty())
        return;

    if(!Type)
        Type = new Vertex(TYPE);

    Type->set(new Vertex(type));
}


std::string Vertex::type()
{
    // TODO: needs context sensitive type
    if(Type)
        return Type->Body->front()->name();

    return VERTEX;
}


bool Vertex::is(const std::string& type)
{
    if(type.empty())
        return false;

    if(!Type)
        return (type == VERTEX);

    VIterator end = Type->Body->end();
    for(VIterator curr=Type->Body->begin(); curr!=end; curr++)
        if((*curr)->name() == type)
            return true;

    return false;
}


bool Vertex::is(RegularExpression* se)
{
    if(!se)
        return false;

    if(!Type)
        return se->Matches(VERTEX);

    VIterator end = Type->Body->end();
    for(VIterator curr=Type->Body->begin(); curr!=end; curr++)
        if(se->Matches((*curr)->name()))
            return true;

    return false;
}


void Vertex::setOwner(Vertex* owner)
{
    VIterator curr = References->find(owner);
    if(curr != References->end())
    {
        if(curr == References->begin())
            return;
        References->erase(curr);
    }
    References->insert(References->begin(), owner);
}


Vertex* Vertex::getOwner()
{
    return References->front();
}


bool Vertex::isOpen(Search* search)
{
    if(Body->size() == 0)
        return true;
    // The search cookie should be the last element
    Vertex* last = Body->back();
    if(last && (last->name()==search->GetCookieName()))
        // Already searched from different branch
        return false;
    return true;
}


Vertex* Vertex::find(const std::string& name, int max_depth)
{
    int depth = 0;
    Vertex* result = nullptr;
    while(depth <= max_depth)
    {
        // Allow call of overloaded _Find
        result = _find(name, depth);

        if(result)
            break;
        depth++;
    }
    return result;
}


Vertex* Vertex::_find(const std::string& name, int depth)
{
    if(depth < 0)
        return nullptr;
    if(depth == 0)
    {
        if(Name == name)
            return this;
        return nullptr;
    }

    Vertex* ret = nullptr;
    uint s = Body->size();
    --depth;
    for(uint i=0; i<s; i++)
    {
        ret = Body->at(i)->Vertex::_find(name, depth);
        if(ret != nullptr)
            break;
    }
    return ret;
}


Vertex* Vertex::find(RegularExpression* expression)
{
    // "Plain" find, don't descend
    uint s = Body->size();
    Vertex* ret;
    for(uint i=0; i<s; i++)
    {
        ret = Body->at(i);
        if(expression->Matches(ret->name()))
            return ret;
    }
    return nullptr;
}
