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
    Vertex::Get(LAYERMANAGER)->Set(this);
    Type(LAYERMANAGER);
    NextRequest = NULL;
    _InputState = NULL;
    MasterView = NULL;
    // TODO: still needed?
    BufferType = COLLECTOR;
}


LayerManager::~LayerManager()
{
    delete(GetDevice());
    // TODO: unregister (DataManager + Server)
    // UnloadTheme();
}


void LayerManager::Init()
{
    // Register default layer types/factories
    FactoryMap* fm = new FactoryMap(LAYER_FACTORIES);
    fm->Add(new TextLayerFactory());
    fm->Add(new ButtonLayerFactory());
    fm->Add(new ListLayerFactory());

    Vertex* layout = new Layout("MasterLayer", LIST);
    Set(layout);
    layout->Add(fm);

    // Theme menu
    List* theme_menu = new List(THEMES_MENU);
    // The path to the theme files
    List* theme_dir = new File(THEMES_DIRECTORY);
    theme_menu->Add(theme_dir);
    // Add a method to create a menu with the themes one HD
    theme_menu->Add(new Method<LayerManager>(THEMES_MENU, this, &LayerManager::GetAllThemes));
    // The default theme (to be loaded) (TODO: get this from persistent object)
    List* curr_theme = new File(DEFAULT_THEME);
    // Add the default theme to the theme folder (without scanning if it really is there)
    theme_dir->Add(curr_theme);
    curr_theme->Get(PATH)->Set(theme_dir);
    // Load the theme
    LoadTheme(curr_theme);

    List* pub = new List("System");
    Add(pub);
    Layer::SetContent(pub);
    pub->Add(theme_menu);
    pub->Add(new Method<LayerManager>("Exit", this, &LayerManager::Exit));
    // Add the exit function to the tree of available funcs
    // Request command at highest level
    GetCommand(pub->Get("Exit"), _InputState->GetNumberOfKeys());
    // Start the focus handling layer
/*    MasterView = CreateLayer(FOCUSLAYER);
    MasterView->Init();
    SubLayers.push_back(MasterView);*/
}


void LayerManager::LoadAppInterface(Vertex* tree, bool make_default)
{
    if(tree == NULL)
        return;

    // Insert to the local Vertex tree
    Add(tree); // or Attach()?
    if(make_default)
        Layer::SetContent(tree);
}


bool LayerManager::UnloadAppInterface(Vertex* vertex)
{
    delete(vertex);
    return true;
}


bool LayerManager::Update(bool forced)
{
    if(NextRequest && Expand(NextRequest))
        forced = true;
    NextRequest = NULL;
    return Layer::Update(forced);
}


Vertex* LayerManager::GetCommandList(Vertex* menu)
{
    Vertex* child;
    uint i = 0;
    while((child=menu->Get(++i)) != NULL)
    {
        if(child->Is(METHOD))
            return menu;

        else if((child=GetCommandList(child)) != NULL)
            return child;
    }
    return NULL;
}


void LayerManager::SetScreen(SDL_Surface* screen)
{
    Screen = screen;
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
    device->Vertex::Get(LAYERMANAGER)->Set(this);
    // Is the Device initialized?
    _InputState = device->GetInputState();
    // Delayed initialization of the LayerManager, needs _InputState ptr
    // TODO: handle switching devices
    Init();
    if(!device->Init())
    {
        // No, show init screen
        LoadAppInterface(device);
        Request(device);
    }
    else
        Expand(device->Find(DEVICE_KEYLIST));
}


bool LayerManager::Request(Vertex* request)
{
    NextRequest = request;

    // Removes animations
//    DrawObject->Animations.Reset();
    return true;
}


bool LayerManager::LoadTheme(Vertex* f)
{
    f->Vertex::Get(REQUEST)->Get(HANDAPP);
    if(!HandServer::GetInstance()->Execute(f))
        return false;
    Theme* theme = dynamic_cast<Theme*>(f->Vertex::Get(HANDAPP, ANY));

    if(!theme)
        return false;

    Vertex* layout = Get(LAYOUT, ANY);
    // Sets the theme for all sublayouts
    layout->Get("Theme")->Set(theme);
    theme->Execute(layout);
    return true;
}


bool LayerManager::GetAllThemes(Vertex* themes_dir)
{
    themes_dir->Vertex::Get(REQUEST)->Get(FILE);
    // (Re-)read the list of available themes
    HandServer::GetInstance()->Execute(themes_dir);
    List* themes_list = dynamic_cast<List*>(themes_dir->Vertex::Get(FILE, ANY));
    if(!themes_list)
        return false;

    // Set switching theme callback to all found themes
    Vertex* loader = new Method<LayerManager>(THEMES_MENU, this, &LayerManager::LoadTheme);
    File* file;
    Vertex* elem;
    uint i = 0;
    while((elem=themes_list->Get(++i)) != NULL)
    {
        file = dynamic_cast<File*>(elem);
        if(file)
            file->Add(loader);
    }

    return Request(themes_dir);
}


bool LayerManager::Expand(Vertex* to_expand)
{
    MasterView = Insert(to_expand, "ListElement");
    if(MasterView)
        return true;
    return false;
}
