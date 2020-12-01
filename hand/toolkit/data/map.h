#ifndef HAND_DATA_MAP_H
#define HAND_DATA_MAP_H

#include "data/list.h"
#include "include/stdfields.h"
#include <map>


class Layer;


namespace Hmi {


class Map : public List
{
public:
    Map(
        const std::string& name,
        const std::string& description,
        Module* manipulator = nullptr)
        : List(name, description, manipulator) {}
    ~Map() { Clear(); }

    /// Insert a child item and assume ownership of it.
    void Add(const std::string& key, Item* child);
    void Add(Hmi::Item* child) { Add(child->GetName(), child); }
    /// Insert a child item without assuming ownership of it.
    void Attach(const std::string& key, Item* child) { m_Value[key] = child; }
    void Attach(Item* child) { m_Value[child->GetName()] = child; }
    void Remove(Item* child) override;

    Item* GetChild(const std::string& name) const override;
    Item* GetChild(unsigned position) override;

    /// Returns the number of children.
    unsigned Size() const override { return m_Value.size(); }
    /// Removes all child items; own items are destroyed.
    void Clear() override;

    std::string GetValueString() override { return "TODO"; }

private:
    std::map<std::string, Item*> m_Value;
};

}
#endif // HAND_DATA_MAP_H
