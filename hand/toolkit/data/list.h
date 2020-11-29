#ifndef HAND_DATA_LIST_H
#define HAND_DATA_LIST_H

#include "data/data.h"


namespace Hmi {


class List : public Data
{
public:
    List(
        const std::string& name,
        const std::string& description,
        Module* manipulator = nullptr)
        : Data(name, description, manipulator) {}
    ~List() { Clear(); }

    /// Insert a child item and assume ownership of it.
    void Add(Item* child);
    /// Insert a child item without assuming ownership of it.
    void Attach(Item* child) { m_Value.push_back(child); }
    /// Removes the item from the children list. Owned children are destroyed.
    void Remove(Item* child);

    Item* GetChild(const std::string& name) const;
    Item* GetChild(unsigned position) { return m_Value[position]; }

    /// Returns the number of children.
    unsigned Size() { return m_Value.size(); }
    /// Removes all child items; own items are destroyed.
    void Clear();

    std::string GetValueString() override { return "TODO"; }

private:
    Layer* CreateExpandedView() override;

    std::vector<Item*> m_Value;
};

}
#endif // HAND_DATA_LIST_H
