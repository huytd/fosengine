/**
 * \RunOn: SGF(Simple Game Framework) - Irrlicht Engine
 * \Summary: Third Person Camera
 * \Filename: ThirdPersonCamera.cpp
 * \Encoding: UTF-8
 * \CreatedDate: 2:17:PM 2008/06/15
 * \CreatedBy: FOSP Team - NamVoDang(kiemkhachchilo)
 * \Copyright: FOS Project
 **/

#include "ThirdPersonCamera.h"
using namespace irr;
using namespace scene;
using namespace core;
ThirdPersonCamera::ThirdPersonCamera(ISceneNode* targetNode)
{    
	    this->targetNode = targetNode;
		Zoom = 2;
		nRotationX = 0;
		nRotationY = 0;
		rotating = false;
		Mouse[0] = false;
		Mouse[1] = false;
		rotation = vector3df(0,15,-25);//Default rotation on level enter.
		
		mouseDelegate.addRef();
		mouseDelegate.bind(this,&ThirdPersonCamera::onMouse);
} 

void ThirdPersonCamera::onAdd()
{
}
void ThirdPersonCamera::onLevelStart()
{
		cursor = manager->getCore()->getGraphicDevice()->getCursorControl();
		ISceneManager* smgr = manager->getCore()->getGraphicDevice()->getSceneManager();
		camera = smgr->addCameraSceneNode();//add camera.
		camera->setFarValue(500);
		//terrain.
		terrain = manager->getCore()->globalVars["terrain"].getAs<irr::scene::ITerrainSceneNode*>();
		manager->getCore()->getInputManager()->getMouseEvent()->addDelegate(&mouseDelegate);
		manager->setActive(this,true);
}

void ThirdPersonCamera::update(float deltaTime)
{	
	if (Mouse[1])//Rotation process.
    {
		if (!rotating)
		{
	       mousePos = cursor->getPosition();
	       curMousePos = mousePos;
		   rotating = true;

		}
		else
		{
		   rotation = vector3df(0,15,-25);
		   mousePos = cursor->getPosition();
		   nRotationX += mousePos.X - curMousePos.X;
		   nRotationY += mousePos.Y - curMousePos.Y;
		   rotation.rotateYZBy((nRotationY)*0.25,vector3df(0,0,0));
		   rotation.rotateXZBy((nRotationX)*-0.25,vector3df(0,0,0));//Hệ số của (mousePos.X - curMousePos.X) quyết định tốc độ và chiều xoay.
		   curMousePos = mousePos;
		}
    }
	    target = targetNode->getPosition();
     	camPos = rotation;
		camPos *= Zoom;//Zoom process.
		camPos += target;
	 //Xử lý "độn thổ", "xuyên tường".
        if (camPos.Y < terrain->getHeight(camPos.X,camPos.Z) + 5)
		{
			camPos.Y = terrain->getHeight(camPos.X,camPos.Z) + 5;
		}
	 //Đặt lại camera theo vị trí mới.
		camera->setTarget(target); 
		camera->setPosition(camPos); 
}

void ThirdPersonCamera::onRemove()
{
        manager->getCore()->getInputManager()->getMouseEvent()->removeDelegate(&mouseDelegate);//Gỡ Event ra.
		camera->remove();//đập cái camera.
}

void ThirdPersonCamera::onMouse(SMouseEvent& args)
{
		if(args.mouseEvent==EMIE_MOUSE_WHEEL)//gamer cuộn nút Wheel.
		{
			Zoom = Zoom + args.wheel*0.25;//thay tốc độ Zoom = cách chỉnh hệ số wheel, số càng lớn, zoom càng ác.
			if (Zoom < 0.5)//Nếu gamer nó zoom thấp quá.
				Zoom = 0.5;//Không cho nó zoom xuống nữa.
			if (Zoom > 3.5)//Nếu gamer nó cứ khoái zoom cao.
				Zoom = 3.5;//Cho cao thế thôi, thằng gamer nào có phản ứng, liên hệ anh NamVoDang.
		}
		else if(args.mouseEvent==EMIE_RMOUSE_PRESSED_DOWN)//Phím phải chuột được nhấn.
		{    
			 Mouse[1] = true;
		}
		else if(args.mouseEvent==EMIE_RMOUSE_LEFT_UP)//Phím phải chuột được nhả.
		{
			 Mouse[1] = false;
			 rotating = false;
		}
}

ICameraSceneNode* ThirdPersonCamera::getPointer() const
{
	return camera;
}