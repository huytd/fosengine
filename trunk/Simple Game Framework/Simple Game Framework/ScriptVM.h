#ifndef _SCRIPTVM_H_
#define _SCRIPTVM_H_

#include "gmMachineEx/gmMachineEx.h"
#include "irrlicht.h"
#include "CoreComponent.h"
#include "StandardEvents.h"

/// \brief a GameMonkey script machine specially designed for this framework
/// It uses the Irrlicth VFS(virtual file system)
class sgfScriptVM:public gmMachineEx,public sgfCoreComponent
{
public:
	sgfScriptVM(sgfCore* core, irr::io::IFileSystem* fs);
	virtual ~sgfScriptVM();

	/// \brief Execute a script file and return error
	int ExecuteFile(const char* scriptFile,gmVariable* a_this=0);
	
	/// \brief Execute a compiled lib
	bool ExecuteLibFile(const char* libFile,gmVariable* a_this=0);

	/// \brief Compile a string into a lib
	int CompileString(const char* str,const char* fileName);

	/// \brief Compile a file into a lib
	int CompileFile(const char* in,const char* out);

	// \brief Get type's id
	gmType GetTypeId(const char* name);

	void Update(SFrameEvent& arg);
	const char* GetString(const char* varName,gmTableObject* table=0);
	gmTableObject* GetTable(const char* varName,gmTableObject* table=0);
	gmFunctionObject* GetFunction(const char* varName,gmTableObject* table=0);
	int GetInt(const char* varName,gmTableObject* table=0);
	float GetFloat(const char* varName,gmTableObject* table=0);
	gmVariable GetVar(const char* varName,gmTableObject* table=0);
	void SetVar(const char* varName,gmVariable value,gmTableObject* table=0);
	void SetInt(const char* varName,int value,gmTableObject* table=0);
	void SetFloat(const char* varName,float value,gmTableObject* table=0);
	void SetString(const char* varName,const char* value,gmTableObject* table=0);
private:
	inline void relocateBuffer(int size);
	irr::io::IFileSystem* fs;
	char* scriptBuffer;
	long bufferSize;
};

#endif