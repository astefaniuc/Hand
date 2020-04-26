#include "view/datatypes/rgb.h"
#include "graph/data.h"


Rgb::Rgb(const std::string& name, Uint8 r_, Uint8 g_, Uint8 b_) : Vertex(name)
{
    type(RGB);
    // For now add plain data directly
    add(new Data<Uint8>("r", r_));
    add(new Data<Uint8>("g", g_));
    add(new Data<Uint8>("b", b_));
    r = r_;
    g = g_;
    b = b_;
}


void Rgb::reset()
{
    r = ((Data<Uint8>*)get("r"))->get();
    g = ((Data<Uint8>*)get("g"))->get();
    b = ((Data<Uint8>*)get("b"))->get();
}


Rgb* GetRgb(const std::string& name, Vertex* tree)
{
    Vertex* rgb = tree->get(ANY, "Color");
    if(rgb)
        rgb = rgb->get();
    return dynamic_cast<Rgb*>(rgb);
}
