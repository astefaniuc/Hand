#include "test2.h"

#include <time.h>
#include <iostream>


using namespace std;


extern "C" Functoid* Create()
{
    // Export interface
    return (new FindTester());
}


extern "C" void Destroy(HandApp* app)
{
    delete app;
}


FindTester::FindTester(void) : HandApp("FindTester")
{
//    TestDelete();
    TestFindBigTree();
    // For setting the breakpoint
    return;
}


void FindTester::TestDelete()
{
    string type = "TestDelete";
    Functoid* last_build;
    Functoid* root_node = _Build(this, 1, 1, type);
    last_build = _Build(root_node, 8, 10, type);
    string last_name = last_build->GetName();
    Delete(root_node);
    // Should print an error
    CheckFound(last_build, TestFind(last_name, TYPE_FUNCTOIDLIST, false));
    for(uint i=0; i<10; i++)
    {
        // Memory consumption shouldn't grow during this
        root_node = _Build(this, 1, 1, type);
        last_build = _Build(root_node, 8, 10, type);
        CheckFound(last_build, TestFind(last_build->GetName(), TYPE_FUNCTOIDLIST, false));
        Delete(root_node);
    }

    return;
}


void FindTester::TestFindBigTree()
{
    string last_name = "";
    string last_type = "";
    Functoid* found;
    // Search for the last one (depth 0 is the deepest)
    Functoid* last_build = Build(this, 8, 10, last_type);
    found = TestFind(last_build->GetName(), TYPE_FUNCTOIDLIST, false);
    CheckFound(last_build, found);

    // Only for building purposes
    // Add a few deep branches
    _Build(TestFind("depth1width1", last_type, true), 500, 1, "dead_deep_branch");

    // The second deep branch from the last element of last level
    last_build = Build(last_build, 490, 1, last_type);
    found = TestFind(last_build->GetName(), last_type, false);
    CheckFound(last_build, found);

    last_build = Build(last_build, 2, 1000, last_type);
    found = TestFind(last_build->GetName(), "", false);
    CheckFound(last_build, found);
    found = TestFind(last_build->GetName(), last_type, false);
    CheckFound(last_build, found);
    found = TestFind(last_build->GetName(), last_type, true);
    CheckFound(last_build, found);
}


Functoid* FindTester::Build
(
        Functoid* entry,
        int depth,
        int width,
        string& last_type
)
{
    ostringstream t;
    t << "width" << width;
    last_type = t.str();
    Functoid* ret = NULL;
    double start_time = clock();
    cout << "     Build(depth " << depth << ", width " << width << ", type "
            << last_type << ") time: ";
    ret =  _Build(entry, depth, width, last_type);
    cout << (clock()-start_time)/CLOCKS_PER_SEC << endl;
    return ret;
}


Functoid* FindTester::_Build
(
        Functoid* entry,
        int depth,
        int width,
        string type
)
{
    Functoid* f = NULL;
    for(int i=1; i<=width; i++)
    {
        {
            ostringstream n;
            n << "depth" << depth << "width" << i;
            string name = n.str();
            f = new FunctoidList(name);
        }
        f->SetType(type);
        ((FunctoidList*)entry)->Add(TAG_RELATION_CHILD, f);
        ((FunctoidList*)f)->Set(TAG_RELATION_PARENT, entry);
        if(depth > 1)
            f = _Build(f, depth-1, i, type);
    }
    return f;
}


Functoid* FindTester::TestFind(string name, string type, bool relation)
{
    double start_time = clock();
    FunctoidSearch search;
    search.SetSearchName(name);
    cout << "Find(name " << name;
    if(type != "")
    {
        search.SetSearchType(type);
        cout << ", type " << type;
        if(relation)
        {
            search.SetSearchRelation(TAG_RELATION_CHILD);
            cout << ", relation ";
        }
    }
    cout << ") time: ";
    search.RemoveDeadBranch = true;
    search.Search(this);
    cout << (clock()-start_time)/CLOCKS_PER_SEC << endl;

    return search.GetFindings();
}


bool FindTester::CheckFound(Functoid* expected, Functoid* found)
{
    if(found != expected)
    {
        cout << "Find Error! Expected: " << expected << ", found: " << found << endl;
        return false;
    }
    return true;
}
