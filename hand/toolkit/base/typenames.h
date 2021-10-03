#ifndef HAND_BASE_TYPESTRINGS_H
#define HAND_BASE_TYPESTRINGS_H

#include <string>


template<typename T>
std::string GetTypeName() { return typeid(T).name(); }

#endif // HAND_BASE_TYPESTRINGS_H
