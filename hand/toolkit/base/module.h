#ifndef HAND_BASE_HANDAPP_H
#define HAND_BASE_HANDAPP_H

#include "graph/interface.h"


class Module
{
public:
    virtual ~Module() = default;

    virtual Interface* GetHmi() = 0;
};

#endif // HAND_BASE_HANDAPP_H
