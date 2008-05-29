#ifndef _SGF_DYNAMIC_VAR_H_
#define _SGF_DYNAMIC_VAR_H_

#include <irrlicht.h>
#include <map>

enum E_DYNAMIC_VAR_TYPE
{
	EDVT_NUMBER,
	EDVT_STRING,
	EDVT_POINTER
};

class sgfDynamicVar
{
public:
	sgfDynamicVar();
	sgfDynamicVar(const sgfDynamicVar& other);
	~sgfDynamicVar();
	sgfDynamicVar& operator=(sgfDynamicVar& var);
	sgfDynamicVar& operator=(void* ptr);
	sgfDynamicVar& operator=(const char* str);
	sgfDynamicVar& operator=(float num);
	E_DYNAMIC_VAR_TYPE getType() const;
	void* getPtr() const;
	float getNumber() const;
	const char* getString() const;
	template<typename T>
	inline T getAs()
	{
		return (T)ptr;
	}
private:
	E_DYNAMIC_VAR_TYPE type;
	void* ptr;
	float num;
	irr::core::stringc str;
};

class sgfDynamicVarSet
{

public:
	sgfDynamicVarSet();
	~sgfDynamicVarSet();
	sgfDynamicVar& operator[](const char* name);
	void clear();
private:
	std::map<irr::core::stringc,sgfDynamicVar> varSet;
};

#endif