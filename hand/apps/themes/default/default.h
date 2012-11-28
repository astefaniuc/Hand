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

#ifndef DEFAULT_DEFAULT_H
#define DEFAULT_DEFAULT_H

#include "include/theme.h"


class Default : public Theme
{
    public:
        Default();
        ~Default(){};

    protected:
        // Layout creators
        bool GetViewLayout(Vertex* out);
        bool GetFramedListLayout(Vertex* out);
        bool GetListLayout(Vertex* out);
        bool GetButtonLayout(Vertex* out);
        bool GetContainerLayout(Vertex* out);
        bool GetFrameLayout(Vertex* out);
        bool GetBackgroundLayout(Vertex* out);
        bool GetTextLayout(Vertex* out);

        // Drawing methods
        bool ColorSurface(Vertex* drawing);
        bool DrawFrame(Vertex* drawing);
        bool DrawList(Vertex* drawing);
        bool DrawText(Vertex* drawing);

        bool DrawView(Vertex* drawing);

    private:
        std::string GetString(Vertex* vs);
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

#endif /* DEFAULT_DEFAULT_H */
