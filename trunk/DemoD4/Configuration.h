#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <IVideoDriver.h>
#include <IEventReceiver.h> //needed by SIrrCreationParameters.h
#include <SIrrCreationParameters.h>

/**
 * An object of this class holds all the variables the user may edit plus
 * some basic stuff like the name and version of the game.
 */
class Configuration
{
private:

	///Holds the name of the game.
	irr::core::stringw projectName;

	//versioning
	irr::u32 majorVersion;
	irr::u32 minorversion;

	//stuff user can change
	bool enableHDR;
	
	//Full screen software anti alias option
	bool antiAliasEnabled;

	///For Irrlicht device creation.
	irr::SIrrlichtCreationParameters params;

	irr::core::stringw noidungUnicode;

#ifdef _SOUND
	///True if user wants to hear sound.
	bool soundEnabled;
#endif
	///Transparency of gui.
	irr::u32 guiTransparency;
	
	///Change this to alter the sensitivity of the mouselook.
	irr::f32 cursorSensitivity;
	
public:
	///The constructor sets up sane default values.	
	Configuration();

	///Reads the configuration from an xml file.
	const void read();

	///Writes the configuration to an xml file.
	const void write() const;

	// getter
	inline const irr::core::stringw& getProjectName() const
	{ return this->projectName; }

	inline const irr::SIrrlichtCreationParameters& getIrrlichtParams() const
	{ return this->params; }
	
#ifdef _SOUND
	inline const bool isSoundEnabled() const
	{ return this->soundEnabled; }
#endif

	inline const irr::u32 getGuiTransparency() const
	{ return this->guiTransparency; }
	
	inline const bool getAntiAliasOption() conse
	{
		return this->antiAliasEnabled;
	}

	inline const irr::f32 getCursorSensitivity() const
	{ return this->cursorSensitivity; }

	inline const bool getHDREnable() const
	{ return this->enableHDR; }

	inline const irr::core::stringw getNoidung() const
	{ return this->noidungUnicode; }

	//setter
	inline const void setIrrlichtParams(const irr::SIrrlichtCreationParameters& params)
	{ this->params = params; }
	
#ifdef _SOUND
	inline const void setSoundEnabled(const bool enabled)
	{ this->soundEnabled = enabled; }
#endif

	inline const void setGuiTransparency(const irr::u32 transparency)
	{ this->guiTransparency = transparency; }	

};

#endif /*CONFIGURATION_H_*/
