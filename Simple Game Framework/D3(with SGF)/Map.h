/**
 * \RunOn: SGF(Simple Game Framework) - Irrlicht Engine
 * \Summary: Mini Map
 * \Filename: Map.h
 * \Encoding: UTF-8
 * \CreatedDate: 9:07 2008/06/22
 * \CreatedBy: FOSP Team 
 * \Copyright: FOS Project
 **/

#ifndef _MAP_H_
#define _MAP_H_

#include <irrlicht.h>
#include "Icon.h"
using namespace irr;
using namespace video;
using namespace core;
using namespace gui;
using namespace scene;
class Map : public IGUIElement
{
private:
	IVideoDriver* driver;
	ITexture* imgMap;
    ITexture* imgIcon[3];
	
	ITexture* borderImage;
	position2d<s32> borderPosition;
    rect<s32> borderRect;
	rect<s32> borderOriginRect;
	rect<s32> borderClippingRect;
	
    dimension2d<s32> imgIconSize[3];
	dimension2d<f32> worldSize;
	dimension2d<s32> mapSize;
	dimension2d<s32> screenSize;
	position2d<s32> mapPosInScreen;
	rect<s32> minimapRect;
	s32 X1,X2,Y1,Y2;
	short miniMapSizeWidth;
    short miniMapSizeHeight;
    short miniMapPaddingRight;
    short miniMapPaddingTop;
	bool isExpand;
	Icon* targetIcon;
	vector3df terrainPos;
    ITerrainSceneNode* terrain;
public:
	Map(const rect<s32>& rect,IGUIEnvironment* env,IGUIElement* parent);
	~Map();
	void setSize(short miniMapSizeWidth,short miniMapSizeHeight,short miniMapPaddingRight,short miniMapPaddingTop);
	void addIcon(Icon* icon,int styleID,IAnimatedMeshSceneNode* node);//Thêm 1 Icon mới
	void removeIcon(Icon* icon);//Gỡ 1 icon
	void setMapTexture(c8 *fileName);
	void setIcon0Texture(c8 *fileName);//Icon texture kiểu 0
	void setIcon1Texture(c8 *fileName);//Icon texture kiểu 1
	void setIcon2Texture(c8 *fileName);//Icon texture kiểu 2
	void setMapBorder(c8 *fileName);//Set image border for the map
	void setTarget(Icon* targetIcon);
	void setExpand(bool IsExpand);
	void setWorld(ITerrainSceneNode* terrain);
	bool getIsExpand();
	void calc();
	void draw();
}; 
#endif




