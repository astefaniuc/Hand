#ifndef HAND_BASE_FILESYSTEM_H
#define HAND_BASE_FILESYSTEM_H

#include "data/data.h"
#include "data/list.h"


class Path : public Hmi::Manipulator<std::string>
{
public:
    std::string GetAbsolute();
    std::string GetRelative();
};


class Folder : public Path
{
public:
    bool IsValid(const std::string&) override;

    /// Returns a list of files and sub-folders.
    // TODO: regular expressions for filtering
    Hmi::List* GetContent();
};


class File : public Path
{
public:
    bool IsValid(const std::string&) override;
    std::string GetExtension();
};

#endif // HAND_BASE_FILESYSTEM_H
