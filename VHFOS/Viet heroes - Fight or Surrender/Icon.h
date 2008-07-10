/**
 * \RunOn: SGF(Simple Game Framework) - Irrlicht Engine
 * \Summary: Icon In Minimap
 * \Filename: Icon.h
 * \Encoding: UTF-8
 * \CreatedDate: 8:49 2008/06/22
 * \CreatedBy: FOSP Team 
 * \Copyright: FOS Project
 **/

#ifndef _ICON_H_
#define _ICON_H_

#include <irrlicht.h>
using namespace irr;
using namespace video;
using namespace scene;
using namespace core;
using namespace gui;
class Icon: public IGUIElement
{
private:
	IVideoDriver* driver;
	short* miniMapSizeWidth;
    short* miniMapSizeHeight;
    short* miniMapPaddingRight;
    short* miniMapPaddingTop;
	s32* X1;
	s32* X2;
	s32* Y1;
	s32* Y2;
	dimension2d<s32> screenSize;
	dimension2d<s32>* mapSize;
	ITexture* img;
	dimension2d<s32>* imgIconSize;
	position2d<s32> posInScreen;
	position2d<s32> posInMap;
    position2d<f32> posInWorld;
    position2d<f32> posInTerrain;
	position2d<f32> terrainPos;
	dimension2d<f32> terrainSize;
	rect<s32>* minimapRect;
	bool* isExpand;
	bool isInside;
	bool* mapVisible;
	IAnimatedMeshSceneNode* parentNode;
	ITerrainSceneNode* terrain;
public:
	Icon(const rect<s32>& rect,IGUIEnvironment* env,IGUIElement* parent);
	void setIconStyle(ITexture* img,dimension2d<s32>* imgIconSize);
	void setData(rect<s32>* minimapRect,bool* isExpand,IAnimatedMeshSceneNode* parentNode,ITerrainSceneNode* terrain,short* miniMapSizeWidth,short* miniMapSizeHeight,short* miniMapPaddingRight,short* miniMapPaddingTop,dimension2d<s32>* mapSize,s32* X1,s32* X2,s32* Y1,s32* Y2,bool* mapVisible);
	position2d<s32> getPosInMap();
	void calc();
	void calcposInMap();
	void calcposInScreen();
    void draw();
};

#endif