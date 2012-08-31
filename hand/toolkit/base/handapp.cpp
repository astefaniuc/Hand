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

#include "base/handapp.h"
#include "base/handapploader.h"
#include "base/handserver.h"
#include "base/filesystem.h"

#include <dlfcn.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


using namespace std;


HandApp::HandApp(string name) : List(name)
{
    _LayerManager = NULL;
    Type(HANDAPP);
}


LayerManager* HandApp::GetLayerManager()
{
    if(_LayerManager == NULL)
        _LayerManager = Server()->GetLayerManager();
    return _LayerManager;
}


void HandApp::SetLayerManager(LayerManager* lm)
{
    _LayerManager = lm;
}


HandServer* HandApp::Server()
{
    return HandServer::GetInstance();
}


HandAppLoader::HandAppLoader(FileVertex* path_obj) : Factory(NAME_NOT_INIT,
                                                             FILEVERTEX,
                                                             HANDAPP, "")
{
    Type(APPLOADER);
    if(!path_obj)
        return; // false
    LibraryPath = path_obj->GetFullPath();
    LoadedLib = dlopen(LibraryPath.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if(LoadedLib)
    {
        Create = (creator*) dlsym(LoadedLib, "Create");
        Destroy = (destroyer*) dlsym(LoadedLib, "Destroy");
        if(Create && Destroy)
        {
            Name(path_obj->Name());
            return; // true
        }
    }

    cout << dlerror() << endl;
    LibraryPath = ""; // "return false"
}


HandAppLoader::~HandAppLoader()
{
    if(LoadedLib != NULL)
        dlclose(LoadedLib);
}


bool HandAppLoader::IsValidInput(Vertex* entry)
{
    FileVertex* path_obj = dynamic_cast<FileVertex*>(entry);
    if(path_obj && (path_obj->GetFullPath()==LibraryPath))
        return true;
    return false;
}


Vertex* HandAppLoader::Produce(Vertex* ignore)
{
//    path_obj->Add(Create());
    return Create();
}


void HandAppLoader::TakeBack(Vertex* app)
{
    HandApp* casted = dynamic_cast<HandApp*>(app);
    if(Destroy && casted)
        Destroy(casted);
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


HandAppLoader_Factory::HandAppLoader_Factory() : Factory("Library Loader",
                                                         FILEVERTEX,
                                                         APPLOADER,
                                                         URI_FILE)
{
    Type(APPLOADERFACTORY);
}



bool HandAppLoader_Factory::IsValidInput(Vertex* entry)
{
    FileVertex* ff = dynamic_cast<FileVertex*>(entry);
    if(!ff)
        return false;

    boost::filesystem::path _path = ff->GetPath();
    if(is_regular_file(_path) && (_path.extension()==LIBRARY_FILE_EXTENSION))
        return true;
    return false;
}


Vertex* HandAppLoader_Factory::Produce(Vertex* input)
{
    FileVertex* path_obj = dynamic_cast<FileVertex*>(input);
    if(!path_obj)
        return NULL;
    HandAppLoader* ret = new HandAppLoader(path_obj);
    // Check if the library could be opened
    if(ret->Name() == input->Name())
        return ret;

    delete(ret);
    return NULL;
}


void HandAppLoader_Factory::TakeBack(Vertex* product)
{
    // TODO: shouldn't delete objects of derived classes
    if(dynamic_cast<HandAppLoader*>(product))
        delete(product);
}
