



#include "CGUISlotWindow.h"
#include "CGUIBringUpSlotWindowButton.h"



#include "CGUIIconSlot.h"





namespace irr
{
namespace gui
{


//! constructor
CGUISlotWindow::CGUISlotWindow(IrrlichtDevice* device, IGUIElement* parent, s32 id, core::rect<s32> rectangle)
: IGUIWindow(device->getGUIEnvironment(), parent, id, rectangle), Dragging(false), Device(device)
{
	#ifdef _DEBUG
	setDebugName("CGUISlotWindow");
	#endif

	IGUISkin* skin = 0;
	if (Environment)

		skin = Environment->getSkin();

	IGUISpriteBank* sprites = 0;
	video::SColor color(255,255,255,255);

	s32 buttonw = 15;
	if (skin)
	{
		buttonw = skin->getSize(EGDS_WINDOW_BUTTON_WIDTH);
		sprites = skin->getSpriteBank();
		color = skin->getColor(EGDC_WINDOW_SYMBOL);
	}
	s32 posx = RelativeRect.getWidth() - buttonw - 4;

	CloseButton = Environment->addButton(core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1,
		L"", skin ? skin->getDefaultText(EGDT_WINDOW_CLOSE) : L"Close" );
	CloseButton->setSubElement(true);
//CloseButton->setTabStop(false);
	CloseButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	if (sprites)
	{
		CloseButton->setSpriteBank(sprites);
		CloseButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_CLOSE), color);
		CloseButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_CLOSE), color);
	}
	posx -= buttonw + 2;

	RestoreButton = Environment->addButton(core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1,
		L"", skin ? skin->getDefaultText(EGDT_WINDOW_RESTORE) : L"Restore" );
	RestoreButton->setVisible(false);
	RestoreButton->setSubElement(true);
//RestoreButton->setTabStop(false);
	RestoreButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	if (sprites)
	{
		RestoreButton->setSpriteBank(sprites);
		RestoreButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_RESTORE), color);
		RestoreButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_RESTORE), color);
	}
	posx -= buttonw + 2;

	MinButton = Environment->addButton(core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1,
		L"", skin ? skin->getDefaultText(EGDT_WINDOW_MINIMIZE) : L"Minimize" );
	MinButton->setVisible(false);
	MinButton->setSubElement(true);
//MinButton->setTabStop(false);
	MinButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	if (sprites)
	{
		MinButton->setSpriteBank(sprites);
		MinButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_MINIMIZE), color);
		MinButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_MINIMIZE), color);
	}

	MinButton->grab();
	RestoreButton->grab();
	CloseButton->grab();
}








//! destructor
CGUISlotWindow::~CGUISlotWindow()

{
	if (MinButton)
		MinButton->drop();

	if (RestoreButton)
		RestoreButton->drop();

	if (CloseButton)
		CloseButton->drop();
}



//! called if an event happened.
bool CGUISlotWindow::OnEvent(const SEvent& event)
{
	switch(event.EventType)
	{
	case EET_GUI_EVENT:
		if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
		{
			//if (event.GUIEvent.Caller == (IGUIElement*)this)
				Dragging = false;
			//return true;
		}
		else
		if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUSED)
		{
			if (Parent && ((event.GUIEvent.Caller == this) || isMyChild(event.GUIEvent.Caller)))
				Parent->bringToFront(this);
		}
		else

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
		{
			if (event.GUIEvent.Caller == CloseButton)
			{
				setVisible(false);
				setEnabled(false);

				return true;

			}
		}


		break;
	case EET_MOUSE_INPUT_EVENT:
		switch(event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			DragStart.X = event.MouseInput.X;
			DragStart.Y = event.MouseInput.Y;
			Dragging = true;
			if (!Environment->hasFocus(this))
			{
				Dragging = true;
				Environment->setFocus(this);
				if (Parent)
					Parent->bringToFront(this);
			}
			return true;
		case EMIE_LMOUSE_LEFT_UP:
			Dragging = false;
			Environment->removeFocus(this);
			return true;
		case EMIE_MOUSE_MOVED:
			if (Dragging)
			{
				// gui window should not be dragged outside its parent
				if (Parent)
					if (event.MouseInput.X < Parent->getAbsolutePosition().UpperLeftCorner.X +1 ||
						event.MouseInput.Y < Parent->getAbsolutePosition().UpperLeftCorner.Y +1 ||
						event.MouseInput.X > Parent->getAbsolutePosition().LowerRightCorner.X -1 ||
						event.MouseInput.Y > Parent->getAbsolutePosition().LowerRightCorner.Y -1)

						return true;

				move(core::position2d<s32>(event.MouseInput.X - DragStart.X, event.MouseInput.Y - DragStart.Y));
				DragStart.X = event.MouseInput.X;
				DragStart.Y = event.MouseInput.Y;
				return true;
			}
			break;
        //just to keep the compiler from posting warnings
        default:{}

		}
    //just to keep the compiler from posting warnings
    default:{}

	}

	//return Parent ? Parent->OnEvent(event) : false;
	return IGUIElement::OnEvent(event);
}


