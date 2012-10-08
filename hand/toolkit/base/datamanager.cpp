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

#include "base/datamanager.h"
#include "base/filesystem.h"
#include "base/handapploader.h"


using namespace std;


DataManager::DataManager() : FactoryMap("DefaultDataManager")
{
    Add(new HandAppLoader());
    Add(new FileFactory());
    Add(new Persistence());
}


DataManager::~DataManager()
{
    // TODO: delete owned resolvers
    //delete();
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


bool Persistence::Execute(Vertex* keys_tree)
{
    // Change name
    keys_tree->Name("settings:Keyboard::1");
    keys_tree = dynamic_cast<List*>(keys_tree->Get("Keylist"));

    // TEMP:
    string key_ids[] = {"97", "119", "101"," 102", "32"};

    for(uint i=0; i < (sizeof(key_ids)/sizeof(key_ids[0])); ++i)
    {
        // Init keys with SDLKey values
//        keys_tree->List[i]->Set(key_ids[i]);
    }
    return true;
}



void Persistence::TakeBack(Vertex* product)
{
    delete(product);
}


bool Persistence::IsValidInput(Vertex* keys_tree)
{
    // Change name
    if(keys_tree->Name() == "settings:Keyboard::0")
        return true;
    return false;
}
