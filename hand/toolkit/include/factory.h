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

#ifndef HAND_FACTORY_H
#define HAND_FACTORY_H

#include "handapp.h"


using namespace std;


#define TYPE_FACTORY "TYPE_FACTORY"
#define RELATION_PRODUCER "RELATION_PRODUCER"
#define TYPE_FACTORY_INPUTSTRING "TYPE_FACTORY_INPUTSTRING"
#define TYPE_FACTORY_OUTPUTSTRING "TYPE_FACTORY_OUTPUTSTRING"
#define TYPE_FACTORY_URISCHEME "TYPE_FACTORY_URISCHEME"

class Factory : public HandApp
{
    public:
        Factory(string name, string input_type, string output_type, string uri_scheme);
        virtual ~Factory(){};

        virtual Functoid* Produce(Functoid* input) = 0;
        virtual void TakeBack(Functoid* product) = 0;

        virtual bool IsValidInput(Functoid* input) = 0;

        string GetInputType();
        string GetOutputType();
        string GetUriScheme();

    private:
        bool SetSpecifierString(string name, string value);
        string GetSpecifierString(string name);
};


#define TYPE_FACTORYMAP "TYPE_FACTORYMAP"

class FactoryMap : public FunctoidNode
{
    public:
        FactoryMap(string name);
        virtual ~FactoryMap(){};

        virtual Functoid* Produce(Functoid* input, string output_type);

        Factory* GetFactory(Functoid* input);
        Factory* GetFactory(string output_type);
//        virtual void RegisterFactory(Factory* resolver);
//        virtual void UnregisterFactory(Factory* resolver);
};

#endif /* HAND_FACTORY_H */
