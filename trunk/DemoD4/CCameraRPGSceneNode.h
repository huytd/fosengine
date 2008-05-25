#ifdef IN_USE_RPG_CAMERA

// Xterm-In'Hate
#ifndef __C_CAMERA_RPG_SCENE_NODE_H_INCLUDED__
#define __C_CAMERA_RPG_SCENE_NODE_H_INCLUDED__

#include <irrlicht.h>
#include "CCameraSceneNode.h"
#include <sstream>
#include <iostream>
//! TODO : lock non moving fps
//! TODO : mouse control can rotate player and camera at higher speed

namespace irr
{
namespace scene
{

class CCameraRPGSceneNode : public CCameraSceneNode
{
public:
    //! constructor
    //!   player : scene node of player
    //!   mgr : irrlicht scene manager
    //!   id : id of camera
    //!   device : irrlicht device
    //!   selector : 3D world collision triangle selector
    CCameraRPGSceneNode(
        ISceneNode * player,
        ISceneManager * mgr,
        s32 const &  id,
        IrrlichtDevice * device,
        ITriangleSelector * selector
    );

   //! configure the camera from a file
    //!   configfilename : full path name of xml config file that contains camera settings
   virtual void configure(
      core::stringc const & configfilename
   );
   
    //! destructor
    virtual ~CCameraRPGSceneNode();

    //! camera event receiver
   virtual bool OnEvent(SEvent event);

    //! camera animation
    virtual void OnAnimate( u32 timeMs );

    //! camera is in first person view
    bool is_first_person() const;

    //! lock camera in first person view
    void first_person( bool const & lock );

protected:

   //! subclasses should call after setting values in configure()
   void enforce_invariants();
   
    //! player move interface (for physic API)
    virtual void player_physic_animator(
        size_t const & elapsed_time,
        irr::f32 const & player_delta_move,
        irr::f32 const & player_move_direction
    );

    //! default player move (NO PHYSIC API)
    virtual void default_player_physic_animator(
        irr::f32 const & player_delta_move,
        irr::f32 const & player_move_direction
    );
   
    // local copy of inputs
    IrrlichtDevice * _device;
    ITriangleSelector * _selector;

    ////////////////////////////////////////////////////////////////////////////
    // time-based settings
    ////////////////////////////////////////////////////////////////////////////

   class TimerSettings {
   public:
      TimerSettings();
      irr::f32 divider;
   };
   
   TimerSettings _timer_settings;

    ////////////////////////////////////////////////////////////////////////////
    // camera settings
    ////////////////////////////////////////////////////////////////////////////

   class CameraSettings {
   public:
      CameraSettings();
      
      //! inverse freelook activation :
      //!  true means freelook is *activated* when pressing mouse right button.
      //!  false means freelock is *desactivated* when pressing mouse right button.
      bool inverse_freelook;
   
      //! camera/player rotation lock :
      //!  true means player rotation and camera rotation are locked altogether.
      //!  false means camera can turn around player.
      bool rotation_lock;
   
      //! distance between player node center and camera target along Y axis in 3rd person camera
      f32 height_offset;                 // unit
   
      //! minimal distance between camera position and camera target position in 3rd person camera
      f32 min_distance;                  // unit
   
      //! maximal distance between camera position and camera target position in 3rd person camera
      //! IMPORTANT : _camera_max_distance == _camera_min_distance + K * _camera_move_step_distance; K is a user-defined interget
      f32 max_distance;                  // unit
   
      //! step distance when increasing/decreasing camera distance in 3rd person camera
      f32 move_step_distance;            // unit
   
      //! starting distance between camera position and camera target position in 3rd person camera
      f32 default_distance;              // unit
   
      //! move speed of the camera when increasing/decreasing camera distance in 3rd person camera
      f32 default_move_speed;            // unit per ms
   
      //! distance between camera position and player node center in 1st person camera
      f32 first_person_radius;                    // unit
   
      //! minimum elevation angle in 1st person camera
      f32 min_1st_z_rotation;            // degre
   
      //! minimum elevation angle in 3rd person camera
      f32 min_3rd_z_rotation;            // degre
   
      //! maximum elevation angle in 1st person camera
      f32 max_1st_z_rotation;            // degre
   
      //! maximum elevation angle in 3rd person camera
      f32 max_3rd_z_rotation;            // degre
   
      //! starting elevation angle in 3rd person camera
      f32 default_z_rotation;            // degre
   
