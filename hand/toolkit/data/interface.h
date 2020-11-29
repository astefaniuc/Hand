#ifndef HAND_GRAPH_INTERFACE_H
#define HAND_GRAPH_INTERFACE_H

#include "data/hmiitem.h"
#include "data/collection.h"


class Layer;

class Interface : public HmiItem
{
public:
    Interface(const std::string& name, const std::string& description)
        : HmiItem(name, description), m_Controls("Controls", "") {}

    void SetView(Layer* view) { m_View = view; }
    Layer* GetView() const { return m_View; }

    Collection& GetControls() { return m_Controls; }

private:
    Layer* CreateExpandedView() override;

    Layer* m_View = nullptr;
    Collection m_Controls;
};

#endif // HAND_GRAPH_INTERFACE_H
