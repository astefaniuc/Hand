#ifndef HAND_GRAPH_HMIITEM_H
#define HAND_GRAPH_HMIITEM_H

#include "input/chord.h"
#include "base/listeners.h"
#include <string>
#include <sstream>
#include <vector>


class Layer;

namespace Hmi {


class List;
class Interface;

class Item
{
public:
    enum InfoLevel
    {
        Hint,
        Info,
        Warning,
        Error,
        Fatal
    };

    Item(const std::string& name, const std::string& description)
        : m_Name(name), m_Info(description) {}
    virtual ~Item();

    void SetName(const std::string& name) { m_Name = name; }
    const std::string& GetName() const { return m_Name; }

    const std::string& GetInfo() const { return m_Info; }
    void SetInfo(const std::string& info, InfoLevel type, bool persistent = true) {}

    /// The parent is the object owner; HmiItems without a parent have to be explicitly deleted.
    void SetParent(Hmi::List* parent) { m_Parent = parent; }
    Hmi::List* GetParent() const { return m_Parent; }

    virtual Interface* GetInterface();


    Layer* GetExpandedView();
    void SetExpandedView(Layer* layer);

    Layer* GetButtonView();
    void SetButtonView(Layer* layer);

    void SetSelected(bool isSelected);
    bool IsSelected() const { return m_IsSelected; }

    void Activate() { ActivationListeners.Execute(this); }

    // Persistent chord suggestion, this item assumes the ownership.
    Chord* GetShortcut() const { return m_Shortcut; }
    void SetShortcut(Chord* chord);
    virtual void GetShortcuts(Hmi::Interface* caller, Hmi::List* out);

    // Layout options

    /// You can keep HmiItems in the closer focus while the user descends into details.
    void SetVisualKeepLevel(int level);

    template <class CbOwner>
    void SetLayerInitializer(CbOwner* obj, void (CbOwner::*method)(Layer*))
    {
        if (m_LayerInitializer)
            delete m_LayerInitializer;
        m_LayerInitializer = new CCallback<CbOwner, Layer>(obj, method);
    }

    Listeners<Item> SelectionListeners;
    Listeners<Item> ActivationListeners;
    Listeners<Item> DataListeners;

protected:
    virtual Layer* CreateExpandedView() = 0;
    virtual Layer* CreateButtonView();

    void NotifyChanged() { DataListeners.Execute(this); }

private:
    std::string m_Name;
    std::string m_Info;

    Hmi::List* m_Parent = nullptr;
    Layer* m_ExpandedView = nullptr;
    Layer* m_ButtonView = nullptr;
    bool m_IsSelected = false;

    Chord* m_Shortcut = nullptr;

    ICallback<Layer>* m_LayerInitializer = nullptr;
};

}
#endif // HAND_GRAPH_HMIITEM_H
