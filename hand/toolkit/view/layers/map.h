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

    Text m_Name;
    Text m_Info;
};

}
#endif // HAND_VIEW_LAYER_MAP_H
