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

#include "base/handapp.h"


#define FACTORY "FACTORY"
#define RELATION_PRODUCER "RELATION_PRODUCER"
#define FACTORY_INPUTSTRING "FACTORY_INPUTSTRING"
#define FACTORY_OUTPUTSTRING "FACTORY_OUTPUTSTRING"
#define FACTORY_URISCHEME "FACTORY_URISCHEME"

class Factory : public HandApp
{
    public:
        Factory(std::string name,
                std::string input_type,
                std::string output_type,
                std::string uri_scheme);
        virtual ~Factory(){};

        virtual Functoid* Produce(Functoid* input) = 0;
        virtual void TakeBack(Functoid* product) = 0;

        virtual bool IsValidInput(Functoid* input) = 0;

        std::string GetInputType();
        std::string GetOutputType();
        std::string GetUriScheme();

    private:
        bool SetSpecifierString(std::string name, std::string value);
        std::string GetSpecifierString(std::string name);
};


#define FACTORYMAP "FACTORYMAP"

class FactoryMap : public FunctoidList
{
    public:
        FactoryMap(std::string name);
        virtual ~FactoryMap(){};

        virtual Functoid* Produce(Functoid* input, std::string output_type);

        Factory* GetFactory(Functoid* input);
        Factory* GetFactory(std::string output_type);
//        virtual void RegisterFactory(Factory* resolver);
//        virtual void UnregisterFactory(Factory* resolver);
};

#endif /* HAND_FACTORY_H */
