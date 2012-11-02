/*
 *  Copyright 2012 Alex Stefaniuc
 *
 *  This file is part of Hand.
 *
 *  Hand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  Hand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with Hand. If not, see <http://www.gnu.org/licenses/>.
 */

#include "view/layer/buttonlayer.h"
#include "view/layer/textlayer.h"
#include "graph/method.h"


using namespace std;


ButtonLayer::ButtonLayer(string name) : Layer(name)
{
    BufferType = COLLECTOR;
    type(BUTTON);

    VisibleItems = 0;
    // BTN_NAME, BTN_DESCRIPTION, BTN_ICON, BTN_CONTROL, BTN_PREVIEW
    VisibleItems |= BTN_DESCRIPTION;
//    VisibleItems |= BTN_CONTROL;
    VisibleItems |= BTN_PREVIEW;
}


void ButtonLayer::SetContent(Vertex* data)
{
    Layer::SetContent(data);
/*
    // Create the sub-elements of the button
    // The preview
    if(Content->is(LIST))
    {
        SetExpandable();
        // Add list as preview // or data->Children[i] ?
        Insert(data, PREVIEW);
    }*/
    // The button name
    Vertex* name = new Vertex(data->name());
    add(name);
    Insert(name, NAME);
    // The button description
    Vertex* test_descr = new Vertex("Description Test");
    Insert(test_descr, DESCRIPTION);
    // The icon
/*    Insert(data->Find(string("Icon")), DRAW_ITEM, ICON);*/
}


void ButtonLayer::SetExpandable()
{
    Method<ButtonLayer>* tmp_cb = new Method<ButtonLayer>("ExpandList", this, &ButtonLayer::ExpandList);
    FunctionBox = tmp_cb;
}


bool ButtonLayer::ExpandList(Vertex* ignore)
{
    if(ParentLayer->Request(GetContent()))
        return true;
    return false;
}


void ButtonLayer::Configure(Vertex* sub_layout)
{
/*    SetMap(VisibleItems);
    // Inherit parent settings
    GetSubRect(sub_layout->Position, sub_layout->SizeAndPosition);
    sub_layout->Alignment = Alignment;
    if(sub_layout->Position & BTN_CONTROL)
    {
        sub_layout->Show_Frame = true;
    }*/
}


void ButtonLayer::PressGui()
{
    // TODO: modify layout (IsPressed = true)
    Layer::PressGui();
}


void ButtonLayer::ReleaseGui()
{
    // TODO: depends on the mode what we do now
    GetContent()->execute(this);
    // TODO: reset layout (IsPressed = false)
    Layer::ReleaseGui();
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


bool ButtonLayerFactory::execute(Vertex* tree)
{
    Vertex* types = tree->Vertex::get(REQUEST);
    types->reset();
    types->get(BUTTON);
    return tree->get(TARGET)->get()->Vertex::set(new ButtonLayer(BUTTON));
}


bool ButtonLayerFactory::IsValidInput(Vertex* input)
{
    Vertex* tgt = input->get(TARGET)->get();
    if(tgt && tgt->is(METHOD) && input->Vertex::get(REQUEST)->get(ANY, BUTTON))
        return true;
    return false;
}
