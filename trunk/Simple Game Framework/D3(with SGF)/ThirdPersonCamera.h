/**
 * \RunOn: SGF(Simple Game Framework) - Irrlicht Engine
 * \Summary: Third Person Camera
 * \Filename: ThirdPersonCamera.h
 * \Encoding: UTF-8
 * \CreatedDate: 2:17:PM 2008/06/15
 * \CreatedBy: FOSP Team - NamVoDang(kiemkhachchilo)
 * \Copyright: FOS Project
 **/

#ifndef _THIRD_PERSON_CAMERA_H_
#define _THIRD_PERSON_CAMERA_H_

#include "SGF.h"
#include <functional>
#include <irrlicht.h>
using namespace irr;
using namespace scene;
using namespace core;
class ThirdPersonCamera: public sgfEntity //Class ThirdPersonCamera kế thừa từ class sgfEntity.
{
public:
	ThirdPersonCamera(ISceneNode* targetNode);
	ICameraSceneNode* getPointer() const;//Trả về con trỏ tới CameraSceneNode đang được dùng bởi class.
private:
	float Zoom;
    s32 Rotation,nRotationX,nRotationY;
	bool rotating;
	bool Mouse[1];//Left 0, Right 1.
	gui::ICursorControl* cursor;
	position2d<s32> mousePos;
	position2d<s32> curMousePos;
	vector3df camPos;//Camera position.
	vector3df target;//Look at.
	vector3df rotation;
	f32 RotateStartX, RotateStartY, curentRotateStartX, curentRotateStartY;
	ISceneNode* targetNode;//Character.
	ICameraSceneNode* camera;//Camera.
	ITerrainSceneNode* terrain;
	sgfMethodDelegate<ThirdPersonCamera,SMouseEvent> mouseDelegate;//Register mouse event.
	void onAdd();
	void onLevelStart();
	void update(float deltaTime);//Update.
	void onRemove();
	void onMouse(SMouseEvent& args);
};
#endif
