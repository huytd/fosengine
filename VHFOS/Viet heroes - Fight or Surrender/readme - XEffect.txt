This folder is a part of  XEffects shader pack
Author: BlindSide
Url: http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=23638
---------------------------------------------------------------------------------------
UPDATE2: 
 - Fixed a bug where shadows were lagging behind the camera projection. 
 - Now supports setting of a custom scene manager using setActiveSceneManager(). 
 - effectHandler initialisation now takes an additional, fourth parameter of type dimension2d<s32>
   to specify the resolution of the screen post processing filter. If passed a value of (0,0), 
   it just uses the current screen resolution. This parameter is useful if you need to support older 
   cards by specifying a low resolution, or want to create your own fullscreen anti-aliasing routine 
   by rendering to a high resolution buffer then filtering.
 - Fixed the bloom shader so that it will work correctly now on ATI cards.

UPDATE: This new version contains many, many API changes, please consult the documentation as
many functions have different effects from before and not modifying the code to meet the new
standards may have undesired effects even if the code compiles. For example, addNodeToDepthPass
is unrelated to shadowmapping now and is instead used for adding nodes to the post processing
depth buffer depth pass. The original functionality is replaced by using addShadowToNode() with
the ESM_CAST shadow mode.

Check out the new interior shadow mapping demo. This release features a fully
revamped interior and a little bonus, packed 16-bit depth precision
(Using a little algorithm I devised myself with the help of Notepad and VBS scripts).
You may notice there are much less shaders in the shaders folder, this is due to a 
new preprocessing system I am using for the shaders which allows me to save on hundreds 
of lines of code, you are free to use this preprocessor in your own work. There is also 
a new filtering mode utilizing 16 samples (EFT_16PCF), and all of the original lighting 
effects are now available on OpenGL.

This package is released under the Irrlicht Engine license, scroll down for terms and conditions.
As the interface has not changed much, the original docs are included in the doc folder,
please refer to them when you want to make use of this wrapper. Keep in mind however, that
some things have changed, one of these is the "useAccurate" variable when intialising the
effectswrapper class, this has been removed and all shaders can be considered "accurate"
regardless. Also keep in mind that with this release all shadow effects are SM 2.0 minimum,
although the other effects support PS 1.4.

A side note on shadow maps: Shadow maps are very complicated in their nature and require fine,
fiiiine tweaking in order to look good. When integrating into your own project, please keep in
mind that many variables will effect how it looks, or whether it works at all. The MaxShadowDistance
variable is essential, and you most likely 100% will need to modify it when adapting to your own
project. It does not just specify the max distance, but also the scaling at which the depth buffering
occurs. This means that if your objects are very spaced apart, then you must set a high value, else
you will not see anything. If your objects are very close together you must ALSO change it to a very
low value, else you will ALSO not see anything, or may see alot of garbage and artifacts. Other things
to keep in mind is the light cameras' near and far values, FOV and other settings. The shadow map is
almost entirely dependant on the properties of the light camera, so make sure to obtain it using
effectHandler->getLightCamera(), then you are free to do as you wish to it.

I hope that was an insightful chapter to read, see you next time.

- Cheers

License:

  The Irrlicht Engine License
  ===========================

  Copyright (C) 2007-2008 Ahmed Hilali

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgement in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be clearly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.