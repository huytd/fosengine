/**
 * \RunOn: SGF(Simple Game Framework) - Irrlicht Engine
 * \Summary: Mini Map
 * \Filename: Map.cpp
 * \Encoding: UTF-8
 * \CreatedDate: 9:07 2008/06/22
 * \CreatedBy: FOSP Team 
 * \Copyright: FOS Project
 **/


#include "Map.h"
using namespace irr;
using namespace video;
using namespace core;
using namespace gui;
using namespace scene;

const SColor KeyColor = SColor(255,255,255,255);
Map::Map(const rect<s32>& rect, IGUIEnvironment* env, IGUIElement* parent)
:IGUIElement(EGUIET_ELEMENT, env, parent, -1, rect)
{
	targetIcon = NULL;
	isExpand = false;
	driver = Environment->getVideoDriver();
	screenSize = driver->getScreenSize();
}

Map::~Map()
{
}

void Map::setSize(short miniMapSizeWidth,short miniMapSizeHeight,short miniMapPaddingRight,short miniMapPaddingTop)
{
	this->miniMapSizeWidth = miniMapSizeWidth;
	this->miniMapSizeHeight = miniMapSizeHeight;
	this->miniMapPaddingRight = miniMapPaddingRight;
	this->miniMapPaddingTop = miniMapPaddingTop;
}

void Map::addIcon(Icon* icon,int styleID,IAnimatedMeshSceneNode* node)
{
	 icon->setData(&minimapRect,&isExpand,node,terrain,&miniMapSizeWidth,&miniMapSizeHeight,&miniMapPaddingRight,&miniMapPaddingTop,&mapSize,&X1,&X2,&Y1,&Y2,&IsVisible);
	 icon->setIconStyle(imgIcon[styleID],&imgIconSize[styleID]);
	 icon->drop();
}

void Map::setMapTexture(c8 *fileName)
{
	imgMap = driver->getTexture(fileName);
	mapSize = imgMap->getOriginalSize();
}

void Map::setIcon0Texture(c8 *fileName)
{	
	imgIcon[0] = driver->getTexture(fileName);
	imgIconSize[0] = imgIcon[0]->getOriginalSize();
}

void Map::setIcon1Texture(c8 *fileName)
{	
	imgIcon[1] = driver->getTexture(fileName);
	imgIconSize[1] = imgIcon[1]->getOriginalSize();
}

void Map::setIcon2Texture(c8 *fileName)
{
	imgIcon[2] = driver->getTexture(fileName);
	imgIconSize[2] = imgIcon[2]->getOriginalSize();
}

void Map::setExpand(bool IsExpand)
{
	isExpand = IsExpand;
}

bool Map::getIsExpand()
{
	return isExpand;
}

void Map::setWorld(ITerrainSceneNode* terrain)
{
     this->terrain = terrain;
} 

void Map::calc()
{
    if (isExpand)//Nếu đang trong tình trạng mở rộng
	{
	   mapPosInScreen = position2d<s32>(screenSize.Width - miniMapPaddingRight - mapSize.Width,miniMapPaddingTop);
       minimapRect = rect<s32>(0,0,mapSize.Width,mapSize.Height);
	}
	else
	{
	   X1 = targetIcon->getPosInMap().X - miniMapSizeWidth/2;
	   Y1 = targetIcon->getPosInMap().Y - miniMapSizeHeight/2;
	   X2 = X1 + miniMapSizeWidth;
	   Y2 = Y1 + miniMapSizeHeight;
	 //Xử lý wrap texture
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
      //Set new value
	   minimapRect = rect<s32>(X1,Y1,X2,Y2);
	   mapPosInScreen = position2d<s32>(screenSize.Width - miniMapPaddingRight - miniMapSizeWidth,miniMapPaddingTop);
	}
}

void Map::draw()
{
	if(!IsVisible)
		return; 

	if (targetIcon)//Nếu có cái để mà bám
	{
		calc();
		driver->draw2DImage(imgMap,mapPosInScreen,minimapRect);//Vẽ map

		//! draw the boder
		driver->draw2DImage(borderImage, borderPosition, borderClippingRect,0, SColor(255,255,255,255), true);
	}
}

void Map::removeIcon(Icon* icon)
{
	if (icon == targetIcon)
        targetIcon = NULL;
	icon->remove();
}

void Map::setTarget(Icon* targetIcon)
{
	 this->targetIcon = targetIcon;
}

void Map::setMapBorder(c8 *fileName)
{
	borderImage = driver->getTexture(fileName);

	borderOriginRect = core::rect<s32>(0, 
										0, 
										borderImage->getOriginalSize().Width, 
										borderImage->getOriginalSize().Height);
	
	//Tam thoi hard code de test
	borderClippingRect = core::rect<s32>(642, 
										10,
										789, 
										156);

	borderPosition = core::position2d<s32>(645,7);
}
