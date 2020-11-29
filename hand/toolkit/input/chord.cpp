#include "input/chord.h"


bool Chord::IsValid(const Chord& input)
{
    if (!keys.size() || (keys.size() != input.keys.size()))
        return false;

    for (unsigned i = 0; i < keys.size(); ++i)
        if (keys[i] != input.keys[i])
            return false;
    return true;
}
