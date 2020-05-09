#ifndef BASE_FACTORY_H
#define BASE_FACTORY_H

#include "graph/list.h"


class Factory
{
public:
    Factory(
        const std::string& name, const std::string& input_type, const std::string& output_type);
    virtual ~Factory() {}

    virtual bool execute(Vertex* input) = 0;
    virtual bool IsValidInput(Vertex* input);
};


class FactoryMap
{
public:
    FactoryMap(const std::string& name);
    virtual ~FactoryMap() {}

    virtual bool execute(Vertex* input);
    virtual bool Resolve(Vertex* input);

    Factory* GetFactory(Vertex* input);
    Factory* GetFactory(const std::string& output_type);
};

#endif // HAND_BASE_FACTORY_H
