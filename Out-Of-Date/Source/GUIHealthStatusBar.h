/*
***********************************************
* Health and Mana Bar 
* *********************************************
* file name: GUIHealthStatusBar.h
* encoding: UTF-8
* tab size: 8
* indentation: 4
* created on: 6:31 PM 5/20/2008
* init code by: eMgz, Irrlicht Forum
* created by: FOSP Team
* copyright: FOS Project
*/
#ifndef GUI_HEALTH_STATUS_BAR_H_
#define GUI_HEALTH_STATUS_BAR_H_

using namespace irr;

class GUIHealthStatusBar : public irr::gui::IGUIElement 
{
private:
	video::IVideoDriver* driver;
	
	int x1Bar, y1Bar, x2Bar, y2Bar;	//Bar position

	int   maxHealthBarValue; //bar size
	double deltaHealthBar;   //interpolation
	double healthBarValue;   //current bar value

	int maxManaBarValue;
	double deltaManaBar;
	double manaBarValue;

	video::ITexture* GUITextures[10];

public:
	GUIHealthStatusBar::GUIHealthStatusBar(const irr::core::rect<irr::s32>& rect, irr::gui::IGUIEnvironment* env, irr::gui::IGUIElement* parent);
	~GUIHealthStatusBar(); //Empty destructor 
	virtual void draw();   //render the GUI
	void setHealthBarValue(int healthValue); 			//change bar values for health status
	void setManaBarValue(int manaValue);          		//change bar values for mana status
	void setHealthBarPosition(int xValue, int yValue); 	//change health bar position
	void setManaBarPosition(int xValue, int yValue); 	//change mana bar position
}; 

#endif /*GUI_HEALTH_STATUS_BAR_H_*/