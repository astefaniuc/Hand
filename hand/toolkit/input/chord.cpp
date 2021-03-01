#include "input/chord.h"
#include "input/hand.h"
#include "view/layers/text.h"


bool Chord::IsValid(const Chord& input)
{
    if (!keys.size() || (keys.size() != input.keys.size()))
        return false;

    for (unsigned i = 0; i < keys.size(); ++i)
        if (keys[i] != input.keys[i])
            return false;
    return true;
}


bool Chord::Assign(Layer* item)
{
    if (m_Item)
        return false;

    m_Item = item;
    return true;
}


Layer* Chord::GetLayer(Hand* hand)
{
    if (m_Layer)
        // TODO: different hands
        return m_Layer;

    std::string shrtct;
    int last = int(keys.size()) - 1;
    for (size_t i = 0; i < keys.size(); ++i)
    {
        shrtct += hand->GetKey(keys[i])->GetValue();
        if (i < last)
            shrtct += "+";
    }

    m_Layer = new Layers::Text(shrtct);
    return m_Layer;
}
