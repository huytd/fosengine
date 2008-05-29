#ifndef _SGF_LEVEL_H_
#define _SGF_LEVEL_H_

#include "sgfObject.h"

class sgfEntityManager;
/// \brief Base class of a level
class sgfLevel :
	public sgfObject
{
public:
	sgfLevel();
	virtual ~sgfLevel();
public:
	/// \brief Called when a level is entered
	virtual void onEnter(sgfEntityManager* emgr)=0;
	/// \brief Called when a level is exited
	virtual void onExit(sgfEntityManager* emgr)=0;
};

#endif