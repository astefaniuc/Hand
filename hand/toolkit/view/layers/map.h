#ifndef HAND_VIEW_LAYER_MAP_H
#define HAND_VIEW_LAYER_MAP_H

#include "view/layers/list.h"
#include "view/layers/text.h"
#include <map>


namespace Layers {


class Map : public List
{
public:
    ~Map();

    unsigned GetChildCount() const override { return m_Sublayers.size(); }
    Layer* GetFirstChild() override;
    Layer* GetNextChild() override;

    /// Returns 'sub'.
    void Insert(const std::string& field, Layer* sub);
    void Remove(Layer* sub) override;

    Layer* GetChild(const std::string& name) const override;

protected:
    void Rebuild() override;
    void SetControl(Layer* ctrl) override { Insert(CONTROL, ctrl); }
    void RemoveControl() override;
    void ClearContainer() override { m_Sublayers.clear(); }

    std::map<std::string, Layer*> m_Sublayers;
    std::map<std::string, Layer*>::const_iterator m_CurrentChild = m_Sublayers.cbegin();

    Text m_Name;
    Text m_Info;
};

}
#endif // HAND_VIEW_LAYER_MAP_H
