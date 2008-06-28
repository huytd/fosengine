#include "Physic.h"
#include "sgfPtr.h"

//callbacks
class EngineCallback
{
public:
	static void NewtonApplyForceAndTorque(const NewtonBody* body)
	{
		sgfPhysicBody* sgfBody=(sgfPhysicBody*)NewtonBodyGetUserData(body);
		sgfBody->forceToAdd+=sgfBody->world->gravity*sgfBody->mass;
		NewtonBodyAddForce(body,reinterpret_cast<float*>(&sgfBody->forceToAdd));
		NewtonBodyAddTorque(body,reinterpret_cast<float*>(&sgfBody->torqueToAdd));
		sgfBody->forceToAdd.set(0,0,0);
		sgfBody->torqueToAdd.set(0,0,0);
	}

	static void NewtonSetTransform(const NewtonBody* body, const dFloat* matrix)
	{
		sgfPhysicBody* sgfBody=(sgfPhysicBody*)NewtonBodyGetUserData(body);
		irr::core::matrix4 m;
		m.setM(matrix);
		m*=sgfBody->offset;
		sgfBody->node->setPosition(m.getTranslation());
		sgfBody->node->setRotation(m.getRotationDegrees());
		
	}

	static void NewtonBodyDestructor(const NewtonBody* body)
	{
		sgfPhysicBody* sgfBody=(sgfPhysicBody*)NewtonBodyGetUserData(body);
		delete sgfBody;
	}
	static int NewtonContactBegin(const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1)
	{
		sgfPhysicWorld::colEvent* ev=(sgfPhysicWorld::colEvent*)NewtonMaterialGetMaterialPairUserData(material);
		ev->arg.body0=(sgfPhysicBody*)NewtonBodyGetUserData(body0);
		ev->arg.body1=(sgfPhysicBody*)NewtonBodyGetUserData(body1);
		return 1;
	}

	static void NewtonContactEnd(const NewtonMaterial* material)
	{
		sgfPhysicWorld::colEvent* ev=(sgfPhysicWorld::colEvent*)NewtonMaterialGetMaterialPairUserData(material);
		ev->fire(ev->arg);
	}
};

//shape
sgfPhysicShape::~sgfPhysicShape()
{
}

sgfPhysicShape::sgfPhysicShape(sgfPhysicWorld* world,ShapeType* innerPtr)
:world(world),ptr(innerPtr)
{
}

class NewtonConvexCollision:public sgfPhysicShape
{
public:
	NewtonConvexCollision(sgfPhysicWorld* world,NewtonCollision* ptr)
		:sgfPhysicShape(world,ptr)
	{
	}
	virtual ~NewtonConvexCollision()
	{
		NewtonReleaseCollision(world->getPtr(),ptr);
	}
};

class NewtonTreeCollision:public sgfPhysicShape
{
public:
	NewtonTreeCollision(sgfPhysicWorld* world,irr::scene::IMesh* mesh,const irr::core::matrix4& transform)
		:sgfPhysicShape(world,0)
	{
		ptr=NewtonCreateTreeCollision(world->getPtr(),NULL);
		NewtonTreeCollisionBeginBuild(ptr);
		unsigned int count=mesh->getMeshBufferCount();
		for(unsigned int i=0;i<count;++i)
		{
			irr::scene::IMeshBuffer* buffer=mesh->getMeshBuffer(i);
			AddMeshBuffer(buffer,transform);
		}
		NewtonTreeCollisionEndBuild(ptr,1);
	}
	
	NewtonTreeCollision(sgfPhysicWorld* world,irr::scene::IMeshBuffer* meshBuffer,const irr::core::matrix4& transform)
		:sgfPhysicShape(world,0)
	{
		ptr=NewtonCreateTreeCollision(world->getPtr(),NULL);
		NewtonTreeCollisionBeginBuild(ptr);
		AddMeshBuffer(meshBuffer,transform);
		NewtonTreeCollisionEndBuild(ptr,1);
	}

