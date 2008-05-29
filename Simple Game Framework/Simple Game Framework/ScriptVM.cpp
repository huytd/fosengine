#include "ScriptVM.h"
#include <iostream>

using namespace irr;
using namespace io;

sgfScriptVM::sgfScriptVM(sgfCore* core, IFileSystem* fs)
	:gmMachineEx(true),sgfCoreComponent(core),fs(fs),scriptBuffer(0),bufferSize(0)
{
	fs->grab();
}

sgfScriptVM::~sgfScriptVM()
{
	fs->drop();
	if(scriptBuffer)
		free(scriptBuffer);
}

int sgfScriptVM::ExecuteFile(const char* scriptFile,gmVariable* a_this)
{
	IReadFile* file=fs->createAndOpenFile(scriptFile);
	if(file)
	{
		if(bufferSize<(file->getSize()+1))//+1 because of the ending null in a string
		{
			scriptBuffer=(char*)realloc(scriptBuffer,file->getSize()+1);
			bufferSize=file->getSize()+1;
		}
		int byteRead=file->read(scriptBuffer,file->getSize());
		file->drop();
		scriptBuffer[byteRead]='\0';
		return ExecuteString(scriptBuffer,0,true,scriptFile,a_this);
	}
	GetLog().LogEntry("sgfScriptVM: File %s not found",scriptFile);
	return 1;
}

void sgfScriptVM::PrintError()
{
	bool first=true;
	const char* entry=GetLog().GetEntry(first);
	while(entry)
	{
		printf("%s\n",entry);
		entry=GetLog().GetEntry(first);
	}
}

const char* sgfScriptVM::GetString(const char* varName,gmTableObject* table)
{
	gmVariable var=GetVar(varName,table);
	if(var.m_type==GM_STRING)
		return ((gmStringObject*)var.m_value.m_ref)->GetString();
	else
		return 0;
}

gmTableObject* sgfScriptVM::GetTable(const char* varName,gmTableObject* table)
{
	gmVariable var=GetVar(varName,table);
	if(var.m_type==GM_TABLE)
		return (gmTableObject*)var.m_value.m_ref;
	else
		return 0;
}

int sgfScriptVM::GetInt(const char* varName,gmTableObject* table)
{
	gmVariable var=GetVar(varName,table);
	if(var.m_type==GM_INT)
		return var.m_value.m_int;
	else
		return 0;
}	

float sgfScriptVM::GetFloat(const char* varName,gmTableObject* table)
{
	gmVariable var=GetVar(varName,table);
	if(var.m_type==GM_FLOAT)
		return var.m_value.m_float;
	else
		return 0;
}

gmVariable sgfScriptVM::GetVar(const char* varName,gmTableObject* table)
{
	if(!table)
		table=GetGlobals();
	return table->Get(this,varName);
}

void sgfScriptVM::SetVar(const char* varName,gmVariable value,gmTableObject* table)
{
	if(!table)
		table=GetGlobals();
	table->Set(this,varName,value);
}

void sgfScriptVM::SetInt(const char* varName,int value,gmTableObject* table)
{
	SetVar(varName,gmVariable(value),table);
}

void sgfScriptVM::SetFloat(const char* varName,float value,gmTableObject* table)
{
	SetVar(varName,gmVariable(value),table);
}

void sgfScriptVM::SetString(const char* varName,const char* value,gmTableObject* table)
{
	SetVar(varName,gmVariable(AllocStringObject(value)),table);
}