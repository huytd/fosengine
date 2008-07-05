Author: JonLT

http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=22812&start=0


I gui thing intented to be like a rpg gui. Move around icons in slots.



Included in the zip:
- CGUIIcon: The icon that can be moved between slots
- CGUIIconSlot: The slot to put the icon in
- CGUISlotWindow: A window with slots
- CGUIBringUpSlotWindowButton: A button to show/hide the slot window.

The button and the window is almost just copies of the original irrlicht gui, but with a few changes.

Slots can also just be added as children of a normal gui window.

How to use:
Code:

//create a slot window
CGUISlotWindow* window = new CGUISlotWindow(device, gui->getRootGUIElement(), -1, rect<s32>(25, 25, 300, 200));
window->drop();

//create a button to show/hide the window
IGUIButton* button = window->createBringUpButton(rect<s32>(10,210,110,242));
button->drop();

//load some very beautiful textures
ITexture* slotTex = driver->getTexture("slot.png");
ITexture* iconTex2 = driver->getTexture("icon.png");
ITexture* iconTex2 = driver->getTexture("icon2.png");

//create an array of slots in the window
core::array<IGUIElement*> slotArray = window->addSlotArray(core::rect<s32>(0,0,32,32), slotTex, 0, -1,
core::position2d<s32>(10,30),core::dimension2d<s32>(6,3),core::dimension2d<s32>(10,10));

//create an icon
CGUIIcon *icon = new CGUIIcon(gui, gui->getRootGUIElement(), -1, rect<s32>(0,0,32,32));
icon->drop();
icon->setImage(iconTex);

//create another icon
CGUIIcon *icon2 = new CGUIIcon(gui, gui->getRootGUIElement(), -1, rect<s32>(0,0,32,32));
icon2->drop();
icon2->setImage(iconTex2);

//let the icons know about the slots
icon->setUsableSlotArray(&slotArray);
icon2->setUsableSlotArray(&slotArray);


--Slots.zip--

Enjoy!