#ifndef _DEFINE_SGF_PHYSIC_
#define _DEFINE_SGF_PHYSIC_

#include "CoreComponent.h"
#include <irrlicht.h>
#include "StandardEvents.h"
#include <map>
#include "Event.h"
//---Newton specific----
#include "Newton.h"

#define ShapeType NewtonCollision
#define WorldType NewtonWorld
#define BodyType NewtonBody
//----------------------

class sgfPhysicBody;
class sgfPhysicShape;
class EngineCallback;

struct SMaterialPair
{
	inline SMaterialPair(int id0,int id1)
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
	inline SMaterialPair(const SMaterialPair& other)
	{
		id0=other.id0;
		id1=other.id1;
	}
	bool operator==(const SMaterialPair& other) const
	{
		return((id0==other.id0)&&(id1==other.id1));
	}
	bool operator < (const SMaterialPair& other) const
	{
		if(id0==other.id0)
		{
			return(id1<other.id1);
		}
		return(id0<other.id0);
	}
	bool operator > (const SMaterialPair& other) const
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

struct SCollisionEvent
{
	sgfPhysicBody* body0;
	sgfPhysicBody* body1;
};
class sgfPhysicWorld:public sgfCoreComponent
{
friend class EngineCallback;
	class colEvent:public sgfEvent<SCollisionEvent>,public sgfObject
	{
	public:
		SCollisionEvent arg;
	};
public:
	sgfPhysicWorld(sgfCore* core);
	~sgfPhysicWorld();

	sgfPhysicShape* createBox(float x, float y, float z, float* offsetMatrix=0);
	sgfPhysicShape* createSphere(float x, float y, float z, float* offsetMatrix=0);
	sgfPhysicShape* createCone(float radius, float height, float* offsetMatrix=0);
	sgfPhysicShape* createCylinder(float radius, float height, float* offsetMatrix=0);
	sgfPhysicShape* createLevelFromMesh(irr::scene::IMesh* mesh,const irr::core::matrix4& transform);
	sgfPhysicShape* createLevelFromMeshBuffer(irr::scene::IMeshBuffer* mesh,const irr::core::matrix4& transform);

	sgfPhysicBody* createBody(sgfPhysicShape* shape);
	void destroyBody(sgfPhysicBody* body);

	irr::core::vector3df getGravity() const;
	void setGravity(irr::core::vector3df& gravity);
	irr::core::aabbox3df getWorldLimit() const;
	void setWorldLimit(irr::core::aabbox3df& limit);

	int getDefaultMaterial() const;
	int createMaterial();

	sgfEvent<SCollisionEvent>* getCollisionEvent(int material0,int material1);

	void update(SFrameEvent& arg);
	inline WorldType* getPtr() const {return ptr;}
private:
	std::map<SMaterialPair,sgfPtr<colEvent>> collisionEvents;
	irr::core::aabbox3df worldLimit;
	irr::core::vector3df gravity;
	WorldType* ptr;
};

class sgfPhysicShape: public sgfObject
{
friend class sgfPhysicWorld;
public:
	virtual ~sgfPhysicShape();
	inline ShapeType* getPtr() const {return ptr;}
protected:
	sgfPhysicShape(sgfPhysicWorld* world,ShapeType* innerPtr);
	sgfPhysicWorld* world;
	ShapeType* ptr;
};

class sgfPhysicBody
{
friend class EngineCallback;
friend class sgfPhysicWorld;
public:
	~sgfPhysicBody();

	void setMass(float mass, float ixx, float iyy, float izz);
	void setMass(float mass, sgfPhysicShape* shape);
	float getMass() const;

	void setRotation(irr::core::vector3df& rotation);
	irr::core::vector3df getRotation() const;
	void setPosition(irr::core::vector3df& position);
	irr::core::vector3df getPosition() const;
	void setTransform(irr::core::matrix4& transform);
	const irr::core::matrix4& getTransform()const;
	void setVelocity(irr::core::vector3df& velocity);
	irr::core::vector3df getVelocity() const;
	void setNode(irr::scene::ISceneNode* node);
	irr::scene::ISceneNode* getNode() const;
	void setOffset(irr::core::matrix4& offset);

	void addGlobalForce(irr::core::vector3df& force);
	void addLocalForce(irr::core::vector3df force);
	void addTorque(irr::core::vector3df& torque);

	void setControlled(bool controlled);
	bool getControlled() const;

	void unfreeze();
	void setMaterial(int material);
	int getMaterial() const;

	inline BodyType* getPtr() const {return ptr;}
	void* userData;
private:
	sgfPhysicBody(sgfPhysicWorld* world, sgfPhysicShape* shape);

	sgfPhysicWorld* world;//the world it belongs to
	bool controlled;
	float mass;
	irr::scene::ISceneNode* node;
	irr::core::vector3df forceToAdd;
	irr::core::vector3df torqueToAdd;
	mutable irr::core::matrix4 transform;
	irr::core::matrix4 offset;
	BodyType* ptr;
};

class sgfPhysicDebugger:public irr::scene::ISceneNode
{
public:
	sgfPhysicDebugger(irr::scene::ISceneManager* smgr,sgfPhysicBody* body);
	static irr::scene::ISceneNode* add(irr::scene::ISceneManager* smgr,sgfPhysicBody* body);
	virtual const irr::core::aabbox3df& getBoundingBox() const;
	virtual void render();
	virtual irr::u32 getMaterialCount() const;
	virtual irr::video::SMaterial& getMaterial(irr::u32 num);
	virtual void OnRegisterSceneNode();
private:
	static void PolyIterator(const NewtonBody* body, int vertexCount, const dFloat* FaceArray, int faceId);
	sgfPhysicBody* body;
	irr::core::aabbox3df box;
	irr::video::SMaterial mat;
	static irr::scene::ISceneManager* smgr;
};
#endif