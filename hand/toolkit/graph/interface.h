#ifndef HAND_GRAPH_INTERFACE_H
#define HAND_GRAPH_INTERFACE_H

#include "graph/vertex.h"


class Layer;

class Interface : public HmiItem
{
public:
    Interface(const std::string& a_name, const std::string& a_description)
        : HmiItem(a_name, a_description) {}

    void SetView(Layer* view) { m_View = view; }
    Layer* GetView() const { return m_View; }

    void SetControls(HmiItem* controls) { m_Controls = controls; }
    HmiItem* GetControls() const { return m_Controls; }

    void SetAuxilliary(HmiItem* aux) { m_Auxilliary = aux; }
    HmiItem* GetAuxilliary() const { return m_Auxilliary; }

private:
    Layer* CreateLayer() override;

    Layer* m_View = nullptr;
    HmiItem* m_Controls = nullptr;
    HmiItem* m_Auxilliary = nullptr;
};

#endif // HAND_GRAPH_INTERFACE_H