	virtual ~NewtonTreeCollision()
	{
		NewtonReleaseCollision(world->getPtr(),ptr);
	}
private:
	void AddMeshBuffer(irr::scene::IMeshBuffer* buffer,const irr::core::matrix4& transform)
	{
		unsigned int count=buffer->getIndexCount();
		irr::u16* indices=buffer->getIndices();
		void* vertices=buffer->getVertices();
		irr::core::vector3df tri[3];
		irr::video::E_VERTEX_TYPE type=buffer->getVertexType();
		for(unsigned int i=0;i<count;i+=3)
		{
			tri[0]=getVertexPos(vertices,type,indices[i]);
			tri[1]=getVertexPos(vertices,type,indices[i+1]);
			tri[2]=getVertexPos(vertices,type,indices[i+2]);
			transform.transformVect(tri[0]);
			transform.transformVect(tri[1]);
			transform.transformVect(tri[2]);
			NewtonTreeCollisionAddFace(ptr,3,reinterpret_cast<float*>(tri),12,0);
		}
	}
	static irr::core::vector3df getVertexPos(void* vertices,irr::video::E_VERTEX_TYPE vertexType,unsigned int pos)
	{
		switch(vertexType)
		{
		case irr::video::EVT_2TCOORDS:
			return ((irr::video::S3DVertex2TCoords*)vertices)[pos].Pos;
		break;
		case irr::video::EVT_STANDARD:
			return ((irr::video::S3DVertex*)vertices)[pos].Pos;
		break;
		case irr::video::EVT_TANGENTS:
			return ((irr::video::S3DVertexTangents*)vertices)[pos].Pos;
		break;
		default:
			printf("Unknown vertex type");
		break;
		}
		return irr::core::vector3df(0,0,0);
	}
};
//body
sgfPhysicBody::sgfPhysicBody(sgfPhysicWorld* world,sgfPhysicShape* shape)
:world(world),mass(0),node(0)
{
	sgfPtr<sgfPhysicShape> Shape(shape);
	ptr=NewtonCreateBody(world->getPtr(),shape->getPtr());
	NewtonBodySetUserData(ptr,this);
	NewtonBodySetForceAndTorqueCallback(ptr,&EngineCallback::NewtonApplyForceAndTorque);
	NewtonBodySetDestructorCallback(ptr,&EngineCallback::NewtonBodyDestructor);
}

sgfPhysicBody::~sgfPhysicBody()
{
}

void sgfPhysicBody::setMass(float mass, float ixx, float iyy, float izz)
{
	this->mass=mass;
	NewtonBodySetMassMatrix(ptr,mass,ixx,iyy,izz);
}

void sgfPhysicBody::setMass(float mass, sgfPhysicShape* shape)
{
	sgfPtr<sgfPhysicShape> Shape(shape);//reference count
	float inertia[3];
	float center[3];
	NewtonConvexCollisionCalculateInertialMatrix(shape->getPtr(),inertia,center);
	setMass(mass,inertia[0],inertia[1],inertia[2]);
}

float sgfPhysicBody::getMass() const
{
	return mass;
}

void sgfPhysicBody::setRotation(irr::core::vector3df& rotation)
{
	getTransform();
	transform.setRotationDegrees(rotation);
	setTransform(transform);
}

irr::core::vector3df sgfPhysicBody::getRotation() const
{
	return getTransform().getRotationDegrees();
}

void sgfPhysicBody::setPosition(irr::core::vector3df& position)
{
	getTransform();
	transform.setTranslation(position);
	setTransform(transform);
}

irr::core::vector3df sgfPhysicBody::getPosition() const
{
	return getTransform().getTranslation();
}

void sgfPhysicBody::setTransform(irr::core::matrix4& transform)
{
	NewtonBodySetMatrix(ptr,transform.pointer());
}

const irr::core::matrix4& sgfPhysicBody::getTransform()const
{
	NewtonBodyGetMatrix(ptr,transform.pointer());
	return transform;
}

void sgfPhysicBody::setVelocity(irr::core::vector3df& velocity)
{
	NewtonBodySetVelocity(ptr,reinterpret_cast<float*>(&velocity));
}

