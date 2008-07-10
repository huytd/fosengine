#include "SGEProjectSetting.h"

using namespace irr;
using namespace core;
using namespace io;

SGEProjectSetting::SGEProjectSetting(IFileSystem* fs)
{
	this->fs=fs;
	fs->grab();
	//fileName="lastTime.sgp";
	workingDir=fs->getWorkingDirectory();
	texturePath=fs->getWorkingDirectory();
	meshPath=fs->getWorkingDirectory();
}
SGEProjectSetting::~SGEProjectSetting()
{
	fs->drop();
}
void SGEProjectSetting::read(const char* fileName)
{
	static char stringBuffer[800];
	IXMLReader* xml=0;
	xml=fs->createXMLReader(fileName);
	zipArchive.clear();
	if(xml)
	{
		this->fileName=fileName;
		while(xml->read())
		{
			const wchar_t* nodeName=xml->getNodeName();
			if(stringw("WorkingDirectory")==nodeName)
			{
				wcstombs(stringBuffer,xml->getAttributeValue(L"value"),800);
				workingDir=stringBuffer;				
			}
			else if(stringw("MeshPath")==nodeName)
			{
				wcstombs(stringBuffer,xml->getAttributeValue(L"value"),800);
				meshPath=stringBuffer;				
			}
			else if(stringw("TexturePath")==nodeName)
			{
				wcstombs(stringBuffer,xml->getAttributeValue(L"value"),800);
				texturePath=stringBuffer;				
			}
			else if(stringw("ZipArchive")==nodeName)
			{
				wcstombs(stringBuffer,xml->getAttributeValue(L"value"),800);
				fs->addZipFileArchive(stringBuffer,true,false);
				zipArchive.push_back(stringc(stringBuffer));
			}

		}
		xml->drop();
		this->fileName=fileName;//update file name
	}
	else
		printf("File %s does not exist\n",fileName);
}

void SGEProjectSetting::write(const char* fileName)
{
	//irr::IrrlichtDevice* device = 0;
	//device = irr::createDevice(irr::video::EDT_NULL);

	if(!fileName)
	{
		fileName=this->fileName.c_str();
	}
	IXMLWriter* xml=0;
	xml=fs->createXMLWriter(fileName);
	static wchar_t wideCharBuffer[800];
	wideCharBuffer[0]='\0';
	if(xml)
	{
		this->fileName=fileName;
		xml->writeXMLHeader(); 
		xml->writeLineBreak();

		xml->writeElement(L"project", false);
		xml->writeLineBreak();
		mbstowcs(wideCharBuffer,workingDir.c_str(),800);
		xml->writeElement(L"WorkingDirectory",true,L"value",wideCharBuffer);
		xml->writeLineBreak();
		mbstowcs(wideCharBuffer,meshPath.c_str(),800);
		xml->writeElement(L"MeshPath",true,L"value",wideCharBuffer);
		xml->writeLineBreak();
		mbstowcs(wideCharBuffer,texturePath.c_str(),800);
		xml->writeElement(L"TexturePath",true,L"value",wideCharBuffer);
		core::list<core::stringc>::Iterator i;
		for(i=zipArchive.begin();i!=zipArchive.end();i++)
		{
			xml->writeLineBreak();
			mbstowcs(wideCharBuffer,i->c_str(),800);
			xml->writeElement(L"ZipArchive",true,L"value",wideCharBuffer);
		}
		xml->writeLineBreak();
		xml->writeClosingTag(L"project");
		xml->drop();
	}
	else
		printf("Cannot write to %s\n",fileName);
	
}
