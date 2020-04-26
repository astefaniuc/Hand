#include "view/layer/factories.h"
#include "view/layer/buttonlayer.h"
#include "view/layer/listlayer.h"
#include "view/layer/textlayer.h"


bool ButtonLayerFactory::execute(Vertex* input)
{
    return input->Vertex::set(new ButtonLayer(BUTTON));
}


bool ButtonLayerFactory::IsValidInput(Vertex* input)
{
    return (input && input->is(METHOD));
}

// ----------------------------------------------------------------

bool ListLayerFactory::execute(Vertex* input)
{
    return input->Vertex::set(new ListLayer(LIST));
}


bool ListLayerFactory::IsValidInput(Vertex* input)
{
    return (input && input->is(LIST));
}

// ----------------------------------------------------------------

bool TextLayerFactory::execute(Vertex* input)
{
    return input->Vertex::set(new TextLayer(TEXT));
}


bool TextLayerFactory::IsValidInput(Vertex* input)
{
    return (input && input->is(NOTE));
}
