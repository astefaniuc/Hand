#ifndef HAND_VIEW_LAYOUTS_EXPANDING_H
#define HAND_VIEW_LAYOUTS_EXPANDING_H

#include "view/layout.h"
#include <vector>


namespace Layouts { namespace Expanding {


class Field : public ::Field
{
public:
    Field(const std::string& name) : ::Field(name) {}

    void SetPosition(const RelRect& pos) { m_Position = pos;}
    SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) override;

private:
    RelRect m_Position;
};


class Map : public Layout
{
public:
    SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) override;

    Field* GetField(const std::string& name) const override;
    void SetField(const std::string& name, const RelRect& coordinates);

private:
    std::vector<Field*> m_Fields;
};


class List : public Layouts::List
{
public:
    SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) override;
};


Map* CreateView();

}}
#endif //HAND_VIEW_LAYOUTS_EXPANDING_H
