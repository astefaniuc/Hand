#ifndef HAND_VIEW_DATATYPES_LAYOUT_H
#define HAND_VIEW_DATATYPES_LAYOUT_H

#include "base/module.h"


class Layout : public Module
{
public:
    virtual void Draw() = 0;
};

#endif // HAND_VIEW_DATATYPES_LAYOUT_H
