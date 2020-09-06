#ifndef HAND_VIEW_LAYOUT_H
#define HAND_VIEW_LAYOUT_H

#include "view/datatypes/rect.h"
#include "base/module.h"


#define DESCRIPTION "Description"
#define ICON "Icon"
#define TITLE "Title"


class Layout : public Module
{
public:
    Layout();

    HmiItem* GetHmi() override { return &m_Settings; }

    // Returns the size and position as values relative to the parent.
    // Values between 0.0 and 1.0.
    Rel_Rect GetField(const std::string& a_name) const;
    void SetField(const std::string& a_name, const Rel_Rect& coordinates);

    bool ShowName() const { return m_ShowName->GetValue(); }
    void SetShowName(bool enabled) { m_ShowName->SetValue(enabled); }

    bool ShowDescription() const { return m_ShowDescription->GetValue(); }
    void SetShowDescription(bool enabled) { m_ShowDescription->SetValue(enabled); }

protected:
    Collection m_Settings;
    Collection* m_Fields = new Collection("Fields", "");

private:
    TData<bool>* m_ShowName = new TData<bool>("Show name", "", true);
    TData<bool>* m_ShowDescription = new TData<bool>("Show description", "", true);
};


class ListLayout : public Layout
{
public:
    enum Alignment { Auto, Horizontal, Vertical };
    ListLayout() { m_Settings.Add(m_Alignment); }

    void SetMaxItemsToShow(unsigned count) { m_MaxItemsToShow->SetValue(count); }
    unsigned GetMaxItemsToShow() { return m_MaxItemsToShow->GetValue(); }

    /// Returns the size and position as values relative to the parent.
    /// Values between 0.0 and 1.0. 'alignment' can't be 'Auto'.
    Rel_Rect GetField(unsigned field, unsigned numFields, Alignment align) const;

    Alignment GetAlignment() const { return m_Alignment->GetValue(); }
    void SetAlignment(Alignment value) { m_Alignment->SetValue(value); }

private:
    TData<Alignment>* m_Alignment = new TData<Alignment>("Alignment", "", Auto);
    TData<unsigned>* m_MaxItemsToShow = new TData<unsigned>("List max", "Items to show", 5);
};


#define CONTROL "Control"
#define VIEW "View"


class ButtonLayout : public Layout
{
public:
    ButtonLayout();
};


class DataLayout : public Layout
{
public:
    DataLayout();
};


class ViewLayout : public Layout
{
public:
    ViewLayout();
};


#endif //HAND_VIEW_LAYOUT_H
