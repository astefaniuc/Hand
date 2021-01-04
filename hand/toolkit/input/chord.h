#ifndef HAND_INPUT_CHORD_H
#define HAND_INPUT_CHORD_H

#include <vector>
#include <initializer_list>


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

    Chord(std::initializer_list<Finger> a_keys = {}) : keys(a_keys) {}
    virtual ~Chord() = default;

    static Chord FullHand() { return Chord({ Thumb, Pointer, Middle, Ring, Little }); }

    virtual bool IsValid(const Chord& input);

    std::vector<Finger> keys;
};

#endif // HAND_INPUT_CHORD_H
