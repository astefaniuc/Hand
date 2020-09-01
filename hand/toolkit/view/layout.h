#ifndef HAND_VIEW_LAYOUT_H
#define HAND_VIEW_LAYOUT_H

#include "view/datatypes/rect.h"
#include "base/module.h"


class Layout : public Module
{
public:
    Layout();

    HmiItem* GetHmi() override { return &m_Settings; }

    const Rel_Rect& GetCoordinates() const { return m_Coordinates; }
    void SetCoordinates(const Rel_Rect& value) { m_Coordinates = value; }

    bool ShowFrame() const { return m_ShowFrame->GetValue(); }
    void SetShowFrame(bool enabled) { m_ShowFrame->SetValue(enabled); }

    bool ShowName() const { return m_ShowName->GetValue(); }
    void SetShowName(bool enabled) { m_ShowName->SetValue(enabled); }

    bool ShowDescription() const { return m_ShowDescription->GetValue(); }
    void SetShowDescription(bool enabled) { m_ShowDescription->SetValue(enabled); }

protected:
    Collection m_Settings;

private:
    Rel_Rect m_Coordinates;
    TData<bool>* m_ShowFrame = new TData<bool>("Show frame", "", true);
    TData<bool>* m_ShowName = new TData<bool>("Show name", "", true);
    TData<bool>* m_ShowDescription = new TData<bool>("Show description", "", true);
};


class ListLayout : public Layout
{
public:
    ListLayout() { m_Settings.Add(m_Alignment); }

    void SetMaxItemsToShow(unsigned count) { m_MaxItemsToShow->SetValue(count); }
    unsigned GetMaxItemsToShow() { return m_MaxItemsToShow->GetValue(); }

    // Returns the size and position as values relative to the parent.
    // Values between 0.0 and 1.0.
    Rel_Rect GetSizeAndPosition(unsigned field, unsigned numFields) const;

    const Rel_Rect& GetAlignment() const { return m_Alignment->GetValue(); }
    void SetAlignment(const Rel_Rect& value) { m_Alignment->SetValue(value); }

private:
    Rect* m_Alignment = new Rect("Alignment", "", 1.0, 0.0, 1.0, 0.0);
    TData<unsigned>* m_MaxItemsToShow = new TData<unsigned>("List max", "Items to show", 5);
};


#define CONTROL "Control"
#define DESCRIPTION "Description"
#define ICON "Icon"
#define TITLE "Title"
#define VIEW "View"


class ViewLayout : public Layout
{
public:
    ViewLayout();
    // Returns the size and position as values relative to the parent.
    // Values between 0.0 and 1.0.
    Rel_Rect GetSizeAndPosition(const std::string& field) const;

private:
    Collection* m_Fields = new Collection("Fields", "");
    Rect* m_Control = new Rect(CONTROL, "", 0.2, 0.8, 0.6, 0.2);
    Rect* m_Description = new Rect(DESCRIPTION, "", 0.8, 0.3, 0.2, 0.4);
    Rect* m_Title = new Rect(TITLE, "", 0.0, 0.0, 1.0, 0.02);
    Rect* m_View = new Rect(VIEW, "", 0.0, 0.02, 0.8, 0.78);
};


#endif //HAND_VIEW_LAYOUT_H
