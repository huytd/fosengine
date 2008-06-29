#include "Physic.h"
#include "sgfPtr.h"

class CollisionEvent:public sgfObject,public sgfEvent<SCollisionEvent>
{
public:
	CollisionEvent()
		:sgfObject(),sgfEvent<SCollisionEvent>()
	{}
	virtual ~CollisionEvent()
	{}
};
//callbacks
class EngineCallback
{
public:
	static DT_Bool CollisionResponse(void *client_data, void *client_object1, void *client_object2, const DT_CollData *coll_data)
	{
		SCollisionEvent arg;
		arg.body1=(sgfPhysicBody*)client_object1;
		arg.body2=(sgfPhysicBody*)client_object2;
		arg.data=coll_data;
		(reinterpret_cast<CollisionEvent*>(client_data))->fire(arg);
		return DT_CONTINUE;
	}
};

//shape
sgfPhysicShape::~sgfPhysicShape()
{
	DT_DeleteShape(ptr);
}

sgfPhysicShape::sgfPhysicShape(ShapeType innerPtr)
:ptr(innerPtr)
{
}

/*class SOLIDMesh:public sgfPhysicShape
{
public:
	SOLIDMesh(sgfPhysicWorld* world,irr::scene::IMesh* mesh,const irr::core::matrix4& transform)
		:sgfPhysicShape(0)
	{
		
	}
	
	SOLIDMesh(sgfPhysicWorld* world,irr::scene::IMeshBuffer* meshBuffer,const irr::core::matrix4& transform)
		:sgfPhysicShape(world,0)
	{
		ptr=NewtonCreateTreeCollision(world->getHandle(),NULL);
		NewtonTreeCollisionBeginBuild(ptr);
		AddMeshBuffer(meshBuffer,transform);
		NewtonTreeCollisionEndBuild(ptr,1);
	}

	virtual ~SOLIDMesh()
	{
		NewtonReleaseCollision(world->getHandle(),ptr);
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
};*/
//body
sgfPhysicBody::sgfPhysicBody(sgfPhysicShape* shape)
:node(0)
{
	this->shape=shape;
	ptr=DT_CreateObject(this,shape->getHandle());
	updateDelegate.addRef();
	updateDelegate.bind(this,&sgfPhysicBody::update);
}

sgfPhysicBody::~sgfPhysicBody()
{
	DT_DestroyObject(ptr);
}

void sgfPhysicBody::setRotation(const irr::core::vector3df& rotation)
{
	irr::core::quaternion q(rotation*irr::core::DEGTORAD);
	float q2[4];
	q2[0]=q.X;
	q2[1]=q.Y;
	q2[2]=q.Z;
	q2[3]=q.W;
	DT_SetOrientation(ptr,q2);
}

irr::core::vector3df sgfPhysicBody::getRotation() const
{
	return getTransform().getRotationDegrees();
}

void sgfPhysicBody::setPosition(const irr::core::vector3df& position)
{
	DT_SetPosition(ptr,reinterpret_cast<const float*>(&position));
}

void sgfPhysicBody::update(char&)
{
	transform=node->getAbsoluteTransformation();
	transform*=offset;
	setTransform(transform);
}

irr::core::vector3df sgfPhysicBody::getPosition() const
{
	return getTransform().getTranslation();
}

void sgfPhysicBody::setTransform(const irr::core::matrix4& transform)
{
	DT_SetMatrixf(ptr,transform.pointer());
}

const irr::core::matrix4& sgfPhysicBody::getTransform()const
{
	DT_GetMatrixf(ptr,transform.pointer());
	return transform;
}

void sgfPhysicBody::setOffset(const irr::core::matrix4& offset)
{
	this->offset=offset;
}

irr::scene::ISceneNode* sgfPhysicBody::getNode() const
{
	return node;
}

//world
sgfPhysicWorld::sgfPhysicWorld(sgfCore* core)
	:sgfCoreComponent(core)
{
	ptr=DT_CreateScene();
	respTable=DT_CreateRespTable();
}

sgfPhysicWorld::~sgfPhysicWorld()
{
	DT_DestroyScene(ptr);
	DT_DestroyRespTable(respTable);
}

