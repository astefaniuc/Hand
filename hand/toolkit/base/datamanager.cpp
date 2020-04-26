#include "base/datamanager.h"
#include "base/filesystem.h"
#include "base/handapploader.h"
#include "view/theme/theme.h"
#include "input/statetree.h"
#include "view/layer/factories.h"


void DataManager::Init()
{
    add(new HandAppLoader());
    add(new FileFactory());
    add(new DirectoryLoader());
    add(new BinaryManager(THEMES, THEMES_DIRECTORY));
    add(new StateGraph());
    add(new Persistence());

    // Register default layer types/factories
    FactoryMap* layers = new FactoryMap(LAYER_FACTORIES);
    layers->add(new TextLayerFactory());
    layers->add(new ButtonLayerFactory());
    layers->add(new ListLayerFactory());
    add(layers);
}


//----------------------------------------------------------------------------


bool Persistence::execute(Vertex* keys_tree)
{
    // Change name
    keys_tree->name("settings:Keyboard::1");
    keys_tree = dynamic_cast<List*>(keys_tree->get("Keylist"));

    // TEMP:
    std::string key_ids[] = { "97", "119", "101"," 102", "32" };

    for(uint i = 0; i < (sizeof(key_ids)/sizeof(key_ids[0])); ++i)
    {
        // Init keys with SDLKey values
//        keys_tree->List[i]->set(key_ids[i]);
    }
    return true;
}


bool Persistence::IsValidInput(Vertex* keys_tree)
{
    // Change name
    return (keys_tree->name() == "settings:Keyboard::0");
}
