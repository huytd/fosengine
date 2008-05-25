// Xterm-In'Hate
#include "CCameraRPGSceneNode.h"

#ifdef IN_USE_RPG_CAMERA

namespace irr
{
namespace scene
{

template< typename T >
void xml_configure(
    io::IXMLReader * xml,
    core::stringw key,
    core::stringw attrib,
    T & value
)
{
    if( key == xml->getNodeName() )
    {
        core::stringw strval = xml->getAttributeValue( attrib.c_str() );
        std::istringstream is( core::stringc( strval.c_str() ).c_str() );
        is >> value;
        std::cout << "xml:" << core::stringc( key.c_str() ).c_str() << " " << core::stringc( attrib.c_str() ).c_str() << "=" << value << std::endl;
    }
}

irr::EKEY_CODE char_to_key( std::string const & key )
{
    if( key == "KEY_KEY_A" )
        return irr::KEY_KEY_A;
    else if( key == "KEY_KEY_B" )
        return irr::KEY_KEY_B;
    else if( key == "KEY_KEY_C" )
        return irr::KEY_KEY_C;
    else if( key == "KEY_KEY_D" )
        return irr::KEY_KEY_D;
    else if( key == "KEY_KEY_E" )
        return irr::KEY_KEY_E;
    else if( key == "KEY_KEY_F" )
        return irr::KEY_KEY_F;
    else if( key == "KEY_KEY_G" )
        return irr::KEY_KEY_G;
    else if( key == "KEY_KEY_H" )
        return irr::KEY_KEY_H;
    else if( key == "KEY_KEY_I" )
        return irr::KEY_KEY_I;
    else if( key == "KEY_KEY_J" )
        return irr::KEY_KEY_J;
    else if( key == "KEY_KEY_K" )
        return irr::KEY_KEY_K;
    else if( key == "KEY_KEY_L" )
        return irr::KEY_KEY_L;
    else if( key == "KEY_KEY_M" )
        return irr::KEY_KEY_M;
    else if( key == "KEY_KEY_N" )
        return irr::KEY_KEY_N;
    else if( key == "KEY_KEY_O" )
        return irr::KEY_KEY_O;
    else if( key == "KEY_KEY_P" )
        return irr::KEY_KEY_P;
    else if( key == "KEY_KEY_Q" )
        return irr::KEY_KEY_Q;
    else if( key == "KEY_KEY_R" )
        return irr::KEY_KEY_R;
    else if( key == "KEY_KEY_S" )
        return irr::KEY_KEY_S;
    else if( key == "KEY_KEY_T" )
        return irr::KEY_KEY_T;
    else if( key == "KEY_KEY_U" )
        return irr::KEY_KEY_U;
    else if( key == "KEY_KEY_V" )
        return irr::KEY_KEY_V;
    else if( key == "KEY_KEY_W" )
        return irr::KEY_KEY_W;
    else if( key == "KEY_KEY_X" )
        return irr::KEY_KEY_X;
    else if( key == "KEY_KEY_Y" )
        return irr::KEY_KEY_Y;
    else if( key == "KEY_KEY_Z" )
        return irr::KEY_KEY_Z;
    else if( key == "KEY_LSHIFT" )
        return irr::KEY_SHIFT;
    else if( key == "KEY_RSHIFT" )
        return irr::KEY_SHIFT;
    else if( key == "KEY_SHIFT" )
        return irr::KEY_SHIFT;
    else if( key == "KEY_LCONTROL" )
        return irr::KEY_CONTROL;
    else if( key == "KEY_RCONTROL" )
        return irr::KEY_CONTROL;
    else if( key == "KEY_CONTROL" )
        return irr::KEY_CONTROL;
    else
        return irr::KEY_ESCAPE;
}

void xml_configure(
    io::IXMLReader * xml,
    core::stringw key,
    core::stringw attrib,
    irr::EKEY_CODE & value
)
{
    if( key == xml->getNodeName() )
    {
        core::stringw strval = xml->getAttributeValue( attrib.c_str() );
        std::istringstream is( core::stringc( strval.c_str() ).c_str() );
        std::string str;
        is >> str;
        std::cout << "xml:" << core::stringc( key.c_str() ).c_str() << " " << core::stringc( attrib.c_str() ).c_str() << "=" << str << std::endl;
        value = char_to_key( str );
    }
}

