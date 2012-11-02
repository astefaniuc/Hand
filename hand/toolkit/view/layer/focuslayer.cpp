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

#include "view/layer/focuslayer.h"
#include "view/layer/textlayer.h"
#include "view/layer/layermanager.h"
#include "input/device.h"


using namespace std;


FocusLayer::FocusLayer() : ListLayer("FocusLayer")
{
    type(FOCUS);
//    DrawObject.BufferType = OVERLAY;
    KeyList = NULL;
}


FocusLayer::~FocusLayer()
{
}


void FocusLayer::Init()
{
    name("FocusLayer");

    KeyList = Vertex::get(LAYERMANAGER)->get()->find(DEVICE_KEYLIST);
}


void FocusLayer::Draw(bool forced)
{

    if(Update(forced))
    {
//        Layer* slayer = Content->GetLayer();
//        slayer->Update(forced);
    }

/*    vector<Layer*>::iterator curr;
    vector<Layer*>::iterator end = SubLayers.end();
    for(curr=SubLayers.begin(); curr!=end; curr++)
    {
        slayer = (*curr);
        slayer->Draw(forced);
    }*/
    Updated = false;
}

/*
void FocusLayer::SetContent(Vertex* data)
{
    Vertex* button;
    if(Content != NULL)
    {
        for(uint i=0; i<Content->Children.size(); i++)
        {
            button = Content->Children[i];
            // Release old commands/controls
            Content->Children[i]->ReleaseNode();
            button->GetLayer()->Insert(NULL, CONTROLID);
        }
    }
    Content = data;

    uint j = 0;
    for(uint i=0; i<data->Children.size(); i++)
    {
        button = data->Children[i];
        if((button->type() == METHOD) &&
            (_LayerManager->GetCommand(button, 1))) // Map command to button
        {
            button->GetLayer()->Insert(GetControlSurface(j), CONTROLID);
            j++;
        }
    }
    SDL_Rect list_size = DrawObject.SizeAndPositionOnBuffer;
    Rect rel_size(0.1, 0.1, 0.8, 0.8);
    rel_size.MultiplyTo(list_size);
    Layer* list = Content->GetLayer();
    if(list)
    {
        list->SetSize(list_size);
    }
}
*/


void FocusLayer::Configure(Vertex* sub_layout)
{
    // Never called
}

VirtualSurface* FocusLayer::GetControlSurface(uint index)
{
//    return (VirtualSurface*)(KeyList->Children[index]->GetLayer());
    return NULL;
}
