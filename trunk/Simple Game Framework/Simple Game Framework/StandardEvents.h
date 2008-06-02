#ifndef _STANDARD_SIGNALS_H_
#define _STANDARD_SIGNALS_H_

/// \brief Paramter type for frame event
struct SFrameEvent
{
	SFrameEvent(unsigned int clock=0, float deltaTime=0.0f)
		:clock(clock),deltaTime(deltaTime)
	{
	}
	unsigned int clock;
	float deltaTime;
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

	float deltaTime;

	SEntityEvent(ESType type=EST_Update,float deltaTime=0.0f)
		:type(type), deltaTime(deltaTime)
	{}
};

#endif