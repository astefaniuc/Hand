#include "graph/data.h"
#include "view/layer/textlayer.h"


void Data::SetManipulator(Module* a_manipulator)
{
    delete m_Manipulator;
    m_Manipulator = a_manipulator;
}


void Data::SetPersistence(Persistence* a_storage)
{
    delete m_Storage;
    m_Storage = a_storage;
}


Layer* Data::CreateLayer()
{
    return new TextLayer();
}
