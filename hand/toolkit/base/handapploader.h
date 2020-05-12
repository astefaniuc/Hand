#ifndef HAND_APPLOADER_H
#define HAND_APPLOADER_H

#include "base/module.h"
#include "base/filesystem.h"


// Class factories for dynamic linking

class ModuleLib : public File
{
public:
    ModuleLib() { /*TODO*/ }
    ~ModuleLib() { Close(); }

    bool IsValid(const std::string&) override;
    // Temp, TODO?
    HmiItem* GetHmi() override;

    bool Load(Path* a_path);
    void Close();

private:
    typedef Module* creator();
    typedef void destroyer(void*);

    // Shared library handle
    void* Library = nullptr;
    creator* Create = nullptr;
    destroyer* Destroy = nullptr;

    Module* m_Module = nullptr;
};

#endif // HAND_APPLOADER_H
