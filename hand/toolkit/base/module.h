#ifndef HAND_BASE_HANDAPP_H
#define HAND_BASE_HANDAPP_H


class Interface;

class Module
{
public:
    virtual ~Module() = default;

    virtual Interface* GetHmi() = 0;
};

#endif // HAND_BASE_HANDAPP_H
