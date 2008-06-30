#include "CGUIIcon.h"
#include "CGUIIconSlot.h"

namespace irr
{
namespace gui
{

//! Constructor
CGUIIcon::CGUIIcon(gui::IGUIEnvironment* environment, gui::IGUIElement* parent, s32 id, core::rect<s32> rectangle)
    : IGUIElement(gui::EGUIET_ELEMENT , environment, parent, id, rectangle), Texture(0),
    Moveable(true), CanBeOutsideSlot(false), Slot(0), Dragging(false) {}

//! Destructor
CGUIIcon::~CGUIIcon()
{
    if(Texture)
        Texture->drop();
}


//! Sets an image which should be displayed as the icon
void CGUIIcon::setImage(video::ITexture *img)
{
    if(Texture)
        Texture->drop();

    Texture = img;

    if(Texture)
        Texture->grab();
}

//! draws the element and its children
void CGUIIcon::draw()
{
    if (!IsVisible)
        return;

    irr::video::IVideoDriver* driver = Environment->getVideoDriver();

    if (Texture)
    {
		driver->draw2DImage(Texture, AbsoluteRect.UpperLeftCorner,
				core::rect<s32>(core::position2d<s32>(0,0), Texture->getOriginalSize()));

        IGUIElement::draw();
    }
}

//! called if an event happened.
bool CGUIIcon::OnEvent(const SEvent& event)
{
    switch(event.EventType)
    {
		
        case EET_GUI_EVENT:
        {
            if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
            {
                if (event.GUIEvent.Caller == (IGUIElement*)this)
                    Dragging = false;
                return true;
            }
            break;
        }
        case EET_MOUSE_INPUT_EVENT:
        {
            switch(event.MouseInput.Event)
            {
			
                //the mouse left button is pressed
                case EMIE_LMOUSE_PRESSED_DOWN:
                {
				
					printf("Press down\n");
                    //get the move position for dragging
                    DragStart.X = event.MouseInput.X;
                    DragStart.Y = event.MouseInput.Y;

                    //if the icon isn't in focus - put it in focus
                    if (!Environment->hasFocus(this))
                    {
						printf("Dragging\n");

                        //if the icon is moveable - dragging is enabled
                        Dragging = true*Moveable;

                        //set the focus
                        Environment->setFocus(this);

                        if(Parent)
                        {
                            //bring the icon to the front
                            Parent->bringToFront(this);

                            //if the icon is in a slot (if the parent isn't the gui root element)
                            if(!(Parent == Environment->getRootGUIElement()))
                            {
                                //make the icon a child of the root element (removing it from the slot)
                                Environment->getRootGUIElement()->addChild(this);
                                //the current position
                                core::rect<s32> currPos = getAbsolutePosition();
                                //move the icon the the right position (ready for dragging)
                                move(core::position2d<s32>(event.MouseInput.X - (event.MouseInput.X-currPos.UpperLeftCorner.X),
                                                           event.MouseInput.Y - (event.MouseInput.Y-currPos.UpperLeftCorner.Y)));
                            }
                        }
                    }
                    return true;
                }

                //the mouse left button is released
                case EMIE_LMOUSE_LEFT_UP:
                {
					printf("Press up\n");

                    //the icon is no longer being draged
                    Dragging = false;

                    //the icon is no longer in focus
                    Environment->removeFocus(this);

                    //bool to check if the icon is dropped in a slot
                    bool movedToSlot = false;

                    //is the icon droped in a useable slot's rect
                    u32 size = UsableSlots->size();
                    IGUIElement *slot = 0;
                    for(u32 i = 0; i < size; i++)
                    {
                        //get the slot
                        slot = (*UsableSlots)[i];

                        if(slot)
                        {
                            //get the slot rect
                            core::rect<s32> slotRect = slot->getAbsolutePosition();

                            //if the position the icon is dropped in, is inside the rect
                            if(slotRect.isPointInside(DragStart))
                            {
                                //move the icon to the slot
                                moveToSlot(slot);
                                movedToSlot = true;
                            }
                        }

                    }

                    //if the icon is dropped outside a slot, and it's not supposed to
                    if(!movedToSlot && !CanBeOutsideSlot)
                    {
                        if(Slot)
                        {
                            //move it back to where it came from
                            moveToSlot(Slot);
                        }
                    }

                    return true;
                }
                //the mouse is moved
                case EMIE_MOUSE_MOVED:
                {
					printf("Mouse move\n");
                    //if the icon is being dragged
                    if (Dragging)
                    {
						printf("Draging when mouse move\n");

                        //move the icon
                        move(core::position2d<s32>(event.MouseInput.X - DragStart.X, event.MouseInput.Y - DragStart.Y));

						//update the position
                        DragStart.X = event.MouseInput.X;
                        DragStart.Y = event.MouseInput.Y;

						return true;
                    }
                    break;
                }
                //just to keep the compiler from posting warnings
                default:{}
            }//switch mouse input
            break;
        }//case mouse input event
        //just to keep the compiler from posting warnings
        default:{}
    }//switch event type

    return Parent ? Parent->OnEvent(event) : false;
}//end function OnEvent

//! Sets whether the icon can be moved
void CGUIIcon::setMoveable(bool state)
{
    Moveable = state;
}

//! Sets whether the icon can be dropped outside a slot
void CGUIIcon::setCanBeOutsideSlot(bool state)
{
    CanBeOutsideSlot = state;
}

//! Adds a single slot to the usable slots
void CGUIIcon::addUsableSlot(IGUIElement* slot)
{
    UsableSlots->push_back(slot);
}

//! Adds the slots in the array to the usable slots
void CGUIIcon::addUsableSlotArray(const core::array<IGUIElement*>& slotArray)
{
    s32 arraySize = slotArray.size();
    for(s32 i = 0; i < arraySize; i++)
    {
        UsableSlots->push_back(slotArray[i]);
    }

}

//! Sets the slots in the passed array as the only usable slots
void CGUIIcon::setUsableSlotArray(core::array<IGUIElement*>* slotArray)
{
    UsableSlots = slotArray;
}

//! Moves the icon to the passed slot
void CGUIIcon::moveToSlot(IGUIElement* slot)
{
    //check if the slot is usable
    bool usable = false;
    u32 size = UsableSlots->size();
    for(u32 i = 0; i < size; i++)
    {
        if((*UsableSlots)[i] == slot)
            usable = true;
    }

    //if the slot is not usable - make it usable
    if(!usable)
        UsableSlots->push_back(slot);

    //if the slot is in use (if the slot has a children)
    if(slot->getChildren().getSize())
    {
        /* get the icon from the slot. Gets the last child of the child list,
        asuming that this is the only child the slot has,
        and that this child is an CGUIIcon */
        CGUIIcon* icon = (CGUIIcon*)(*slot->getChildren().getLast());
        //move the icon to where the currently moving icon came from
        if(Slot)
        {
            icon->moveToSlot(Slot);
        }
    }

    //add the currently moving icon to the slot
    slot->addChild(this);

    //save the slot
    Slot = slot;

    //set the relative position
    setRelativePosition(core::rect<s32>(core::position2d<s32>(0,0), Texture->getOriginalSize()));
}

//! Gets the slot the icon is in
IGUIElement* CGUIIcon::getSlot()
{
    return Slot;
}

}//end namespace gui
}//end namespace irr
