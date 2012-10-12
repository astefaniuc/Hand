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

#ifndef VIEW_LAYER_LAYERMANAGER_H
#define VIEW_LAYER_LAYERMANAGER_H

#include "view/layer/listlayer.h"


class Device;
class InputState;


class LayerManager : public ListLayer
{
    public:
                LayerManager();
        virtual ~LayerManager();

        bool    Update(bool force);
        void    SetScreen(SDL_Surface* screen);

        bool    GetCommand(Vertex* func, int level);
        bool    GetCommand(Layer* func, int level);
        // Returns the list which should be mapped to the InputState
        // TODO: better move it to the "view" layer or to base layer?
        Vertex* GetCommandList(Vertex* base);
        Device* GetDevice();
        void    SetDevice(Device*);

        // Stores the pointer to the (SDL) drawing object
        bool    Request(Vertex* request);
        // Called by HandApps to export their interface
        void    LoadAppInterface(Vertex* app,  bool make_default=false);
        bool    UnloadAppInterface(Vertex*);
        Layer*  CreateLayer(Vertex* content);

    protected:
        void    Init();

    private:
        bool    Exit(Vertex* ignore);
        bool    GetAllThemes(Vertex* themes_dir);
        bool    LoadTheme(Vertex* theme);
        void    UnloadTheme();
        bool    Expand(Vertex* list);

        // Contains the only pointer to the Device
        InputState*  _InputState;
        Vertex*    NextRequest;
        SDL_Surface* Screen;
        Layer*       MasterView;
};

#endif /* VIEW_LAYER_LAYERMANAGER_H */
