#ifndef HAND_INPUT_DEVICE_H
#define HAND_INPUT_DEVICE_H

#include <vector>
#include <SDL/SDL.h>
#include "base/module.h"
#include "graph/data.h"


extern const std::string DriverNames[];


class Hand;

class Device : public virtual Module
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
    Interface* GetHmi() override { return m_Hmi; }

    const std::string& GetTypeName() { return GetDriverName(m_Type); }
    Driver GetTypeId() { return m_Type; }

    // Returns the Key symbol at specified position
    virtual void GetKeyName(int k, Note* a_out) = 0;

    virtual bool Process(const SDL_Event& event) = 0;

protected:
    Driver m_Type;
    Hand* m_Hand = nullptr;
    Interface* m_Hmi = nullptr;
};


class EventHandler
{
public:
    ~EventHandler();

    Hand* CreateHand(Device::Driver device);
    void GetUserInput();

private:
    std::vector<Device*> m_Devices;
};

#endif // HAND_INPUT_DEVICE_H
