#ifndef HAND_VIEW_DRAWER_H
#define HAND_VIEW_DRAWER_H


class Layer;
class Theme;

class Drawer
{
public:
    virtual ~Drawer() {}

    virtual void Draw(bool forced) = 0;
    virtual Theme* GetParentTheme() = 0;
    virtual void SetLayer(Layer* layer) { m_Layer = layer; }

protected:
    Layer* m_Layer = nullptr;
};

#endif // HAND_VIEW_DRAWER_H
