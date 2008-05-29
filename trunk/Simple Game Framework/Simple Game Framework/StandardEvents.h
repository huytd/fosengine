#ifndef _STANDARD_SIGNALS_H_
#define _STANDARD_SIGNALS_H_

/// \brief Paramter type for frame event
struct SFrameEvent
{
	SFrameEvent(unsigned long clock=0, int deltaTime=0)
		:clock(clock),deltaTime(deltaTime)
	{
	}
	unsigned long clock;
	int deltaTime;
};

/// \brief Paramter type for entity event
struct SEntityEvent
{
	enum ESType
	{
	EST_Update=0,
	EST_LevelStart,
	EST_LevelEnd,
	EST_GameEnd,
	EST_Add,
	EST_Remove
	} type;

	int deltaTime;

	SEntityEvent(ESType type=EST_Update,int deltaTime=0)
		:type(type),deltaTime(deltaTime)
	{}
};

#endif