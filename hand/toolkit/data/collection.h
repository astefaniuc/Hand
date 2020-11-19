#ifndef HAND_GRAPH_LIST_H
#define HAND_GRAPH_LIST_H

#include "data/data.h"


class Collection : public Data
{
public:
    Collection(
        const std::string& name,
        const std::string& description,
        Module* manipulator = nullptr)
        : Data(name, description, manipulator) {}
    ~Collection() { Clear(); }

    /// Insert a child item and assume ownership of it.
    void Add(HmiItem* child);
    /// Insert a child item without assuming ownership of it.
    void Attach(HmiItem* child) { m_Value.push_back(child); }
    /// Removes the item from the children list. Owned children are destroyed.
    void Remove(HmiItem* child);

    HmiItem* GetChild(const std::string& name) const;
    HmiItem* GetChild(unsigned position) { return m_Value[position]; }

    /// Returns the number of children.
    unsigned Size() { return m_Value.size(); }
    /// Removes all child items; own items are destroyed.
    void Clear();

    std::string GetValueString() override { return "TODO"; }

private:
    Layer* CreateExpandedView() override;

    std::vector<HmiItem*> m_Value;
};

#endif // HAND_GRAPH_LIST_H
