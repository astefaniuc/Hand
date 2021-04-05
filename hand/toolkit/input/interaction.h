#ifndef HAND_INPUT_INTERACTION_H
#define HAND_INPUT_INTERACTION_H

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
    void Update();

    Hand* GetHand() { return m_Hand; }

    void SetTarget(Layers::Interface* target);

    void AddGroup(Layers::List* child, bool hasFocus = false);
    void Remove(Group* child);
    void SetFocus(Group* child);

    void Execute(const Chord& chord);

private:
    void Clear();

    // Callbacks.
    void OnTargetExit(Layer* target);
    void OnTargetShow(Layer* inserted);
    void PopTargetCb(Layers::Item*);

    Hand* m_Hand;

    std::vector<Layers::Interface*> m_Stack;

    std::vector<Group*> m_Groups;
    Group* m_Focus = nullptr;

    Hmi::Item* m_MoveControlUp;

    bool m_IsModified = false;
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

    Group(Layers::List* layer, Control* parent);
    ~Group();

    void Update();
    void Clear();

    Control* GetControl() { return m_Parent; }

    void Add(Command* child) { m_Commands.push_back(child); }
    void Remove(Command* child);

    Layers::List* GetTarget() { return m_Target; }

    void SetFocus();
    void RemoveFocus();
    bool HasFocus() { return m_HasFocus; }

    bool Execute(const Chord& chord);

private:
    void OnTargetExit(Layer*);
    void OnFocusExit(Layer*) { m_Focus = nullptr; }
    void OnTargetDataChanged(Layer*) { Update(); }

    Layers::List* m_Target;
    Control* m_Parent;
    std::vector<Command*> m_Commands;

    Filter m_Filter = Visible;
    Mode m_Mode = Activate;

    Group* m_Focus = nullptr;
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

private:
    void OnTargetExit(Layer*);

    Group* m_Parent;
    Chord* m_Chord;
    Layers::List* m_Layer = nullptr;
};

}
#endif // HAND_INPUT_INTERACTION_H
