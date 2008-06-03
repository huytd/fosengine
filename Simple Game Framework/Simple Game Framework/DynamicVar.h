#ifndef _SGF_DYNAMIC_VAR_H_
#define _SGF_DYNAMIC_VAR_H_

#include <string>
#include "scriptVM.h"
#include "sgfPtr.h"

class sgfDynamicVarSet;
//variable helper
class sgfDynamicVar
{
public:
	sgfDynamicVar(sgfDynamicVarSet* varSet,const char* name);
	sgfDynamicVar(const sgfDynamicVar& other);
	~sgfDynamicVar();
	sgfDynamicVar& operator=(sgfDynamicVar& var);
	sgfDynamicVar& operator=(void* ptr);
	sgfDynamicVar& operator=(const char* str);
	sgfDynamicVar& operator=(float num);
	sgfDynamicVar& operator=(int num);
	gmType getType() const;
	void* getPtr() const;
	int getInt() const;
	float getFloat() const;
	const char* getString() const;
	template<typename T>
	inline void setAs(T obj,gmType Type)
	{
		gmVariable var=CONVERT<GM_OBJ_PTR,T>::ToGm(obj,varSet->vm,Type);
		varSet->table->Set(varSet->vm,name.c_str(),var);
	}
	template<typename T>
	inline void setAs(T obj,const char* typeName)
	{
		gmType Type=varSet->vm->GetTypeId(typeName);
		GM_ASSERT(Type);
		gmVariable var=CONVERT<GM_OBJ_PTR,T>::ToGm(obj,varSet->vm,Type);
		varSet->table->Set(varSet->vm,name.c_str(),var);
	}
	template<typename T>
	inline T getAs()
	{
		return (T)getPtr();
	}
	static gmType any_ptr;
private:
	gmVariable getVar() const;
	std::string name;
	sgfDynamicVarSet* varSet;
};

class sgfDynamicVarSet// a table helper
{
friend class sgfDynamicVar;
public:
	sgfDynamicVarSet();
	~sgfDynamicVarSet();
	sgfDynamicVar operator[](const char* name);
	void setTable(sgfScriptVM* vm,gmTableObject* table);
private:
	sgfPtr<sgfScriptVM> vm;
	gmTableObject* table;
};

#endif