#include "view/datatypes/rgb.h"
#include "data/data.h"


Rgb::Rgb(
    const std::string& name, const std::string& description,
    uint8_t r, uint8_t g, uint8_t b)
    : Hmi::List(name, description)
{
    // For now add plain data directly
    Add(new Hmi::TData<uint8_t>("r", "", r));
    Add(new Hmi::TData<uint8_t>("g", "", g));
    Add(new Hmi::TData<uint8_t>("b", "", b));
    m_r = r;
    m_g = g;
    m_b = b;
}


void Rgb::Reset()
{
    m_r = ((Hmi::TData<uint8_t>*)GetChild("r"))->GetValue();
    m_g = ((Hmi::TData<uint8_t>*)GetChild("g"))->GetValue();
    m_b = ((Hmi::TData<uint8_t>*)GetChild("b"))->GetValue();
}
