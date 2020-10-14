#ifndef HAND_VIEW_LAYOUTS_ALIGNED_H
#define HAND_VIEW_LAYOUTS_ALIGNED_H

#include "view/layout.h"


namespace Layouts { namespace Aligned {


class Field : public ::Field
{
public:
    Field(const std::string& name) : ::Field(name) {}

    SAlignment GetAlignment() { return m_Alignment; }
    void SetAlignment(SAlignment alignment) { m_Alignment = alignment;}

    SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) override;

private:
    SAlignment m_Alignment;
};


class Map : public Layout
{
public:
    SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) override;

    Field* GetField(const std::string& name) const override;
    void SetField(const std::string& name, SAlignment alignment);

private:
    std::vector<Field*> m_Fields;
};


Map* CreateView();

}}
#endif //HAND_VIEW_LAYOUTS_ALIGNED_H
