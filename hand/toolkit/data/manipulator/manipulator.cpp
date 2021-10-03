#include "data/manipulator/manipulator.h"
#include "view/layers/hmi/interface.h"


template<> std::string GetTypeName<int>() { return "int"; }
template<> std::string GetTypeName<bool>() { return "bool"; }
template<> std::string GetTypeName<uint8_t>() {  return "uint8_t"; }
template<> std::string GetTypeName<std::string>() {  return "string"; }


namespace Manipulator
{

void Base::GetHmi(Layer* target)
{
    target->GetInterface()->Show(m_Interface->GetExpandedView());
}

}
