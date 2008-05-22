#ifndef ANIMSPRITE_CPP
#define ANIMSPRITE_CPP

#include "AnimSprite.h"


AnimSprite::AnimSprite()
{

}

// constructor
// \param idevice -pointer to Irrlicht device
// \param itexture -pointer to texture used, must be power of two size
// \param divX - number of frames along X axis
// \param divY - number of frames along Y axis
AnimSprite::AnimSprite(IrrlichtDevice *idevice, video::ITexture *itexture, s32 divX, s32 divY)
{
	Device = idevice;

	texture = itexture;

	core::dimension2d<s32> spriteSize(texture->getSize());
	spriteSize.Width = spriteSize.Width / divX;
	spriteSize.Height = spriteSize.Height / divY;

	frame.set_used(divX * divY);

	u32 n = 0;
	for(s32 j=0; j<divY; j++)
		for(s32 i=0; i<divX; i++)
		{
			frame[n] = core::rect<s32>(
				i*spriteSize.Width, j*spriteSize.Height, 
				(i+1)*spriteSize.Width, (j+1)*spriteSize.Height);
			n++;
		}

		currentFrame = 0;

		position.UpperLeftCorner = core::position2d<s32>(0,0);
		position.LowerRightCorner = core::position2d<s32>(spriteSize.Width,spriteSize.Height);

		currentAnim = 0;
}

// create object after created by null constructor
// \param idevice -pointer to Irrlicht device
// \param itexture -pointer to texture used, must be power of two size
// \param divX - number of frames along X axis
// \param divY - number of frames along Y axis

void AnimSprite::Create(IrrlichtDevice *idevice, video::ITexture *itexture, s32 divX, s32 divY)
{
	Device = idevice;

	texture = itexture;

	core::dimension2d<s32> spriteSize(texture->getSize());
	spriteSize.Width = spriteSize.Width / divX;
	spriteSize.Height = spriteSize.Height / divY;

	frame.set_used(divX * divY);

	u32 n = 0;
	for(s32 j=0; j<divY; j++)
		for(s32 i=0; i<divX; i++)
		{
			frame[n] = core::rect<s32>(
				i*spriteSize.Width, j*spriteSize.Height, 
				(i+1)*spriteSize.Width, (j+1)*spriteSize.Height);
			n++;
		}

		currentFrame = 0;

		position.UpperLeftCorner = core::position2d<s32>(0,0);
		position.LowerRightCorner = core::position2d<s32>(spriteSize.Width,spriteSize.Height);

		currentAnim = 0;
}


// copy constructor
// \param other -sprite to coppy
AnimSprite::AnimSprite(AnimSprite &other)
{
	texture = other.texture;

	frame.set_used(other.frame.size());
	for(u32 i=0; i<frame.size(); i++) frame[i] = other.frame[i];

	currentFrame = 0;

	position.UpperLeftCorner = core::position2d<s32>(0,0);
	position.LowerRightCorner = other.position.LowerRightCorner - other.position.UpperLeftCorner;

	animation.set_used(other.animation.size());
	for(u32 i=0; i<animation.size(); i++)
	{
		animation[i].start = other.animation[i].start;
		animation[i].end = other.animation[i].end;
		animation[i].speed = other.animation[i].speed;
		animation[i].loop = other.animation[i].loop;
		animation[i].reverse = other.animation[i].reverse;
	}

	currentAnim = 0;

	Device = other.Device;
}

void AnimSprite::Clone(AnimSprite &other)
{
	texture = other.texture;

	frame.set_used(other.frame.size());
	for(u32 i=0; i<frame.size(); i++) frame[i] = other.frame[i];

	currentFrame = 0;

	position.UpperLeftCorner = core::position2d<s32>(0,0);
	position.LowerRightCorner = other.position.LowerRightCorner - other.position.UpperLeftCorner;

	animation.set_used(other.animation.size());
	for(u32 i=0; i<animation.size(); i++)
	{
		animation[i].start = other.animation[i].start;
		animation[i].end = other.animation[i].end;
		animation[i].speed = other.animation[i].speed;
		animation[i].loop = other.animation[i].loop;
		animation[i].reverse = other.animation[i].reverse;
	}

	currentAnim = 0;

	Device = other.Device;
}

// destructor
AnimSprite::~AnimSprite(){}

// draw frame
// \param num -frame id number based on 0 based index
void AnimSprite::draw(u32 num)
{
	Device->getVideoDriver()->draw2DImage(texture, position, frame[num], 0, 0, true);
}

