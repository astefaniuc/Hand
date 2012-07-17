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

#ifndef HANDAPPLOADER_H
#define HANDAPPLOADER_H

#include "factory.h"


using namespace std;


#define TYPE_APPLOADER "HandAppLoader"
#define NAME_NOT_INIT "Not initialized"

// Class factories for dynamic linking
typedef FunctoidNode* creator();
typedef void destroyer(void*);

class FileFunctoid;

class HandAppLoader : public Factory
{
    public:
        HandAppLoader(FileFunctoid* path);
        ~HandAppLoader();

        Functoid* Produce(Functoid* ignore);
        void TakeBack(Functoid* product);

        bool IsValidInput(Functoid* input);
    private:
        // Shared library handle
        void* LoadedLib;

        creator*   Create;
        destroyer* Destroy;
        // Stores a path to the library;
        // once this variable is set it can't be changed anymore
        string LibraryPath;
};


#define TYPE_APPLOADERFACTORY "HandAppLoader_Factory"

class HandAppLoader_Factory : public Factory
{
    public:
        HandAppLoader_Factory();
        ~HandAppLoader_Factory(){};

        virtual Functoid* Produce(Functoid* descriptor);
        virtual void TakeBack(Functoid* product);

        bool IsValidInput(Functoid* input);
 };

#endif /* HANDAPPLOADER_H */