      //! rotation speed of the camera in azimuth
      //! INPORTANT : _player_default_rotation_speed == _camera_default_y_rotation_speed
      f32 default_y_rotation_speed;      // degre per ms
   
      //!  rotation speed of the camera in elevation
      f32 default_z_rotation_speed;      // degre per ms
   
      //! maximum angle between current and final rotation angle when camera is rotating in azimuth
      f32 current_to_target_y_max_angle;         // degre
   
      //! maximum angle between current and final rotation angle when camera is rotating in elevation
      f32 current_to_target_z_max_angle;         // degre
   
      //! when colliding, camera is moved foward player according this distance
      f32 collision_distance;             // unit
   };
   
   CameraSettings _camera_settings;

    ////////////////////////////////////////////////////////////////////////////
    // player settings
    ////////////////////////////////////////////////////////////////////////////

   class PlayerSettings {
   public:
      PlayerSettings();
   
      //! non moving fps camera
      //!  true means that player cannot move in 1rd person camera
      //!  false means that player can move in 1rd person camera
      bool non_moving_fps;
   
      //! rotation correction of player node : (0,90,0), (0,180,0), (0,270,0)
      core::vector3df offset_rotation;   // degre

      //! move speed of player
      f32 default_move_speed;            // unit per ms
   
      //! rotation speed of player in azimuth
      //! INPORTANT : _player_default_rotation_speed == _camera_default_y_rotation_speed
      f32 default_rotation_speed;        // degre per ms
   
      //! maximum angle between current and final rotation angle when camera is rotating in azimuth
      f32 current_to_target_max_angle;         // degre
   
      //! step distance when player is moving forward or backward
      f32 move_step;                     // unit per key press
   
      //! rotation angle when player is rotating in azimuth using keys (does not concern freeloock)
      f32 turn_step;                     // degre per key press
   
      //! multiplier factor when player is running
      f32 run_multiplier;                     // coef.

      //! makes run control a toggle
      bool run_toggle;
   };
   
   PlayerSettings _player_settings;
   
    ////////////////////////////////////////////////////////////////////////////
    // mouse settings
    ////////////////////////////////////////////////////////////////////////////

   class MouseSettings {
   public:
      MouseSettings();
       //! mouse sensitivity
      f32 sensitivity;

      // locks
      bool lock_first_person_camera;
   
      // inverse mousewheel
      bool inverse_mousewheel;
   };
   
   MouseSettings _mouse_settings;

    // controls (set by event receiver)
   class Controls {
      public:
         Controls();
         union {
            bool array[16];
            struct {
               bool foward;
               bool backward;
               bool turn_left;
               bool turn_right;
               bool strafe_left;
               bool strafe_right;
               bool run;
               bool increase_cam_distance;
               bool decrease_cam_distance;
               bool freelook;
               bool unused[6];
            };
         };
         
            // cursor changes
            irr::f32 change_xy;
            irr::f32 change_z;
   };
   
   Controls _controls, _controls_instant, _old_controls_instant;

   // command keys.  order matches Controls struct   
    EKEY_CODE _commands[16];


    // mode
    bool _camera_fps_mode;
    bool _camera_min_distance_lock;

    // old mode
    bool _old_camera_fps_mode;

    // player status
    core::vector3df _player_current_y_rotation;
    core::vector3df _player_target_y_rotation;
    bool _player_is_moving;
    f32 _player_move_direction;

    // old player status
    bool _old_player_is_moving;

    // camera status
    f32 _camera_current_y_rotation;
    f32 _camera_target_y_rotation;
    f32 _camera_current_z_rotation;
    f32 _camera_target_z_rotation;
    f32 _camera_current_distance;
    f32 _camera_target_distance;

    // cursor position
    core::position2d<f32> _cursor_position;
    core::position2d<f32> _cursor_out_of_freelook_position;

    // time marks
    size_t _last_frame_time;
    size_t _current_frame_time;

    // player material save
    video::E_MATERIAL_TYPE _player_material_type;

    // calculate the new value from the current value, the target value and the delta :
    // || new - current || <= delta
    // || new - target || < || current - target ||
    // reduce speed when || new - target || << step
    static void current_to_target_calculation( f32 & current, f32 const & target, f32 const & step, f32 const & delta );

    // adjust the current value according the target value :
    // || current - target || <= 180
    static void current_to_target_angle_adjust( f32 & current, f32 const & target );

};

} // end namespace
} // end namespace

#endif

// Xterm-In'Hate




#endif