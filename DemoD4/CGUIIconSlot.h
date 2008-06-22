#ifndef __C_GUI_ICON_SLOT_H_INCLUDED__
#define __C_GUI_ICON_SLOT_H_INCLUDED__

#include <irrlicht.h>

namespace irr
{
namespace gui
{

//! A slot to put icons in
class CGUIIconSlot : public gui::IGUIElement
{
public:

    //! Constructor
    CGUIIconSlot(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle)
        : IGUIElement(EGUIET_ELEMENT , environment, parent, id, rectangle), Texture(0) {}

    //! Destructor
    ~CGUIIconSlot()
    {
        if(Texture)
            Texture->drop();
    }

    //! Sets an image which should be displayed as the slot
    virtual void setImage(video::ITexture *img)
    {
        if(Texture)
            Texture->drop();

        Texture = img;

        if(Texture)
            Texture->grab();
    }

    //! draws the element and its children
    virtual void draw()
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

private:
    video::ITexture* Texture;

};

}//end namespace gui
}//end namespace irr
#endif
