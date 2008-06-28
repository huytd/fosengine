#ifndef _DEFINE_SGF_PHYSIC_
#define _DEFINE_SGF_PHYSIC_

#include "CoreComponent.h"
#include <irrlicht.h>
#include "StandardEvents.h"
#include <list>
#include <map>
#include "Event.h"
//---SOLID specific----
#include <SOLID.h>
#define ShapeType DT_ShapeHandle
#define WorldType DT_SceneHandle
#define BodyType DT_ObjectHandle
//----------------------

class sgfPhysicBody;
class sgfPhysicShape;
class EngineCallback;

struct SCollisionEvent
{
	sgfPhysicBody* body1;
	sgfPhysicBody* body2;
	const DT_CollData* data;
};

struct SClassPair
{
	inline SClassPair(int id0,int id1)
	{
		if(id0<=id1)
		{
			this->id0=id0;
			this->id1=id1;
		}
		else
		{
			this->id0=id1;
			this->id1=id0;
		}
	}
	inline SClassPair(const SClassPair& other)
	{
		id0=other.id0;
		id1=other.id1;
	}
	bool operator==(const SClassPair& other) const
	{
		return((id0==other.id0)&&(id1==other.id1));
	}
	bool operator < (const SClassPair& other) const
	{
		if(id0==other.id0)
		{
			return(id1<other.id1);
		}
		return(id0<other.id0);
	}
	bool operator > (const SClassPair& other) const
	{
		if(id0==other.id0)
		{
			return(id1>other.id1);
		}
		return(id0>other.id0);
	}
	int id0;
	int id1;
};

	enum EResponseType { 
         ERT_NO_RESPONSE,
         ERT_SIMPLE_RESPONSE, 
         ERT_WITNESSED_RESPONSE,
         ERT_DEPTH_RESPONSE
     };

class CollisionEvent;
class sgfPhysicWorld:public sgfCoreComponent
{
friend class EngineCallback;

public:
	sgfPhysicWorld(sgfCore* core);
	~sgfPhysicWorld();

	sgfPhysicShape* createBox(float x, float y, float z);
	sgfPhysicShape* createSphere(float radius);
	sgfPhysicShape* createCylinder(float radius, float height);
	sgfPhysicShape* createCone(float radius, float height);
	/*sgfPhysicShape* createComplexFromMesh(irr::scene::IMesh* mesh);
	sgfPhysicShape* createComplexFromMeshBuffer(irr::scene::IMeshBuffer* mesh);*/

	void addBody(sgfPhysicBody* body);
	void removeBody(sgfPhysicBody* body);
	void sgfPhysicWorld::attachNode(sgfPhysicBody* body,irr::scene::ISceneNode* node);

	int createCollisionClass();
	void setBodyCollisionClass(sgfPhysicBody* body,int Class);

	sgfEvent<SCollisionEvent>* getClassCollisionEvent(int Class,EResponseType resType);
	sgfEvent<SCollisionEvent>* getPairCollisionEvent(int class1,int class2,EResponseType resType);
	void setDefaultCollision(bool value,EResponseType resType);

	void update(SFrameEvent& arg);
	inline WorldType getHandle() const {return ptr;}
	sgfEvent<DT_CollData> defaultCollision;
private:
	bool defaultCollisionEnabled;
	std::list<sgfPtr<sgfPhysicBody>> bodies;
	std::map<int,sgfPtr<CollisionEvent>> classCollisionEvents;
	std::map<SClassPair,sgfPtr<CollisionEvent>> pairCollisionEvents;
	sgfEvent<char> updateEvent;
	WorldType ptr;
	DT_RespTableHandle respTable;
};

class sgfPhysicShape: public sgfObject
{
friend class sgfPhysicWorld;
public:
	virtual ~sgfPhysicShape();
	inline ShapeType getHandle() const {return ptr;}
protected:
	sgfPhysicShape(ShapeType innerPtr);
	ShapeType ptr;
};

class sgfPhysicBody:public sgfObject
{
friend class EngineCallback;
friend class sgfPhysicWorld;
public:
	sgfPhysicBody(sgfPhysicShape* shape);
	~sgfPhysicBody();

	void setRotation(const irr::core::vector3df& rotation);
	irr::core::vector3df getRotation() const;
	void setPosition(const irr::core::vector3df& position);
	irr::core::vector3df getPosition() const;
	void setTransform(const irr::core::matrix4& transform);
	const irr::core::matrix4& getTransform()const;
	irr::scene::ISceneNode* getNode() const;

	void sgfPhysicBody::setOffset(const irr::core::matrix4& offset);

	inline BodyType getHandle() const {return ptr;}
	void update(char&);
	void* userData;
private:
	sgfMethodDelegate<sgfPhysicBody,char> updateDelegate;
	mutable irr::core::matrix4 transform;
	irr::core::matrix4 offset;
	BodyType ptr;
	sgfPtr<sgfPhysicShape> shape;
	irr::scene::ISceneNode* node;
};

#endif