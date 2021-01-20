#ifndef HAND_VIEW_LAYER_MAP_H
#define HAND_VIEW_LAYER_MAP_H

#include "view/layers/list.h"
#include "view/layers/text.h"
#include <map>


namespace Layers {


class Map : public List
{
public:
    ~Map() { Exit(); }

    /// Returns 'sub'.
    void Insert(const std::string& field, Layer* sub);

protected:
    void Rebuild() override;
    void SetControl(Layer* ctrl) override { m_Control = ctrl; SetModifiedContent(); }
    void RemoveControl() override;

    Text m_Name;
    Text m_Info;
    Layer* m_Control = nullptr;
};

}
#endif // HAND_VIEW_LAYER_MAP_H