//! Updates the absolute position.
void CGUISlotWindow::updateAbsolutePosition()
{
	IGUIElement::updateAbsolutePosition();
}



//! draws the element and its children
void CGUISlotWindow::draw()
{
	if (!IsVisible)
		return;

	IGUISkin* skin = Environment->getSkin();

	core::rect<s32> rect = AbsoluteRect;
	core::rect<s32> *cl = &AbsoluteClippingRect;

	// draw body fast
	rect = skin->draw3DWindowBackground(this, true, skin->getColor(EGDC_ACTIVE_BORDER),
		AbsoluteRect, &AbsoluteClippingRect);

	if (Text.size())
	{
		rect.UpperLeftCorner.X += skin->getSize(EGDS_TEXT_DISTANCE_X);
		rect.UpperLeftCorner.Y += skin->getSize(EGDS_TEXT_DISTANCE_Y);
		rect.LowerRightCorner.X -= skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 5;

		IGUIFont* font = skin->getFont(EGDF_WINDOW);
		if (font)
			font->draw(Text.c_str(), rect, skin->getColor(EGDC_ACTIVE_CAPTION), false, true, cl);
	}

	IGUIElement::draw();
}



//! Returns pointer to the close button
IGUIButton* CGUISlotWindow::getCloseButton() const
{
	return CloseButton;
}



//! Returns pointer to the minimize button
IGUIButton* CGUISlotWindow::getMinimizeButton() const
{
	return MinButton;
}



//! Returns pointer to the maximize button
IGUIButton* CGUISlotWindow::getMaximizeButton() const
{
	return RestoreButton;
}

//! adds a bring-up button
CGUIBringUpSlotWindowButton* CGUISlotWindow::createBringUpButton(core::rect<s32> rectangle, IGUIElement* parent,
                                                              s32 id, const wchar_t* text,
                                                              const wchar_t* tooltip)
{
    if(!parent)
        parent = Environment->getRootGUIElement();

    CGUIBringUpSlotWindowButton* button = new CGUIBringUpSlotWindowButton(Device, parent, id, rectangle, this);

    if(text)
        button->setText(text);

    if(tooltip)
        button->setToolTipText(tooltip);

    return button;
}

//! adds a slot array to the window
core::array<IGUIElement*> CGUISlotWindow::addSlotArray(core::rect<s32> slotRect, video::ITexture* texture,
                                                        IGUIElement* parent, s32 id ,
                                                        core::position2d<s32> relPos,
                                                        core::dimension2d<s32> arrayDim,
                                                        core::dimension2d<s32> spacing)
{
    if(!parent)
        parent = this;

    //create the array
    core::array<IGUIElement*> slotArray;
    for(s32 x = 0; x < arrayDim.Width; x++)
    {
        for(s32 y = 0; y < arrayDim.Height; y++)
        {
            CGUIIconSlot* slot = new CGUIIconSlot(Environment, parent, id, slotRect);
            slot->setImage(texture);
            slot->setRelativePosition(core::rect<s32>(relPos.X + x*(spacing.Width + slotRect.LowerRightCorner.X),
                                                      relPos.Y + y*(spacing.Height + slotRect.LowerRightCorner.Y),
                                                      slotRect.LowerRightCorner.X + (relPos.X + x*(spacing.Width + slotRect.LowerRightCorner.X)),
                                                      slotRect.LowerRightCorner.Y +(relPos.Y + y*(spacing.Height + slotRect.LowerRightCorner.Y))));
            slotArray.push_back(slot);
            slot->drop();
        }
    }

    return slotArray;
}

}//end namespace gui
}//end namespace irr



