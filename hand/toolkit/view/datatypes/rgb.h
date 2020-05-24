#ifndef HAND_VIEW_DATATYPES_RGB_H
#define HAND_VIEW_DATATYPES_RGB_H

#include "graph/collection.h"


class Rgb : public Collection
{
public:
    Rgb(const std::string& name, const std::string& description, uint8_t r, uint8_t g, uint8_t b);
    void Reset();

    uint8_t m_r;
    uint8_t m_g;
    uint8_t m_b;
};

#endif // HAND_VIEW_DATATYPES_RGB_H
