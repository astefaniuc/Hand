#ifndef HAND_INPUT_CHORD_H
#define HAND_INPUT_CHORD_H

#include <vector>
#include <initializer_list>


class Layer;
class Hand;
namespace Hmi { class Item; }

class Chord
{
public:
    enum Finger
    {
        Thumb = 0,
        Pointer,
        Middle,
        Ring,
        Little
    };
    enum Match
    {
        Exactly,
        Pressed,
    };

    Chord(std::initializer_list<Finger> a_keys = {}) : keys(a_keys) {}
    virtual ~Chord() = default;

    static Chord* FullHand() { return new Chord({ Thumb, Pointer, Middle, Ring, Little }); }

    virtual bool IsValid(const Chord& input);

    bool Assign(Hmi::Item* item);
    Hmi::Item* GetItem() const { return m_Item; }
    void ClearItem() { m_Item = nullptr; }

    Layer* GetLayer(Hand* hand);

    std::vector<Finger> keys;

private:
    Hmi::Item* m_Item = nullptr;
    Layer* m_Layer = nullptr;
};

#endif // HAND_INPUT_CHORD_H
