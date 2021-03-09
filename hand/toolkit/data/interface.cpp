#include "data/interface.h"
#include "view/layers/hmi/interface.h"
#include "view/layers/vector.h"


namespace Hmi {


Interface::Interface(const std::string& name, const std::string& description, Module* manipulator)
    : Map(name, description, manipulator),
      m_ViewStack("View", "")
{
    Add(CONTROL, new Vector("Controls", ""));
    Attach(VIEW, &m_ViewStack);
}


void Interface::GetShortcuts(Hmi::Interface* caller, Hmi::List* out)
{
    if (caller != this)
        // Don't descend into sub-interfaces. TODO: exported shortcuts.
        return Item::GetShortcuts(caller, out);

    Map::GetShortcuts(caller, out);
}


void Interface::Execute(Layers::Item* caller)
{
    caller->GetInterface()->Show(GetExpandedView());
}


Layer* Interface::CreateExpandedView()
{
    return new Layers::Interface();
}


Layer* Interface::CreateExpandedData()
{
    Layers::List* ret = new Layers::Vector();
    ret->SetExpandChildren(true);
    ret->SetData(GetView());
    return ret;
}

}