// play sprite
void AnimSprite::play()
{
	if(animation.empty())
	{
		draw(0);
		return;
	}

	u32 time = Device->getTimer()->getTime();

	if(oldTime + animation[currentAnim].speed <= time)
	{
		if(animation[currentAnim].reverse)
		{
			currentFrame--;
			if(currentFrame < animation[currentAnim].start)
			{
				if(animation[currentAnim].loop)currentFrame = animation[currentAnim].end;
				else currentFrame++;
			}
		}
		else
		{
			currentFrame++;

			if(currentFrame > animation[currentAnim].end)
			{
				if(animation[currentAnim].loop)currentFrame = animation[currentAnim].start;
				else currentFrame--;
			}
		}

		oldTime = time;
	}

	draw(currentFrame);
}

// add new animation
// returns id number of animation
// \param start -start frame number
// \param end - end frame number
// \param speed - animation speed in miliseconds
// \param loop - true if animation should loop, false if not
// \param reverse - true if animation should play reversed, false if not
u32 AnimSprite::addAnimation(u32 start, u32 end, u32 speed, bool loop, bool reverse)
{
	Animation tmp;
	tmp.start = start;
	tmp.end = end;
	tmp.speed = speed;
	tmp.loop = loop;
	tmp.reverse = reverse;

	animation.push_back(tmp);

	return animation.size()-1;
}

// remove animation
// \param num -animation id number
void AnimSprite::removeAnimation(u32 num)
{
	animation.erase(num);

	if(currentAnim == num)
	{
		currentAnim = 0;
		currentFrame = 0;
	}

	if(currentAnim > num) currentAnim--;
}

// remove all animations
void AnimSprite::clearAnimations()
{
	animation.clear();
	currentAnim = 0;
	currentFrame = 0;
}

// set animation to play
// \param num -animation id number
void AnimSprite::setAnimation(u32 num)
{
	currentAnim = num;
	if(animation[currentAnim].reverse)
		currentFrame = animation[currentAnim].end;
	else
		currentFrame = animation[currentAnim].start;
	oldTime = Device->getTimer()->getTime();
}

// return id number of animation played
u32 AnimSprite::getCurrentAnimation()
{
	return currentAnim;
}

// set speed of animation
// \param num -animation id number
// \param newspeed -new animation speed in miliseconds
void AnimSprite::setAnimationSpeed(u32 num, u32 newspeed)
{
	if(animation.empty()) return;
	animation[num].speed = newspeed;
}

// return speed of animation
// \param num -animation id number
u32 AnimSprite::getAnimationSpeed(u32 num)
{
	if(animation.empty()) return 0;
	return animation[num].speed;
}

// return id number of animation start frame
// \param num -animation id number
u32 AnimSprite::getAnimationStart(u32 num)
{
	return animation[num].start;
}

// return id number of animation end frame
// \param num -animation id number
u32 AnimSprite::getAnimationEnd(u32 num)
{
	return animation[num].end;
}

// set animation loop state
// \param num -animation id number
// \param loop -true if animation should loop, false if not
void AnimSprite::setAnimationLoopState(u32 num, bool loop)
{
	animation[num].loop = loop;
}

// return true if animation loopes, false if not
// \param num -animation id number
bool AnimSprite::getAnimationLoopState(u32 num)
{
	return animation[num].loop;
}

// set animation reverse state
// \param num -animation id number
// \param reverse -true if animation should play reversed, false if not
void AnimSprite::reverseAnimation(u32 num, bool reverse)
{
	animation[num].reverse = reverse;
}

// return true if animation play reverse, false if not
// \param num -animation id number
bool AnimSprite::isAnimationReversed(u32 num)
{
	return animation[num].reverse;
}

// set position of sprite on screen
// \param x - screen X coordinate of new position
// \param y - screen Y coordinate of new position
void AnimSprite::setPosition(s32 x, s32 y)
{
	position.LowerRightCorner = position.LowerRightCorner - position.UpperLeftCorner + core::position2d<s32>(x,y);
	position.UpperLeftCorner = core::position2d<s32>(x,y);
}

// return position of sprite on screen
core::position2d<s32> AnimSprite::getPosition()
{
	return position.UpperLeftCorner;
}

// return number of frames in sprite
u32 AnimSprite::getFrameCount()
{
	return frame.size();
}

// return number of animations in sprite
u32 AnimSprite::getAnimationCount()
{
	return animation.size();
}

// set new texture sprite should use
// \param newtexture - pointer to texture sprite should use
//                     must be the same size as original texture
void AnimSprite::setTexture(video::ITexture *newtexture)
{
	texture = newtexture;
}

// return pointer to texture sprite is using
video::ITexture* AnimSprite::getTexture()
{
	return texture;
}

// return size of sprite in pixels
core::dimension2d<s32> AnimSprite::getSize()
{
	core::dimension2d<s32> size;

	size.Width = position.LowerRightCorner.X - position.UpperLeftCorner.X;
	size.Height = position.LowerRightCorner.Y - position.UpperLeftCorner.Y;

	return size;
}

#endif
