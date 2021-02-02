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
        Module* manipulator = nullptr)
        : Map(name, description, manipulator) { Add(CONTROL, new Vector("Controls", "")); }

    void SetView(Item* view) { Add(VIEW, view); }
    Item* GetView() const { return GetChild(VIEW); }

    Vector* GetControls() { return static_cast<Vector*>(GetChild(CONTROL)); }
    void AddControl(Item* control) { GetControls()->Add(control); }
    void AttachControl(Item* control) { GetControls()->Attach(control); }

    Interface* GetInterface() override { return this; }

private:
    Layer* CreateExpandedView() override;
    void ConnectButton(Layer* view) override;
};

}
#endif // HAND_GRAPH_INTERFACE_H
