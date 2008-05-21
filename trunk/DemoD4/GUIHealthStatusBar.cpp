/*
***********************************************
* Health and Mana Bar  
* *********************************************
* file name: GUIHealthStatusBar.cpp
* encoding: UTF-8
* tab size: 8
* indentation: 4
* created on: 6:36 PM 5/20/2008
* init code by: eMgz, Irrlicht Forum
* created by: FOSP Team
* copyright: FOS Project
*/
#include <irrlicht.h>
#include "GUIHealthStatusBar.h"

GUIHealthStatusBar::~GUIHealthStatusBar()
{

}

GUIHealthStatusBar::GUIHealthStatusBar(const irr::core::rect<irr::s32>& rect, irr::gui::IGUIEnvironment* env, irr::gui::IGUIElement* parent)
: 	irr::gui::IGUIElement(irr::gui::EGUIET_ELEMENT, env, parent, -1, rect)
{
	//class constructor
	irr::video::IVideoDriver* driver = env->getVideoDriver();

	//health and mana bars  default size
	x1Bar = 52;
	y1Bar = 37;
	x2Bar = 58;
	y2Bar = 53;

	//max bar values
	maxHealthBarValue = 100;
	maxManaBarValue = 100;

	//current bar values
	healthBarValue = 100;
	manaBarValue = 100;

	deltaHealthBar = 100;
	deltaManaBar = 100;

	//load textures

	//load some texture for spellbar and make it transparent with ColorKey 0 (black) and put it into a texture array
	GUITextures[0] = driver->getTexture("media\\images\\gui\\healthbar\\spellbar.bmp");
	driver->makeColorKeyTexture(GUITextures[0], video::SColor(0,0,0,0));

	//same for the status bar (health and mana)
	GUITextures[1] = driver->getTexture("media\\images\\gui\\healthbar\\statusbar.bmp");
	driver->makeColorKeyTexture(GUITextures[1], video::SColor(0,0,0,0));

	//load some icons for the spellbar
	GUITextures[5] = driver->getTexture("media\\images\\gui\\healthbar\\icon_firebolt.bmp");

	GUITextures[6] = driver->getTexture("media\\images\\gui\\healthbar\\icon_fireblaze.bmp");

	GUITextures[7] = driver->getTexture("media\\images\\gui\\healthbar\\icon_firenova.bmp");
} 

void GUIHealthStatusBar::draw()
{
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();  

	float delta; //status bar variation
	core::dimension2d<s32> screenSize = driver->getScreenSize();

	double x=17; //spell bar position
	double y=screenSize.Height*0.85;;

	double iconSpace = 30;
	core::position2d<s32> iconPos = core::position2d<s32>(x+50, y+23);

	//here is some cool stuff, it changes the bar value from X to Y smoothly
	if ((int)deltaHealthBar != (int)healthBarValue){
		delta = healthBarValue - deltaHealthBar;
		if (delta > 0)
			healthBarValue -= 0.2;
		if (delta < 0)
			healthBarValue += 0.2;
	}

	//same for mana
	if ((int)deltaManaBar != (int)manaBarValue){
		delta = manaBarValue - deltaManaBar;
		if (delta > 0)
			manaBarValue -= 0.2;
		if (delta < 0)
			manaBarValue += 0.2;
	}

	//another cool stuff,  it draws the health bar according to its value (green for high, decreasing to yellow till red), and some others rectangles for a cool look
	driver->draw2DImage(GUITextures[1], core::position2d<s32>(x1Bar-50, y1Bar-29), core::rect<s32>(0,0,605,94), 0, video::SColor(255,255,255,255), true);

	driver->draw2DRectangle(video::SColor(255, 100, 100, 100), core::rect<s32>(x1Bar, y1Bar, (maxHealthBarValue*5)+x2Bar, y2Bar));

	driver->draw2DRectangle(video::SColor(255, 125, 125, 125), core::rect<s32>(x1Bar+1, y1Bar+1, maxHealthBarValue*5+x2Bar-1, y2Bar-1));

	driver->draw2DRectangle(video::SColor(255, 150, 150, 150), core::rect<s32>(x1Bar+3, y1Bar+3, maxHealthBarValue*5+x2Bar-3, y2Bar-3));

	driver->draw2DRectangle(core::rect<s32>(x1Bar+3, y1Bar+3, healthBarValue*5+x2Bar-3, y2Bar-3),

		video::SColor(255, 255-healthBarValue*2.55, healthBarValue*2.55, 0),

		video::SColor(255, 255-healthBarValue*2.55, healthBarValue*2.55, 0),

		video::SColor(255, 255-healthBarValue*2.55, healthBarValue*2.55-150, 0),

		video::SColor(255, 255-healthBarValue*2.55, healthBarValue*2.55-150, 0));

	//mana bar
	driver->draw2DRectangle(video::SColor(255, 100, 100, 100), core::rect<s32>(x1Bar, y1Bar+20, maxManaBarValue*5+x2Bar, y2Bar+20));

	driver->draw2DRectangle(video::SColor(255, 125, 125, 125), core::rect<s32>(x1Bar+1, y1Bar+21, maxManaBarValue*5+x2Bar-1, y2Bar+19));

	driver->draw2DRectangle(video::SColor(255, 150, 150, 150), core::rect<s32>(x1Bar+3, y1Bar+23, maxManaBarValue*5+x2Bar-3, y2Bar+17));

	driver->draw2DRectangle(core::rect<s32>(x1Bar+3, y1Bar+23, manaBarValue*5+x2Bar-3, y2Bar+17),

		video::SColor(255, 100, 100, 200),

		video::SColor(255, 100, 100, 200),

		video::SColor(255, 0, 0, 100),

		video::SColor(255, 0, 0, 100));


	//draw the spells bar and the icons on it according to the iconSpace
	driver->draw2DImage(GUITextures[0], core::position2d<s32>(x+3, y), core::rect<s32>(0,0,456,73), 0, video::SColor(255,255,255,255), true);
	
	for (int i=5; i<=7; i++){
		driver->draw2DImage(GUITextures[i], iconPos);
		iconPos.X += iconSpace;
	}

} 

void GUIHealthStatusBar::setHealthBarValue(int healthValue)
{
   deltaHealthBar = healthValue;
}

void GUIHealthStatusBar::setManaBarValue(int manaValue)
{
   deltaManaBar = manaValue;
} 

void GUIHealthStatusBar::setHealthBarPosition(int xValue, int yValue)
{
	x1Bar = xValue;
	y1Bar = yValue;
}

void GUIHealthStatusBar::setManaBarPosition(int xValue, int yValue)
{
	x2Bar = xValue;
	y2Bar = yValue;
}
