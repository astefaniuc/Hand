#ifndef HAND_VIEW_LAYER_LAYER_H
#define HAND_VIEW_LAYER_LAYER_H

#include "view/layer/virtualsurface.h"


class Layer : public VirtualSurface
{
public:
    virtual ~Layer() { Collapse(); }

    // Checks and updates content and triggers a re-draw if needed
    virtual bool Update(bool force);

    // Methods to (re-)set links to external objects:
    void SetParentLayer(Layer* parent) { ParentLayer = parent; }
    // TODO: any use for this?
    Layer* GetParentLayer() { return ParentLayer; }


    // Set pointer to a data tree node
    virtual void SetContent(HmiItem* content);
    HmiItem* GetContent() { return m_Data; }

    void Collapse();

    virtual bool SetCommand(HmiItem* cmd);

    // Reacting on user input
    virtual bool Request(HmiItem* request);

    virtual void NotifyChanged() { Changed = true; }

    virtual void Init() {}

    virtual void Draw(bool forced);

protected:
    // Insert "data" as layer of type layer_type at "position"
    Layer* Insert(HmiItem* data, const std::string& position);
    void Insert(HmiItem* data);

    virtual void DrawChilds(bool forced);

    virtual bool Exit();

    Layer* ParentLayer = nullptr;
    CUser* m_User;
    VirtualSurface* m_ScreenRect;
//        bool Updated;
public:
    HmiItem* m_Data;
    bool IsVisible = true;
    bool IsExpanded = false;
};

#endif // HAND_VIEW_LAYER_LAYER_H
