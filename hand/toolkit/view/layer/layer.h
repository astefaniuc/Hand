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


#ifndef VIEW_LAYER_LAYER_H
#define VIEW_LAYER_LAYER_H

#include "view/layer/virtualsurface.h"


class Layer : public VirtualSurface
{
    public:
                      Layer(std::string name);
        virtual      ~Layer();
        // Checks and updates content and triggers a re-draw if needed
        virtual bool Update(bool force);

        // Methods to (re-)set links to external objects:
        void         SetParentLayer(Layer* parent);
        Layer*       GetParentLayer();
        // Set pointer to a data tree node
        virtual void SetContent(Vertex* content);
        Vertex*      GetContent();
        void         Collapse();

        virtual bool SetCommand(Vertex* cmd);

        // Reacting on user input
        virtual bool Request(Vertex* request);

        virtual void NotifyChanged();
        virtual void Init(){};

        virtual void SetLayout(Vertex* layout);
        virtual void Draw(bool forced);

    protected:
        // Insert "data" as layer of type layer_type at "position"
        virtual Layer* Insert(Vertex* data, std::string position);
        Vertex* GetLayerFactory(Vertex* data, std::string position);
        virtual Vertex* GetLayout(Vertex* data, Vertex* layer_factory);
        bool AddToUpdate(Vertex* layout, std::string position);

        virtual void DrawChilds(bool forced);

        virtual bool Exit();

        Layer* ParentLayer;

//        bool Updated;
    public:
        bool IsVisible;
        bool IsExpanded;
};


#endif /* VIEW_LAYER_LAYER_H */
