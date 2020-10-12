#ifndef HAND_VIEW_LAYER_MAP_H
#define HAND_VIEW_LAYER_MAP_H

#include "view/layer.h"
#include <map>


namespace Layers {


class Map : public Layer
{
public:
    ~Map();

    Layer* GetFirstChild() override;
    Layer* GetNextChild() override;

    /// Returns 'sub'.
    Layer* Insert(const std::string& field, Layer* sub);
    void Remove(Layer* sub) override;

    Layer* GetField(const std::string& name);

protected:
    std::map<std::string, Layer*> m_Sublayers;
    std::map<std::string, Layer*>::const_iterator m_CurrentChild = m_Sublayers.cbegin();
};

}
#endif // HAND_VIEW_LAYER_MAP_H
