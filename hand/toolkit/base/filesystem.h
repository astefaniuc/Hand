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

#ifndef BASE_FILESYSTEM_H
#define BASE_FILESYSTEM_H

#include "base/factory.h"
#include "boost/filesystem/path.hpp"


#define PATH "Path"
#define FILEVERTEX "File"


class DirectoryLoader : public Factory
{
    public:
        DirectoryLoader() : Factory("DirectoryLoader",
                                    FILEVERTEX,
                                    FILEVERTEX){};
        ~DirectoryLoader(){};

        bool Execute(Vertex* descriptor);
        virtual void TakeBack(Vertex* product);

        bool IsValidInput(Vertex* input);
};


class File : public List
{
    public:
        File(std::string file);
        virtual ~File(){};

        std::string GetFullPath();
        boost::filesystem::path GetPath();
};


class FileFactory : public Factory
{
    public:
        FileFactory() : Factory("FileFactory",
                                       NOTE,
                                       FILEVERTEX){};
        virtual ~FileFactory(){};

        bool Execute(Vertex* input);
        void TakeBack(Vertex* product);

        bool IsValidInput(Vertex* input);
};

#endif /* BASE_FILESYSTEM_H */
