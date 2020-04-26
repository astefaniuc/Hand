#ifndef HAND_VIEW_LAYER_FACTORIES_H
#define HAND_VIEW_LAYER_FACTORIES_H

#include "base/factory.h"


class ButtonLayerFactory : public Factory
{
public:
    ButtonLayerFactory() : Factory("ButtonLayerFactory", METHOD, BUTTON) {}

    bool execute(Vertex* input) override;
    bool IsValidInput(Vertex* input) override;
};


class ListLayerFactory : public Factory
{
public:
    ListLayerFactory() : Factory("ListLayerFactory", LIST, LIST) {}

    bool execute(Vertex* input) override;
    bool IsValidInput(Vertex* input) override;
};


class TextLayerFactory : public Factory
{
public:
    TextLayerFactory() : Factory("TextLayerFactory", DATA, TEXT) {}

    bool execute(Vertex* input) override;
    bool IsValidInput(Vertex* input) override;
};

#endif // HAND_VIEW_LAYER_FACTORIES_H
