/*
***********************************************
* Character control
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
class GUIHealthStatusBar {

private:
	video::IVideoDriver* driver;
	int x1Bar, y1Bar, x2Bar, y2Bar;

	int   maxHealthBarValue; //bar size
	double deltaHealthBar;      //interpolation
	double healthBarValue;  //current bar value

	int maxManaBarValue;
	double deltaManaBar;
	double manaBarValue;

	video::ITexture* GUITextures[10];

public:

	GUIHealthStatusBar(video::IVideoDriver* videoDriver, int x1, int y1, int x2, int y2, int maxHealthValue, int maxManaValue); //constructor
	~GUIHealthStatusBar(); //destructor :O
	void renderGUIGame(); //render the GUI, must be called AFTER the 3D draw, or it  wont show
	void setHealthBarValue(int healthValue); //change bar values
	void setManaBarValue(int manaValue);

}; 


#endif /*GUIHEALTHSTATUSBAR_H_*/