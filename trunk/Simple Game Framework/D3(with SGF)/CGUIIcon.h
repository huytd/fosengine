#ifndef __C_GUI_ICON_H_INCLUDED__
#define __C_GUI_ICON_H_INCLUDED__

#include <irrlicht.h>
#include "CGUIIconSlot.h"

namespace irr
{
namespace gui
{

//! The icon can be moved around between slots
class CGUIIcon : public gui::IGUIElement
{
public:

    //! Constructor
    CGUIIcon(gui::IGUIEnvironment* environment, gui::IGUIElement* parent, s32 id, core::rect<s32> rectangle);

    //! Destructor
    ~CGUIIcon();

    //! Sets an image which should be displayed as the icon
    virtual void setImage(video::ITexture *img);

    //! draws the element and its children
    virtual void draw();

    //! called if an event happened.
    bool OnEvent(const SEvent& event);

    //! Sets whether the icon can be moved
    void setMoveable(bool state);

    //! Sets whether the icon can be dropped outside a slot
    void setCanBeOutsideSlot(bool state);

    //! Adds a single slot to the usable slots
    void addUsableSlot(gui::IGUIElement* slot);

    //! Adds the slots in the array to the usable slots
    /** Depending on the size of the array passed, this may be expensive to do */
    void addUsableSlotArray(const core::array<gui::IGUIElement*>& slotArray);

    //! Sets the slots in the passed array as the only usable slots (more slots can be added)
    void setUsableSlotArray(core::array<gui::IGUIElement*>* slotArray);

    //! Moves the icon to the passed slot
    void moveToSlot(gui::IGUIElement* slot);

    //! Gets the slot the icon is in
    IGUIElement* getSlot();

private:
    //the texture of the icon
    video::ITexture *Texture;
    //can the icon move
    bool Moveable;
    //can the icon exist outside a slot
    bool CanBeOutsideSlot;
    //the slot the is in
    IGUIElement* Slot;
    //the usable slots
    core::array<gui::IGUIElement*>* UsableSlots;

    bool Dragging;

    core::position2di DragStart;

};

}//end namespace gui
}//end namespace irr

#endif
