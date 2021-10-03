#ifndef HAND_GRAPH_INTERFACE_H
#define HAND_GRAPH_INTERFACE_H

#include "data/map.h"
#include "data/vector.h"
#include "include/stdfields.h"
#include "base/factory.h"


class Layer;
namespace Layers { class Interface; }

namespace Data {


class Interface : public Map
{
public:
    Interface(
        const std::string& name,
        const std::string& description,
        Manipulator::Base* manip = nullptr);

    Vector* GetView() { return &m_ViewStack; }

    Vector* GetControls() { return static_cast<Vector*>(GetChild(CONTROLS)); }
    void AddControl(Item* control) { GetControls()->Add(control); }
    void AttachControl(Item* control) { GetControls()->Attach(control); }

    Interface* GetInterface() override { return this; }

    void GetShortcuts(Interface* caller, List* out) override;

    void DeleteCb(Layer*) { delete this; }


    void AddFactory(IFactory<Manipulator::Base>* f, const std::string& target)
    {
        m_ManipulatorFactories.Register(f, target);
    }

    Manipulator::Base* GetManipulator(const std::string& type);

protected:
    void Execute(Layers::Item* caller) override;
    Layer* CreateExpandedView() override;
    Layer* CreateExpandedData() override;

    Vector m_ViewStack;

    FactorySet<Manipulator::Base> m_ManipulatorFactories;
};

}
#endif // HAND_GRAPH_INTERFACE_H
