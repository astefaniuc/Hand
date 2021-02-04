#ifndef HAND_INPUT_FOCUS_H
#define HAND_INPUT_FOCUS_H

#include <vector>


class Chord;
class Hand;
class Layer;
namespace Hmi { class Item; }
namespace Layers {
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

    void Add(Group* child);
    void Remove(Group* child);
    void SetFocus(Group* child);
    void SetShortcuts(Group* s);

    void Execute(const Chord& chord);

private:
    void Rebuild();
    void Clear();
    void RemoveTargetCb(Layers::Interface* target);

    Hand* m_Hand;

    std::vector<Layers::Interface*> m_Stack;

    std::vector<Group*> m_Groups;
    Group* m_Focus = nullptr;
    Group* m_ShortCuts = nullptr;
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

    Layers::List* GetTarget() { return m_Layer; }

private:
    Control* m_Parent = nullptr;
    std::vector<Command*> m_Commands;

    Layers::List* m_Layer;

    Filter m_Filter = Visible;
    Mode m_Mode = Activate;

    bool m_Update = true;
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
};

}
#endif // HAND_INPUT_FOCUS_H
