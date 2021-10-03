#include "view/datatypes/rgb.h"


Rgb::Rgb(
    const std::string& name, const std::string& description,
    uint8_t r, uint8_t g, uint8_t b)
    : Data::Map(name, description)
{
    // For now add plain data directly
    Add("r", new Data::Uint8("r", "", r));
    Add("g", new Data::Uint8("g", "", g));
    Add("b", new Data::Uint8("b", "", b));
    m_r = r;
    m_g = g;
    m_b = b;
}


void Rgb::Reset()
{
    m_r = ((Data::Uint8*)GetChild("r"))->GetValue();
    m_g = ((Data::Uint8*)GetChild("g"))->GetValue();
    m_b = ((Data::Uint8*)GetChild("b"))->GetValue();
}
