#ifndef HAND_BASE_FILESYSTEM_H
#define HAND_BASE_FILESYSTEM_H

#include <boost/filesystem/path.hpp>
#include "base/factory.h"


class DirectoryLoader : public Factory
{
public:
    DirectoryLoader() : Factory("DirectoryLoader", FILE_, FILE_) {}

    bool execute(Vertex* descriptor);
    bool IsValidInput(Vertex* input);
};


class File : public List
{
public:
    File(const std::string& file);

    std::string GetFullPath();
    boost::filesystem::path GetPath();
};


class FileFactory : public Factory
{
public:
    FileFactory() : Factory("FileFactory", NOTE, FILE_) {}

    bool execute(Vertex* input);
    bool IsValidInput(Vertex* input);
};

#endif // HAND_BASE_FILESYSTEM_H
