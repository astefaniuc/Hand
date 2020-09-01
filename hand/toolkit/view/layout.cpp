#include "layout.h"
#include <assert.h>


Layout::Layout()
    : m_Settings("Layout", "Settings")
{
    m_Settings.Add(m_ShowFrame);
    m_Settings.Add(m_ShowName);
    m_Settings.Add(m_ShowDescription);
}


Rel_Rect ListLayout::GetSizeAndPosition(unsigned a_field, unsigned a_numFields) const
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
    m_Fields->Add(m_Control);
    m_Fields->Add(m_Description);
    m_Fields->Add(m_Title);
    m_Fields->Add(m_View);
    m_Settings.Add(m_Fields);
}


Rel_Rect ViewLayout::GetSizeAndPosition(const std::string& a_field) const
{
    Rect* field = dynamic_cast<Rect*>(m_Fields->GetChild(a_field));
    if (field)
        return field->GetValue();
    return Rel_Rect();
}
