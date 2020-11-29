#ifndef HAND_BASE_HANDAPP_H
#define HAND_BASE_HANDAPP_H


namespace Hmi { class Item; }

class Module
{
public:
    virtual ~Module() = default;

    virtual Hmi::Item* GetHmi() = 0;
};

#endif // HAND_BASE_HANDAPP_H
