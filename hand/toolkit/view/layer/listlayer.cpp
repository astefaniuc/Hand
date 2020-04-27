#include "view/layer/listlayer.h"
#include "view/layer/layermanager.h"
#include "graph/method.h"
#include "graph/data.h"


ListLayer::ListLayer(const std::string& name) : Layer(name)
{
    type(LIST);
    BufferType = COLLECTOR;
}


void ListLayer::Init()
{
    Vertex* focus = new Method<ListLayer>("SetFocus", this, &ListLayer::SetFocus);
    focus->get("Parameter")->set(get(CONTENT)->get());
    get(FOCUS)->set(focus);

    focus = new Method<ListLayer>("SetFocusOnControls", this, &ListLayer::SetFocus);
    focus->get("Parameter")->set(GetControlsList(get(LINK, CHILDREN)));
    get(FOCUS)->attach(focus);

    get(EXECUTE)->set(get(FOCUS)->get());
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
    Vertex* tgt= GetControlsList(get(LINK, CHILDREN));
    if(!tgt)
        return false;

    Vertex* cmd = Vertex::get(LAYERMANAGER)->get()->get(ANY, COMMANDS);

    // Destroy the previous focus
    Vertex* foc_cmds = cmd->Vertex::get(DEVICE, ANY)->Vertex::get(FOCUS);
    unsigned i = 0;
    {
        Vertex* fc;
        while((fc=foc_cmds->get(++i)) != nullptr)
        {
            fc->Vertex::detach(fc->Vertex::get(METHOD, ANY));
            delete(fc->get(VIEW)->Vertex::get(LAYER, CONTROLID));
        }
        foc_cmds->reset();
    }

    {
        Vertex* back = get(BACK)->get();
        if(back)
        {
            (dynamic_cast<LayerManager*>(Vertex::get(LAYERMANAGER)->get()))->GetCommand(back, 2);
            foc_cmds->attach(back->Vertex::get(COMMAND)->get());
        }
    }

    Layer* child;
    i = 0;
    Vertex* curr_cmd;
    while((child=dynamic_cast<Layer*>(tgt->get(++i))) != nullptr)
    {
        curr_cmd = cmd->get(i);
        if(child->Layer::SetCommand(curr_cmd))
        {
            // Store current focus
            foc_cmds->attach(curr_cmd);
            // Add a back reference
            child->get(BACK)->set(get(EXECUTE)->get());
        }
    }
    return true;
}


Vertex* ListLayer::GetControlsList(Vertex* curr_list)
{
    if(!curr_list)
        return nullptr;

    Vertex* child;
    unsigned i = 0;
    while((child=curr_list->get(++i)) != nullptr)
    {
        if(child->is(BUTTON))
            return curr_list;

        else if((child=GetControlsList(child->get(LINK, CHILDREN))) != nullptr)
            return child;
    }
    return nullptr;
}


Vertex* ListLayer::GetLayout(Vertex* data)
{
    // The LIST needs two different COORDINATES rects: one for the blit
    // on the parent surface and one to calculate the children
    Vertex* layout = get(THEME)->get(THEME, ANY)->get(LAYOUT)->get();
    layout->name("Buffer");
    layout->get(COORDINATES)->Vertex::get(REQUEST)->get(RECT)->get(FULL);
    layout->add(Layer::GetLayout(data));

    return layout;
}
