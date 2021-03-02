#include "input/chord.h"
#include "input/hand.h"
#include "view/layers/text.h"


bool Chord::IsValid(const Chord& input)
{
    if (!Keys.size() || (Keys.size() != input.Keys.size()))
        return false;

    for (unsigned i = 0; i < Keys.size(); ++i)
        if (Keys[i] != input.Keys[i])
            return false;
    return true;
}


Layer* Chord::CreateLayer(Hand* hand)
{
    std::string shrtct;
    int last = int(Keys.size()) - 1;
    for (size_t i = 0; i < Keys.size(); ++i)
    {
        shrtct += hand->GetKey(Keys[i])->GetValue();
        if (i < last)
            shrtct += "+";
    }

    return new Layers::Text(shrtct);
}
