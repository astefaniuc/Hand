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

#include "buttonlayer.h"
#include "textlayer.h"


using namespace std;


ButtonLayer::ButtonLayer() : Layer()
{
    BufferType = COLLECTOR;
    SetType(TYPE_BUTTONLAYER);

    VisibleItems = 0;
    // BTN_NAME, BTN_DESCRIPTION, BTN_ICON, BTN_CONTROL, BTN_PREVIEW
    VisibleItems |= BTN_DESCRIPTION;
//    VisibleItems |= BTN_CONTROL;
    VisibleItems |= BTN_PREVIEW;
}


ButtonLayer::~ButtonLayer()
{
}


void ButtonLayer::SetContent(Functoid* data)
{
    Layer::SetContent(data);
/*
    // Create the sub-elements of the button
    // The preview
    if(Content->IsType(TYPE_FUNCTOIDLIST))
    {
        SetExpandable();
        // Add list as preview // or data->Children[i] ?
        Insert(data, BTN_FIELD_PREVIEW);
    }*/
    // The button name
    Insert(data, BTN_FIELD_NAME);
    // The button description
    Insert(data, BTN_FIELD_DESCRIPTION);
    // The icon
/*    Insert(data->Find(string("Icon")), DRAW_ITEM, BTN_FIELD_ICON);*/
}


void ButtonLayer::SetExpandable()
{
    Callback<ButtonLayer>* tmp_cb = new Callback<ButtonLayer>("ExpandList", this, &ButtonLayer::ExpandList);
    FunctionBox = tmp_cb;
}


bool ButtonLayer::ExpandList(Functoid* ignore)
{
    if(ParentLayer->Request(GetContent()))
    {
        return true;
    }
    return false;
}


void ButtonLayer::Configure(Functoid* sub_layout)
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
    GetContent()->Execute(this);
    // TODO: reset layout (IsPressed = false)
    Layer::ReleaseGui();
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


bool ButtonLayerFactory::IsValidInput(Functoid* input)
{
//    if(input->IsType(BUTTON_ITEM) || input->IsType(TYPE_DESCRIPTOR))
    if(input)
        return true;
    return false;
}


Functoid* ButtonLayerFactory::Produce(Functoid* descriptor)
{
    Layer* ret = new ButtonLayer();
    return ret;
}


void ButtonLayerFactory::TakeBack(Functoid* product)
{
    // TODO: shouldn't delete objects of derived classes
    if(dynamic_cast<ButtonLayer*>(product))
        delete(product);
}
