#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <irrlicht.h>

/**
 * An object of this class holds all the variables the user may edit plus
 * some basic stuff like the name and version of the game.
 */
class Configuration
{
private:
	irr::SIrrlichtCreationParameters params;

public:
	///The constructor sets up sane default values.
	Configuration();

	///Reads the configuration from an xml file.
	bool read(irr::IrrlichtDevice* device);

	///Writes the configuration to an xml file.
	void write(irr::IrrlichtDevice* device) const;

	inline irr::SIrrlichtCreationParameters& getIrrlichtParams()
	{ return this->params; }

	inline const void setIrrlichtParams(const irr::SIrrlichtCreationParameters& params)
	{ this->params = params; }
};

#endif /*CONFIGURATION_H_*/
