/*
 *  Copyright 2012 Alex Stefaniuc
 *
 *  This file is part of Hand.
 *
 *  Hand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  Hand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with Hand. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VIEW_THEME_THEME_H
#define VIEW_THEME_THEME_H

#include "base/factory.h"
#include "base/handapp.h"

#include "SDL/SDL_ttf.h"
// #include "SDL/SDL_Image.h"
#include <map>


class Rgb;
class Rel_Rect;
class Layout;
class VirtualSurface;

class Theme : public HandApp
{
    public:
        Theme(std::string name);
        virtual ~Theme();

        bool execute(Vertex* request);
        void Register(Factory* f);
    protected:
        virtual TTF_Font* GetFont(int size);
        virtual void GetFontHeight(Vertex* layout, uint& max_size);
        VirtualSurface* GetSurface(Vertex* layout);

        // Drawing interface
        virtual void FillRect(SDL_Surface* sf, SDL_Rect* r, Rgb* c);
        virtual SDL_Surface* RenderText(std::string* text, int size, Rgb* color);

        // Stores once rendered fonts
        std::map<int, TTF_Font*> Fonts;

    private:
        bool FillOut(Vertex* layout);
        bool _FillOut(Vertex* layout);
        // Drawer callbacks  & layout generators
    protected:
        FactoryMap* Map;
};

//void SqueezeRect(SDL_Rect& rect, Rect factor);
//void SqueezeRect(SDL_Rect& rect, double factor);
// Changes x and y of source
void PlaceCentered(SDL_Surface* source, SDL_Rect& target, Rel_Rect& out);


#endif /* VIEW_THEME_THEME_H */
