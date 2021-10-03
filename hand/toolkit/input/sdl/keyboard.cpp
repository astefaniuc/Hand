#include "input/sdl/keyboard.h"
#include "input/hand.h"


bool Keyboard::Process(const SDL_Event& event)
{
    if ((event.type == SDL_KEYDOWN) && (event.key.repeat == 0))
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


void Keyboard::GetKeyName(int k, Data::String* a_out)
{
    a_out->SetValue(SDL_GetKeyName((SDL_Keycode)k));
}
