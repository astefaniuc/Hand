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

#include "view/layer/layermanager.h"
#include "input/device.h"
#include "input/inputstate.h"
#include "view/datatypes/layout.h"
#include "graph/method.h"


using namespace std;


LayerManager::LayerManager() : ListLayer(LAYERMANAGER)
{
    Vertex::get(LAYERMANAGER)->set(this);
    type(LAYERMANAGER);
    NextRequest = NULL;
    _Device = NULL;
    MasterView = NULL;
    // TODO: still needed?
    BufferType = COLLECTOR;
}


LayerManager::~LayerManager()
{
    delete(_Device);
    // Don't delete screen
    Buffer = NULL;
}


void LayerManager::Init()
{
    get(THEME)->add(new Method<LayerManager>(LOADER, this, &LayerManager::LoadTheme));
    // Load the theme
    LoadTheme(Vertex::get(FACTORY, THEMES)->get(DEFAULT));

    List* pub = new List("System");
    add(pub);
    Layer::SetContent(pub);
    pub->add(new Method<LayerManager>("Exit", this, &LayerManager::Exit));
    // Add the exit function to the tree of available funcs
    // Request command at highest level
    GetCommand(pub->get("Exit"), _Device->GetNumberOfKeys());
}


bool LayerManager::Update(bool forced)
{
    if(NextRequest)
    {
        if(MasterView)
        {
            delete(MasterView);
            MasterView = NULL;
        }
        if(Expand(NextRequest))
            forced = true;
    }
    NextRequest = NULL;
    return Layer::Update(forced);
}


Vertex* LayerManager::GetCommandList(Vertex* menu)
{
    Vertex* child;
    uint i = 0;
    while((child=menu->get(++i)) != NULL)
    {
        if(child->is(METHOD))
            return menu;

        else if((child=GetCommandList(child)) != NULL)
            return child;
    }
    return NULL;
}


bool LayerManager::GetCommand(Vertex* f, int level)
{
    if(f)
        return _Device->GetInputState()->GetCommand(f, level);
    return false;
}


bool LayerManager::Exit(Vertex* content)
{
    content = GetContent();
    // Check if the default location is currently active
    if(MasterView && (MasterView->GetContent()!=content))
        return Request(content);

    // Suicide
    delete(this);
    return false;
}


void LayerManager::SetDevice(Device* device)
{
    _Device = device;
    device->Vertex::get(LAYERMANAGER)->set(this);
    if(!device->Init())
        // No, show init screen
        Request(device->get(VIEW));
    else
        // TODO: load controls vertices also with the init screen
        Expand(device->Vertex::get(KEYLIST));
    // Delayed initialization of the LayerManager, needs device ptr
    // TODO: handle switching devices
    Init();
}


bool LayerManager::Request(Vertex* request)
{
    NextRequest = request;

    // Removes animations
//    DrawObject->Animations.reset();
    return true;
}


bool LayerManager::LoadTheme(Vertex* f)
{
    Vertex* bin = f->Vertex::get(APPLOADER, ANY);
    if(!bin)
        return false;

    Vertex* theme = bin->get();
    if(!theme)
        return false;

    Vertex* layout = get(LAYOUT, ANY);
    if(!layout)
    {

        layout = theme->get(LAYOUT)->get(LIST)->get();
        layout->name("MasterLayer");
        set(layout);

    }
    // (Re-)sets the theme for all sub-layers
    Vertex* prev = get(THEME)->get(THEME, ANY);
    if(prev)
        delete(prev);
    get(THEME)->set(theme);
    theme->execute(layout);
    return true;
}


bool LayerManager::GetAllThemes(Vertex* themes_list)
{
    themes_list = Vertex::get(FACTORY, THEMES);

    // Set switching theme callback to all found themes
    Vertex* loader = get(THEME)->get(METHOD, LOADER);
    Vertex* theme;
    uint i = 0;
    while((theme=themes_list->get(++i)) != NULL)
        theme->add(loader);

    return Request(themes_list);
}


bool LayerManager::Expand(Vertex* to_expand)
{
    MasterView = Insert(to_expand, ELEMENT);
    if(!MasterView)
        return false;
    MasterView->SetCommand(get(COMMANDS));
    return true;
}


void LayerManager::SetSize(SDL_Rect size)
{
    CoordinatesOnBuffer = size;
}


void LayerManager::SetBufferType(buffer_type bt)
{
    BufferType = bt;
}


void LayerManager::SetBuffer(SDL_Surface* buffer)
{
    Buffer = buffer;
}

