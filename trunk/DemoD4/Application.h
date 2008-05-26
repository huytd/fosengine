/**
 * \Summary Application manager
 * \Filename: application.h
 * \Encoding: UTF-8
 * \Tabsize: 8
 * \Indentation: 4
 * \CreatedDate: 18:33 2008/05/26
 * \InitializedBy: Irrlicht Forum  - randomMesh
 * \CreatedBy: FOSP Team
 * \Copyright: FOS Project
 */
 
#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <IrrlichtDevice.h>
#ifdef _SOUND
#include <irrKlang.h>
#endif
#include "Configuration.h"

class Application
{

private:

	Configuration configuration;

	irr::IrrlichtDevice* device;
#ifdef _SOUND
	irrklang::ISoundEngine* soundEngine;
#endif

public:

	Application();

	const bool init();

	const void run();
};

#endif /*APPLICATION_H_*/
