#include "Map.h"
const short int miniMapSizeWidth = 150;
const short int miniMapSizeHeight = 180;
const SColor KeyColor = SColor(255,255,255,255);
//Khởi tạo đối tượng.
Map::Map(const rect<s32>& rect, IGUIEnvironment* env, IGUIElement* parent)
:IGUIElement(EGUIET_ELEMENT, env, parent, -1, rect)
{
	isExpand = false;//Đặt map trong trạng thái bình thường.
	driver = Environment->getVideoDriver();
	screenSize = driver->getScreenSize();
}
//Hủy đối tượng.
Map::~Map()
{
}
//Đặt Texture cho map.
void Map::setMapTexture(c8 *fileName)
{
	imgMap = driver->getTexture(fileName);
	mapSize = imgMap->getOriginalSize();
}
//Đặt Texture cho NPC.
void Map::setNPCTexture(c8 *fileName)
{	
	imgNPC = driver->getTexture(fileName);
	imgNPCSize = imgNPC->getOriginalSize();
}
//Đặt Texture cho Nhân vật chính.
void Map::setCharacterTexture(c8 *fileName)
{
	imgCharacter = driver->getTexture(fileName);
	imgCharacterSize = imgCharacter->getOriginalSize();
}

void Map::setCharPosition(vector3df CharPos)
{
	characterPosInWorld.X = CharPos.X;
	characterPosInWorld.Y = CharPos.Z;
}

void Map::setWorldSize(f32 Width,f32 Height)
{
	WorldSize.Width = Width;
	WorldSize.Height = Height;
}

void Map::setExpand(bool IsExpand)
{
	isExpand = IsExpand;
}

bool Map::getIsExpand()
{
	return isExpand;
}

void Map::Calc()
{
	int X,Y;
	characterPosInMap.X = (characterPosInWorld.X*mapSize.Width) / WorldSize.Width;//Vị trí nhân vật trên bản đồ.
	characterPosInMap.Y = (characterPosInWorld.Y*mapSize.Height) / WorldSize.Height;
	if (isExpand)
	{
		X = screenSize.Width - mapSize.Width + characterPosInMap.X - imgCharacterSize.Width/2;
		Y = characterPosInMap.Y - imgCharacterSize.Width/2;
		//Điều chỉnh vụ Icon nhân vật lấn biên.
		if (X < screenSize.Width - mapSize.Width)
		{
			X = screenSize.Width - mapSize.Width;
		}
		if (X + imgCharacterSize.Width > screenSize.Width)
		{
			X = screenSize.Width - imgCharacterSize.Width;
		}
		if (Y < 0)
		{
			Y = 0;
		}
		if (Y + imgCharacterSize.Height > mapSize.Height)
		{
			Y = mapSize.Height - imgCharacterSize.Height;
		}
		characterIconPos = position2d<s32>(X,Y);
	}
	else
	{
		//Xác định phần Map sẽ vẽ lên màn hình.
		int X1,Y1,X2,Y2;
		X1 = characterPosInMap.X - miniMapSizeWidth/2;
		Y1 = characterPosInMap.Y - miniMapSizeHeight/2;
		X2 = X1 + miniMapSizeWidth;
		Y2 = Y1 + miniMapSizeHeight;
		//Điều chỉnh vụ wrap texture.
		if (X1<0)
		{
			X1 = 0;
			X2 = miniMapSizeWidth;
		}
		if (X2>mapSize.Width)
		{
			X1 = mapSize.Width - miniMapSizeWidth;
			X2 = mapSize.Width;
		}
		if (Y1<0)
		{
			Y1 = 0;
			Y2 = miniMapSizeHeight;
		}
		if (Y2>mapSize.Height)
		{
			Y1 = mapSize.Height - miniMapSizeHeight;
			Y2 = mapSize.Height;
		}
		miniMapRect = rect<s32>(X1,Y1,X2,Y2);
		//Vị trí vẽ Icon nhân vật.
		X = characterPosInMap.X - X1 - imgCharacterSize.Width/2 + screenSize.Width - miniMapSizeWidth;
		Y = characterPosInMap.Y - Y1 - imgCharacterSize.Height/2;
		//Giải quyết vụ Icon nhân vật lấn biên.
		if (X < screenSize.Width - miniMapSizeWidth)
		{
			X = screenSize.Width - miniMapSizeWidth;
		}
		if (X + imgCharacterSize.Width > screenSize.Width)
		{
			X = screenSize.Width - imgCharacterSize.Width;
		}
		if (Y < 0)
		{
			Y = 0;
		}
		if (Y + imgCharacterSize.Height > miniMapSizeHeight)
		{
			Y = miniMapSizeHeight - imgCharacterSize.Height;
		}
		characterIconPos = position2d<s32>(X,Y);
	}
}


void Map::draw()
{	
	Calc();   
	if (isExpand)//nếu trong tình trạng mở rộng.
	{

		//Draw Map.
		driver->draw2DImage(imgMap,position2d<s32>(screenSize.Width - mapSize.Width,0));

		//Draw Character Icon.
		driver->draw2DImage(imgCharacter,characterIconPos);
	}
	else
	{

		//Draw Map.
		driver->draw2DImage(imgMap,position2d<s32>(screenSize.Width - miniMapSizeWidth,0),miniMapRect);

		//Draw Character Icon.
		driver->draw2DImage(imgCharacter,characterIconPos);
	}
}