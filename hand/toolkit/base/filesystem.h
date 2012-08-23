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

#ifndef HAND_FILESYSTEM_H
#define HAND_FILESYSTEM_H

#include "base/factory.h"
#include "boost/filesystem/path.hpp"


#define URISCHEME_FILE "file://"
#define RELATION_PARENT_PATH "RELATION_PARENT_PATH"
#define FILEFUNCTOID "FileFunctoid"


class DirectoryLoader : public Factory
{
    public:
        DirectoryLoader() : Factory("DirectoryLoader",
                                    FILEFUNCTOID,
                                    FILEFUNCTOID,
                                    URISCHEME_FILE){};
        ~DirectoryLoader();

        virtual Functoid* Produce(Functoid* descriptor);
        virtual void TakeBack(Functoid* product);

        bool IsValidInput(Functoid* input);
};


class FileFunctoid : public FunctoidList
{
    public:
        FileFunctoid(std::string file);
        virtual ~FileFunctoid(){};

        std::string GetFullPath();
        boost::filesystem::path GetPath();
};


class FileFunctoid_Factory : public Factory
{
    public:
        FileFunctoid_Factory() : Factory("FileFunctoid_Factory",
                                 DATA_STRING,
                                 FILEFUNCTOID,
                                 URISCHEME_FILE){};
        virtual ~FileFunctoid_Factory(){};

        Functoid* Produce(Functoid* input);
        void TakeBack(Functoid* product);

        bool IsValidInput(Functoid* input);
};

#endif /* HAND_FILESYSTEM_H */
