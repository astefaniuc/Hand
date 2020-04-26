#ifndef HAND_BASE_HANDAPP_H
#define HAND_BASE_HANDAPP_H

#include "graph/list.h"


class HandApp : public List
{
public:
    HandApp(const std::string& name) : List(name) { type(HANDAPP); }
};

#endif // HAND_BASE_HANDAPP_H
