#ifndef _HEALTH_MANA_BAR_H_
#define _HEALTH_MANA_BAR_H_

#include <irrlicht.h>
using namespace irr;
using namespace video;
using namespace core;
using namespace gui;
//Định ngĩa class.
class HealthManaBar : public IGUIElement //Class HealthManaBar kế thừa từ class IGUIElement.
{
private:
	IVideoDriver* driver;
	int maxValue;//Lượng máu lớn nhất.
	int value;   //Lượng máu hiện tại.
	short int transparent;
	ITexture* imgBar;
	position2d<s32> pos;
	dimension2d<s32> BarSize;//Kích thước bar.
public:
  //Khởi tạo đối tượng.
	HealthManaBar(const rect<s32>& rect,IGUIEnvironment* env,IGUIElement* parent);
  //Hủy đối tượng.
	~HealthManaBar();
  //Đặt các giá trị mới cho Máu, Mana,V.v....
	void setValue(int Value);
	void setMaxValue(int MaxValue);
	void setTexture(c8 *fileName);
	void setPosition(int X,int Y);
	void setTransparent(short int Transparent);
  //Vẽ.
	void draw();

}; 
#endif