   //! configure the camera from a file
void CCameraRPGSceneNode::configure(
   core::stringc const & configfilename
)
{
   // read XML config file
    io::IXMLReader * xml = _device->getFileSystem()->createXMLReader( configfilename.c_str() );
    while( xml && xml->read() )
    {
        switch( xml->getNodeType() )
        {
        case io::EXN_TEXT:
            {
                // nop
            }
            break;
        case io::EXN_ELEMENT:
            {
                xml_configure( xml, L"timer_divider",                        L"div",         _timer_settings.divider );
                xml_configure( xml, L"foward",                               L"key",         _commands[0] );
                xml_configure( xml, L"backward",                             L"key",         _commands[1] );
                xml_configure( xml, L"turn_left",                            L"key",         _commands[2] );
                xml_configure( xml, L"turn_right",                           L"key",         _commands[3] );
                xml_configure( xml, L"strafe_left",                          L"key",         _commands[4] );
                xml_configure( xml, L"strafe_right",                         L"key",         _commands[5] );
                xml_configure( xml, L"run",                                  L"key",         _commands[6] );
                xml_configure( xml, L"camera_inverse_freelook",              L"flag",        _camera_settings.inverse_freelook );
                xml_configure( xml, L"camera_rotation_lock",                 L"flag",        _camera_settings.rotation_lock );
                xml_configure( xml, L"camera_height_offset",                 L"distance",    _camera_settings.height_offset );
                xml_configure( xml, L"camera_min_distance",                  L"distance",    _camera_settings.min_distance );
                xml_configure( xml, L"camera_max_distance",                  L"distance",    _camera_settings.max_distance );
                xml_configure( xml, L"camera_move_step_distance",            L"distance",    _camera_settings.move_step_distance );
                xml_configure( xml, L"camera_default_move_speed",            L"speed",       _camera_settings.default_move_speed );
                xml_configure( xml, L"camera_1st_radius",                    L"distance",    _camera_settings.first_person_radius );
                xml_configure( xml, L"camera_min_1st_z_rotation",            L"angle",       _camera_settings.min_1st_z_rotation );
                xml_configure( xml, L"camera_min_3rd_z_rotation",            L"angle",       _camera_settings.min_3rd_z_rotation );
                xml_configure( xml, L"camera_max_1st_z_rotation",            L"angle",       _camera_settings.max_1st_z_rotation );
                xml_configure( xml, L"camera_max_3rd_z_rotation",            L"angle",       _camera_settings.max_3rd_z_rotation );
                xml_configure( xml, L"camera_default_z_rotation",            L"angle",       _camera_settings.default_z_rotation );
                xml_configure( xml, L"camera_default_y_rotation_speed",      L"speed",       _camera_settings.default_y_rotation_speed );
                xml_configure( xml, L"camera_default_z_rotation_speed",      L"speed",       _camera_settings.default_z_rotation_speed );
                xml_configure( xml, L"camera_current_to_target_y_max_angle", L"angle",       _camera_settings.current_to_target_y_max_angle );
                xml_configure( xml, L"camera_current_to_target_z_max_angle", L"angle",       _camera_settings.current_to_target_z_max_angle );
                xml_configure( xml, L"camera_collision_distance",            L"distance",    _camera_settings.collision_distance );
                xml_configure( xml, L"player_non_moving_fps",                L"flag",        _player_settings.non_moving_fps );
                xml_configure( xml, L"player_offset_rotation_x",             L"angle",       _player_settings.offset_rotation.X );
                xml_configure( xml, L"player_offset_rotation_y",             L"angle",       _player_settings.offset_rotation.Y );
                xml_configure( xml, L"player_offset_rotation_z",             L"angle",       _player_settings.offset_rotation.Z );
                xml_configure( xml, L"player_default_move_speed",            L"speed",       _player_settings.default_move_speed );
                xml_configure( xml, L"player_current_to_target_max_angle",   L"angle",       _player_settings.current_to_target_max_angle );
                xml_configure( xml, L"player_move_step",                     L"distance",    _player_settings.move_step );
                xml_configure( xml, L"player_turn_step",                     L"angle",       _player_settings.turn_step );
                xml_configure( xml, L"player_run_multiplier",                L"coef",        _player_settings.run_multiplier );
                xml_configure( xml, L"player_run_toggle",                     L"flag",        _player_settings.run_toggle );
                xml_configure( xml, L"mouse_sensitivity",                    L"distance",    _mouse_settings.sensitivity );
                xml_configure( xml, L"mouse_lock_first_person_camera",       L"flag",        _mouse_settings.lock_first_person_camera );
                xml_configure( xml, L"mouse_inverse_mousewheel",             L"flag",        _mouse_settings.inverse_mousewheel );
            }
            break;
        default:
            {
                // nop
            }
            break;
        }
    }
    if (xml)
        xml->drop();
    /*
    else
        std::cout << "XML NOT FOUND " << configfilename.c_str() << std::endl;
    */
   
   enforce_invariants();
}

void CCameraRPGSceneNode::enforce_invariants()
{
   // enfore invariant
    _camera_settings.default_distance = _camera_settings.min_distance + 2*_camera_settings.move_step_distance;
    _player_settings.default_rotation_speed = _camera_settings.default_y_rotation_speed;
   
   if (getParent()) {
      _player_current_y_rotation = irr::core::vector3df( 0.0f, getParent()->getRotation().Y - _player_settings.offset_rotation.Y, 0.0f );
      _player_target_y_rotation = _player_current_y_rotation;
      _camera_current_y_rotation = _player_current_y_rotation.Y;
      _camera_target_y_rotation = _camera_current_y_rotation;
      _player_material_type = getParent()->getMaterial(0).MaterialType;
   }
   
    _camera_current_z_rotation = _camera_settings.default_z_rotation;
    _camera_target_z_rotation = _camera_settings.default_z_rotation;
    _camera_current_distance =_camera_settings.default_distance;
    _camera_target_distance = _camera_settings.default_distance;
}

CCameraRPGSceneNode::Controls::Controls() : change_xy(0.0f), change_z(0.0f) {
   for (int i=0; i<16; i++) array[i] = false;   
}

CCameraRPGSceneNode::CameraSettings::CameraSettings() :
   inverse_freelook( false ),              // default setting
   rotation_lock( false ),                 // default setting
   height_offset( 20 ),                    // default setting
   min_distance( 40 ),                     // default setting
   max_distance( 350 ),                    // default setting
   move_step_distance( 50 ),               // default setting
   default_distance( min_distance + 2*move_step_distance ), // DO NOT CHANGE!!!
   default_move_speed( 0.25f ),            // default setting
   first_person_radius( 3 ),                        // default setting
   min_1st_z_rotation( -55 ),              // default setting
   min_3rd_z_rotation( -45 ),              // default setting
   max_1st_z_rotation( 45 ),               // default setting
   max_3rd_z_rotation( 25 ),               // default setting
   default_z_rotation( -15 ),              // default setting
   default_y_rotation_speed( 0.2f ),       // default setting
   default_z_rotation_speed( 0.1f ),       // default setting
   current_to_target_y_max_angle( 25 ),    // default setting
   current_to_target_z_max_angle( 15 ),    // default setting
   collision_distance( 25.0f )            // default setting
{}

CCameraRPGSceneNode::PlayerSettings::PlayerSettings() :
   non_moving_fps( false ),                // default setting
   offset_rotation(  irr::core::vector3df( 0, 0, 0 ) ), // user has to adjust this setting
   default_move_speed( 0.2f ),              // default setting
   current_to_target_max_angle( 22.5 ),    // default setting
   move_step( 10 ),                        // default setting
   turn_step( 22.5f ),                     // default setting
   run_multiplier( 2.0f ),                 // default setting
   run_toggle( false )
{}

CCameraRPGSceneNode::MouseSettings::MouseSettings() :
   sensitivity( 256.0f ),                   // user has to adjust this setting
   lock_first_person_camera( false ),
   inverse_mousewheel( false )
{}

CCameraRPGSceneNode::TimerSettings::TimerSettings() :
   divider( 1000.0f )
{}

CCameraRPGSceneNode::CCameraRPGSceneNode(
    ISceneNode * player,
    ISceneManager * mgr,
    s32 const &  id,
    IrrlichtDevice * device,
    ITriangleSelector * selector
) :
ICameraSceneNode( player, mgr, id),
_device( device ),
_selector( selector ),
_camera_fps_mode( false ),
_camera_min_distance_lock( true ),
_old_camera_fps_mode( false ),
//_player_current_y_rotation( irr::core::vector3df( 0.0f, getParent()->getRotation().Y - _player_settings.offset_rotation.Y, 0.0f ) ),
_player_target_y_rotation( _player_current_y_rotation ),
_player_is_moving( false ),
_player_move_direction( 0 ),
_old_player_is_moving( false ),
_camera_current_y_rotation( _player_current_y_rotation.Y ),
_camera_target_y_rotation( _camera_current_y_rotation ),
_cursor_position( irr::core::position2d<irr::f32>( 0.5f, 0.5f ) ),
_cursor_out_of_freelook_position( _cursor_position ),
_last_frame_time( _device->getTimer()->getRealTime() ),
_current_frame_time( _last_frame_time )
{
   #ifdef _DEBUG
   setDebugName("CCameraRPGSceneNode");
   #endif

   for (int i=0; i<16; i++)
      _commands[i] = (irr::EKEY_CODE) 0;
   
   _player_settings.default_rotation_speed = _camera_settings.default_y_rotation_speed; // DO NOT CHANGE!!!
   _camera_current_z_rotation = _camera_settings.default_z_rotation;
   _camera_target_z_rotation = _camera_settings.default_z_rotation;
   _camera_current_distance = _camera_settings.default_distance;
   _camera_target_distance = _camera_settings.default_distance;
   

   enforce_invariants();
}

//! destructor
CCameraRPGSceneNode::~CCameraRPGSceneNode()
{
}

//! postrender
void CCameraRPGSceneNode::OnAnimate( u32 timeMs )
{
    if (SceneManager->getActiveCamera() != this)
        return;

    ////////////////////////////////////////////////////////////////////////////
    // 1.) make a local instant copy of controls

    _controls_instant = _controls;

    ////////////////////////////////////////////////////////////////////////////
    // 2.) reset controls

    _controls.increase_cam_distance = false;
    _controls.decrease_cam_distance = false;
   _controls.change_xy = 0.0f;
   _controls.change_z = 0.0f;

    ////////////////////////////////////////////////////////////////////////////
    // 3.) time based calculation

    // calculate delta rotation and delta move according time elapsed.
    // note that position and rotation are not updated here.

    // elapsed time between current and last frames (sorry, I prefered ignore timeMs argument)
    _last_frame_time = _current_frame_time;
    _current_frame_time = _device->getTimer()->getRealTime();
    irr::f32 elapsed_time = ( _current_frame_time - _last_frame_time ) / _timer_settings.divider;

   // calculate player's deltas
    irr::f32 player_delta_y_angle  = elapsed_time * _player_settings.default_rotation_speed;
   irr::f32 player_delta_move     = elapsed_time * _player_settings.default_move_speed;
   if( _controls_instant.run )
        player_delta_move *= _player_settings.run_multiplier;

    // calculate camera's deltas
    irr::f32 camera_delta_y_angle  = elapsed_time * _camera_settings.default_y_rotation_speed;
    irr::f32 camera_delta_z_angle  =  elapsed_time * _camera_settings.default_z_rotation_speed;
   irr::f32 camera_delta_distance = elapsed_time * _camera_settings.default_move_speed;

    // player rotation correction : angle difference between current/target values must be less than 360?.
    current_to_target_angle_adjust( _player_current_y_rotation.Y, _player_target_y_rotation.Y );

   // time based player rotation
   current_to_target_calculation( _player_current_y_rotation.Y, _player_target_y_rotation.Y, _player_settings.turn_step, player_delta_y_angle );

    // time based camera rotation/distance
   current_to_target_calculation( _camera_current_y_rotation,  _camera_target_y_rotation, _player_settings.turn_step, camera_delta_y_angle );
   current_to_target_calculation( _camera_current_z_rotation,  _camera_target_z_rotation, _camera_settings.current_to_target_z_max_angle, camera_delta_z_angle );
   current_to_target_calculation( _camera_current_distance,    _camera_target_distance,   _camera_settings.move_step_distance, camera_delta_distance );

    ////////////////////////////////////////////////////////////////////////////
    // 4.) camera mode switch detection

    // locking
    if( _mouse_settings.lock_first_person_camera )
    {
        _camera_current_distance = 0;
        _camera_target_distance = 0;
    }

    // 1st / 3rd person camera mode switching
    _camera_fps_mode = ( _camera_current_distance == 0 );

    ////////////////////////////////////////////////////////////////////////////
    // 5.) GUI cursor control switch detection

   // do not show cursor in first person camera
    _device->getCursorControl()->setVisible( ! _controls_instant.freelook );

    ////////////////////////////////////////////////////////////////////////////
    // 6.) process controls that modify camera distance
   
    // camera distance increase
   if( ! _mouse_settings.lock_first_person_camera && _controls_instant.increase_cam_distance && ! _controls_instant.decrease_cam_distance )
   {
       if( _camera_target_distance == 0 )
         _camera_target_distance = _camera_settings.min_distance;
      else if( _camera_target_distance >= _camera_settings.min_distance )
         _camera_target_distance += _camera_settings.move_step_distance;
   }
    // camera distance decrease
   else if( ! _mouse_settings.lock_first_person_camera && ! _controls_instant.increase_cam_distance && _controls_instant.decrease_cam_distance )
   {
        if( _camera_target_distance > _camera_settings.min_distance )
        {
            _camera_target_distance -= _camera_settings.move_step_distance;
        }
       else if( _camera_target_distance == _camera_settings.min_distance )
       {
           // prevent 1st to 3rd camera unwanted transition
            if(  _camera_min_distance_lock && _camera_target_distance >= _camera_current_distance )
            {
                // current distance reach min distance, so unlock
               _camera_min_distance_lock = false;
            }
            else if( _camera_min_distance_lock )
            {
                // no change until current distance reach min distance
                _camera_target_distance = _camera_settings.min_distance;
                _camera_min_distance_lock = true;
            }
            else
            {
                // current distance reached min distance, so decrease current distance until 0
                _camera_target_distance = 0;
                _camera_min_distance_lock = true;
            }
       }
   }

    ////////////////////////////////////////////////////////////////////////////
    // 7.) control player invisibility during first/third person camera transition

    if( _camera_target_distance == 0 && _camera_current_distance == 0 )
        // player visible in 1st person camera
        getParent()->getMaterial(0).MaterialType = _player_material_type;
    else if( _camera_target_distance == 0 && _camera_current_distance > 0 && _camera_current_distance < _camera_settings.min_distance )
        // player transparency during 1st-3rd camera transition
        getParent()->getMaterial(0).MaterialType = irr::video::EMT_TRANSPARENT_ADD_COLOR;
    else
        // player visible in 3rd person camera
        getParent()->getMaterial(0).MaterialType = _player_material_type;

    ////////////////////////////////////////////////////////////////////////////
    // 8.) process 1st from/to 3rd camera changes

   if( _camera_fps_mode )
   // 1st person camera
   {
        // when entering 1st person camera
       if( ! _old_camera_fps_mode )
        {
            // player rotation must equal camera rotation
            _player_target_y_rotation.Y = _camera_current_y_rotation;

            // force 1st person camera min/max rotation
            if( _camera_target_z_rotation < _camera_settings.min_1st_z_rotation )
                _camera_target_z_rotation = _camera_settings.min_1st_z_rotation;
            else if( _camera_target_z_rotation > _camera_settings.max_1st_z_rotation )
                _camera_target_z_rotation = _camera_settings.max_1st_z_rotation;

            // update old status
            _old_camera_fps_mode = true;
        }
   }
    else
    // 3rd person camera
    {
        // when quiting fps
       if( _old_camera_fps_mode )
        {
            // force 3rd person camera min/max rotation
            if( _camera_target_z_rotation < _camera_settings.min_3rd_z_rotation )
                _camera_target_z_rotation = _camera_settings.min_3rd_z_rotation;
            else if( _camera_target_z_rotation > _camera_settings.max_3rd_z_rotation )
                _camera_target_z_rotation = _camera_settings.max_3rd_z_rotation;

            // update old status
            _old_camera_fps_mode = false;
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // 9.) process controls that modify player properties

    // reset action (not really necessary...)
   _player_move_direction  = 0.0f;
   _player_is_moving       = false;

   // turn keys interpreted as strafe keys when freelooking
   if ( _controls_instant.freelook ) {
      if (_controls_instant.turn_left)
         _controls_instant.strafe_left = true;
      if (_controls_instant.turn_right)
         _controls_instant.strafe_right = true;
   }
   
   // throw away some absurd combinations
   if ( _controls_instant.foward && _controls_instant.backward )
      _controls_instant.foward = _controls_instant.backward = false;
   if ( _controls_instant.strafe_left && _controls_instant.strafe_right )
      _controls_instant.strafe_left = _controls_instant.strafe_right = false;

   // player move with control correction
   if( _camera_fps_mode && _player_settings.non_moving_fps )
   {
       // do not move in first person camera
      _player_move_direction = 0.0f;
      _player_is_moving = false;
   }
   else
   {
      int offset = 0;
      if ( _controls_instant.foward )
         offset |= 0x02;  // 0010
      else if ( _controls_instant.backward )
         offset |= 0x03;  // 0011
      if ( _controls_instant.strafe_left )
         offset |= 0x08;  // 1000
      else if ( _controls_instant.strafe_right )
         offset |= 0x0C;  // 1100

      static float player_move_directions[16] = {
         0.0f, 0.0f,    // 0000, 0001 = unused
         0.0f, 180.0f,    // 0010, 0011 = foward, backward
         0.0f, 0.0f,    // 0100, 0101 = unused
         0.0f, 0.0f,    // 0110, 0111 = unused
         -90.0f, 0.0f,    // 1000, 1001 = strafe_left, unused
         -45.0f, -135.0f,    // 1010, 1011 = (foward, backward) + strafe_left
         90.0f, 0.0f,    // 1100, 1101 = strafe_right, unused
         45.0f, 135.0f,    // 1110, 1111 = (foward, backward) + strafe_right
      };         
      
      _player_move_direction = player_move_directions[offset];

      _player_is_moving = (offset > 0);
      
      if (_player_is_moving)
         offset |= 0x10;
      
   }

   // player and cam rotation
   _cursor_position = _device->getCursorControl()->getRelativePosition();
   if( _controls_instant.freelook )
   {
        // when entering freelook...
       if( ! _old_controls_instant.freelook )
       {
           // store cursor position
           _cursor_out_of_freelook_position = _cursor_position;

           // reset cursor position
            _device->getCursorControl()->setPosition( 0.5f, 0.5f );

            // update old status
            _old_controls_instant.freelook = true;
       }
       else // ignore player/cam rotation when entering !
       {
            // cursor changes
            _controls_instant.change_xy = ( _cursor_position.X - 0.5f );
         _controls_instant.change_z = ( _cursor_position.Y - 0.5f );
         
            // reset cursor position
            _device->getCursorControl()->setPosition( 0.5f, 0.5f );
      }
   } else {
      // when quiting freelook...
       if( _old_controls_instant.freelook )
       {
            // restore cursor position
            _device->getCursorControl()->setPosition( _cursor_out_of_freelook_position );

            // update old status
            _old_controls_instant.freelook = false;
       }
   }
         
   if (_controls_instant.change_xy || _controls_instant.change_z) {      
      _controls_instant.change_xy *= _mouse_settings.sensitivity;
      _controls_instant.change_z *= _mouse_settings.sensitivity;
   
      // camera rotation
      _camera_target_y_rotation += _controls_instant.change_xy;
      _camera_target_z_rotation -= _controls_instant.change_z;

      // limit target angle // current angle
      if( _camera_target_y_rotation > ( _camera_current_y_rotation + _camera_settings.current_to_target_y_max_angle ) )
         _camera_target_y_rotation = _camera_settings.current_to_target_y_max_angle + _camera_current_y_rotation;
      else if( _camera_target_y_rotation < ( _camera_current_y_rotation - _camera_settings.current_to_target_y_max_angle ) )
         _camera_target_y_rotation = _camera_current_y_rotation - _camera_settings.current_to_target_y_max_angle;

      // limit target angle // current angle
      if( _camera_target_z_rotation > ( _camera_current_z_rotation + _camera_settings.current_to_target_z_max_angle ) )
         _camera_target_z_rotation = _camera_settings.current_to_target_z_max_angle + _camera_current_z_rotation;
      else if( _camera_target_z_rotation < ( _camera_current_z_rotation - _camera_settings.current_to_target_z_max_angle ) )
         _camera_target_z_rotation = _camera_current_z_rotation - _camera_settings.current_to_target_z_max_angle;

      // limit target angle // min/max angle
      if( _camera_fps_mode )
      {
         // first person camera bounds
         if( _camera_target_z_rotation < _camera_settings.min_1st_z_rotation )
            _camera_target_z_rotation = _camera_settings.min_1st_z_rotation;
         else if( _camera_target_z_rotation > _camera_settings.max_1st_z_rotation )
            _camera_target_z_rotation = _camera_settings.max_1st_z_rotation;
      }
      else
      {
         // third person camera bounds
         if( _camera_target_z_rotation < _camera_settings.min_3rd_z_rotation )
            _camera_target_z_rotation = _camera_settings.min_3rd_z_rotation;
         else if( _camera_target_z_rotation > _camera_settings.max_3rd_z_rotation )
            _camera_target_z_rotation = _camera_settings.max_3rd_z_rotation;
      }

      // lock player and camera rotation when : player is moving or first person camera mode
      if( _player_is_moving )
      {
         // lock player orientation on cam orientation
         _player_target_y_rotation.Y = _camera_target_y_rotation;
      }
      else
      {
         // lock player orientation on cam orientation
         if( _camera_fps_mode || _camera_settings.rotation_lock )
             _player_target_y_rotation.Y = _camera_target_y_rotation;
      }
      _old_player_is_moving = _player_is_moving;
      
   } else {

       // player rotation is controlled by turn keys
        if( _controls_instant.turn_left && ! _controls_instant.turn_right )
        {
            _player_target_y_rotation.Y = _player_current_y_rotation.Y - _player_settings.turn_step;
            _camera_target_y_rotation = _camera_current_y_rotation - _player_settings.turn_step;

            // limit target angle // current
            if( _player_target_y_rotation.Y > ( _player_current_y_rotation.Y + _player_settings.current_to_target_max_angle ) )
                _player_target_y_rotation.Y = _player_settings.current_to_target_max_angle + _player_current_y_rotation.Y;
            else if( _player_target_y_rotation.Y < ( _player_current_y_rotation.Y - _player_settings.current_to_target_max_angle ) )
                _player_target_y_rotation.Y = _player_current_y_rotation.Y - _player_settings.current_to_target_max_angle;
            if( _camera_target_y_rotation > ( _camera_current_y_rotation + _player_settings.current_to_target_max_angle ) )
                _camera_target_y_rotation = _player_settings.current_to_target_max_angle + _camera_current_y_rotation;
            else if( _camera_target_y_rotation < ( _camera_current_y_rotation - _player_settings.current_to_target_max_angle ) )
                _camera_target_y_rotation = _camera_current_y_rotation - _player_settings.current_to_target_max_angle;
        }
        else if( ! _controls_instant.turn_left && _controls_instant.turn_right )
        {
            _player_target_y_rotation.Y = _player_current_y_rotation.Y + _player_settings.turn_step;
            _camera_target_y_rotation = _camera_current_y_rotation + _player_settings.turn_step;

            // limit target angle // current
            if( _player_target_y_rotation.Y > ( _player_current_y_rotation.Y + _player_settings.current_to_target_max_angle ) )
                _player_target_y_rotation.Y = _player_settings.current_to_target_max_angle + _player_current_y_rotation.Y;
            else if( _player_target_y_rotation.Y < ( _player_current_y_rotation.Y - _player_settings.current_to_target_max_angle ) )
                _player_target_y_rotation.Y = _player_current_y_rotation.Y - _player_settings.current_to_target_max_angle;
            if( _camera_target_y_rotation > ( _camera_current_y_rotation + _player_settings.current_to_target_max_angle ) )
                _camera_target_y_rotation = _player_settings.current_to_target_max_angle + _camera_current_y_rotation;
            else if( _camera_target_y_rotation < ( _camera_current_y_rotation - _player_settings.current_to_target_max_angle ) )
                _camera_target_y_rotation = _camera_current_y_rotation - _player_settings.current_to_target_max_angle;
        }

        // player rotation is locked on camera rotation when starting moving
        if( _player_is_moving )
        {
            if( ! _old_player_is_moving )
                _player_target_y_rotation.Y = _camera_target_y_rotation;
            else
                _camera_target_y_rotation = _player_target_y_rotation.Y;
        }
      _old_player_is_moving = _player_is_moving;
   }

    ////////////////////////////////////////////////////////////////////////////
    // 10.) update player position and rotation

   // calculate player rotation
   getParent()->setRotation( _player_current_y_rotation + _player_settings.offset_rotation );

    // if finally, player is not moving, reset time based player move distance
    if( ! _player_is_moving )
        player_delta_move = 0;

    // calculate player position

   //! call player phisic interface
    player_physic_animator( ( _current_frame_time - _last_frame_time ), player_delta_move, _player_move_direction );


   // the old trick... in order to smooth render
   getParent()->updateAbsolutePosition();

    ////////////////////////////////////////////////////////////////////////////
    // 11.) update camera position and rotation

    // calculate camera position and target position
    if( _camera_fps_mode )
    // 1st person cam
    {
        // calculate camera position
        {
            //! camera is placed at _camera_settings.height_offset distance along oY axis from player center
            //! camera is placed at _camera_settings.first_person_radius distance, along player view axis, from oY axis that pass by player center
            //!
            //!     Y axis in the absolute world
            //!     |
            //!     |
            //!     |
            //!     |
            //!     O-------->view axis in the XoZ absolute world plane
            //!
            //!
            //!      .----(distance=_camera_settings.first_person_radius)-----X    <= camera relative pos
            //!      |
            //!      |
            //!      |
            //!(distance=_camera_settings.height_offset)
            //!      |
            //!      |
            //!      |
            //!      O    <= player center
            //!
            //! So, camera position rotates with view axis
            //! Why ? Because, my own eyes are not centered on my head articulation !!!!
            //! This behavior can be supressed by reseting _camera_settings.first_person_radius value (=0).
            irr::core::vector3df delta_position = irr::core::vector3df( _camera_settings.first_person_radius, 0.0f, 0.0f );
            irr::core::matrix4 transformation;
            transformation.setRotationDegrees( irr::core::vector3df( 0.0f, 0.0f, _camera_current_z_rotation ) );
            transformation.transformVect( delta_position );
            transformation.setRotationDegrees( irr::core::vector3df( 0.0f, _camera_current_y_rotation - _player_current_y_rotation.Y - _player_settings.offset_rotation.Y, 0.0f ) );
            transformation.transformVect( delta_position );
            setPosition( delta_position + irr::core::vector3df( 0.0f, _camera_settings.height_offset, 0.0f ) );
        }
        // calculate camera target
        {
            // target in absolute world
            irr::core::vector3df delta_position = irr::core::vector3df( _camera_settings.first_person_radius + 100.0f, 0.0f, 0.0f );
            irr::core::matrix4 transformation;
            transformation.setRotationDegrees( irr::core::vector3df( 0.0f, 0.0f, _camera_current_z_rotation ) );
            transformation.transformVect( delta_position );
            transformation.setRotationDegrees( irr::core::vector3df( 0.0f, _camera_current_y_rotation, 0.0f ) );
            transformation.transformVect( delta_position );
            // target correction in absolute world according camera position
            irr::core::vector3df delta_position2 = irr::core::vector3df( _camera_settings.first_person_radius, 0.0f, 0.0f );
            irr::core::matrix4 transformation2;
            transformation2.setRotationDegrees( irr::core::vector3df( 0.0f, 0.0f, _camera_current_z_rotation ) );
            transformation2.transformVect( delta_position2 );
            transformation2.setRotationDegrees( irr::core::vector3df( 0.0f, _camera_current_y_rotation, 0.0f ) );
            transformation2.transformVect( delta_position2 );
            //
            Target = getParent()->getAbsolutePosition() + delta_position + delta_position2 + irr::core::vector3df( 0.0f, _camera_settings.height_offset, 0.0f );
        }
    }
    else
    // 3rd party cam
    {
        // calculate camera target
        {
            irr::core::vector3df delta_position = irr::core::vector3df( _camera_settings.first_person_radius, 0.0f, 0.0f );
            irr::core::matrix4 transformation;
            transformation.setRotationDegrees( irr::core::vector3df( 0.0f, 0.0f, _camera_current_z_rotation ) );
            transformation.transformVect( delta_position );
            transformation.setRotationDegrees( irr::core::vector3df( 0.0f, _camera_current_y_rotation, 0.0f ) );
            transformation.transformVect( delta_position );
            Target = getParent()->getAbsolutePosition() + delta_position + irr::core::vector3df( 0.0f, _camera_settings.height_offset, 0.0f );
        }
        // calculate camera position
        {
            // camera position in relative world
            irr::core::vector3df delta_position = irr::core::vector3df( _camera_current_distance, 0.0f, 0.0f );
            irr::core::matrix4 transformation;
            transformation.setRotationDegrees( irr::core::vector3df( 0.0f, 0.0f, -_camera_current_z_rotation ) );
            transformation.transformVect( delta_position );
            transformation.setRotationDegrees( irr::core::vector3df( 0.0f, _camera_current_y_rotation - _player_current_y_rotation.Y - _player_settings.offset_rotation.Y + 180.0f, 0.0f ) );
            transformation.transformVect( delta_position );
            // position correction in relative world according target position
            irr::core::vector3df delta_position2 = irr::core::vector3df( _camera_settings.first_person_radius, 0.0f, 0.0f );
            irr::core::matrix4 transformation2;
            transformation2.setRotationDegrees( irr::core::vector3df( 0.0f, 0.0f, _camera_current_z_rotation ) );
            transformation2.transformVect( delta_position2 );
            transformation2.setRotationDegrees( irr::core::vector3df( 0.0f, _camera_current_y_rotation - _player_current_y_rotation.Y - _player_settings.offset_rotation.Y, 0.0f ) );
            transformation2.transformVect( delta_position2 );
            // camera position in absolute world
            irr::core::vector3df delta_position3 = irr::core::vector3df( _camera_settings.collision_distance + _camera_current_distance, 0.0f, 0.0f );
            irr::core::matrix4 transformation3;
            transformation3.setRotationDegrees( irr::core::vector3df( 0.0f, 0.0f, -_camera_current_z_rotation ) );
            transformation3.transformVect( delta_position3 );
            transformation3.setRotationDegrees( irr::core::vector3df( 0.0f, _camera_current_y_rotation + 180.0f, 0.0f ) );
            transformation3.transformVect( delta_position3 );
            // collision ray from target to camera position in absolute world
            irr::core::line3d<irr::f32> line;
            line.start = Target;
            line.end = Target + delta_position3;
            irr::core::vector3df intersection;
            irr::core::triangle3df tri;
         // TODO: this is where code needs to go to check the camera against the pmap, also
            if ( _selector && SceneManager->getSceneCollisionManager()->getCollisionPoint( line, _selector, intersection, tri) )
            {
                // adjust current camera distance
                _camera_current_distance = static_cast<f32>( ( Target - intersection ).getLength() ) - _camera_settings.collision_distance;
                setPosition( delta_position.normalize() * _camera_current_distance + delta_position2 + irr::core::vector3df( 0.0f, _camera_settings.height_offset, 0.0f ) );
            }
            else {
            irr::core::vector3df pos = delta_position + delta_position2 + irr::core::vector3df( 0.0f, _camera_settings.height_offset, 0.0f );
                setPosition( pos );
         }
        }
    }

//  19 Nov 2006.  Avoid the virtual method call.
//  setRotation( irr::core::vector3df( 0.0f, _camera_current_y_rotation, 0.0f ) );
//  irr::ISceneNode::setRotation( irr::core::vector3df( 0.0f, _camera_current_y_rotation, 0.0f ) );
   RelativeRotation = irr::core::vector3df( 0.0f, _camera_current_y_rotation, 0.0f );

    // the old trick... in order to smooth render
   updateAbsolutePosition();

    // update children
   core::list<ISceneNode*>::Iterator it = Children.begin();
    for (; it != Children.end(); ++it)
        (*it)->OnAnimate(timeMs);
}

//! Camera RPG receives mouse and key events.
bool CCameraRPGSceneNode::OnEvent(irr::SEvent event)
{
   if (!InputReceiverEnabled)
      return false;

    bool status = false;
    if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT )
    {
        switch( event.MouseInput.Event )
        {
        case irr::EMIE_RMOUSE_PRESSED_DOWN :
            {
                _controls.freelook = true;
                status = true;
            }
            break;
        case irr::EMIE_RMOUSE_LEFT_UP  :
            {
                _controls.freelook = false;
                status = true;
            }
            break;
        case irr::EMIE_MOUSE_WHEEL :
            {
            if (_mouse_settings.inverse_mousewheel) {
               _controls.decrease_cam_distance = event.MouseInput.Wheel < 0.0f;
               _controls.increase_cam_distance = event.MouseInput.Wheel > 0.0f;
            } else {
               _controls.decrease_cam_distance = event.MouseInput.Wheel > 0.0f;
               _controls.increase_cam_distance = event.MouseInput.Wheel < 0.0f;
            }
                status = true;
            }
            break;
        default:
            {
            
            }
            break;
        }
    }
    else if ( event.EventType == irr::EET_KEY_INPUT_EVENT )
    {
        // handle other keys
      for (int i=0; i<16; i++)
         if( event.KeyInput.Key == _commands[i] ) {
            _controls.array[i] = event.KeyInput.PressedDown;
            status = true;
            break;
         }
    }
    return status;
}

void CCameraRPGSceneNode::current_to_target_calculation(
    irr::f32 & current,
    irr::f32 const & target,
    irr::f32 const & step,
    irr::f32 const & delta )
{
    irr::f32 smooth_delta = delta;
    if( current < target )
    {
        if( (target - current) < (step / 4) )
            smooth_delta /= 2;
        if( (target - current) < (step / 8) )
            smooth_delta /= 2;
        if( (target - current) < (step / 16) )
            smooth_delta /= 2;
        if( (current + smooth_delta) < target )
         if (smooth_delta == 0.0f)
            current = target;
         else
            current += smooth_delta;
        else
            current = target;
    }
    else if( target < current )
    {
        if( current - target < step / 4 )
            smooth_delta /= 2;
        if( current - target < step / 8 )
            smooth_delta /= 2;
        if( current - target < step / 16 )
            smooth_delta /= 2;
        if( target < current - smooth_delta )
         if (smooth_delta == 0.0f)
            current = target;
         else
            current -= smooth_delta;
        else
            current = target;
    }
}

void CCameraRPGSceneNode::current_to_target_angle_adjust( irr::f32 & current, irr::f32 const & target )
{
    if( current < target )
    {
        while( current + 360.0f < target )
            current += 360.0f;
        if( current + 180.0f < target )
            current += 360.0f;
    }
    else if( target < current )
    {
        while( current - 360.0f > target )
            current -= 360.0f;
        if( current - 180.0f > target )
            current -= 360.0f;
    }
}

void CCameraRPGSceneNode::player_physic_animator(
size_t const & elapsed_time,
irr::f32 const & player_delta_move,
irr::f32 const & player_move_direction
)
{
    default_player_physic_animator( player_delta_move, player_move_direction );
}

void CCameraRPGSceneNode::default_player_physic_animator(
    irr::f32 const & player_delta_move,
    irr::f32 const & player_move_direction
)
{
    //! default behavior (NO PHYSICS API)
    irr::core::vector3df delta_position =  irr::core::vector3df( player_delta_move, 0.0f, 0.0f );
    irr::core::matrix4 transformation;
    transformation.setRotationDegrees( irr::core::vector3df( 0.0f, player_move_direction + _player_target_y_rotation.Y, 0.0f ) );
    transformation.transformVect( delta_position );
    getParent()->setPosition( getParent()->getPosition() + delta_position );
}

bool CCameraRPGSceneNode::is_first_person() const
{
    return _camera_current_distance == 0;
}

void CCameraRPGSceneNode::first_person( bool const & lock )
{
    _mouse_settings.lock_first_person_camera = lock;
}

} // end namespace
} // end namespace

// Xterm-In'Hate 



#endif