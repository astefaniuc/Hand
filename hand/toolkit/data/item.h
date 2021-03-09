#ifndef HAND_GRAPH_HMIITEM_H
#define HAND_GRAPH_HMIITEM_H

#include "input/chord.h"
#include "base/listeners.h"
#include <string>
#include <sstream>
#include <vector>


class Layer;
namespace Layers { class Item; }

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
    Layer* GetButtonView();
    Layer* GetExpandedData();
    Layer* GetCompressedData();

    /// Called on button activation.
    virtual void Execute(Layers::Item* callerButton) = 0;

    void SetSelected(bool isSelected);
    bool IsSelected() const { return m_IsSelected; }

    /// Persistent chord suggestion, this item assumes the ownership.
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

    Listeners<Item> DataListeners;
    Listeners<Item> ExitListeners;
    Listeners<Item> SelectionListeners;

protected:
    virtual Layer* CreateExpandedView()  { return CreateExpandedData(); }
    virtual Layer* CreateButtonView();
    virtual Layer* CreateExpandedData() { return nullptr; }
    virtual Layer* CreateCompressedData() { return nullptr; }

    void NotifyChanged() { DataListeners.Notify(this); }

private:
    std::string m_Name;
    std::string m_Info;

    Hmi::List* m_Parent = nullptr;
    bool m_IsSelected = false;

    Chord* m_Shortcut = nullptr;

    ICallback<Layer>* m_LayerInitializer = nullptr;
};

}
#endif // HAND_GRAPH_HMIITEM_H
