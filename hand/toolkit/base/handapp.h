#ifndef HAND_BASE_HANDAPP_H
#define HAND_BASE_HANDAPP_H

#include "graph/vertex.h"


class HandApp
{
public:
    virtual ~HandApp() = default;

    virtual HmiItem* GetHmi() = 0;
};

#endif // HAND_BASE_HANDAPP_H
