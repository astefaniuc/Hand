#ifndef HAND_INPUT_CHORD_H
#define HAND_INPUT_CHORD_H

#include <vector>
#include <initializer_list>


class Layer;
class Hand;

struct Chord
{
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

    Chord(std::initializer_list<Finger> a_keys = {}) : Keys(a_keys) {}
    virtual ~Chord() = default;

    static Chord* FullHand() { return new Chord({ Thumb, Pointer, Middle, Ring, Little }); }

    virtual bool IsValid(const Chord& input);

    Layer* CreateLayer(Hand* hand);

    std::vector<Finger> Keys;
    Layer* Item = nullptr;
};

#endif // HAND_INPUT_CHORD_H
