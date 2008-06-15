#ifndef MAP_H
#define MAP_H
#include <irrlicht.h>
using namespace irr;
using namespace video;
using namespace core;
using namespace gui;
//Định ngĩa class.
class Map : public IGUIElement //Class Map kế thừa từ class IGUIElement.
{
private:
	IVideoDriver* driver;
	ITexture* imgMap;
	ITexture* imgCharacter;
	ITexture* imgNPC;
	position2d<s32> characterPosInMap;//Vị trí nhân vật trên bản đồ.
	position2d<f32> characterPosInWorld;//Vị trí nhân vật trên trục thế giới.
	position2d<s32> characterIconPos;//Vị trí sẽ vẽ Icon của nhân vật trên màn hình.
	dimension2d<f32> WorldSize;//Kích thước thế giới.
	dimension2d<s32> mapSize;//Kích thước map.
	dimension2d<s32> imgNPCSize;//Kích thước icon NPC.
	dimension2d<s32> imgCharacterSize;//Kích thước icon Character.
	dimension2d<s32> screenSize;//Kích thước màn hình(hay cửa sổ).
	rect<s32> miniMapRect;
	bool isExpand;//Mở rộng?.
public:
	//Khởi tạo đối tượng.
	Map(const rect<s32>& rect,IGUIEnvironment* env,IGUIElement* parent);
	//Hủy đối tượng.
	~Map();
	//Đặt các giá trị mới.

	void setMapTexture(c8 *fileName);//Đặt Texture cho map.
	void setCharacterTexture(c8 *fileName);//Đặt Texture cho nhân vật.
	void setNPCTexture(c8 *fileName);//Đặt Texture cho NPC.
	void setCharPosition(vector3df CharPos);//Vị trí nhân vật trên trục thế giới.
	void setWorldSize(f32 Width,f32 Height);//Kích thước thế giới.
	void Calc();//Tính toán.
	void setExpand(bool IsExpand);//Đặt mini map ở chế độ mở rộng hoặc thu hẹp.
	bool getIsExpand();//Trả về tình trạng(thu hẹp, mở rộng) của map.
	//Vẽ.
	void draw();
}; 
#endif




