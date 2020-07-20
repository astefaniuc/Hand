#ifndef HAND_VIEW_DRAWER_H
#define HAND_VIEW_DRAWER_H


class Layer;

class Drawer
{
public:
    Drawer(Layer* layer) : m_Layer(layer) {}
    virtual ~Drawer() {}

    virtual void Draw(bool forced) = 0;

protected:
    Layer* m_Layer = nullptr;
};

#endif // HAND_VIEW_DRAWER_H
