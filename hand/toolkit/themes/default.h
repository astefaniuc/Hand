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

#ifndef THEME_DEFAULT_H
#define THEME_DEFAULT_H

#include "theme.h"


using namespace std;


class Default : public Theme
{
    public:
        Default();
        //~Default();

    protected:
        // Layout creators
        bool GetLMLayout(Functoid* out);
        bool GetViewLayout(Functoid* out);
        bool GetFramedListLayout(Functoid* out);
        bool GetListLayout(Functoid* out);
        bool GetButtonLayout(Functoid* out);
        bool GetControlLayout(Functoid* drawing);
        bool GetContainerLayout(Functoid* out);
        bool GetFrameLayout(Functoid* out);
        bool GetBackgroundLayout(Functoid* out);
        bool GetTextLayout(Functoid* out);
       // Colors
        bool GetColorFrame(Functoid* out);
        bool GetColorBgrdList(Functoid* out);
        bool GetColorBgrdButton(Functoid* out);
        bool GetColorFont(Functoid* out);

        // Drawing methods
        bool ColorSurface(Functoid* drawing);
        bool DrawFrame(Functoid* drawing);
        bool DrawList(Functoid* drawing);
        bool DrawButton(Functoid* drawing);
        bool DrawText(Functoid* drawing);

        bool DrawView(Functoid* drawing);

    private:
        string GetString(Functoid* vs);
};

/*
class ZoomIn : public virtual Animation
{
    public:
        ZoomIn();
        ~ZoomIn(){};
        virtual bool GetSize(SDL_Rect& size);
        virtual bool GetNextProgress();
};
*/

#endif /* THEME_DEFAULT_H */
