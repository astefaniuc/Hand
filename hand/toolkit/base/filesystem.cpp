#include "base/filesystem.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>


namespace bfs = boost::filesystem;


bfs::path GetPath(Hmi::TData<std::string>* a_in)
{
    // We can't do this in the ctor because the parent is not set.
    bfs::path path(a_in->GetValue());
    if (path.is_relative())
    {
        Folder* parentManip = nullptr;
        Hmi::TData<std::string>* parentItem =
            dynamic_cast<Hmi::TData<std::string>*>(a_in->GetParent());
        if (parentItem)
            parentManip = dynamic_cast<Folder*>(parentItem->GetManipulator());

        if (parentManip)
            // Get path from parent folder.
            path = parentManip->GetAbsolute() + "/" + a_in->GetValue();
        else
            system_complete(path);
    }

    return path;
}


std::string Path::GetAbsolute()
{
    return GetPath(m_Item).string();
}


std::string Path::GetRelative()
{
    return GetPath(m_Item).relative_path().string();
}


bool Folder::IsValid(const std::string&)
{
    return is_directory(GetPath(m_Item));
}

Hmi::List* Folder::GetContent()
{
    bfs::path path = GetPath(m_Item);
    if (!is_directory(path))
        return nullptr;

    Hmi::Vector* ret = new Hmi::Vector(m_Item->GetValue(), "Content");

    bfs::directory_iterator end;
    for (bfs::directory_iterator iter(path); iter != end ; ++iter)
    {
        Manipulator<std::string>* manip = nullptr;
        if (is_regular_file(iter->status()))
            manip = new File();
        else if (is_directory(iter->status()))
            manip = new Folder();

        ret->Add(new Hmi::TData<std::string>(iter->path().filename().string(), "", "", manip));
    }

    return ret;
}

// ----------------------------------------------------------------

bool File::IsValid(const std::string&)
{
    return is_regular_file(GetPath(m_Item));
}


std::string File::GetExtension()
{
    return GetPath(m_Item).extension().string();
}
