#include "SimpleCameraManager.h"

// class constructor
SimpleCameraManager::SimpleCameraManager(irr::scene::ISceneNode* targetNode, irr::scene::ISceneManager* smgr,
										 irr::f32 height , irr::f32 leash , irr::f32 speed, irr::s32 id )
{
	m_speed = speed;
	m_height = height;
	m_leash = leash;

	//old camera height
	oldCameraHeight = 0;

	//create a camera
	lastTargetPos = targetNode->getPosition();
	m_cam = smgr->addCameraSceneNode(0, lastTargetPos + irr::core::vector3df(0, 10, 30), lastTargetPos, id );

	m_cam->setUpVector(core::vector3df(0.0f, 1000.0f, 0.0f));
	m_cam->setFarValue(25000.0);

	m_pTargetNode = targetNode;
	m_pTargetNode->grab(); //make sure the node wont dissappear on us
}

// class destructor
SimpleCameraManager::~SimpleCameraManager()
{
	m_pTargetNode->drop(); //make sure we let go of the node resource
}

void SimpleCameraManager::Update() {
	if(!m_cam || !m_pTargetNode) return;

	irr::core::vector3df currTargetPos = m_pTargetNode->getPosition();
	
	//printf("\nX=%f,y=%f, z=%f", currTargetPos.X, currTargetPos.Y, currTargetPos.Z);
	//if(currTargetPos.Y == 0) currTargetPos.Y = lastTargetPos.Y;
	irr::core::vector3df temp;

	//if too far away, move camera closer
	irr::core::vector3df camToTarg = currTargetPos - m_cam->getPosition() ;

	//leash is only in the X-Z plane, so only count distance using X and Z
	irr::core::vector2df xzDist( camToTarg.X, camToTarg.Z );

	if(xzDist.getLength() > m_leash) 
	{ //need to move closer
		camToTarg = irr::core::vector3df( xzDist.X, 0, xzDist.Y);
		camToTarg = camToTarg.normalize() * m_speed;

		//set X-Z position
		m_cam->setPosition( m_cam->getPosition() + camToTarg ); //move closer

		//set Y position
		irr::f32 h_tolerance = (float)m_height / 10.0f;  //the ammount of leway given to how close we need to be to the right height
		irr::f32 h_delta = m_cam->getPosition().Y - currTargetPos.Y + m_height; //distance from prefered height position

		if( m_cam->getPosition().Y < ( currTargetPos.Y + m_height) - h_tolerance) 
		{
			temp = ( irr::core::vector3df( m_cam->getPosition().X, m_cam->getPosition().Y + (int)h_delta/20, m_cam->getPosition().Z) );
			//printf("\ncam.Y=%f,hdelta=%f", m_cam->getPosition().Y + h_delta/2,h_delta/2);
			m_cam->setPosition(temp);

		}
		else if( m_cam->getPosition().Y > ( currTargetPos.Y + m_height) + h_tolerance ) 
		{
			temp = ( irr::core::vector3df( m_cam->getPosition().X, m_cam->getPosition().Y - (int)h_delta/20, m_cam->getPosition().Z) );
			//printf("\ncam.Y=%f,hdelta=%f", m_cam->getPosition().Y - h_delta/2,h_delta/2);
			m_cam->setPosition(temp);
		}
	}
	else{
	//printf("\ncam.Y=%f", m_cam->getPosition().Y );
	}
	
	//printf("\ncurrentHeight=%f", temp.Y);
	m_cam->setTarget( core::vector3df( currTargetPos.X , currTargetPos.Y , currTargetPos.Z) ); //look at Target position
	lastTargetPos = currTargetPos;
}