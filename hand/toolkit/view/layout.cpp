#include "layout.h"
#include <assert.h>


Layout::Layout() : m_Settings("Layout", "Settings")
{
    m_Settings.Add(m_ShowFrame);
    m_Settings.Add(m_ShowName);
    m_Settings.Add(m_ShowDescription);

    m_Settings.Add(m_Fields);
    SetField(TITLE, { 0.0, 0.0, 1.0, 0.4 });
    SetField(DESCRIPTION, { 0.0, 0.4, 1.0, 0.6 });
}


Rel_Rect Layout::GetField(const std::string& a_name) const
{
    Rect* field = dynamic_cast<Rect*>(m_Fields->GetChild(a_name));
    if (field)
        return field->GetValue();
    return Rel_Rect();
}


void Layout::SetField(const std::string& a_name, const Rel_Rect& a_coordinates)
{
    Rect* field = dynamic_cast<Rect*>(m_Fields->GetChild(a_name));
    if (field)
        field->SetValue(a_coordinates);
    else
        m_Fields->Add(new Rect(a_name, "",
            a_coordinates.x, a_coordinates.y, a_coordinates.w, a_coordinates.h));
}


Rel_Rect ListLayout::GetField(unsigned a_field, unsigned a_numFields) const
{
    assert(a_field < a_numFields);

    const Rel_Rect& align = GetAlignment();
    Rel_Rect ret;
    ret.x = a_field * align.x / a_numFields;
    ret.y = a_field * align.y / a_numFields;
    ret.w = align.w / a_numFields;
    ret.h = align.h / a_numFields;

    return ret;
}


ViewLayout::ViewLayout()
{
    SetField(DESCRIPTION, { 0.8, 0.3, 0.2, 0.4 });
    SetField(TITLE, { 0.0, 0.0, 1.0, 0.04 });

    SetField(CONTROL, { 0.2, 0.8, 0.6, 0.2 });
    SetField(VIEW, { 0.0, 0.04, 0.8, 0.76 });
}
