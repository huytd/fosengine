#ifndef _THIRD_PERSON_CAMERA_H_
#define _THIRD_PERSON_CAMERA_H_

#include "SGF.h"
#include <irrlicht.h>
using namespace irr;
using namespace scene;
using namespace core;
class ThirdPersonCamera: public sgfEntity //Class ThirdPersonCamera kế thừa từ class sgfEntity.
{
public:
	ThirdPersonCamera(ISceneNode* targetNode);
private:
	float Zoom; 
    s32 Rotation;
	bool rotating;
	bool Mouse[1];//Trái 0, Phải 1.
	gui::ICursorControl* cursor;
	position2d<s32> mousePos;
	position2d<s32> curMousePos;
	vector3df camPos;//Vị trí đặt camera.
	vector3df target;//Điểm camera nhìn tới.
	vector3df rotation;
	f32 RotateStartX, RotateStartY, curentRotateStartX, curentRotateStartY;
	ISceneNode* targetNode;//Nhân vật.
	ICameraSceneNode* camera;//Camera.
	ITerrainSceneNode* terrain;
	ISceneNode* node;
	sgfMethodDelegate<ThirdPersonCamera,SMouseEvent> mouseDelegate;//Đăng ký event mouse.
	void onAdd();
	void update(float deltaTime);//Update.
	void onRemove();
	void onMouse(SMouseEvent& args);
};
#endif
