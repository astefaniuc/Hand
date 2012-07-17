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

#include "drawer.h"
#include "theme.h"
#include "virtualsurface.h"


using namespace std;


DrawerFactory::DrawerFactory
(
        Functoid* draw_func
) : Factory(DRAWOBJ_DRAWER, TYPE_DESCRIPTOR, draw_func->GetName(), "")
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


Functoid* DrawerFactory::Produce(Functoid* descr)
{
    descr->Set(DrawFunction);
    return DrawFunction;
}


bool DrawerFactory::IsValidInput(Functoid* input)
{
    if(dynamic_cast<Link*>(input) && input->IsType(GetOutputType()))
        return true;
    return false;
}
