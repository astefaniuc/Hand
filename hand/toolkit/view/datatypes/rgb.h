#ifndef HAND_VIEW_DATATYPES_RGB_H
#define HAND_VIEW_DATATYPES_RGB_H

#include "data/map.h"


class Rgb : public Data::Map
{
public:
    Rgb(
        const std::string& name, const std::string& description = std::string(),
        uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
    void Reset();

    uint8_t m_r;
    uint8_t m_g;
    uint8_t m_b;
};

#endif // HAND_VIEW_DATATYPES_RGB_H
