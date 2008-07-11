#ifndef _HEALTH_MANA_BAR_H_
#define _HEALTH_MANA_BAR_H_

#include <irrlicht.h>
using namespace irr;
using namespace video;
using namespace core;
using namespace gui;

//! Class Define
class HealthManaBar : public IGUIElement 
{
protected:
	IVideoDriver* driver;

	//! Max status value
	int maxValue;

	//! Current status value
	int value; 

	ITexture* borderImage;
	position2d<s32> borderPosition;
    rect<s32> borderRect;
	rect<s32> borderOriginRect;
	rect<s32> borderClippingRect;

	short int transparent;

	ITexture* imgBar;

	//! Bar position
	position2d<s32> pos;

	//! Bar size
	dimension2d<s32> BarSize;

public:
	//! Default constructor
	HealthManaBar(const rect<s32>& rect,IGUIEnvironment* env,IGUIElement* parent);

	//! Default destructor
	~HealthManaBar();

	//! Set new value
	void setValue(int Value);
	void setMaxValue(int MaxValue);
	void setTexture(c8 *fileName);
	void setPosition(int X,int Y);
	void setTransparent(short int Transparent);

	void setMapBorder(c8 *fileName);//Set image border for the map

	//Render the bar
	void draw();

}; 
#endif