sgfPhysicShape* sgfPhysicWorld::createBox(float x, float y, float z)
{
	return new sgfPhysicShape(DT_NewBox(x,y,z));
}
sgfPhysicShape* sgfPhysicWorld::createSphere(float radius)
{
	return new sgfPhysicShape(DT_NewSphere(radius));
}
sgfPhysicShape* sgfPhysicWorld::createCone(float radius, float height)
{
	return new sgfPhysicShape(DT_NewCone(radius,height));
}
sgfPhysicShape* sgfPhysicWorld::createCylinder(float radius, float height)
{
	return new sgfPhysicShape(DT_NewCylinder(radius,height));
}
/*sgfPhysicShape* sgfPhysicWorld::createLevelFromMesh(irr::scene::IMesh* mesh,const irr::core::matrix4& transform)
{
	return new SOLIDMesh(this,mesh,transform);
}
sgfPhysicShape* sgfPhysicWorld::createLevelFromMeshBuffer(irr::scene::IMeshBuffer* mesh,const irr::core::matrix4& transform)
{
	return new SOLIDMesh(this,mesh,transform);
}*/

void sgfPhysicWorld::addBody(sgfPhysicBody* body)
{
	bodies.push_back(sgfPtr<sgfPhysicBody>(body));
	DT_AddObject(ptr,body->getHandle());
}

void sgfPhysicWorld::removeBody(sgfPhysicBody* body)
{
	sgfPtr<sgfPhysicBody> Body(body);
	bodies.remove(Body);
	DT_RemoveObject(ptr,Body->getHandle());
	attachNode(body,0);
}

void sgfPhysicWorld::attachNode(sgfPhysicBody* body,irr::scene::ISceneNode* node)
{
	if(body->node)
	{
		if(!node)//switch off
			updateEvent.removeDelegate(&body->updateDelegate);
	}
	else
	{
		if(node)//switch on
			updateEvent.addDelegate(&body->updateDelegate);
	}
	body->node=node;
}

void sgfPhysicWorld::update(SFrameEvent& arg)
{
	char a='\0';
	updateEvent(a);
	DT_Test(ptr,respTable);
}

int sgfPhysicWorld::getCollisionClassID(const char* className)
{
	std::string name(className);
	std::map<std::string,DT_ResponseClass>::iterator i=colClass.find(name);
	if(i==colClass.end())//not found
	{
		int newClass=DT_GenResponseClass(respTable);
		colClass[name]=newClass;
		return newClass;
	}
	return i->second;
}

void sgfPhysicWorld::setBodyCollisionClass(sgfPhysicBody* body,int Class)
{
	DT_SetResponseClass(respTable, body->getHandle(), Class);
}

sgfEvent<SCollisionEvent>* sgfPhysicWorld::getClassCollisionEvent(int Class,EResponseType resType)
{
	std::map<int,sgfPtr<CollisionEvent>>::iterator i=classCollisionEvents.find(Class);
	if(i==classCollisionEvents.end())//not found
	{
		sgfPtr<CollisionEvent> ev=new CollisionEvent;
		classCollisionEvents.insert(std::make_pair<int,sgfPtr<CollisionEvent>>(Class,ev));		
		DT_AddClassResponse(respTable,//setup callback
                            Class,
							EngineCallback::CollisionResponse,
                            (DT_ResponseType)resType,
                            ev);		
		return ev;
     
	}
	return i->second;
}
sgfEvent<SCollisionEvent>* sgfPhysicWorld::getPairCollisionEvent(int class1,int class2,EResponseType resType)
{
	SClassPair pair(class1,class2);
	std::map<SClassPair,sgfPtr<CollisionEvent>>::iterator i=pairCollisionEvents.find(pair);
	if(i==pairCollisionEvents.end())//not found
	{
		CollisionEvent* ev=new CollisionEvent;
		pairCollisionEvents.insert(std::make_pair<SClassPair,sgfPtr<CollisionEvent>>(pair,ev));
		DT_AddPairResponse(respTable,
							pair.id0,
							pair.id1, 
                            EngineCallback::CollisionResponse,
                            (DT_ResponseType)resType, 
							ev);
		return ev;
	}
	return i->second;
}

void sgfPhysicWorld::setDefaultCollision(bool value,EResponseType resType)
{
	if(defaultCollisionEnabled)
	{
		if(!value)
			DT_RemoveDefaultResponse(respTable, EngineCallback::CollisionResponse);
	}
	else
	{
		if(value)
			DT_AddDefaultResponse(respTable,
                                EngineCallback::CollisionResponse,
                                (DT_ResponseType)resType,
                                &defaultCollision);
	}
	defaultCollisionEnabled=value;
}