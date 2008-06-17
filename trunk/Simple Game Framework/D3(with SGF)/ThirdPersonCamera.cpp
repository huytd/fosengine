#include "ThirdPersonCamera.h"
using namespace irr;
using namespace scene;
using namespace core;
ThirdPersonCamera::ThirdPersonCamera(ISceneNode* targetNode)
{    
	    this->targetNode = targetNode;
		Zoom = 2;
		rotating = false;
		Mouse[0] = false;
		Mouse[1] = false;
		rotation = vector3df(0,15,-25);//Xoay mặc định khi vào game.
		mouseDelegate.addRef();
		mouseDelegate.bind(this,&ThirdPersonCamera::onMouse);
} 

void ThirdPersonCamera::onAdd()
{
		cursor = manager->getCore()->getGraphicDevice()->getCursorControl();
		ISceneManager* smgr = manager->getCore()->getGraphicDevice()->getSceneManager();
		camera = smgr->addCameraSceneNode();//Thêm 1 camera.
	 //Lấy terrain.
		terrain = manager->getCore()->globalVars["terrain"].getAs<irr::scene::ITerrainSceneNode*>();
		manager->getCore()->getInputManager()->getMouseEvent()->addDelegate(&mouseDelegate);
		manager->setActive(this,true);
}

void ThirdPersonCamera::update(float deltaTime)
{	
	if (Mouse[1])//xử lý xoay.
    {
		if (!rotating)
		{
	       mousePos = cursor->getPosition();
	       curMousePos = mousePos;
		   rotating = true;
		}
		else
		{
		   mousePos = cursor->getPosition();
		   rotation.rotateXZBy((mousePos.X - curMousePos.X)*-0.25,vector3df(0,0,0));
		   //rotation.rotateYZBy((mousePos.Y - curMousePos.Y)*0.25,vector3df(0,0,0));
		   curMousePos = mousePos;
		}
    }
	    target = targetNode->getPosition();
     	camPos = rotation;
		camPos *= Zoom;//Xử lý zoom.
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
		if(args.mouseEvent==EMIE_MOUSE_WHEEL)//gamer cuộn nút Whell.
		{
			Zoom = Zoom + args.wheel*0.25;//thay tốc độ Zoom = cách chỉnh hệ số wheel.
			if (Zoom < 0.7)//Nếu gamer nó zoom thấp quá.
				Zoom = 0.7;//Không cho nó zoom xuống nữa.
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