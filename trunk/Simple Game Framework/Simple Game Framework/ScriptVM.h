#ifndef _SCRIPTVM_H_
#define _SCRIPTVM_H_

#include "gmMachineEx/gmMachineEx.h"
#include "irrlicht.h"
#include "CoreComponent.h"

/// \brief a GameMonkey script machine specially designed for this framework
/// It uses the Irrlicth VFS(virtual file system)
class sgfScriptVM:public gmMachineEx,public sgfCoreComponent
{
public:
	sgfScriptVM(sgfCore* core, irr::io::IFileSystem* fs);
	virtual ~sgfScriptVM();

	/// \brief Execute a script file and return error
	int ExecuteFile(const char* scriptFile,gmVariable* a_this=0);
	
	/// \brief Print all errors if there is any
	void PrintError();

	const char* GetString(const char* varName,gmTableObject* table=0);
	gmTableObject* GetTable(const char* varName,gmTableObject* table=0);
	int GetInt(const char* varName,gmTableObject* table=0);
	float GetFloat(const char* varName,gmTableObject* table=0);
	gmVariable GetVar(const char* varName,gmTableObject* table=0);
	void SetVar(const char* varName,gmVariable value,gmTableObject* table=0);
	void SetInt(const char* varName,int value,gmTableObject* table=0);
	void SetFloat(const char* varName,float value,gmTableObject* table=0);
	void SetString(const char* varName,const char* value,gmTableObject* table=0);
private:
	irr::io::IFileSystem* fs;
	char* scriptBuffer;
	long bufferSize;
};

#endif