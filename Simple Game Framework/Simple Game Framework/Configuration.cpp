#include "Configuration.h"
#include <irrlicht.h>

Configuration::Configuration()
{
	//setup sane default values for irrlicht device in case of invalid / not readable configuration file
	this->params.DriverType = irr::video::EDT_OPENGL;
	this->params.WindowSize.Width = 1024;
	this->params.WindowSize.Height = 768;
	this->params.Bits = 32;
	this->params.Fullscreen = false;
	this->params.Stencilbuffer = false;
	this->params.Vsync = false;
	this->params.AntiAlias = false;
}

bool Configuration::read(irr::IrrlichtDevice* device)
{
	//irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_NULL);
	//create xml reader
	irr::io::IXMLReader* xml = 0;
	xml = device->getFileSystem()->createXMLReader("config.xml"); //create xmlreader

	if (xml == 0)
	{
		device->getLogger()->log(L"Cannot find configuration file. Falling back to defaults.");
		return false;
	}

	device->getLogger()->log(L"Reading configuration file...");

	while(xml && xml->read()) 
	{
		if (irr::core::stringw("graphic") == xml->getNodeName())
		{
			const irr::u32 driver = xml->getAttributeValueAsInt(L"driver");
			irr::video::E_DRIVER_TYPE driverType=(irr::video::E_DRIVER_TYPE)driver;
			this->params.DriverType = driverType;
			this->params.Fullscreen = (bool)(xml->getAttributeValueAsInt(L"fullscreen"));
			this->params.Stencilbuffer = (bool)(xml->getAttributeValueAsInt(L"stencilbuffer"));
			this->params.Vsync = (bool)(xml->getAttributeValueAsInt(L"vsync"));
		}

		else if (irr::core::stringw("resolution") == xml->getNodeName()) 
		{
			this->params.WindowSize.Width = xml->getAttributeValueAsInt(L"xres");
			this->params.WindowSize.Height = xml->getAttributeValueAsInt(L"yres");
			///\todo Check resolution

			this->params.Bits = xml->getAttributeValueAsInt(L"bits");
			
			//Check color depth.
			if ( (this->params.Bits != 16) || (this->params.Bits != 24))
				this->params.Bits = 32;
		}
	}

	xml->drop();
	return true;
}

void Configuration::write(irr::IrrlichtDevice* device) const
{
	//irr::IrrlichtDevice* device = 0;
	//device = irr::createDevice(irr::video::EDT_NULL);

	irr::io::IXMLWriter* xmlw = 0;
	if (device->getFileSystem()->existFile("config.xml"))
		xmlw = device->getFileSystem()->createXMLWriter("config.xml");
	else
	{
		irr::io::IWriteFile* file=device->getFileSystem()->createAndWriteFile("config.xml");
		xmlw= device->getFileSystem()->createXMLWriter(file);
		file->drop();
		//device->getLogger()->log(L"Configuration file does not exist. Please create an empty file 'config.xml'.");
	}

	if (xmlw == 0)
	{	//could not open file for writing
		return;
	}

	device->getLogger()->log(L"Writing configuration file...");

	xmlw->writeXMLHeader(); 
	xmlw->writeLineBreak();

	xmlw->writeElement(L"configuration", false);
	xmlw->writeLineBreak();
	xmlw->writeLineBreak();

	//write grafic driver config
	//xmlw->writeComment(L"Irrlicht device settings: driver can be 0 = OPENGL, 1 = Burning's Video or 2 = Software");
	xmlw->writeLineBreak();

	irr::core::stringw driver(L"");
	driver+=(unsigned int)params.DriverType;

	xmlw->writeElement(L"graphic", true,
		L"driver", driver.c_str(),
		L"fullscreen", irr::core::stringw(this->params.Fullscreen).c_str(),
		L"stencilbuffer", irr::core::stringw(this->params.Stencilbuffer).c_str(),
		L"vsync", irr::core::stringw(this->params.Vsync).c_str()
	);

	xmlw->writeLineBreak();
	xmlw->writeLineBreak();

	//write resolution and color depth
	xmlw->writeComment(L"Resolution and color depth settings");
	xmlw->writeLineBreak();
	xmlw->writeElement(L"resolution", true,
		L"xres", irr::core::stringw(this->params.WindowSize.Width).c_str(),
		L"yres", irr::core::stringw(this->params.WindowSize.Height).c_str(),
		L"bits", irr::core::stringw(this->params.Bits).c_str()
	);

	xmlw->writeLineBreak();

	xmlw->writeClosingTag(L"configuration");
	
	//clean up writer
	xmlw->drop();
	
	device->getLogger()->log(L"Done.");
}
