#include "ScriptVM.h"
#include <iostream>
#include "GameMonkey/gmStream.h"

using namespace irr;
using namespace io;

//! Used internally. Use gm with irrlicht's VFS
class gmFileStream:public gmStream
{
public:
	gmFileStream(IReadFile* rFile)
		:gmStream(),write(false),rFile(rFile),wFile(0)
	{
		rFile->grab();
	}
	gmFileStream(IWriteFile* wFile)
		:gmStream(),write(true),rFile(0),wFile(wFile)
	{
		wFile->grab();
	}

	virtual ~gmFileStream()
	{
		if(rFile)
			rFile->drop();
		if(wFile)
			wFile->drop();
	}

	  /// \brief Seek()
	  /// \return the p_pos before the seek if seek is supported, else ILLEGAL_POS.
	unsigned int Seek(unsigned int p_pos)
	{
		unsigned int ret;
		if(write)
		{
			ret=wFile->getPos();
			if(!(wFile->seek(p_pos)))
				ret=(unsigned int)ILLEGAL_POS;
		}
		else
		{
			ret=rFile->getPos();
			if(!(rFile->seek(p_pos)))
				ret=(unsigned int)ILLEGAL_POS;
		}
		return ret;
	}

  /// \brief Tell()
  /// \return the p_pos if tell is supported, else ILLEGAL_POS
    unsigned int Tell() const 
	{
		if(write)
			return (unsigned int)(wFile->getPos());
		return (unsigned int)(rFile->getPos());
	}

  /// \brief Read() will read p_n bytes from the stream into p_buffer.
  /// \return the number of bytes successfully read.
  unsigned int Read(void * p_buffer, unsigned int p_n)
  {
	  if(write)
		  return 0;
	  return rFile->read(p_buffer,p_n);
  }

  /// \brief Write() will write p_n bytes from p_buffer to the stream.
  /// \return the number of bytes successfully written
  unsigned int Write(const void * p_buffer, unsigned int p_n)
  {
	  if(!write)
		  return 0;
	  return wFile->write(p_buffer,p_n);
  }
private:
	bool write;
	IReadFile* rFile;
	IWriteFile* wFile;
};

static int GM_CDECL doFile(gmThread* a_thread)
{
	GM_CHECK_NUM_PARAMS(1);
	GM_CHECK_STRING_PARAM(file,0);
	gmVariable param;
	param.Nullify();
	param=a_thread->Param(1,param);
	sgfScriptVM* vm=(sgfScriptVM*)(a_thread->GetMachine());//this function is only available in our vm
	int ret=vm->ExecuteFile(file,&param);
	a_thread->PushInt(ret);
	return GM_OK;
}

sgfScriptVM::sgfScriptVM(sgfCore* core, IFileSystem* fs)
	:gmMachineEx(false),sgfCoreComponent(core),fs(fs),scriptBuffer(0),bufferSize(0)
{
	fs->grab();
	RegisterLibraryFunction("doFile",doFile);
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
	int ret=1;
	if(file)
	{
		relocateBuffer(file->getSize()+1);//+1 for the \0
		int byteRead=file->read(scriptBuffer,file->getSize());
		file->drop();
		scriptBuffer[byteRead]='\0';//terminate the string
		ret=ExecuteString(scriptBuffer,0,true,scriptFile,a_this);
	}
	else
		GetLog().LogEntry("sgfScriptVM: File %s not found",scriptFile);
	return ret;
}

bool sgfScriptVM::ExecuteLibFile(const char* libFile,gmVariable* a_this)
{
	IReadFile* file=fs->createAndOpenFile(libFile);
	bool ret=false;
	if(file)
	{
		gmFileStream stream(file);
		ret=ExecuteLib(stream,0,true,libFile);
		file->drop();
	}
	else
		GetLog().LogEntry("sgfScriptVM: File %s not found",libFile);
	return ret;
}

int sgfScriptVM::CompileString(const char* str,const char* fileName)
{
	int ret=1;
	IWriteFile* file=fs->createAndWriteFile(fileName);
	if(file)
	{
		gmFileStream stream(file);
		ret=CompileStringToLib(str,stream);
		file->drop();
	}
	else
		GetLog().LogEntry("sgfScriptVM: Cannot write to %s",fileName);
	return ret;
}

int sgfScriptVM::CompileFile(const char* in,const char* out)
{
	IReadFile* file=fs->createAndOpenFile(in);
	int ret=1;
	if(file)
	{
		relocateBuffer(file->getSize()+1);//+1 for the \0
		int byteRead=file->read(scriptBuffer,file->getSize());
		file->drop();
		scriptBuffer[byteRead]='\0';//terminate the string
		ret=CompileString(scriptBuffer,out);
	}
	else
		GetLog().LogEntry("sgfScriptVM: File %s not found",in);
	return ret;
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

gmFunctionObject* sgfScriptVM::GetFunction(const char* varName,gmTableObject* table)
{
	gmVariable var=GetVar(varName,table);
	if(var.m_type==GM_FUNCTION)
		return (gmFunctionObject*)var.m_value.m_ref;
	else
		return 0;
}

gmType sgfScriptVM::GetTypeId(const char* name)
{
	gmMachine::Type* types=m_types.GetData();
	for(unsigned int i=0;i<m_types.GetSize();++i)
	{
		if(!strcmp(types[i].m_name->GetString(),name))
			return i;
	}
	return 0;
}

void sgfScriptVM::Update(SFrameEvent& arg)
{
	Execute(arg.deltaTime*1000);
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

inline void sgfScriptVM::relocateBuffer(int size)
{
	if(bufferSize<size)
	{
		scriptBuffer=(char*)realloc(scriptBuffer,size);
		bufferSize=size;
	}
}