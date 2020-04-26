#ifndef HAND_APPS_TEST_TEST2_H
#define HAND_APPS_TEST_TEST2_H

#include "include/hand.h"


class FindTester : public HandApp
{
public:
    FindTester(void);
    void TestRemove();
    void TestFindBigTree();

private:
    Vertex* Build(Vertex* entry, int depth, int width, std::string& last_type);
    Vertex* _Build(Vertex* entry, int depth, int width, const std::string& type);

    Vertex* TestFind(const std::string& name, const std::string& type, bool relation);
    bool CheckFound(Vertex* expected, Vertex* found);
};

#endif // HAND_APPS_TEST_TEST2_H
