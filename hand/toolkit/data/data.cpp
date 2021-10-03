#include "data/data.h"
#include "view/layers/hmi/interface.h"
#include "view/layers/hmi/item.h"
#include "view/layers/text.h"


namespace Data {


Base::~Base()
{
    delete m_Manipulator;
}


void Base::SetManipulator(Manipulator::Base* manip)
{
    delete m_Manipulator;
    m_Manipulator = manip;
}


Manipulator::Base* Base::GetManipulator()
{
    if (m_Manipulator)
        m_Manipulator = GetInterface()->GetManipulator(GetTypeName());

    return m_Manipulator;
}


void Base::Execute(Layers::Item* caller)
{
    if (!m_Manipulator)
        return;

    m_Manipulator->GetHmi(caller);
}


void Base::SetPersistence(Persistence* storage)
{
    delete m_Storage;
    m_Storage = storage;
}


Layer* Base::CreateCompressedData()
{
    return new Layers::DataText();
}


template<>
std::string Typed<uint8_t>::GetValueString()
{
    return std::to_string(m_Value);
}

}
