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

#include "base/factory.h"


#define APPLOADER "HandAppLoader"
#define NAME_NOT_INIT "Not initialized"

// Class factories for dynamic linking
typedef List* creator();
typedef void destroyer(void*);

class FileVertex;

class Binary : public Vertex
{
    public:
        Binary();
        ~Binary();

        bool Execute(Vertex* ignore);
        void Reset();
    private:
        // Shared library handle
        void* LoadedLib;
        creator*   Create;
        destroyer* Destroy;
};


#define APPLOADERFACTORY "HandAppLoader_Factory"

class HandAppLoader : public Factory
{
    public:
        HandAppLoader();
        ~HandAppLoader(){};

        bool Execute(Vertex* descriptor);
        void TakeBack(Vertex* product);

        bool IsValidInput(Vertex* input);
 };

#endif /* HANDAPPLOADER_H */
