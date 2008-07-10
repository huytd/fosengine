#ifndef _SGE_PROJECT_SETTING_H_
#define _SGE_PROJECT_SETTING_H_

#include <irrlicht.h>

class SGEProjectSetting
{
public:
	SGEProjectSetting(irr::io::IFileSystem* fs);
	~SGEProjectSetting();
	void read(const char* fileName);
	void write(const char* fileName=0);

	irr::core::stringc fileName;
	irr::core::stringc workingDir;
	irr::core::stringc meshPath;
	irr::core::stringc texturePath;
	irr::core::list<irr::core::stringc> zipArchive;
private:
	irr::io::IFileSystem* fs;
};

#endif