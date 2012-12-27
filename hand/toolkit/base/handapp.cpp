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

#include <dlfcn.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "base/handapp.h"
#include "base/handapploader.h"
#include "base/filesystem.h"
#include "base/handserver.h"


using namespace std;


HandApp::HandApp(string name) : List(name)
{
    type(HANDAPP);
}


Binary::Binary() : Vertex(NAME_NOT_INIT)
{
    type(APPLOADER);
    Library = NULL;
    Create = NULL;
    Destroy = NULL;
}


Binary::~Binary()
{
    reset();
}


bool Binary::execute(Vertex* input)
{
    reset();
    File* path_obj = dynamic_cast<File*>(input);
    if(!path_obj)
        return false;

    Library = dlopen(path_obj->GetFullPath().c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if(Library)
    {
        Create = (creator*) dlsym(Library, "Create");
        Destroy = (destroyer*) dlsym(Library, "Destroy");
        if(Create && Destroy)
            return true;
    }

    cout << dlerror() << endl;
    return false;
}


Vertex* Binary::_get()
{
    return Create();
}


void Binary::reset()
{
    if(Library)
    {
        HandApp* casted = dynamic_cast<HandApp*>(get(ANY, HANDAPP));
        if(Destroy && casted)
            Destroy(casted);
        dlclose(Library);
    }
    Library = NULL;
    Create = NULL;
    Destroy = NULL;
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


HandAppLoader::HandAppLoader() : Factory("HandApp Loader", FILE_, HANDAPP)
{
    type(APPLOADERFACTORY);
}


bool HandAppLoader::IsValidInput(Vertex* entry)
{
    File* ff = dynamic_cast<File*>(entry);
    if(!ff)
        return false;

    boost::filesystem::path _path = ff->GetPath();
    if(is_regular_file(_path) && (_path.extension()==LIBRARY_FILE_EXTENSION))
        return true;
    return false;
}


bool HandAppLoader::execute(Vertex* input)
{
    Binary* bin = new Binary();
    // Open the library
    if(!bin->execute(input))
    {
        delete(bin);
        return false;
    }
    Vertex* prod = bin->get();
    input->Vertex::add(prod);
    prod->Vertex::add(bin);
    input->Vertex::get(REQUEST)->set(get(OUTPUTTYPE)->get());
    return true;
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


BinaryManager::BinaryManager(string n, string d) : List(n)
{
    type(FACTORY);
    // The path to the theme files
    Vertex* dir = new File(d);
    // Add hidden
    Vertex::get("Directories")->add(dir);
    // (Re-)read the list of available themes
    dir->Vertex::get(REQUEST)->get(FILE_);
    HandServer::GetInstance()->execute(dir);
    // Load all themes once
    uint i = 0;
    Vertex* file;
    while((file=dir->get(++i)) != NULL)
    {
        file->Vertex::get(REQUEST)->get(HANDAPP);
        HandServer::GetInstance()->execute(file);
        // Add the theme under its internal name
        attach(file->Vertex::get(HANDAPP, ANY));
    }
}
