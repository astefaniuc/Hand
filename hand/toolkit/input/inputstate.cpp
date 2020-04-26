#include "input/inputstate.h"
#include "graph/vertex.h"
#include "view/layer/layermanager.h"


using namespace std;


InputState::InputState(Vertex* dev)
{
    LM = dynamic_cast<LayerManager*>(dev->Vertex::get(LAYERMANAGER)->get());
    dev->get(FACTORY, STATEGRAPH)->execute(dev);
    NullKey = PressedKey = ReleasedKey = dynamic_cast<StateNode*>(dev->Vertex::get(STATENODE, ANY));
    LM->set(GetCommands(1));
}


bool InputState::Press(uint k)
{
    PressedKey = ReleasedKey = ReleasedKey->GetChild(k);
    if(!PressedKey)
    {
        reset();
        return false;
    }

    return true;
}


bool InputState::Release(uint k)
{
    // Move the ReleasedKey up the the three
    // and the ReleasedKey down to the current key combination node
    if((PressedKey == NullKey) ||
        ((ReleasedKey=ReleasedKey->GetParent(k)) == nullptr))
    {
        // Some events get lost, this command is corrupted
        reset();
        return false;
    }

    // Execute command when all pressed keys have been released
    if(ReleasedKey == NullKey)
    {
        // Inform the GUI
        Vertex* method = PressedKey->Vertex::get(METHOD, ANY);
        if(method)
        {
            Vertex* param = method->Vertex::get("Parameter")->get();
            if(!param)
                param = method;
            method->execute(param);
        }

        reset();
    }
    return true;
}


void InputState::reset(void)
{
    PressedKey = ReleasedKey = NullKey;
}


StateNode* InputState::GetKey(key_pointer key)
{
    switch(key)
    {
    case ROOT:
        return NullKey;
    case PRESSED:
        return PressedKey;
    case RELEASED:
        return ReleasedKey;
    default:
        return nullptr;
    }
}


bool InputState::GetCommand(Vertex* target, uint level)
{
    if(!NullKey)
        // Not initialized
        return false;

    // Get the list of available commands
    Vertex* free_cmds = GetCommands(level);
    Vertex* f_cmd;
    uint i = 0;
    while((f_cmd=free_cmds->get(++i)) != nullptr)
    {
        // Bind the functional list to the state graph (command) node,
        // bidirectional
        if(f_cmd->set(target))
        {
            target->Vertex::get(COMMAND)->set(f_cmd);

            return true;
        }
    }
    return false;
}


Vertex* InputState::GetCommands(uint level)
{
    if(level > NullKey->size())
        return nullptr;

    // Search from bottom up
    StateNode* tmp_node = NullKey;
    for(uint i=1; i<=level; i++)
        tmp_node = tmp_node->GetChild(i);

    return tmp_node->Vertex::get(COMMANDS);
}
