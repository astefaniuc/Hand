#ifndef HAND_BASE_DATAMANAGER_H
#define HAND_BASE_DATAMANAGER_H

#include "base/factory.h"


class DataManager : public FactoryMap
{
public:
    DataManager() : FactoryMap("DefaultDataManager") {}

protected:
    void Init();
};


class Persistence : public Factory
{
public:
    Persistence() : Factory("Persistence", LIST, LIST) {}

    bool execute(Vertex* descriptor);
    bool IsValidInput(Vertex* input);
};

#endif // HAND_BASE_DATAMANAGER_H
