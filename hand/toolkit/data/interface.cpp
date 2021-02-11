#include "data/interface.h"
#include "view/layers/interface.h"
#include "view/layers/vector.h"


namespace Hmi {


Interface::Interface(const std::string& name, const std::string& description, Module* manipulator)
    : Map(name, description, manipulator),
      m_ViewStack("View", "")
{
    Add(CONTROL, new Vector("Controls", ""));

    static_cast<Layers::Vector*>(m_ViewStack.GetExpandedView())->SetExpandChildren(true);
    Attach(VIEW, &m_ViewStack);
}


void Interface::GetShortcuts(Hmi::Interface* caller, Hmi::List* out)
{
    if (caller != this)
        // Don't descend into sub-interfaces. TODO: exported shortcuts.
        return Item::GetShortcuts(caller, out);

    Map::GetShortcuts(caller, out);
}


void Interface::Expand(Item*)
{
    GetButtonView()->GetInterface()->Show(this, false);
}


Layer* Interface::CreateExpandedView()
{
    return new Layers::Interface();
}

}
