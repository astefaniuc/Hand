#ifndef HAND_VIEW_DATATYPES_RGB_H
#define HAND_VIEW_DATATYPES_RGB_H

#include <SDL/SDL.h>
#include "graph/vertex.h"


class Rgb : public Vertex
{
public:
    Rgb(const std::string& name, Uint8 r_, Uint8 g_, Uint8 b_);
    void reset() override;

    Uint8 r;
    Uint8 g;
    Uint8 b;
};


// Helper func for extracting a Rgb from a Vertex tree
Rgb* GetRgb(const std::string& rgb_name, Vertex* from_tree);


#endif // HAND_VIEW_DATATYPES_RGB_H
