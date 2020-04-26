#include <boost/filesystem/operations.hpp>
#include "base/filesystem.h"
#include "graph/data.h"


using namespace boost::filesystem;


File::File(const std::string& file_name) : List(file_name)
{
    type(FILE_);
    path file_path(file_name);
    if(is_regular_file(file_path))
    {
        if(file_path.has_parent_path())
            get(PATH)->set(new File(file_path.parent_path().string()));

        name(file_path.filename().string());
    }
    else // is directory
        name(file_name);
}


std::string File::GetFullPath()
{
    return GetPath().string();
}


path File::GetPath()
{
    path _path;
    // Get directory information from parent path
    Vertex* dir_rel = get(PATH);
    if(dir_rel)
    {
        File* parent_path = dynamic_cast<File*>(dir_rel->get());
        if(parent_path)
            _path = parent_path->GetPath();
    }
    // Check if it's still a relative path
    _path /= name();
    if(_path.is_relative())
        system_complete(_path);

    return _path;
}


// ----------------------------------------------------------------


bool FileFactory::IsValidInput(Vertex* input)
{
    path file_path(input->name());
    return (is_regular_file(file_path) || is_directory(file_path));
}


bool FileFactory::execute(Vertex* tree)
{
    Note* d_path = dynamic_cast<Note*>(tree);
    if(!d_path)
        return false;

    return tree->Vertex::add(new File(d_path->get()));
}


// ----------------------------------------------------------------


bool  DirectoryLoader::IsValidInput(Vertex* entry)
{
    File* ff = dynamic_cast<File*>(entry);
    return (ff && is_directory(ff->GetPath()));
}


bool  DirectoryLoader::execute(Vertex* tree)
{
    File* ff_dir = dynamic_cast<File*>(tree);
    if(!ff_dir)
        return false;
    path dir_path = ff_dir->GetPath();
    if(!is_directory(dir_path))
        return false;

    directory_iterator end;
    // (Re-)read the themes in
    ff_dir->reset();
    File* file;
    for(directory_iterator dir_it(dir_path); dir_it != end ; ++dir_it)
    {
        if(is_regular_file(dir_it->status()))
        {
            if(dir_it->path().extension() == LIBRARY_FILE_EXTENSION)
            {
                file = new File(dir_it->path().filename().string());
                file->get(PATH)->set(ff_dir);
                ff_dir->add(file);
            }
        }
        else if(is_directory(dir_it->status()))
        {
            // TODO: check or add dir
        }
    }
    return true;
}
