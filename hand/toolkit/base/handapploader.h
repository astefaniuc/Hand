#ifndef HAND_APPLOADER_H
#define HAND_APPLOADER_H

#include "base/factory.h"


// Class factories for dynamic linking
typedef List* creator();
typedef void destroyer(void*);

class File;

class Binary
{
public:
    Binary() { /*TODO*/ }
    ~Binary() { reset(); }

    bool execute(Vertex* ignore);
    void reset();

private:
    // Shared library handle
    void* Library = nullptr;
    creator* Create = nullptr;
    destroyer* Destroy = nullptr;
};


class HandAppLoader : public Factory
{
public:
    HandAppLoader();

    bool execute(Vertex* descriptor) override;
    bool IsValidInput(Vertex* input);
 };


class BinaryManager : public List
{
public:
    BinaryManager(const std::string& name, const std::string& dir);
};

#endif // HAND_APPLOADER_H