irr::core::vector3df sgfPhysicBody::getVelocity() const
{
	irr::core::vector3df vec;
	NewtonBodyGetVelocity(ptr,reinterpret_cast<float*>(&vec));
	return vec;
}

void sgfPhysicBody::addGlobalForce(irr::core::vector3df& force)
{
	forceToAdd+=force;
}

void sgfPhysicBody::addLocalForce(irr::core::vector3df force)
{
	irr::core::matrix4 m;
	m.setRotationDegrees(getRotation());
	m.transformVect(force);
	addGlobalForce(force);
}
void sgfPhysicBody::addTorque(irr::core::vector3df& torque)
{
	torqueToAdd+=torque;
}

void sgfPhysicBody::setNode(irr::scene::ISceneNode* node)
{
	this->node=node;
	if(node)
		NewtonBodySetTransformCallback(ptr,&EngineCallback::NewtonSetTransform);
	else
		NewtonBodySetTransformCallback(ptr,NULL);
}
void sgfPhysicBody::setOffset(irr::core::matrix4& offset)
{
	this->offset=offset;
}
irr::scene::ISceneNode* sgfPhysicBody::getNode() const
{
	return node;
}

void sgfPhysicBody::unfreeze()
{
	NewtonWorldUnfreezeBody(world->getPtr(),ptr);
}

void sgfPhysicBody::setMaterial(int material)
{
	NewtonBodySetMaterialGroupID(ptr,material);
}

int sgfPhysicBody::getMaterial() const
{
	return NewtonBodyGetMaterialGroupID(ptr);
}
void sgfPhysicBody::setControlled(bool controlled)
{
	this->controlled=controlled;
}
bool sgfPhysicBody::getControlled() const
{
	return controlled;
}
//world
sgfPhysicWorld::sgfPhysicWorld(sgfCore* core)
	:sgfCoreComponent(core)
{
	ptr=NewtonCreate(0,0);
}

sgfPhysicWorld::~sgfPhysicWorld()
{
	NewtonDestroy((NewtonWorld*)ptr);
}

sgfPhysicShape* sgfPhysicWorld::createBox(float x, float y, float z, float* offsetMatrix)
{
	return new NewtonConvexCollision(this,NewtonCreateBox((NewtonWorld*)ptr,x,y,z,offsetMatrix));
}
sgfPhysicShape* sgfPhysicWorld::createSphere(float x, float y, float z, float* offsetMatrix)
{
	return new NewtonConvexCollision(this,NewtonCreateSphere((NewtonWorld*)ptr,x,y,z,offsetMatrix));
}
sgfPhysicShape* sgfPhysicWorld::createCone(float radius, float height, float* offsetMatrix)
{
	return new NewtonConvexCollision(this,NewtonCreateCone((NewtonWorld*)ptr,radius,height,offsetMatrix));
}
sgfPhysicShape* sgfPhysicWorld::createCylinder(float radius, float height, float* offsetMatrix)
{
	return new NewtonConvexCollision(this,NewtonCreateCylinder((NewtonWorld*)ptr,radius,height,offsetMatrix));
}
sgfPhysicShape* sgfPhysicWorld::createLevelFromMesh(irr::scene::IMesh* mesh,const irr::core::matrix4& transform)
{
	return new NewtonTreeCollision(this,mesh,transform);
}
sgfPhysicShape* sgfPhysicWorld::createLevelFromMeshBuffer(irr::scene::IMeshBuffer* mesh,const irr::core::matrix4& transform)
{
	return new NewtonTreeCollision(this,mesh,transform);
}

sgfPhysicBody* sgfPhysicWorld::createBody(sgfPhysicShape* shape)
{
	return new sgfPhysicBody(this, shape);
}

void sgfPhysicWorld::destroyBody(sgfPhysicBody* body)
{
	NewtonDestroyBody(ptr,body->getPtr());
}

irr::core::vector3df sgfPhysicWorld::getGravity() const
{
	return gravity;
}

