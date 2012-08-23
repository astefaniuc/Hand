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

#include "base/filesystem.h"
#include "base/handserver.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


using namespace std;
using namespace boost::filesystem;


bool FileFunctoid_Factory::IsValidInput(Functoid* input)
{
    // Very basic check here
    Note* d_path = dynamic_cast<Note*>(input);
    if(!d_path)
        return false;
    string s_path = d_path->Get();
    if(s_path.find(URISCHEME_FILE) == 0)
        return true;
    return false;
}


Functoid* FileFunctoid_Factory::Produce(Functoid* descriptor)
{
    Note* d_path = dynamic_cast<Note*>(descriptor);
    if(!d_path)
        return NULL;
    return new FileFunctoid(d_path->Get());
}


void FileFunctoid_Factory::TakeBack(Functoid* product)
{
    if(dynamic_cast<FileFunctoid*>(product))
        delete(product);
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


FileFunctoid::FileFunctoid(string file_name) : FunctoidList(file_name)
{
    SetType(FILEFUNCTOID);
    if(file_name.find(URISCHEME_FILE) == 0)
        // Remove the URI string from the name
        file_name.erase(0, sizeof(URISCHEME_FILE)-1);

    path file_path(file_name);
    if(is_regular_file(file_path))
    {
        if(file_path.has_parent_path())
            Get(RELATION_PARENT_PATH)->Set(new FileFunctoid(file_path.parent_path().string()));

        SetName(file_path.filename().string());
    }
    else // is directory
        SetName(file_name);
}


string FileFunctoid::GetFullPath()
{
    return GetPath().string();
}


path FileFunctoid::GetPath()
{
    path _path;
    // Get directory information from parent path
    Functoid* dir_rel = Get(RELATION_PARENT_PATH);
    if(dir_rel)
    {
        FileFunctoid* parent_path = dynamic_cast<FileFunctoid*>(dir_rel->Get(1));
        if(parent_path)
            _path = parent_path->GetPath();
    }
    // Check if it's still a relative path
    _path /= GetName();
    if(_path.is_relative())
        system_complete(_path);

    return _path;
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


DirectoryLoader::~DirectoryLoader()
{
//    Unload();
}


bool  DirectoryLoader::IsValidInput(Functoid* entry)
{
    FileFunctoid* ff = dynamic_cast<FileFunctoid*>(entry);
    if(ff && is_directory(ff->GetPath()))
        return true;
    return false;
}


Functoid*  DirectoryLoader::Produce(Functoid* dir_obj)
{
    Functoid* ret = NULL;
    FileFunctoid* ff_dir = dynamic_cast<FileFunctoid*>(dir_obj);
    if(!ff_dir)
        return ret;
    path dir_path = ff_dir->GetPath();
    if(!is_directory(dir_path))
        return ret;

    directory_iterator end;
    // (Re-)read the themes in
    ff_dir->CleanUp();
    FileFunctoid* file;
    for(directory_iterator dir_it(dir_path); dir_it!=end; dir_it++)
    {
        if(is_regular_file(dir_it->status()))
        {
            if(dir_it->path().extension() == LIBRARY_FILE_EXTENSION)
            {
                file = new FileFunctoid(dir_it->path().filename().string());
                file->Get(RELATION_PARENT_PATH)->Set(ff_dir);
                ff_dir->Add(file);
                // Set return value != NULL
                ret = dir_obj;
            }
        }
        else if(is_directory(dir_it->status()))
        {
            // TODO: check or add dir
        }
    }
    return ret;
}


void DirectoryLoader::TakeBack(Functoid* dir_obj)
{
    FileFunctoid* ff_dir = dynamic_cast<FileFunctoid*>(dir_obj);
    if(ff_dir && is_directory(ff_dir->GetPath()))
        ff_dir->CleanUp();
}

