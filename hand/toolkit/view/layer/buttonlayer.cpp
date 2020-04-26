#include "view/layer/buttonlayer.h"
#include "view/layer/textlayer.h"
#include "graph/method.h"
#include "graph/data.h"


ButtonLayer::ButtonLayer(const std::string& name) : Layer(name)
{
    BufferType = COLLECTOR;
    type(BUTTON);
}


ButtonLayer::~ButtonLayer()
{
    delete FunctionBox;
}


void ButtonLayer::SetContent(Vertex* data)
{
    Layer::SetContent(data);
    get(EXECUTE)->set(data);

    // Create the sub-elements of the button
    // The preview
/*    if(data->is(LIST))
    {
        SetExpandable();
        // Add list as preview
        Insert(data, PREVIEW);
    }*/

    Insert(data, NAME);
    Insert(data->Vertex::get(NOTE, DESCRIPTION), DESCRIPTION);
    Insert(data->Vertex::get(ANY, ICON), ICON);
    Vertex* cmd = data->Vertex::get(COMMAND)->get();
    if(cmd)
        Insert(cmd->get(VIEW), CONTROLID);
}


void ButtonLayer::SetExpandable()
{
    FunctionBox = new Method<ButtonLayer>("ExpandList", this, &ButtonLayer::ExpandList);
}


bool ButtonLayer::ExpandList(Vertex*)
{
    return ParentLayer->Request(GetContent());
}
