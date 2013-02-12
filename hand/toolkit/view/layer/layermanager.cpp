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


LayerManager::LayerManager() : Layer(LAYERMANAGER)
{
    Vertex::get(LAYERMANAGER)->set(this);
    type(LAYERMANAGER);

    NextRequest = NULL;
    _Device = NULL;
    MainView = NULL;
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

    Vertex* pub = get(SYSTEM);
    Layer::SetContent(pub);
    pub->add(new Method<LayerManager>("Exit", this, &LayerManager::Exit));
    // Add the exit function to the tree of available funcs
    // Request command at highest level
    GetCommand(pub->get("Exit"), _Device->GetNumberOfKeys());
    Insert(pub, SYSTEM);
}


bool LayerManager::Update(bool forced)
{
    if(NextRequest)
    {
        if(MainView)
        {
            delete(MainView);
            MainView = NULL;
        }
        if(Expand(NextRequest))
            forced = true;
    }
    NextRequest = NULL;
    return Layer::Update(forced);
}


bool LayerManager::Expand(Vertex* to_expand)
{
    MainView = Insert(to_expand, VIEW);
    if(!MainView)
        return false;
    MainView->get(EXECUTE)->get()->execute(get(COMMANDS));
    return true;
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
    if(MainView && (MainView->GetContent()!=content))
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
        layout = theme->get(LAYOUT)->get();
        layout->get(COORDINATES)->Vertex::get(REQUEST)->get(RECT)->get(FULL);
        layout->get(ALIGNMENT)->Vertex::get(REQUEST)->get(ALIGNMENT)->get(SCALEDHORIZONTAL);
        layout->get(DRAWER)->Vertex::get(REQUEST)->get(DRAWER)->get(LIST);

        layout->get(CHILDREN)->get(VIEW)->Vertex::get(REQUEST)->get(LAYOUT)->get(ANY);
        layout->get(CHILDREN)->get(SYSTEM)->Vertex::get(REQUEST)->get(LAYOUT)->get(ANY);
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
