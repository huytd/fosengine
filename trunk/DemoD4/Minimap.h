/*********************************
Minimap by_Gogolian
*********************************/
#ifndef MINIMAP_H
#define MINIMAP_H

#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

class Minimap
{
public:
 //vbnvbn
 core::stringw Go(ISceneManager* smgr, IVideoDriver* video)
 {
   core::array< scene::ISceneNode * > allC,allD;
   ISceneNode* node;
   float scale = 1;
   core::stringw tmp(L"Minimap [");
   smgr->getSceneNodesFromType(ESNT_CAMERA,allD);
   for(int i = 0; i < allD.size(); i++){
     video->draw2DRectangle(SColor(255,128,0,0), rect<s32>(500.0f+allD[i]->getPosition().X,280.0f+allD[i]->getPosition().Z,500.0f+allD[i]->getPosition().X+5,280.0f+allD[i]->getPosition().Z+5)) ;
   }
   smgr->getSceneNodesFromType(ESNT_CUBE,allC);
   for(int i = 0; i < allC.size(); i++){
     video->draw2DRectangle(SColor(255,0,0,0), rect<s32>(500.0f+allC[i]->getPosition().X,280.0f+allC[i]->getPosition().Z,500.0f+allC[i]->getPosition().X+5,280.0f+allC[i]->getPosition().Z+5)) ;
   tmp += allC[i]->getPosition().X;
   tmp += " ; ";
   tmp += allC[i]->getPosition().Y;
   tmp += " ; ";
   tmp += allC[i]->getPosition().Z;
   tmp += " :: "; 
   }
  return tmp;
 }
 
};
#endif 
/*
#include <irrlicht.h>
#include "minimap.h"
#pragma comment(lib, "Irrlicht.lib");

 using namespace irr;
 using namespace core;
 using namespace scene;
 using namespace video;
 using namespace gui;

IrrlichtDevice* device = 0;

bool keys[KEY_KEY_CODES_COUNT];

class MyEventReceiver : public IEventReceiver {
 public:
  virtual bool OnEvent(const SEvent& event)  {
  if(event.EventType == EET_KEY_INPUT_EVENT){
   keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
   return false;
  }
   return false;
  }
};

int main() {
   
 Minimap map;
   
 MyEventReceiver receiver;

 IrrlichtDevice* device = createDevice( EDT_DIRECT3D9, core::dimension2d<s32>(640, 480),
      16, false, false, false, &receiver);
 IVideoDriver* video = device->getVideoDriver();
 ISceneManager* smgr = device->getSceneManager();
 
 video::ITexture* images = video->getTexture("./map.bmp");
 
 for(int x=0; x<irr::KEY_KEY_CODES_COUNT; x++) keys[x] = false;
 
 ICameraSceneNode* cam = smgr->addCameraSceneNode();
   
 smgr->addCubeSceneNode();
 ISceneNode* cube = smgr->addCubeSceneNode();
 
 cam->setPosition(vector3df(0,0,120.0f));
 
 cube->setPosition(vector3df(0,0.0f,40.0f));
 //cube->setScale(vector3df(2.0f,3.0f,4.0f));

 while(device->run() && device) {
 
  if(keys[KEY_KEY_W]) {
   cube->setPosition(cube->getPosition()+vector3df(0,0,0.1));
   cube->setRotation(cube->getRotation()+vector3df(0,0,0.1));
  }
  if(keys[KEY_KEY_S]) {
   cube->setPosition(cube->getPosition()-vector3df(0,0,0.1));
   cube->setRotation(cube->getRotation()-vector3df(0,0,0.1));
  }
  if(keys[KEY_KEY_A]) {
   cube->setPosition(cube->getPosition()-vector3df(0.1,0,0));
   cube->setRotation(cube->getRotation()-vector3df(0.1,0,0));
  }
  if(keys[KEY_KEY_D]) {
   cube->setPosition(cube->getPosition()+vector3df(0.1,0,0));
   cube->setRotation(cube->getRotation()+vector3df(0.1,0,0));
  }
  core::stringw tmp(L"Minimap [");
  tmp += map.Go(smgr,video);
  tmp += L"]";
  device->setWindowCaption(tmp.c_str());
  cam->setTarget(cube->getPosition());
  video->beginScene(true, true, video::SColor(255,128,128,128));
  smgr->drawAll();
  video->draw2DImage(images, position2d<s32>(400,310));
  map.Go(smgr,video);
  video->endScene();
 }
}
*/
