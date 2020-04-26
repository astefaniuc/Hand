#include "view/layer/textlayer.h"


TextLayer::TextLayer(const std::string& name) : Layer(name)
{
    BufferType = OVERLAY;
    type(TEXT);
}
