/*
***********************************************
* Main entry for program
* *********************************************
* file name: main.cpp
* encoding: UTF-8
* tab size: 8
* indentation: 4
* created on: 6:36 PM 5/2/2008
* init code by: _REPLACE_IN_ALL_PROJECT_ ,Irrlicht Forum
* created by: FOSP Team
* copyright: FOS Project
*/
#include "windows.h"
#include "Application.h"
#pragma comment(lib, "Irrlicht.lib")

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
int main(int argc, char** argv)
{	
	Application app;

	if (app.init())
		app.run();

	return 0;
}
