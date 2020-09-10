#ifndef HAND_INPUT_DEVICE_H
#define HAND_INPUT_DEVICE_H

#include <vector>
#include <SDL/SDL.h>
#include "base/module.h"
#include "graph/data.h"


extern const std::string DriverNames[];


class Hand;

class Device : public Module
{
public:
    enum Driver
    {
        Keyboard,
        Mouse,
        Controller
    };

    Device(Driver id) : m_Type(id) {}
    virtual ~Device() {}

    static Driver GetDriverId(const std::string& typeName);
    static const std::string& GetDriverName(Driver id);

    virtual void SetUser(Hand* hand) { m_Hand = hand; }

    const std::string& GetTypeName() { return GetDriverName(m_Type); }
    Driver GetTypeId() { return m_Type; }

    // Returns the Key symbol at specified position
    virtual void GetKeyName(int k, Note* a_out) = 0;

    virtual bool Process(const SDL_Event& event) = 0;

protected:
    Driver m_Type;
    Hand* m_Hand = nullptr;
};

#endif // HAND_INPUT_DEVICE_H
