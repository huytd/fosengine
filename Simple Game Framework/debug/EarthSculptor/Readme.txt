EearthSculptor Beta 1.05
Copyright (c) 2007 Ernest Szoka
Real Time 3D Terrain Heightmap Editor and Painter

If your having problems running the application, make sure you have proper video card drivers installed on your machine. Earthsculptor takes advantage of vertex/pixel shaders, multiple texture units and push buffers(rendertargets) using OpenGL.

KEYS (based on my fps key controls) if your having trouble with the keys you can always switch to WASD keys or remap the keys in the Camera menu.


CAMERA CONTROLS

move forward/back/left/right e/d/s/f
up/down	a/z

numeric key pad (numlock on)
forward/back/left/right 8/2/4/6
rotate camera roll 7/9

look - right mouse button
terraform/paint - left mouse button
zoom - scroll wheel
0 (zero) - frustum snapshot

alt + left mouse button - rotate around cursor point
alt + middle mouse button - pan
alt + right mouse button - zoom


TOOL CONTROLS

control - special function key:
	terraform: reverses raise/lower terraform, forces uniform height when using level terraform 
	color: color picker - sets the current working color from the colormap at the cursors position 
shift + left mouse button - up/down mouse movement changes tool radius 
tab - switch to previously used terrain tool/detail texture
caps lock - hides/unhides UI windows
control + z - undo
control + shift + z - redo
Page up/ Page down - switch to a predetermined view
Home - goes to home position
space bar - when terraforming switches to a vertex point tool to modify vertices 
1 - toggle LOD
2 - toggle Wireframe
3 - toggle Grid
4 - toggle Color Map
5 - toggle Detail Map
6 - toggle Light Map


SETTING LIMITATIONS
In the EarthSculptor.ini file you can change the limitations of various tools and parameters in EarthSculptor that you can't inside the program

maxTerrainHeight 300 - changes the maximum height slider in terrain panel
maxClippingPlane 2000 - changes the max clipping plane of the program (lower this to improve performance or increase to get larger vistas)
terraformStrengthMultiplier 100 - changes the default strength multiplier of some of the terraform tools except ramp,push and grab
toolMaxRadius 100 - changes the maximum tool radius for terraform,detail and color