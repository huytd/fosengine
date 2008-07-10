/**
 * \RunOn: SGF(Simple Game Framework) - Irrlicht Engine
 * \Summary: Icon In Minimap
 * \Filename: Icon.cpp
 * \Encoding: UTF-8
 * \CreatedDate: 8:49 2008/06/22
 * \CreatedBy: FOSP Team 
 * \Copyright: FOS Project
 **/

#include "Icon.h"

using namespace irr;
using namespace video;
using namespace scene;
using namespace core;
using namespace gui;
const SColor KeyColor = SColor(255,255,255,255);
Icon::Icon(const rect<s32>& rect, IGUIEnvironment* env, IGUIElement* parent)
:IGUIElement(EGUIET_ELEMENT, env, parent, -1, rect)
{
	driver = Environment->getVideoDriver();
	screenSize = driver->getScreenSize();
}

void Icon::setIconStyle(ITexture* img,dimension2d<s32>* imgIconSize)
{
	this->img = img;
	this->imgIconSize = imgIconSize;
}

void Icon::setData(rect<s32>* minimapRect,bool* isExpand,IAnimatedMeshSceneNode* parentNode,ITerrainSceneNode* terrain,short* miniMapSizeWidth,short* miniMapSizeHeight,short* miniMapPaddingRight,short* miniMapPaddingTop,dimension2d<s32>* mapSize,s32* X1,s32* X2,s32* Y1,s32* Y2,bool* mapVisible)
{
	this->minimapRect = minimapRect;
	this->isExpand = isExpand;
	this->parentNode = parentNode;
	this->terrain = terrain;
	this->miniMapPaddingRight = miniMapPaddingRight;
	this->miniMapPaddingTop = miniMapPaddingTop;
	this->miniMapSizeHeight = miniMapSizeHeight;
	this->miniMapSizeWidth = miniMapSizeWidth;
	this->mapSize = mapSize;
	this->X1 = X1;
	this->X2 = X2;
	this->Y1 = Y1;
	this->Y2 = Y2;
	this->mapVisible = mapVisible;
}
void Icon::draw()
{
	if(!IsVisible)
		return;
	if(*mapVisible)
	{
	calc();
	if (isInside)//Nếu icon đó nằm trong "vùng phủ sóng"
	driver->draw2DImage(img,posInScreen,rect<s32>(0,0,imgIconSize->Width,imgIconSize->Height),0,KeyColor,true);//Vẽ icon
	}
	
}

void Icon::calc()
{
	calcposInMap();//Tính toán vị trí mới của icon trên map
    calcposInScreen();//Tính toán vị trí vẽ icon mới trên màn hình
}

void Icon::calcposInMap()//!Tính toán vị trí mới của icon trên map
{
	terrainPos = position2d<f32>(terrain->getPosition().X,terrain->getPosition().Z); 
	posInWorld = position2d<f32>(parentNode->getPosition().X,parentNode->getPosition().Z); 
	posInTerrain.X = abs(posInWorld.X - terrainPos.X);
    posInTerrain.Y = abs(posInWorld.Y - terrainPos.Y);
	terrainSize.Width = abs((terrain->getTerrainCenter().X - terrainPos.X)*2);
    terrainSize.Height = abs((terrain->getTerrainCenter().Z - terrainPos.Y)*2);
	posInMap.X = (posInTerrain.X * mapSize->Width)/terrainSize.Width;
    posInMap.Y = (posInTerrain.Y * mapSize->Height)/terrainSize.Height;
	isInside = minimapRect->isPointInside(posInMap);//Xét xem cái icon đó có nằm trong "vùng phủ sóng"
}

void Icon::calcposInScreen()//!Tính toán vị trí vẽ icon mới trên màn hình
{
	if(isInside)//Nếu icon đó nằm trong "vùng phủ sóng"
	{
	   if(*isExpand)//Nếu map đang trong tình trạng mở rộng
	   {
          posInScreen.X = screenSize.Width - *miniMapPaddingRight - mapSize->Width + posInMap.X - imgIconSize->Width/2;
		  posInScreen.Y = *miniMapPaddingTop + posInMap.Y - imgIconSize->Height/2;
	   //Xử lý lấn biên
		   if (posInScreen.X < screenSize.Width - *miniMapPaddingRight - mapSize->Width)
		   {
			   posInScreen.X = screenSize.Width - *miniMapPaddingRight - mapSize->Width;
		   }
		   if (posInScreen.X + imgIconSize->Width > screenSize.Width - *miniMapPaddingRight)
		   {
			   posInScreen.X = screenSize.Width - *miniMapPaddingRight - imgIconSize->Width;
		   }
		   if (posInScreen.Y < *miniMapPaddingTop)
		   {
		       posInScreen.Y = *miniMapPaddingTop;
		   }
		   if (posInScreen.Y + imgIconSize->Height > *miniMapPaddingTop + mapSize->Height)
		   {
			   posInScreen.Y = *miniMapPaddingTop + mapSize->Height - imgIconSize->Height;
		   }
	   }
	   else//Nếu map đang trong tình trạng thu gọn
	   {
           posInScreen.X = posInMap.X - *X1 - imgIconSize->Width/2 + screenSize.Width - *miniMapPaddingRight - *miniMapSizeWidth;
           posInScreen.Y = posInMap.Y - *Y1 - imgIconSize->Height/2 + *miniMapPaddingTop;
		   //Giải quyết vụ Icon nhân vật lấn biên.
		   if (posInScreen.X < screenSize.Width - *miniMapPaddingRight - *miniMapSizeWidth)
		   {
			   posInScreen.X = screenSize.Width - *miniMapPaddingRight - *miniMapSizeWidth;
		   }
		   if (posInScreen.X + imgIconSize->Width > screenSize.Width - *miniMapPaddingRight)
		   {
			   posInScreen.X = screenSize.Width - *miniMapPaddingRight - imgIconSize->Width;
		   }
		   if (posInScreen.Y < *miniMapPaddingTop)
		   {
			   posInScreen.Y = *miniMapPaddingTop;
		   }
		   if (posInScreen.Y + imgIconSize->Height > *miniMapPaddingTop + *miniMapSizeHeight)
		   {
			   posInScreen.Y = *miniMapPaddingTop + *miniMapSizeHeight - imgIconSize->Height;
		   }
	   }
	}
}

position2d<s32> Icon::getPosInMap() 
{
	return posInMap;
}