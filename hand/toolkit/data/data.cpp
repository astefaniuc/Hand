#include "data/data.h"
#include "view/layers/data.h"


Data::~Data()
{
    delete m_Manipulator;
    for (ICallback* ic : m_DataChanged)
        delete ic;
}


void Data::SetManipulator(Module* manipulator)
{
    delete m_Manipulator;
    m_Manipulator = manipulator;
}


void Data::SetPersistence(Persistence* storage)
{
    delete m_Storage;
    m_Storage = storage;
}


Layer* Data::CreateLayer()
{
    return new Layers::DataLayer();
}
