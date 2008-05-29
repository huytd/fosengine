#ifndef _SGF_IRR_LEVEL_H_
#define _SGF_IRR_LEVEL_H_

#include <map>
#include <irrlicht.h>
#include "Level.h"

class sgfEntity;
class sgfEntityManager;

class sgfIrrLevel :
	public sgfLevel
{
	typedef sgfEntity* (*createFunctionPtr)(irr::scene::ISceneNode*);
	struct strCmp 
	{
		bool operator()( const char* s1, const char* s2 ) const 
		{
		  return strcmp( s1, s2 ) < 0;
		}
	};
public:
	sgfIrrLevel(const char* FileName);
	virtual ~sgfIrrLevel(void);
public:
	virtual void onEnter(sgfEntityManager* emgr);
	virtual void onExit(sgfEntityManager* emgr);
	static void _registerClass(const char* className,sgfIrrLevel::createFunctionPtr function);
protected:
	char* fileName;
	static std::map<const char*,createFunctionPtr,strCmp> createFunctions;
};

#define registerClass(className) sgfIrrLevel::_registerClass( #className , className::createFromNode)

#endif