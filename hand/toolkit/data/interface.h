#ifndef HAND_GRAPH_INTERFACE_H
#define HAND_GRAPH_INTERFACE_H

#include "data/item.h"
#include "data/list.h"


class Layer;

namespace Hmi {

class Interface : public Item
{
public:
    Interface(const std::string& name, const std::string& description)
        : Item(name, description), m_Controls("Controls", "") {}

    void SetView(Layer* view) { m_View = view; }
    Layer* GetView() const { return m_View; }

    List& GetControls() { return m_Controls; }

private:
    Layer* CreateExpandedView() override;

    Layer* m_View = nullptr;
    List m_Controls;
};

}
#endif // HAND_GRAPH_INTERFACE_H
