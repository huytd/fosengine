#include "Irrlicht.h"


/*
 * Init code: Keless Irrlicht Forum
 */

using namespace irr;
class SimpleCameraManager
{
    irr::scene::ISceneNode* m_pTargetNode;
    irr::scene::ICameraSceneNode* m_cam;

    irr::core::vector3df lastTargetPos;
   
    irr::f32 m_height; //distance above object camera tries to float at
    irr::f32 m_leash;  //max distance object can be from camera before it moves
    irr::f32 m_speed;  //rate at which camera moves per Update()
   
   public:
      // class constructor
      SimpleCameraManager(irr::scene::ISceneNode* targetNode, irr::scene::ISceneManager* smgr,
                              irr::f32 height = 10.0f, irr::f32 leash = 20.0f, irr::f32 speed = 0.5f, irr::s32 id = -1);
      // class destructor
      ~SimpleCameraManager();
      
      irr::scene::ICameraSceneNode* getCam() {  return m_cam; }
      
      void Update();
};


/*

void WoWCamera(ICameraSceneNode* cam,ISceneNode* camTarget ,float camPan ,float camTilt,float camZoom)
{
vector3df CPosVector;
vector3df NewCamLocation;
vector3df Target1;

//camPan==0 places camera behind Model "farie" camPan range 0-360
//camTilt inputs should be between -89 and +89

CPosVector.X=cos((camPan+180)*PI/180)*sin((camTilt+90)*PI/180);
CPosVector.Y=cos((camTilt+90)*PI/180);
CPosVector.Z=sin((camPan+180)*PI/180)*sin((camTilt+90)*PI/180);

matrix4 m2;
m2.setRotationDegrees(camTarget->getRotation());
m2.transformVect(CPosVector);

//all i know is that the vector transformation somehow gives a vector which you can multiply by a zoom value

Target1=camTarget->getPosition();
NewCamLocation.X=Target1.X+CPosVector.X*camZoom;
NewCamLocation.Y=Target1.Y+CPosVector.Y*camZoom;
NewCamLocation.Z=Target1.Z+CPosVector.Z*camZoom;

cam->setPosition(NewCamLocation);
cam->setUpVector(vector3df(0,1,0));
cam->setTarget(vector3df(Target1));
cam->updateAbsolutePosition();

//TODO: IF Camera Distance == CLOSE then make model semi transparent
//if(camZoom<10){
//   {
} 

*/