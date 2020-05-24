#include "view/layer/listlayer.h"
#include "graph/method.h"
#include "graph/data.h"
#include "user.h"


void ListLayer::Init()
{
    // TODO?
}


void ListLayer::SetContent(Vertex* data)
{
    // Connect list and layer
    Layer::SetContent(data);

    unsigned nr_of_childs = data->size();
    nr_of_childs--;
    Data<unsigned>* max_c = dynamic_cast<Data<unsigned>*>(get(LAYOUT, ANY)->get("MaxSize"));
    if(max_c)
    {
        unsigned max_size = max_c->get();
        if(max_size < nr_of_childs)
            nr_of_childs = max_size;
    }

    Vertex* child;
    unsigned i = 0;
    while((child=data->get(++i)) != nullptr)
        // Create the sub-objects
        Insert(child, ELEMENT);
}


bool ListLayer::SetFocus(Vertex*)
{
    HmiItem* tgt= GetControlsList();
    if(!tgt)
        return false;

    // TODO
    return true;
}


HmiItem* ListLayer::GetControlsList()
{
    if(!m_Data)
        return nullptr;

    // TODO
    return nullptr;
}
