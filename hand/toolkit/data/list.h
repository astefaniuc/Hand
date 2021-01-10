#ifndef HAND_DATA_LIST_H
#define HAND_DATA_LIST_H

#include "data/data.h"


namespace Hmi {


class List : public Data
{
public:
    List(const std::string& name, const std::string& description, Module* manipulator)
        : Data(name, description, manipulator) {}

    /// Insert a child item and assume ownership of it.
    virtual void Add(Item* child) = 0;
    /// Insert a child item without assuming ownership of it.
    virtual void Attach(Item* child) = 0;
    /// Removes the item from the children list. Owned children are destroyed.
    virtual void Remove(Item* child) = 0;

    virtual Item* GetChild(const std::string& name) const = 0;
    virtual Item* GetChild(unsigned position) = 0;

    /// Returns the number of children.
    virtual unsigned Size() const = 0;
    /// Removes all child items; own items are destroyed.
    virtual void Clear() = 0;

protected:
    Layer* CreateButtonView() override;
    void ConnectButton(Layer* view) override;

    ICallback* m_ButtonActivation = nullptr;
};

}
#endif // HAND_DATA_LIST_H
