#ifndef HANDAPP_TEST2_H
#define HANDAPP_TEST2_H

#include "include/hand.h"


using namespace std;


class FindTester : public HandApp
{
    public:
        FindTester(void);
        void TestDelete();
        void TestFindBigTree();
    private:
        Vertex* Build(Vertex* entry,
                   int depth,
                   int width,
                   string& last_type);
        Vertex* _Build(Vertex* entry,
                    int depth,
                    int width,
                    string type);
        Vertex* TestFind(string name, string type, bool relation);
        bool CheckFound(Vertex* expected, Vertex* found);
};

#endif /* HANDAPP_TEST2_H */
