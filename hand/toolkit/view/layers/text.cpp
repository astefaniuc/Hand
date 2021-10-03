#include "view/layers/text.h"
#include "data/data.h"


namespace Layers {


void RawText::SetData(const std::string& text)
{
    if (m_Text != text)
    {
        m_Text = text;
        SetModified();
    }
}


std::string DataText::GetData()
{
    return static_cast<Data::Base*>(m_Data)->GetValueString();
}

}
