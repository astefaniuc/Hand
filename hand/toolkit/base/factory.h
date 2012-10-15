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

#ifndef BASE_FACTORY_H
#define BASE_FACTORY_H

#include "graph/list.h"


#define FACTORY "FACTORY"
#define INPUTTYPE "Input type"
#define OUTPUTTYPE "Output type"

class Factory : public Vertex
{
    public:
        Factory(std::string name,
                std::string input_type,
                std::string output_type);
        virtual ~Factory(){};

        virtual bool Execute(Vertex* input) = 0;
        virtual bool IsValidInput(Vertex* input);
};


#define FACTORYMAP "FACTORYMAP"

class FactoryMap : public List
{
    public:
        FactoryMap(std::string name);
        virtual ~FactoryMap(){};

        virtual bool Execute(Vertex* input);
        virtual bool Resolve(Vertex* input);

        Factory* GetFactory(Vertex* input);
        Factory* GetFactory(std::string output_type);
};

#endif /* BASE_FACTORY_H */
