#ifndef HAND_GRAPH_INTERFACE_H
#define HAND_GRAPH_INTERFACE_H

#include "data/map.h"
#include "data/vector.h"


class Layer;

namespace Hmi {


class Interface : public Map
{
public:
    Interface(
        const std::string& name,
        const std::string& description,
        Module* manipulator = nullptr);

    Vector* GetView() { return &m_ViewStack; }

    Vector* GetControls() { return static_cast<Vector*>(GetChild(CONTROL)); }
    void AddControl(Item* control) { GetControls()->Add(control); }
    void AttachControl(Item* control) { GetControls()->Attach(control); }

    Interface* GetInterface() override { return this; }

    void GetShortcuts(Hmi::Interface* caller, Hmi::List* out) override;

private:
    void Expand(Item*) override;
    Layer* CreateExpandedView() override;

    Vector m_ViewStack;
};

}
#endif // HAND_GRAPH_INTERFACE_H
