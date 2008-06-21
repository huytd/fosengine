#ifndef __C_GUI_SLOT_WINDOW_H_INCLUDED__
#define __C_GUI_SLOT_WINDOW_H_INCLUDED__

#include <irrlicht.h>


namespace irr
{
namespace gui
{

class CGUIBringUpSlotWindowButton;

//! The slot window is pretty much just a copy of the irrlicht gui window exept for some small changes
/** The close button doesn't remove the window but just makes it invisible and disableling it, the window can
brought up again with the bring-up button */
class CGUISlotWindow : public IGUIWindow
{
    public:
        //! Constructor
        CGUISlotWindow(IrrlichtDevice* device, IGUIElement* parent, s32 id, core::rect<s32> rectangle);

        //! Destrutor
        ~CGUISlotWindow();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! update absolute position
		virtual void updateAbsolutePosition();

		//! draws the element and its children
		virtual void draw();

		//! Returns pointer to the close button
		virtual IGUIButton* getCloseButton() const;

		//! Returns pointer to the minimize button
		virtual IGUIButton* getMinimizeButton() const;

		//! Returns pointer to the maximize button
		virtual IGUIButton* getMaximizeButton() const;

		//! creates a bring-up button
		virtual CGUIBringUpSlotWindowButton* createBringUpButton(core::rect<s32> rectangle, IGUIElement* parent = 0,
                                                              s32 id = -1, const wchar_t* text = 0,
                                                              const wchar_t* tooltip = 0);

        //! adds a slot array to the window
        /** \param slotRect: the relative rect of the slots
            \param texture: the texture to apply to the slots
            \param parent: the parent
            \param id: the shared id of the slots
            \param relPos: the relative position of the upper left slot of the array
            \param arrayDim: the number of slots to create
            \param spacing: the space between the slots */
        virtual core::array<IGUIElement*> addSlotArray(core::rect<s32> slotRect, video::ITexture* texture,
                                                        IGUIElement* parent = 0, s32 id = -1,
                                                        core::position2d<s32> relPos = core::position2d<s32>(10,20),
                                                        core::dimension2d<s32> arrayDim = core::dimension2d<s32>(2,2),
                                                        core::dimension2d<s32> spacing = core::dimension2d<s32>(5,5));


    private:

        core::position2d<s32> DragStart;
		bool Dragging;

		IGUIButton* CloseButton;
		IGUIButton* MinButton;
		IGUIButton* RestoreButton;

		IrrlichtDevice* Device;

};

}//end namespace gui
}//end namespace irr

#endif
