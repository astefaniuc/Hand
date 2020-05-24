#include "view/datatypes/rgb.h"
#include "graph/data.h"


Rgb::Rgb(
    const std::string& name, const std::string& description,
    uint8_t r, uint8_t g, uint8_t b)
    : Collection(name, description)
{
    // For now add plain data directly
    Add(new TData<uint8_t>("r", "", r));
    Add(new TData<uint8_t>("g", "", g));
    Add(new TData<uint8_t>("b", "", b));
    m_r = r;
    m_g = g;
    m_b = b;
}


void Rgb::Reset()
{
    m_r = ((TData<uint8_t>*)GetChild("r"))->GetValue();
    m_g = ((TData<uint8_t>*)GetChild("g"))->GetValue();
    m_b = ((TData<uint8_t>*)GetChild("b"))->GetValue();
}
