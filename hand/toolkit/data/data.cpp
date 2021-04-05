#include "data/data.h"
#include "view/layers/hmi/item.h"
#include "view/layers/text.h"


namespace Hmi {


Data::~Data()
{
    delete m_Manipulator;
}


void Data::SetManipulator(Manipulator::Base* manip)
{
    delete m_Manipulator;
    m_Manipulator = manip;
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


template<>
std::string TData<uint8_t>::GetValueString()
{
    return std::to_string(m_Value);
}

}
