#ifndef HAND_GRAPH_LIST_H
#define HAND_GRAPH_LIST_H

#include "graph/data.h"


class Collection : public Data
{
public:
    Collection(
        const std::string& a_name,
        const std::string& a_description,
        Module* a_manipulator = nullptr)
        : Data(a_name, a_description, a_manipulator) {}
    ~Collection() { Clear(); }

    /// Insert a child item and assume ownership of it.
    void Add(HmiItem* a_child);
    /// Insert a child item without assuming ownership of it.
    void Attach(HmiItem* a_child) { m_Value.push_back(a_child); }
    /// Removes the item from the children list. If it is the owner of the child it destroys it.
    void Remove(HmiItem* a_child);

    HmiItem* GetChild(const std::string& a_name);
    HmiItem* GetChild(unsigned a_position) { return m_Value[a_position]; }

    /// Returns the number of children.
    unsigned Size() { return m_Value.size(); }
    /// Removes all child items; own items are destroyed.
    void Clear();

    HmiItem::Type GetType() const override { return HmiItem::EList; }

private:
    std::vector<HmiItem*> m_Value;
};

#endif // HAND_GRAPH_LIST_H
