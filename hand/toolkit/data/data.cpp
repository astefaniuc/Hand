#include "data/data.h"
#include "view/layers/hmi/data.h"
#include "view/layers/hmi/interface.h"
#include "view/layers/text.h"


namespace Hmi {


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

    caller->GetInterface()->Show(m_Manipulator->GetHmi()->GetExpandedView());
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


Layer* Data::CreateCompressedData()
{
    return new Layers::Text(GetValueString());
}

}
