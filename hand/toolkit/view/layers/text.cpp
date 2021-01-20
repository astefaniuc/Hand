#include "view/layers/text.h"


namespace Layers {


void Text::SetData(const std::string& text)
{
    if (m_Text != text)
    {
        m_Text = text;
        SetModified();
    }
}

}
