#include "Irrlicht.h"

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
