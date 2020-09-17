#ifndef HAND_MODULELIB_H
#define HAND_MODULELIB_H

#include "base/module.h"
#include "base/filesystem.h"


#define LIBRARY_FILE_EXTENSION ".so"


class ModuleLib : public File
{
public:
    ModuleLib() { /*TODO*/ }
    ~ModuleLib() { Close(); }

    bool IsValid(const std::string&) override;

    Layer* GetHmi() override;

    bool Load();
    void Close();
    Module* GetObject();

private:
    typedef Module* creator();
    typedef void destroyer(Module*);

    // Shared library handle
    void* Library = nullptr;
    creator* Create = nullptr;
    destroyer* Destroy = nullptr;

    Module* m_Module = nullptr;
};

#endif // HAND_MODULELIB_H
