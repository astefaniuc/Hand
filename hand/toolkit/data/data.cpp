#include "data/data.h"
#include "view/layers/hmi/interface.h"
#include "view/layers/text.h"


namespace Hmi {


void Show(Layer* target, Interface* view)
{
    target->GetInterface()->Show(view->GetExpandedView());
}



Data::Data(const std::string& name, const std::string& description, Module* manipulator)
    : Item(name, description), m_Manipulator(manipulator)
{
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


void Data::Execute(Layers::Item* caller)
{
    if (!m_Manipulator)
        return;

    m_Manipulator->GetHmi(caller);
}


void Data::SetPersistence(Persistence* storage)
{
    delete m_Storage;
    m_Storage = storage;
}


Layer* Data::CreateCompressedData()
{
    return new Layers::DataText();
}

}