void sgfPhysicWorld::setGravity(irr::core::vector3df& gravity)
{
	this->gravity=gravity;
}
irr::core::aabbox3df sgfPhysicWorld::getWorldLimit() const
{
	return worldLimit;
}

int sgfPhysicWorld::getDefaultMaterial() const
{
	return NewtonMaterialGetDefaultGroupID(ptr);
}

int sgfPhysicWorld::createMaterial()
{
	return NewtonMaterialCreateGroupID(ptr);
}

void sgfPhysicWorld::setWorldLimit(irr::core::aabbox3df& limit)
{
	worldLimit=limit;
	NewtonSetWorldSize((NewtonWorld*)ptr,reinterpret_cast<float*>(&limit.MinEdge),reinterpret_cast<float*>(&limit.MaxEdge));
}
sgfEvent<SCollisionEvent>* sgfPhysicWorld::getCollisionEvent(int material0,int material1)
{
	std::map<SMaterialPair,sgfPtr<colEvent>>::iterator i=collisionEvents.find(SMaterialPair(material0,material1));
	if(i==collisionEvents.end())//not found
	{
		i->second=new colEvent;
		NewtonMaterialSetCollisionCallback(ptr,material0,material1,i->second,&EngineCallback::NewtonContactBegin,NULL,&EngineCallback::NewtonContactEnd);
	}
	else
	{
		return i->second;
	}
	return 0;
}
void sgfPhysicWorld::update(SFrameEvent& arg)
{
	NewtonUpdate((NewtonWorld*)ptr,arg.deltaTime);
}

//debugger
sgfPhysicDebugger::sgfPhysicDebugger(irr::scene::ISceneManager* smgr,sgfPhysicBody* body)
:irr::scene::ISceneNode(smgr->getRootSceneNode(),smgr),body(body)
{
	mat.Wireframe=true;
	mat.Lighting=false;
	setAutomaticCulling(irr::scene::EAC_OFF);// this will be slow
}
const irr::core::aabbox3df& sgfPhysicDebugger::getBoundingBox() const
{
	return box;
}
void sgfPhysicDebugger::OnRegisterSceneNode()
{          
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);
    ISceneNode::OnRegisterSceneNode();
}
irr::u32 sgfPhysicDebugger::getMaterialCount() const
{
	return 1;
}
irr::video::SMaterial& sgfPhysicDebugger::getMaterial(irr::u32 num)
{
	return mat;
}
void sgfPhysicDebugger::render()
{
	smgr=SceneManager;//pass the current sceneManager;
	SceneManager->getVideoDriver()->setTransform(irr::video::ETS_WORLD,irr::core::matrix4());
	SceneManager->getVideoDriver()->setMaterial(irr::video::SMaterial());
	NewtonBodyForEachPolygonDo(body->getPtr(),&sgfPhysicDebugger::PolyIterator);
}
void sgfPhysicDebugger::PolyIterator(const NewtonBody* body, int vertexCount, const dFloat* faceArray, int faceId)
{
	irr::core::vector3df p0=irr::core::vector3df(faceArray[0],faceArray[1],faceArray[2]);
	irr::core::vector3df p1,p2;
	irr::core::triangle3df tri;
	tri.pointC=p0;
	for(int i=2;i<vertexCount;i++)
	{
		p1=irr::core::vector3df(faceArray[(i-1) * 3 + 0], faceArray[(i-1) * 3 + 1], faceArray[(i-1) * 3 + 2]);
		p2=irr::core::vector3df(faceArray[i * 3 + 0], faceArray[i * 3 + 1], faceArray[i * 3 + 2]);
		tri.pointA=p1;
		tri.pointB=p2;
		smgr->getVideoDriver()->draw3DTriangle(tri,irr::video::SColor(0,0,0,255));
	}
}

irr::scene::ISceneNode* sgfPhysicDebugger::add(irr::scene::ISceneManager* smgr,sgfPhysicBody* body)
{
	irr::scene::ISceneNode* node=new sgfPhysicDebugger(smgr,body);
	node->drop();
	return node;
}

irr::scene::ISceneManager* sgfPhysicDebugger::smgr;