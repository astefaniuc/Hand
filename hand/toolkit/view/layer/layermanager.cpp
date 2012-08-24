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


using namespace std;


LayerManager::LayerManager() : ListLayer()
{
    SetLayerManager(this);
    SetType(LAYERMANAGER);
    _Theme = NULL;
    NextRequest = NULL;
    _InputState = NULL;
    MasterView = NULL;
    // TODO: still needed?
    BufferType = COLLECTOR;
    Layer::SetContent(this);

    // Register default layer types/factories. TODO: dynamic loading
    LayerTopos = new FactoryMap("LayerTopos");
    // The last added factory has the highest relevance (HACK?)
    RegisterLayerFactory(new TextLayerFactory());
    RegisterLayerFactory(new ButtonLayerFactory());
    RegisterLayerFactory(new ListLayerFactory());
}


LayerManager::~LayerManager()
{
    delete(LayerTopos);
    // TODO: unregister (DataManager + Server)
    // UnloadTheme();
}


void LayerManager::Init()
{
    SetName("System");

    // Theme menu
    List* theme_menu = new List(THEMES_MENU);
    Add(theme_menu);
    // The path to the theme files
    List* theme_dir = new FileVertex(THEMES_DIRECTORY);
    theme_menu->Add(theme_dir);
    // Add a method to create a menu with the themes one HD
    theme_menu->Add(new Method<LayerManager>(THEMES_MENU, this, &LayerManager::GetAllThemes));
    // The default theme (to be loaded) (TODO: get this from persistent object)
    List* curr_theme = new FileVertex(DEFAULT_THEME);
    // Add the default theme to the theme folder (without scanning if it really is there)
    theme_dir->Add(curr_theme);
    curr_theme->Get(RELATION_PARENT_PATH)->Set(theme_dir);
    // Load the theme
    LoadTheme(curr_theme);


    // Add the exit function to the tree of available funcs
    Add(new Method<LayerManager>("Exit", this, &LayerManager::Exit));

    // Request command at highest level
    GetCommand(Get("Exit"), _InputState->GetNumberOfKeys());
    // Start the focus handling layer
/*    MasterView = CreateLayer(FOCUS_ITEM_HACK);
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


bool LayerManager::UnloadAppInterface(Vertex* functoid)
{
    delete(functoid);
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
    List* menu_list = dynamic_cast<List*>(menu);
    if(!menu_list)
        return NULL;
    Vertex* child;
    for(uint i=0; i<menu_list->size(); i++)
    {
        child = menu_list->at(i);
        if(child->IsType(BUTTON_ITEM))
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
    Layer* l = CreateLayer(f, BUTTONLAYER);
    return GetCommand(l, level);
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

    Server()->Exit(this);
    return false;
}


Device* LayerManager::GetDevice()
{
    return _InputState->GetDevice();
}


void LayerManager::SetDevice(Device* device)
{
    device->SetLayerManager(this);
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
/* This would be the proper solution but gives link error:
 * undefined reference to `typeinfo for Theme'
 *
    Theme* theme = dynamic_cast<Theme*>(Server()->Produce(f, HANDAPP));
*/
    Theme* theme = (Theme*)(Server()->Produce(f, HANDAPP));

    if(!theme)
        return false;
    _Theme = theme;
    SetTheme(theme);
    return true;
}


bool LayerManager::GetAllThemes(Vertex* themes_dir)
{
    // (Re-)read the list of available themes
    List* themes_list = dynamic_cast<List*>(Server()->Produce(themes_dir, FILEFUNCTOID));
    if(!themes_list)
        return false;

    // Set switching theme callback to all found themes
    Vertex* loader = new Method<LayerManager>(THEMES_MENU, this, &LayerManager::LoadTheme);
    FileVertex* file;
    for (uint i=0; i<themes_list->size(); i++)
    {
        file = dynamic_cast<FileVertex*>(themes_list->at(i));
        if(file)
            file->Add(loader);
    }

    return Request(themes_dir);
}


bool LayerManager::Expand(Vertex* to_expand)
{
    /*// Does it have a Layer attached?
    Layer* new_view = GetAttachedLayer(to_expand);
    if(!new_view)
    {
        string type = GetContentType(to_expand);
        if(type == "")
            return false;
        if(MasterView && MasterView->IsType(type))
        {
            // In this case we re-use the layer
            MasterView->SetContent(to_expand);
            return true;
        }
        new_view = CreateLayer(to_expand, type);
        if(!new_view)
            return false;
    }
    new_view->SetSize(GetSize());

    if(MasterView)
    {
        if(MasterView == new_view)
            // Ever possible?
            return false;

        if(MasterView->IsType("Expansive"))
        {
            // Insert() = Add() != SetContent()
            MasterView->Set(CHILDREN)->Add(new_view);
            return true;
        }

        Delete(MasterView);
    }
    MasterView = new_view;
    MasterView->SetParent(this);

    // TODO: ?
    MasterView->IsExpanded = true;
    Updated = true;

    return true;*/
    MasterView = Insert(to_expand, "MasterLayer");
    if(MasterView)
        return true;
    return false;
}


void LayerManager::RegisterLayerFactory(Factory* target)
{
    // TODO: complex topographies should be tried first
    if(target)
        LayerTopos->Add(target);
}


string LayerManager::GetContentType(Vertex* target)
{
    Factory* f = LayerTopos->GetFactory(target);
    if(f)
        return f->GetOutputType();
    return "";
}


Layer* LayerManager::CreateLayer(Vertex* content, string layer_type)
{
    if(layer_type.empty())
        return NULL;

    if(layer_type == "Any")
    {
        Vertex* attached_layout = content->Get(RELATION, "Layout");
        if(attached_layout)
            layer_type = attached_layout->Get(1)->GetType();
        else
            layer_type = GetContentType(content);
    }

    Layer* layer = dynamic_cast<Layer*>(LayerTopos->Produce(content, layer_type));
    if(!layer)
        return NULL;
    layer->SetLayerManager(this);
    layer->SetLayout(_Theme);
    layer->SetContent(content);

    return layer;
}
