#include "base/modulelib.h"
#include <dlfcn.h>
#include <iostream>


bool ModuleLib::Load()
{
    Close();

    Library = dlopen(GetAbsolute().c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (Library)
    {
        Create = (creator*) dlsym(Library, "Create");
        Destroy = (destroyer*) dlsym(Library, "Destroy");
        if (Create && Destroy)
            return true;
    }

    std::cout << dlerror() << std::endl;
    Close();
    return false;
}


void ModuleLib::Close()
{
    if (Library)
    {
        if (Destroy && m_Module)
            Destroy(m_Module);
        dlclose(Library);
    }
    Library = nullptr;
    Create = nullptr;
    Destroy = nullptr;
    m_Module = nullptr;
}


bool ModuleLib::IsValid(const std::string& a_ignore)
{
    return (File::IsValid(a_ignore) && (GetExtension() == LIBRARY_FILE_EXTENSION));
}


Module* ModuleLib::GetObject()
{
    if (!m_Module && (Create || Load()))
        m_Module = Create();

    return m_Module;
}
