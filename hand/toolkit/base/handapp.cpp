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
#include "base/filesystem.h"

#include <dlfcn.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


using namespace std;


HandApp::HandApp(string name) : List(name)
{
    Type(HANDAPP);
}


Binary::Binary() : Vertex(NAME_NOT_INIT)
{
    Type(APPLOADER);
    LoadedLib = NULL;
    Create = NULL;
    Destroy = NULL;
}


Binary::~Binary()
{
    Reset();
}


bool Binary::Execute(Vertex* input)
{
    Reset();
    File* path_obj = dynamic_cast<File*>(input);
    if(!path_obj)
        return false;

    LoadedLib = dlopen(path_obj->GetFullPath().c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if(LoadedLib)
    {
        Create = (creator*) dlsym(LoadedLib, "Create");
        Destroy = (destroyer*) dlsym(LoadedLib, "Destroy");
        if(Create && Destroy)
            return input->Vertex::Attach(Create());
    }

    cout << dlerror() << endl;
    return false;
}


void Binary::Reset()
{
    if(LoadedLib)
    {
        HandApp* casted = dynamic_cast<HandApp*>(Get(ANY, HANDAPP));
        if(Destroy && casted)
            Destroy(casted);
        dlclose(LoadedLib);
    }
    LoadedLib = NULL;
    Create = NULL;
    Destroy = NULL;
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


HandAppLoader::HandAppLoader() : Factory("HandApp Loader",
                                         FILE,
                                         HANDAPP)
{
    Type(APPLOADERFACTORY);
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


bool HandAppLoader::Execute(Vertex* input)
{
    Binary* bin = new Binary();
    // Open the library
    if(!bin->Execute(input))
    {
        delete(bin);
        return false;
    }
    input->Vertex::Add(bin);
    input->Vertex::Get(OUTPUTTYPE)->Set(Get(OUTPUTTYPE)->Get());
    return true;
}
