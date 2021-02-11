#ifndef HAND_BASE_FILESYSTEM_H
#define HAND_BASE_FILESYSTEM_H

#include "data/data.h"
#include "data/interface.h"
#include "data/vector.h"


class Path : public Hmi::Manipulator<std::string>
{
public:
    ~Path() { delete m_Interface; }

    std::string GetAbsolute();
    std::string GetRelative();
};


class Folder : public Path
{
public:
    Folder() { m_Interface = new Hmi::Interface("Folder", "TODO"); }

    bool IsValid(const std::string&) override;

    /// Returns a list of files and sub-folders.
    // TODO: regular expressions for filtering
    Hmi::List* GetContent();
};


class File : public Path
{
public:
    File() { m_Interface = new Hmi::Interface("File", ""); }

    void Init() override;

    bool IsValid(const std::string&) override;
    std::string GetExtension();
};

#endif // HAND_BASE_FILESYSTEM_H
