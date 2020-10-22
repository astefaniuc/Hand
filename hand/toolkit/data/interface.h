#ifndef HAND_GRAPH_INTERFACE_H
#define HAND_GRAPH_INTERFACE_H

#include "data/hmiitem.h"


class Layer;

class Interface : public HmiItem
{
public:
    Interface(const std::string& name, const std::string& description)
        : HmiItem(name, description) {}

    void SetView(Layer* view) { m_View = view; }
    Layer* GetView() const { return m_View; }

    void SetControls(HmiItem* controls) { m_Controls = controls; }
    HmiItem* GetControls() const { return m_Controls; }

private:
    Layer* CreateLayer() override;

    Layer* m_View = nullptr;
    HmiItem* m_Controls = nullptr;
};

#endif // HAND_GRAPH_INTERFACE_H
