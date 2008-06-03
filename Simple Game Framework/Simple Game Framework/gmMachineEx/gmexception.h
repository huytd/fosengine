#ifndef _GM_EXCEPTION_H_
#define _GM_EXCEPTION_H_

#include <exception>
#include "../GameMonkey/gmMachine.h"
#include <list>

#ifndef _DEBUG
	#define breakThread(A)
#endif

#define gmAssert(exp,msg)\
	if(!(exp))\
	{\
		printf(msg);\
		breakThread(pThread);\
	}

#ifdef _DEBUG
static void breakThread(gmThread* thread)
{
	thread->m_user=-1;;
}

static bool userBreakCallBack(gmThread* thread)
{
	if(thread->m_user==-1)
		return true;
	return false;
}

#endif

#define scriptAssert(exp,msg) if(!(exp)){pThread->GetMachine()->GetLog().LogEntry(msg);return GM_EXCEPTION;}
	
#endif