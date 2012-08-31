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

#ifndef GRAPH_DEFINES_H
#define GRAPH_DEFINES_H


// Class type IDs
#define VERTEX   "Vertex"
#define LIST     "List"
#define METHOD   "Method"
#define RELATION "Relation"
#define DESCRIPTOR "Link"
#define DATA     "Data"
#define SEARCH   "Search"
// TODO: The string returned by typeid() for 'string' varies
// on different platforms
#define NOTE "Ss"


// Names of "runtime" Vertex elements
#define OWNER "OWNER"
#define TYPE "TYPE"
#define PUBLICLIST "PublicList"


// Wildcard for Get(type, name)
#define ANY "*"
// Arbitrary search depth (should be big enough to find everything
// and small enough to return fast from circles)
#define MAX_SEARCH_DEPTH 1024
#define MAX_POOL_SIZE 500


#endif /* GRAPH_DEFINES_H */
