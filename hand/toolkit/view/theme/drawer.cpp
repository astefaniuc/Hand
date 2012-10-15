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

#include "view/theme/drawer.h"
#include "view/theme/theme.h"
#include "view/layer/virtualsurface.h"
#include "graph/link.h"


using namespace std;


DrawerFactory::DrawerFactory
(
        Vertex* draw_func
) : Factory(DRAWOBJ_DRAWER, LINK, draw_func->Name())
{
    // Basic implementation check
    if(!draw_func)
        exit(666);
    DrawFunction = draw_func;
}


DrawerFactory::~DrawerFactory()
{
    delete(DrawFunction);
}


bool DrawerFactory::Execute(Vertex* tree)
{
    return tree->Attach(DrawFunction);
}
