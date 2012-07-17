#ifndef HANDAPP_TEST2_H
#define HANDAPP_TEST2_H

#include "handapp.h"


using namespace std;


class FindTester : public HandApp
{
    public:
        FindTester(void);
        void TestDelete();
        void TestFindBigTree();
    private:
        Functoid* Build(Functoid* entry,
                   int depth,
                   int width,
                   string& last_type);
        Functoid* _Build(Functoid* entry,
                    int depth,
                    int width,
                    string type);
        Functoid* TestFind(string name, string type, bool relation);
        bool CheckFound(Functoid* expected, Functoid* found);
};

#endif /* HANDAPP_TEST2_H */
