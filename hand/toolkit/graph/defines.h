#ifndef HAND_GRAPH_DEFINES_H
#define HAND_GRAPH_DEFINES_H


// Class type IDs
#define VERTEX   "Vertex"
#define LIST     "List"
#define METHOD   "Method"
#define LINK     "Link"
#define DATA     "Data"
#define SEARCH   "Search"
// TODO: The string returned by typeid() for 'string' varies
// on different platforms
#define NOTE "Ss"


// Names of "runtime" Vertex elements
#define TYPE "TYPE"
#define PUBLIC "Public"


// Wildcard for get(type, name)
#define ANY "*"
// Arbitrary search depth (should be big enough to find everything
// and small enough to return fast from circles)
#define MAX_SEARCH_DEPTH 1024


#endif // HAND_GRAPH_DEFINES_H
