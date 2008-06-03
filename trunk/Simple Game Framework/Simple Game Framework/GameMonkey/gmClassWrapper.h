#ifndef _GM_CLASS_WRAPPER_H_
#define _GM_CLASS_WRAPPER_H_

#include "gmMachine.h"

template<class T>
T* construtor<T>()
{
	return new T();	
}

template <class T,typename type0>
T* constructor<T>(type0 param0)
{
	return new T(param0);
}

template <class T,typename type0,typename type1>
T* constructor<T>(T* obj, type0 param0, type1 param1)
{
	return new T(param0,param1);
}

template <class T,typename type0,typename type1,typename type2>
T* constructor<T>(T* obj, type0 param0, type1 param1,type2 param2)
{
	return new T(param0,param1,param2);
}

template <class T,typename type0,typename type1,typename type2,typename type3>
T* constructor<T>(T* obj, type0 param0, type1 param1,type2 param2,type3 param3)
{
	return new T(param0,param1,param2,param3);
}

template <class T,typename type0,typename type1,typename type2,typename type3,typename type4>
T* constructor<T>(T* obj, type0 param0, type1 param1,type2 param2,type3 param3,type4 param4)
{
	return new T(param0,param1,param2,param3,param4);
}


template<class T>
T* destructor<T>(T* obj)
{
	delete obj;
}

#endif