#ifndef HAND_THEMES_DEFAULT_H
#define HAND_THEMES_DEFAULT_H

#include "include/theme.h"
#include "graph/collection.h"

class Default : public Theme
{
public:
    Default();
    HmiItem* GetHmi() override { return &m_Hmi; }

protected:
    // Layout creators
    bool GetViewLayout(Layout* a_out);
    bool GetFramedListLayout(Layout* a_out);
    bool GetListLayout(Layout* a_out);
    bool GetButtonLayout(Layout* a_out);
    bool GetContainerLayout(Layout* a_out);
    bool GetFrameLayout(Layout* a_out);
    bool GetBackgroundLayout(Layout* a_out);
    bool GetTextLayout(Layout* a_out);

    // Drawing methods
    bool ColorSurface(HmiItem* drawing);
    bool DrawList(HmiItem* drawing);
    bool DrawText(HmiItem* drawing);

    bool DrawView(HmiItem* drawing);

    std::string GetString(HmiItem* vs);

    Collection m_Hmi;
};

#endif // HAND_THEMES_DEFAULT_H
