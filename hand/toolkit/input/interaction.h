#ifndef HAND_INPUT_FOCUS_H
#define HAND_INPUT_FOCUS_H

#include <vector>


struct Chord;
class Hand;
class Layer;
namespace Hmi { class Item; }
namespace Layers {
    class Item;
    class List;
    class Interface;
}

namespace Interaction {


class Group;

class Control
{
public:
    Control(Hand* hand);
    ~Control();

    void Start();

    Hand* GetHand() { return m_Hand; }

    void SetTarget(Layers::Interface* target);
    void RemoveTarget(Layers::Interface* target);

    void AddGroup(Layers::List* child, bool hasFocus = false);
    void Remove(Group* child);
    void SetFocus(Group* child);

    void Execute(const Chord& chord);

private:
    void Rebuild();
    void Clear();
    void RemoveTargetCb(Layer* target);
    void PopTargetCb(Layers::Item*);
    void RemoveTarget(std::vector<Layers::Interface*>::reverse_iterator& target);

    Hand* m_Hand;

    std::vector<Layers::Interface*> m_Stack;

    std::vector<Group*> m_Groups;
    Group* m_Focus = nullptr;

    Hmi::Item* m_MoveControlUp;
};


class Command;

class Group
{
public:
    enum Mode
    {
        Activate,
        Select
    };

    enum Filter
    {
        FirstLevel,
        Visible,
        AllShortcuts
    };

    Group(Layers::List* layer);
    ~Group();

    void Clear();

    Control* GetControl() { return m_Parent; }
    void SetControl(Control* parent) { m_Parent = parent; }

    const std::vector<Command*>& GetCommands() const { return m_Commands; }

    void Release();
    void Update();

    void Add(Command* child) { m_Commands.push_back(child); }
    void Remove(Command* child);

    Layers::List* GetTarget() { return m_Target; }

    void SetFocus();
    void RemoveFocus();
    bool HasFocus() { return m_HasFocus; }


private:
    Control* m_Parent = nullptr;
    std::vector<Command*> m_Commands;

    Layers::List* m_Target;

    Filter m_Filter = Visible;
    Mode m_Mode = Activate;

    bool m_Update = true;
    bool m_HasFocus = false;
};


class Command
{
public:
    Command(Group* parent, Chord* chord);
    ~Command();

    /// For a clean decomposition.
    void RemoveParent() { m_Parent = nullptr; }
    // Active chord, external ownership.
    Chord* GetChord() const { return m_Chord; }
    Layer* GetLayer();

private:
    Group* m_Parent;
    Chord* m_Chord;
    Layer* m_Layer = nullptr;
};

}
#endif // HAND_INPUT_FOCUS_H
