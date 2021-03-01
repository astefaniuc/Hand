#ifndef HAND_INPUT_CHORD_H
#define HAND_INPUT_CHORD_H

#include <vector>
#include <initializer_list>


class Layer;
class Hand;

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

    bool Assign(Layer* item);
    Layer* GetItem() const { return m_Item; }
    void ClearItem() { m_Item = nullptr; }

    Layer* CreateLayer(Hand* hand);

    std::vector<Finger> keys;

private:
    Layer* m_Item = nullptr;
};

#endif // HAND_INPUT_CHORD_H
