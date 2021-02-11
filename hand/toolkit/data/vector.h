#ifndef HAND_DATA_VECTOR_H
#define HAND_DATA_VECTOR_H

#include "data/list.h"


namespace Hmi {


class Vector : public List
{
public:
    Vector(
        const std::string& name,
        const std::string& description,
        Module* manipulator = nullptr)
        : List(name, description, manipulator) {}
    ~Vector() { Clear(); }

    /// Insert a child item and assume ownership of it.
    void Add(Item* child) override;
    /// Insert a child item without assuming ownership of it.
    void Attach(Item* child) override;
    bool Contains(Item* item) override;
    /// Removes the item from the children list. Owned children are destroyed.
    void Remove(Item* child) override;

    Item* GetChild(const std::string& name) const override;
    Item* GetChild(unsigned position) override { return m_Value[position]; }

    /// Returns the number of children.
    unsigned Size() const override { return m_Value.size(); }
    /// Removes all child items; own items are destroyed.
    void Clear() override;

    std::string GetValueString() override { return "TODO"; }

private:
    Layer* CreateExpandedView() override;

    std::vector<Item*> m_Value;
};

}
#endif //HAND_DATA_VECTOR_H
