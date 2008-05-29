#include "DynamicVar.h"

sgfDynamicVar::sgfDynamicVar()
	:ptr(0),num(0)
{
}

sgfDynamicVar::sgfDynamicVar(const sgfDynamicVar& other)
{
	type=other.type;
	num=other.num;
	ptr=other.ptr;
	str=other.str.c_str();
}

sgfDynamicVar::~sgfDynamicVar()
{
}

sgfDynamicVar& sgfDynamicVar::operator=(void* ptr)
{
	type=EDVT_POINTER;
	this->ptr=ptr;
	return *this;
}

sgfDynamicVar& sgfDynamicVar::operator=(const char* str)
{
	type=EDVT_STRING;
	this->str=str;
	return *this;
}

sgfDynamicVar& sgfDynamicVar::operator=(float num)
{
	type=EDVT_NUMBER;
	this->num=num;
	return *this;
}

sgfDynamicVar& sgfDynamicVar::operator=(sgfDynamicVar& var)
{
	type=var.type;
	num=var.num;
	ptr=var.ptr;
	str=var.str.c_str();
	return *this;
}

E_DYNAMIC_VAR_TYPE sgfDynamicVar::getType() const
{
	return type;
}

void* sgfDynamicVar::getPtr() const
{
	return ptr;
}

float sgfDynamicVar::getNumber() const
{
	return num;
}

const char* sgfDynamicVar::getString() const
{
	return str.c_str();
}

sgfDynamicVarSet::sgfDynamicVarSet()
{
}
sgfDynamicVarSet::~sgfDynamicVarSet()
{
}
sgfDynamicVar& sgfDynamicVarSet::operator[](const char* name)
{
	irr::core::stringc str=name;
	std::map<irr::core::stringc,sgfDynamicVar>::iterator i=varSet.find(str);
	if(i==varSet.end())
	{// not found
		sgfDynamicVar var;
		varSet.insert(std::make_pair<irr::core::stringc,sgfDynamicVar>(name,var));
		i=varSet.find(str);
	}
	return (*i).second;
}

void sgfDynamicVarSet::clear()
{
	varSet.clear();
}