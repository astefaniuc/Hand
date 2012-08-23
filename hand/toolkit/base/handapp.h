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

#ifndef HANDAPP_H
#define HANDAPP_H

#include "graph/functoidlist.h"
#include "graph/search/functoidsearch.h"


using namespace std;


#define HANDAPP "HandApp"


class HandServer;
class LayerManager;

class HandApp : public FunctoidList
{
    public:
                      HandApp(string name);
              virtual ~HandApp(){};
        HandServer*  Server();
        LayerManager* GetLayerManager();
        void          SetLayerManager(LayerManager* lm);

        typedef bool  (*tFunction)();

    protected:
        LayerManager* _LayerManager;
};

#endif /* HANDAPP_H */
