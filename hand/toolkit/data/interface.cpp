#include "data/interface.h"
#include "view/layers/hmi/interface.h"
#include "view/layers/vector.h"


namespace Data {


Interface::Interface(
    const std::string& name, const std::string& description, Manipulator::Base* manip)
    : Map(name, description, manip),
      m_ViewStack("View", "")
{
    Add(CONTROLS, new Vector("Controls", ""));
    Attach(VIEW, &m_ViewStack);
}


void Interface::GetShortcuts(Interface* caller, List* out)
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
    return ret;
}


Manipulator::Base* Interface::GetManipulator(const std::string& type)
{
    Manipulator::Base* ret = m_ManipulatorFactories.Create(type);
    if (!ret)
    {
        List* parent = GetParent();
        if (parent)
        {
            Interface* parentIface = parent->GetInterface();
            if (parentIface)
                return parentIface->GetManipulator(type);
        }
    }

    return ret;
}

}
