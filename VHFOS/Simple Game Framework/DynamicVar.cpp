#include "DynamicVar.h"

gmType sgfDynamicVar::any_ptr=0;

sgfDynamicVar::sgfDynamicVar(sgfDynamicVarSet* varSet,const char* name)
{
	this->name=name;
	this->varSet=varSet;
}

sgfDynamicVar::sgfDynamicVar(const sgfDynamicVar& other)
{
	this->name=other.name;
	this->varSet=other.varSet;	
}

sgfDynamicVar::~sgfDynamicVar()
{
}

sgfDynamicVar& sgfDynamicVar::operator=(void* ptr)
{
	gmVariable var;
	var.SetUser(varSet->vm,ptr,sgfDynamicVar::any_ptr);
	varSet->table->Set(varSet->vm,name.c_str(),var);
	return *this;
}

sgfDynamicVar& sgfDynamicVar::operator=(const char* str)
{
	gmVariable var;
	var.SetString(varSet->vm,str);
	varSet->table->Set(varSet->vm,name.c_str(),var);
	return *this;
}

sgfDynamicVar& sgfDynamicVar::operator=(float num)
{
	gmVariable var(num);
	varSet->table->Set(varSet->vm,name.c_str(),var);
	return *this;
}

sgfDynamicVar& sgfDynamicVar::operator=(int num)
{
	gmVariable var(num);
	varSet->table->Set(varSet->vm,name.c_str(),var);
	return *this;
}

sgfDynamicVar& sgfDynamicVar::operator=(sgfDynamicVar& var)
{
	varSet->table->Set(varSet->vm,name.c_str(),var.getVar());
	return *this;	
}

gmVariable sgfDynamicVar::getVar() const
{
	return varSet->table->Get(varSet->vm,name.c_str());
}

gmType sgfDynamicVar::getType() const
{
	return getVar().m_type;
}

void* sgfDynamicVar::getPtr() const
{
	return ((gmUserObject*)(getVar().m_value.m_ref))->m_user;
}

int sgfDynamicVar::getInt() const
{
	return getVar().GetIntSafe();
}

float sgfDynamicVar::getFloat() const
{
	return getVar().GetFloatSafe();
}

const char* sgfDynamicVar::getString() const
{
	return getVar().GetCStringSafe();
}

sgfDynamicVarSet::sgfDynamicVarSet()
{
}
sgfDynamicVarSet::~sgfDynamicVarSet()
{
}
sgfDynamicVar sgfDynamicVarSet::operator[](const char* name)
{
	return sgfDynamicVar(this,name);
}

void sgfDynamicVarSet::setTable(sgfScriptVM* vm,gmTableObject* table)
{
	this->vm=vm;
	this->table=table;
}