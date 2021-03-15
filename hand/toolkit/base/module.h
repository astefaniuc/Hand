#ifndef HAND_BASE_HANDAPP_H
#define HAND_BASE_HANDAPP_H


class Layer;

class Module
{
public:
    virtual ~Module() = default;

    virtual void GetHmi(Layer* caller) = 0;
};

#endif // HAND_BASE_HANDAPP_H
