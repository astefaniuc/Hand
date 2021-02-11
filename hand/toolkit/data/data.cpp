#include "data/data.h"
#include "view/layers/data.h"
#include "view/layers/interface.h"


namespace Hmi {


Data::Data(const std::string& name, const std::string& description, Module* manipulator)
    : Item(name, description), m_Manipulator(manipulator)
{
    ActivationListeners.Add(this, &Data::ShowHide);
}


Data::~Data()
{
    delete m_Manipulator;
}


void Data::SetManipulator(Module* manipulator)
{
    delete m_Manipulator;
    m_Manipulator = manipulator;
}


void Data::ShowHide(Item*)
{
    if (!m_Manipulator)
        return;

    GetButtonView()->GetInterface()->Show(m_Manipulator->GetHmi(), false);
}


void Data::SetPersistence(Persistence* storage)
{
    delete m_Storage;
    m_Storage = storage;
}


Layer* Data::CreateExpandedView()
{
    return new Layers::Data();
}


Layer* Data::CreateButtonView()
{
    return new Layers::Data();
}

}
