#ifndef HAND_APPLOADER_H
#define HAND_APPLOADER_H

#include "base/factory.h"


// Class factories for dynamic linking
typedef List* creator();
typedef void destroyer(void*);

class File;

class Binary : public Vertex
{
public:
    Binary() : Vertex(NAME_NOT_INIT) { type(APPLOADER); }
    ~Binary() { reset(); }

    bool execute(Vertex* ignore) override;
    void reset();

private:
    Vertex* _get() override;

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
