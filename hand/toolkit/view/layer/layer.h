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


#ifndef HAND_LAYER_H
#define HAND_LAYER_H

#include "view/layer/virtualsurface.h"
#include "base/factory.h"


enum maximum_number_of_buttons
{
    MAX_NUMBER_OF_BUTTONS = 5
};


class LayerManager;
class Node;
class Theme;
class FactoryMap;

#define LAYER "Layer"
#define LAYER_TOPOGRAPHY "Layer Topography"


class Layer : public VirtualSurface
{
    public:
                      Layer();
        virtual      ~Layer();
        // Checks and updates content and triggers a re-draw if needed
        virtual bool Update(bool force);

        // Methods to (re-)set links to external objects:
        void         SetParentLayer(Layer* parent);
        Layer*       GetParentLayer();
        // Set pointer to a data tree node
        virtual void SetContent(Vertex*);
        Vertex*    GetContent();
        void         SetCommand(Node* cmd);
        void         ReleaseCommand();
        void         Collapse();

        // Reacting on user input
        virtual void PressGui();
        virtual void ReleaseGui();
        virtual bool Request(Vertex* request);

        virtual uint GetLevel();
        virtual void NotifyChanged();
        virtual void Init(){};

        virtual void SetTheme(Theme* theme);
        // Loads and stores from the layout library the layout for the current layer
        virtual void SetLayout(Vertex* layouts_lib);
        virtual void Draw(bool forced);

    protected:
        virtual bool Exit();
        // Insert "data" as layer of type layer_type at "position"
        virtual Layer* Insert(Vertex* data, std::string position);
        virtual void Configure(Vertex* parent_layout) = 0;
        virtual void DrawChilds(bool forced);

        Layer*         ParentLayer;
        // Internal map of enabled key/button pairs
        std::vector<Layer*> SubLayers;

//        bool Updated;
    public:
        bool IsVisible;
        bool IsExpanded;
    private:
        Node* _Node;
};


#endif /* HAND_LAYER_H */
