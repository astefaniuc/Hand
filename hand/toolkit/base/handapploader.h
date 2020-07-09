#ifndef HAND_APPLOADER_H
#define HAND_APPLOADER_H

#include "base/module.h"
#include "base/filesystem.h"


#define LIBRARY_FILE_EXTENSION ".so"


class ModuleLib : public File
{
public:
    ModuleLib() { /*TODO*/ }
    ~ModuleLib() { Close(); }

    bool IsValid(const std::string&) override;

    HmiItem* GetHmi() override;

    bool Load();
    void Close();

private:
    typedef Module* creator();
    typedef void destroyer(Module*);

    // Shared library handle
    void* Library = nullptr;
    creator* Create = nullptr;
    destroyer* Destroy = nullptr;

    Module* m_Module = nullptr;
};

#endif // HAND_APPLOADER_H
