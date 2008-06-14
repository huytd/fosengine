#include "HealthManaBar.h"

//Khởi tạo đối tượng.
HealthManaBar::HealthManaBar(const rect<s32>& rect, IGUIEnvironment* env, IGUIElement* parent)
   :IGUIElement(EGUIET_ELEMENT, env, parent, -1, rect)
   {
	   transparent = 30;
   }
  //Hủy đối tượng.
HealthManaBar::~HealthManaBar()
   {
   }
 
  //Set Value.
void HealthManaBar::setValue(int Value)
   {
	   value = Value;
   }


  //Set max value.
void HealthManaBar::setMaxValue(int MaxValue)
   {
	   maxValue = MaxValue;
   }

void HealthManaBar::setTexture(c8 *fileName)
   {
       IVideoDriver* driver = Environment->getVideoDriver();
	   imgBar = driver->getTexture(fileName);
	   BarSize = imgBar->getOriginalSize();
   }
void HealthManaBar::setPosition(int X,int Y)
   {
	   pos.X = X;
	   pos.Y = Y;
   }
void HealthManaBar::setTransparent(short int Transparent)
   {
       transparent = Transparent;
   }
void HealthManaBar::draw()
   {
	   IVideoDriver* driver = Environment->getVideoDriver();	

	   //Draw Background.
	   driver->draw2DRectangle(SColor(transparent,0,0,0),core::rect<s32>(pos.X,pos.Y,pos.X + BarSize.Width,pos.Y + BarSize.Height));
	   driver->draw2DImage(imgBar,pos,rect<s32>(0,0,(BarSize.Width*value)/maxValue,BarSize.Height));
   }