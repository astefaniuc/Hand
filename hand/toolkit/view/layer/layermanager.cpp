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
#include "view/layer/focuslayer.h"
#include "view/layer/buttonlayer.h"
#include "view/layer/textlayer.h"
#include "input/device.h"
#include "input/inputstate.h"
#include "base/handserver.h"
#include "view/theme/theme.h"
#include "base/filesystem.h"
#include "view/datatypes/layout.h"
#include "graph/method.h"


using namespace std;


LayerManager::LayerManager() : ListLayer(LAYERMANAGER)
{
    Vertex::get(LAYERMANAGER)->set(this);
    type(LAYERMANAGER);
    NextRequest = NULL;
    _InputState = NULL;
    MasterView = NULL;
    // TODO: still needed?
    BufferType = COLLECTOR;
}


LayerManager::~LayerManager()
{
    delete(GetDevice());
    // Don't delete screen
    Buffer = NULL;
}


void LayerManager::Init()
{
    // Register default layer types/factories
    FactoryMap* fm = new FactoryMap(LAYER_FACTORIES);
    fm->add(new TextLayerFactory());
    fm->add(new ButtonLayerFactory());
    fm->add(new ListLayerFactory());
    Vertex::add(fm);

    // Load the theme
    LoadTheme(Vertex::get(THEMES)->get(DEFAULT));

    List* pub = new List("System");
    add(pub);
    Layer::SetContent(pub);
    pub->add(new Method<LayerManager>("Exit", this, &LayerManager::Exit));
    // Add the exit function to the tree of available funcs
    // Request command at highest level
    GetCommand(pub->get("Exit"), _InputState->GetNumberOfKeys());
    // Start the focus handling layer
/*    MasterView = CreateLayer(FOCUSLAYER);
    MasterView->Init();
    SubLayers.push_back(MasterView);*/
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
    // HACK: create a Layer without Layout
    Layer* sub_layer = new ButtonLayer("ButtonLayer");
    sub_layer->Layer::SetContent(f);
    return GetCommand(sub_layer, level);
}


bool LayerManager::GetCommand(Layer* l, int level)
{
    if(l)
        return _InputState->GetCommand(l, level);
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


Device* LayerManager::GetDevice()
{
    return _InputState->GetDevice();
}


void LayerManager::SetDevice(Device* device)
{
    device->Vertex::get(LAYERMANAGER)->set(this);
    // Is the Device initialized?
    _InputState = device->GetInputState();
    // Delayed initialization of the LayerManager, needs _InputState ptr
    // TODO: handle switching devices
    Init();
    if(!device->Init())
    {
        // No, show init screen
        Vertex* init_screen = new List("Init screen");
        device->execute(init_screen);
        Request(init_screen);
    }
    else
        // TODO: load controls vertices also with the init screen
        Expand(device->Vertex::get(DEVICE_KEYLIST));
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

    Theme* theme = dynamic_cast<Theme*>(bin->get());

    if(!theme)
        return false;

    Vertex* layout = get(LAYOUT, ANY);
    if(!layout)
    {

        layout = theme->get(LAYOUT)->get(LIST)->get();
        layout->name("MasterLayer");
        set(layout);

    }
    // Sets the theme for all sub-layers
    get(THEME)->set(theme);
    theme->execute(layout);
    return true;
}


bool LayerManager::GetAllThemes(Vertex* themes_dir)
{
    // The path to the theme files
    themes_dir = new File(THEMES_DIRECTORY);
    themes_dir->Vertex::get(REQUEST)->get(FILE_);
    // (Re-)read the list of available themes
    HandServer::GetInstance()->execute(themes_dir);
    List* themes_list = dynamic_cast<List*>(themes_dir->Vertex::get(FILE_, ANY));
    if(!themes_list)
        return false;

    // Set switching theme callback to all found themes
    Vertex* loader = new Method<LayerManager>(THEMES, this, &LayerManager::LoadTheme);
    File* file;
    Vertex* elem;
    uint i = 0;
    while((elem=themes_list->get(++i)) != NULL)
    {
        file = dynamic_cast<File*>(elem);
        if(file)
            file->add(loader);
    }

    return Request(themes_dir);
}


bool LayerManager::Expand(Vertex* to_expand)
{
    MasterView = Insert(to_expand, ELEMENT);
    if(MasterView)
        return true;
    return false;
}


void LayerManager::SetSize(SDL_Rect size)
{
    SizeAndPositionOnBuffer = size;
}


void LayerManager::SetBufferType(buffer_type bt)
{
    BufferType = bt;
}


void LayerManager::SetBuffer(SDL_Surface* buffer)
{
    Buffer = buffer;
}

