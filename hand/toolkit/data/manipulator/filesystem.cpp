#include "data/manipulator/filesystem.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>


namespace bfs = boost::filesystem;
namespace Manipulator {


bfs::path GetPath(Data::String* a_in)
{
    // We can't do this in the ctor because the parent is not set.
    bfs::path path(a_in->GetValue());
    if (path.is_relative())
    {
        Folder* parentManip = nullptr;
        Data::String* parentItem =
            dynamic_cast<Data::String*>(a_in->GetParent());
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


Data::List* Folder::GetContent()
{
    bfs::path path = GetPath(m_Item);
    if (!is_directory(path))
        return nullptr;

    Data::Vector* ret = new Data::Vector(m_Item->GetValue(), "Content");

    bfs::directory_iterator end;
    for (bfs::directory_iterator iter(path); iter != end ; ++iter)
    {
        Typed<std::string>* manip = nullptr;
        if (is_regular_file(iter->status()))
            manip = new File();
        else if (is_directory(iter->status()))
            manip = new Folder();

        ret->Add(new Data::String(iter->path().filename().string(), "", "", manip));
    }

    return ret;
}



void File::Init()
{
    m_Interface->GetView()->Attach(m_Item);
}


bool File::IsValid(const std::string&)
{
    return is_regular_file(GetPath(m_Item));
}


std::string File::GetExtension()
{
    return GetPath(m_Item).extension().string();
}

}
