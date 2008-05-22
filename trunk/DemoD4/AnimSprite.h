/*-----------------------------------------------------------------------------*
| headerfile AnimSprite.h                                                      |
|                                                                              |
| version 1.1                                                                  |
| date: (04.09.2007)                                                           |
|                                                                              |
| author:  Michal Švantner                                                     |
|                                                                              |
| for Irrlicht engine                                                          |
| 2d animated sprite class                                                     |
*-----------------------------------------------------------------------------*/

#ifndef ANIMSPRITE_H
#define ANIMSPRITE_H

#include <irrlicht.h>
using namespace irr;

class AnimSprite
{
	struct Animation
	{
		s32 start;
		s32 end;
		u32 speed;
		bool loop;
		bool reverse;
	};

	video::ITexture *texture;
	core::array< core::rect<s32> > frame;
	s32 currentFrame;
	core::rect<s32> position;

	core::array<Animation> animation;
	u32 currentAnim;

	u32 oldTime;

	IrrlichtDevice *Device;

public:
	//Null constructor
	AnimSprite();

	// constructor
	// \param idevice -pointer to Irrlicht device
	// \param itexture -pointer to texture used, must be power of two size
	// \param divX - number of frames along X axis
	// \param divY - number of frames along Y axis
	AnimSprite(IrrlichtDevice *idevice, video::ITexture *itexture, s32 divX = 1, s32 divY = 1)
		;

	// create object after created by null constructor
	// \param idevice -pointer to Irrlicht device
	// \param itexture -pointer to texture used, must be power of two size
	// \param divX - number of frames along X axis
	// \param divY - number of frames along Y axis

	virtual void Create(IrrlichtDevice *idevice, video::ITexture *itexture, s32 divX = 1, s32 divY = 1)
		;

	// copy constructor
	// \param other -sprite to coppy
	AnimSprite(AnimSprite &other)
		;
	virtual void Clone(AnimSprite &other)
		;

	// destructor
	~AnimSprite();

	// draw frame
	// \param num -frame id number based on 0 based index
	virtual void draw(u32 num = 0)
		;

	// play sprite
	virtual void play()
		;

	// add new animation
	// returns id number of animation
	// \param start -start frame number
	// \param end - end frame number
	// \param speed - animation speed in miliseconds
	// \param loop - true if animation should loop, false if not
	// \param reverse - true if animation should play reversed, false if not
	virtual u32 addAnimation(u32 start, u32 end, u32 speed, bool loop = false, bool reverse = false)
		;

	// remove animation
	// \param num -animation id number
	virtual void removeAnimation(u32 num)
		;

	// remove all animations
	virtual void clearAnimations()
		;

	// set animation to play
	// \param num -animation id number
	virtual void setAnimation(u32 num)
		;

	// return id number of animation played
	virtual u32 getCurrentAnimation()
		;

	// set speed of animation
	// \param num -animation id number
	// \param newspeed -new animation speed in miliseconds
	virtual void setAnimationSpeed(u32 num, u32 newspeed)
		;

	// return speed of animation
	// \param num -animation id number
	virtual u32 getAnimationSpeed(u32 num)
		;

	// return id number of animation start frame
	// \param num -animation id number
	virtual u32 getAnimationStart(u32 num)
		;

	// return id number of animation end frame
	// \param num -animation id number
	virtual u32 getAnimationEnd(u32 num)
		;

	// set animation loop state
	// \param num -animation id number
	// \param loop -true if animation should loop, false if not
	virtual void setAnimationLoopState(u32 num, bool loop)
		;

	// return true if animation loopes, false if not
	// \param num -animation id number
	virtual bool getAnimationLoopState(u32 num)
		;

	// set animation reverse state
	// \param num -animation id number
	// \param reverse -true if animation should play reversed, false if not
	virtual void reverseAnimation(u32 num, bool reverse)
		;

	// return true if animation play reverse, false if not
	// \param num -animation id number
	virtual bool isAnimationReversed(u32 num)
		;

	// set position of sprite on screen
	// \param x - screen X coordinate of new position
	// \param y - screen Y coordinate of new position
	virtual void setPosition(s32 x, s32 y)
		;

	// return position of sprite on screen
	virtual core::position2d<s32> getPosition()
		;

	// return number of frames in sprite
	virtual u32 getFrameCount()
		;

	// return number of animations in sprite
	virtual u32 getAnimationCount()
		;

	// set new texture sprite should use
	// \param newtexture - pointer to texture sprite should use
	//                     must be the same size as original texture
	virtual void setTexture(video::ITexture *newtexture)
		;

	// return pointer to texture sprite is using
	virtual video::ITexture* getTexture()
		;

	// return size of sprite in pixels
	virtual core::dimension2d<s32> getSize()
		;
};

#endif
