#include "view/layer/buttonlayer.h"
#include "view/layer/textlayer.h"
#include "graph/method.h"
#include "graph/data.h"


ButtonLayer::ButtonLayer() : Layer()
{
    BufferType = COLLECTOR;
    // TODO: make this configurable
    m_Name = new TextLayer();
    m_Description = new TextLayer();
    m_Command = new TextLayer();

}


void ButtonLayer::SetContent(HmiItem* a_data)
{
    Layer::SetContent(a_data);

    m_Name->SetData(a_data->GetName());
    m_Description->SetData(a_data->GetDescription());
    // TODO: m_Command
}
