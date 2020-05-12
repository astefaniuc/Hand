#include "test2.h"

#include <time.h>
#include <iostream>


using namespace std;


extern "C" Vertex* Create()
{
    // Export interface
    return (new FindTester());
}


extern "C" void Destroy(Module* app)
{
    delete app;
}


FindTester::FindTester(void) : Module("FindTester")
{
//    TestRemove();
    TestFindBigTree();
    // For setting the breakpoint
    return;
}


void FindTester::TestRemove()
{
    string type = "TestDelete";
    Vertex* last_build;
    Vertex* root_node = new Vertex("RootNode");
    last_build = _Build(root_node, 8, 10, type);
    string last_name = last_build->name();
    delete(root_node);
    // Should print an error
    CheckFound(last_build, TestFind(last_name, LIST, false));
    for(unsigned i = 0; i < 10; ++i)
    {
        // Memory consumption shouldn't grow during this
        root_node = new Vertex("RootNode");
        last_build = _Build(root_node, 8, 10, type);
        CheckFound(last_build, TestFind(last_build->name(), LIST, false));
        delete(root_node);
    }

    return;
}


void FindTester::TestFindBigTree()
{
    string last_type = "";
    Vertex* found;
    // Search for the last one (depth 0 is the deepest)
    Vertex* last_build = Build(this, 8, 10, last_type);
    found = TestFind(last_build->name(), LIST, false);
    CheckFound(last_build, found);

    // Only for building purposes
    // Add a few deep branches
    _Build(TestFind("depth1width1", last_type, true), 500, 1, "dead_deep_branch");

    // The second deep branch from the last element of last level
    last_build = Build(last_build, 490, 1, last_type);
    found = TestFind(last_build->name(), last_type, false);
    CheckFound(last_build, found);

    last_build = Build(last_build, 2, 1000, last_type);
    found = TestFind(last_build->name(), "", false);
    CheckFound(last_build, found);
    found = TestFind(last_build->name(), last_type, false);
    CheckFound(last_build, found);
    found = TestFind(last_build->name(), last_type, true);
    CheckFound(last_build, found);
}


Vertex* FindTester::Build(Vertex* entry, int depth, int width, std::string& last_type)
{
    ostringstream t;
    t << "width" << width;
    last_type = t.str();
    Vertex* ret = nullptr;
    double start_time = clock();
    cout << "     Build(depth " << depth << ", width " << width << ", type "
            << last_type << ") time: ";
    ret =  _Build(entry, depth, width, last_type);
    cout << (clock()-start_time)/CLOCKS_PER_SEC << endl;
    return ret;
}


Vertex* FindTester::_Build(Vertex* entry, int depth, int width, const std::string& type)
{
    Vertex* f = nullptr;
    for(int i=1; i<=width; i++)
    {
        {
            ostringstream n;
            n << "depth" << depth << "width" << i;
            f = new Collection(n.str());
        }
        f->type(type);
        entry->get(CHILDREN)->add(f);
        if(depth > 1)
            f = _Build(f, depth - 1, i, type);
    }
    return f;
}


Vertex* FindTester::TestFind(const std::string& name, const std::string& type, bool relation)
{
    double start_time = clock();
    Search search("Test search");
    search.SetSearchname(name);
    cout << "Find(name " << name;
    if(type != "")
    {
        search.SetSearchType(type);
        cout << ", type " << type;
        if(relation)
        {
            search.SetSearchLink(CHILDREN);
            cout << ", relation ";
        }
    }
    cout << ") time: ";
    bool found = search.execute(this);
    cout << (clock()-start_time)/CLOCKS_PER_SEC << endl;

    if(found)
        return search.get("Findings")->get();
    return nullptr;
}


bool FindTester::CheckFound(Vertex* expected, Vertex* found)
{
    if(found != expected)
    {
        cout << "Find Error! Expected: " << expected << ", found: " << found << endl;
        return false;
    }
    return true;
}
