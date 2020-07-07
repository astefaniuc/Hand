#include "input/keyboard.h"
#include "input/hand.h"


bool Keyboard::Process(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        for (Hand* hand : m_Hands)
            if (hand->Press(event.key.keysym.sym))
                break;
        return true;
    }
    if (event.type == SDL_KEYUP)
    {
        for (Hand* hand : m_Hands)
            if (hand->Release(event.key.keysym.sym))
                break;
        return true;
    }
    return false;
}


void Keyboard::GetKeyName(int k, Note* a_out)
{
    a_out->SetValue(SDL_GetKeyName((SDLKey)k));
}
