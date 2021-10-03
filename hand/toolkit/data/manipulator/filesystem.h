#ifndef HAND_DATA_MANIPULATOR_FILESYSTEM_H
#define HAND_DATA_MANIPULATOR_FILESYSTEM_H

#include "data/data.h"
#include "data/interface.h"
#include "data/vector.h"


namespace Manipulator {


class Path : public Typed<std::string>
{
public:
    ~Path() { delete m_Interface; }

    std::string GetAbsolute();
    std::string GetRelative();
};


class Folder : public Path
{
public:
    Folder() { m_Interface = new Data::Interface("Folder", "TODO"); }

    bool IsValid(const std::string&) override;

    /// Returns a list of files and sub-folders.
    // TODO: regular expressions for filtering
    Data::List* GetContent();
};


class File : public Path
{
public:
    File() { m_Interface = new Data::Interface("File", ""); }

    void Init() override;

    bool IsValid(const std::string&) override;
    std::string GetExtension();
};

}
#endif // HAND_DATA_MANIPULATOR_FILESYSTEM_H
