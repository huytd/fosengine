#ifndef GM_TMP_H
#define GM_TMP_H

#include "gmException.h"

#define FUNCTION_TYPE 1
#define FUNCTION_NORET_TYPE -1
#define METHOD_TYPE 3
#define METHOD_NORET_TYPE -3
#define CONSTRUCTOR_TYPE 4
#define FUNCTION_OPERATOR_TYPE 5
#define METHOD_OPERATOR_TYPE 6
#define VARIABLE_TYPE 7

#define DESTRUCTOR_VIRTUAL_TYPE 9
#define BASECLASS_VIRTUAL_TYPE 10
#define POOL_VIRTUAL_TYPE 11
#define END_VIRTUAL_TYPE 12
#define ASSTRING_VIRTUAL_TYPE 13

#define IS_CONST 1
#define IS_NOT_CONST 0

#define GET_SET_ACCESS 8
#define DIRECT_ACCESS 9
#define DIRECT_ACCESS_READ_ONLY 10
#define GM_READ_ONLY 1 

#define GM_UNKNOWN 7
#define GM_THREAD_LAUNCH_INFO 8
#define GM_ANY_PTR 9
#define GM_C_STRING 10
#define GM_COUT 23

#define GM_BOOL_PTR 11
#define GM_INT_PTR 12
#define GM_SHORT_PTR 13
#define GM_LONG_PTR 14
#define GM_LONGLONG_PTR 15
#define GM_UINT_PTR 16
#define GM_USHORT_PTR 17
#define GM_ULONG_PTR 18
#define GM_ULONGLONG_PTR 19

#define GM_FLOAT_PTR 20
#define GM_DOUBLE_PTR 21
#define GM_LONGDOUBLE_PTR 22

#define GM_OBJ -3
#define GM_OBJ_REF -4
#define GM_OBJ_CONST_REF -5
#define GM_OBJ_PTR -6
#define GM_OBJ_CONST_PTR -7

#define GM_CONST_ANY_PTR -9 //DOIT 黎re egal ・l'oppos・de GM_ANY_PTR
#define GM_CONST_C_STRING -10

/*=============================================================================================
  Template qui permet d'avoir le gmType correspondant a un type C++ au moment de la compilation
===============================================================================================*/

template<typename T> struct GET					{ static const int StaticId = GM_OBJ; };
template<typename T> struct GET<const T>		{ static const int StaticId = GET<T>::StaticId; };
template<typename T> struct GET<T*>				{ static const int StaticId = GM_OBJ_PTR; };
template<typename T> struct GET<const T*>		{ static const int StaticId = GM_OBJ_CONST_PTR; };
template<typename T> struct GET<T&>				{ static const int StaticId = GM_OBJ_REF; };
template<typename T> struct GET<const T&>		{ static const int StaticId = GM_OBJ_CONST_REF; };

template<> struct GET<void>						{ static const int StaticId = GM_NULL; };

template<> struct GET<bool>						{ static const int StaticId = GM_INT; };
template<> struct GET<char>						{ static const int StaticId = GM_INT; };
template<> struct GET<int>						{ static const int StaticId = GM_INT; };
template<> struct GET<short>					{ static const int StaticId = GM_INT; };
template<> struct GET<long>						{ static const int StaticId = GM_INT; };
template<> struct GET<long long>				{ static const int StaticId = GM_INT; };
template<> struct GET<unsigned char>			{ static const int StaticId = GM_INT; };
template<> struct GET<unsigned int>				{ static const int StaticId = GM_INT; };
template<> struct GET<unsigned short>			{ static const int StaticId = GM_INT; };
template<> struct GET<unsigned long>			{ static const int StaticId = GM_INT; };
template<> struct GET<unsigned long long>		{ static const int StaticId = GM_INT; };

template<> struct GET<float>					{ static const int StaticId = GM_FLOAT; };
template<> struct GET<double>					{ static const int StaticId = GM_FLOAT; };
template<> struct GET<long double>				{ static const int StaticId = GM_FLOAT; };

template<> struct GET<char*>					{ static const int StaticId = GM_C_STRING; };
template<> struct GET<const char*>				{ static const int StaticId = GM_CONST_C_STRING; };

template<> struct GET<gmStringObject*>			{ static const int StaticId = GM_STRING; };
template<> struct GET<const gmStringObject*>	{ static const int StaticId = GM_STRING; };

template<> struct GET<gmTableObject*>			{ static const int StaticId = GM_TABLE; };
template<> struct GET<const gmTableObject*>		{ static const int StaticId = GM_TABLE; };

template<> struct GET<gmFunctionObject*>		{ static const int StaticId = GM_FUNCTION; };
template<> struct GET<const gmFunctionObject*>	{ static const int StaticId = GM_FUNCTION; };

template<> struct GET<gmUserObject*>			{ static const int StaticId = GM_USER; };
template<> struct GET<const gmUserObject*>		{ static const int StaticId = GM_USER; };


/*======================================================
  Template qui fait la conversion C++ -> GM et GM -> C++
========================================================*/

template<gmType GM_TYPE, typename CPP_TYPE> struct CONVERT {};

template<typename CPP_TYPE> struct CONVERT<GM_INT,CPP_TYPE>
{
	static inline int ToCpp( gmVariable& Var ){ return ConvertType<int>(&Var); }
	static inline gmVariable ToGm( int Val, gmMachine*, gmType ){ return gmVariable( Val ); }
};

template<typename CPP_TYPE> struct CONVERT<GM_FLOAT,CPP_TYPE>
{
	static inline float ToCpp( gmVariable& Var ){ return ConvertType<float>(&Var); }
	static inline gmVariable ToGm( float Val, gmMachine*, gmType ){ return gmVariable( Val ); }
};

template<typename CPP_TYPE> struct CONVERT<GM_STRING,CPP_TYPE>
{
	static inline gmStringObject* ToCpp( gmVariable& Var ){ return (gmStringObject*)Var.m_value.m_ref; }
	static inline gmVariable ToGm( const gmStringObject* pStringObj, gmMachine*, gmType ){ return gmVariable( (gmStringObject*)pStringObj ); }
};

template<typename CPP_TYPE> struct CONVERT<GM_TABLE,CPP_TYPE>
{
	static inline gmTableObject* ToCpp( gmVariable& Var ){ return (gmTableObject*)Var.m_value.m_ref; }
	static inline gmVariable ToGm( const gmTableObject* Table, gmMachine*, gmType ){ return gmVariable( (gmTableObject*)Table ); }
};

template<typename CPP_TYPE> struct CONVERT<GM_FUNCTION,CPP_TYPE>
{
	static inline gmFunctionObject* ToCpp( gmVariable& Var ){ return (gmFunctionObject*)Var.m_value.m_ref; }
	static inline gmVariable ToGm( const gmFunctionObject* Function, gmMachine*, gmType ){ return gmVariable( (gmFunctionObject*)Function ); }
};

template<typename CPP_TYPE> struct CONVERT<GM_USER,CPP_TYPE>
{
	static inline gmUserObject* ToCpp( gmVariable& Var ){ return (gmUserObject*)Var.m_value.m_ref; }
	static inline gmVariable ToGm( const gmUserObject* UserObj, gmMachine*, gmType ){ return gmVariable( (gmUserObject*)UserObj ); }
};

template<typename CPP_TYPE> struct CONVERT<GM_OBJ,CPP_TYPE>
{
	static inline CPP_TYPE ToCpp( gmVariable& Var ){ return *(CPP_TYPE*)((gmUserObject*)Var.m_value.m_ref)->m_user; }
	static inline gmVariable ToGm( const CPP_TYPE& Obj, gmMachine* VM, gmType Type ){ return gmVariable( VM->AllocUserObject( new CPP_TYPE(Obj), Type ) ); }
};

template<typename CPP_TYPE> struct CONVERT<GM_OBJ_REF,CPP_TYPE>
{
	static inline CPP_TYPE ToCpp( gmVariable& Var ){ return *(REMOVE_REF<CPP_TYPE>::Type*)((gmUserObject*)Var.m_value.m_ref)->m_user; }
	static inline gmVariable ToGm( CPP_TYPE Obj, gmMachine* VM, gmType Type )
	{ 
		gmUserObject* pGmObj = VM->AllocUserObject( &Obj, Type );
		pGmObj->m_properties = GM_IS_NATIVE;
		return gmVariable( pGmObj ); 
	}
};

template<typename CPP_TYPE> struct CONVERT<GM_OBJ_CONST_REF,CPP_TYPE>
{
	static inline CPP_TYPE ToCpp( gmVariable& Var ){ return *(REMOVE_REF<CPP_TYPE>::Type*)((gmUserObject*)Var.m_value.m_ref)->m_user; }
	static inline gmVariable ToGm( CPP_TYPE Obj, gmMachine* VM, gmType Type )
	{ 
		gmUserObject* pGmObj = VM->AllocUserObject( (void*)&Obj, Type );
		pGmObj->m_properties = GM_IS_NATIVE | GM_IS_CONST;
		return gmVariable( pGmObj ); 
	}
};

template<typename CPP_TYPE> struct CONVERT<GM_OBJ_PTR,CPP_TYPE>
{
	static inline CPP_TYPE ToCpp( gmVariable& Var ){ return (CPP_TYPE)((gmUserObject*)Var.m_value.m_ref)->m_user; }
	static inline gmVariable ToGm( CPP_TYPE pObj, gmMachine* VM, gmType Type )
	{
		gmUserObject* pGmObj = VM->AllocUserObject( pObj, Type );
		pGmObj->m_properties = GM_IS_NATIVE;
		return gmVariable( pGmObj );
	}
};

template<typename CPP_TYPE> struct CONVERT<GM_OBJ_CONST_PTR,CPP_TYPE>
{
	static inline CPP_TYPE ToCpp( gmVariable& Var ){ return (CPP_TYPE)((gmUserObject*)Var.m_value.m_ref)->m_user; }
	static inline gmVariable ToGm( CPP_TYPE pObj, gmMachine* VM, gmType Type )
	{
		gmUserObject* pGmObj = VM->AllocUserObject( (void*)pObj, Type );
		pGmObj->m_properties = GM_IS_NATIVE | GM_IS_CONST;
		return gmVariable( pGmObj );
	}
};

template<> struct CONVERT<GM_C_STRING,char*>
{
	static inline char* ToCpp( gmVariable& Var ){ return const_cast<char*>( ConvertType<const char*>(&Var) ); }
	static inline gmVariable ToGm( char* Text, gmMachine* VM, gmType )
	{
		gmUserObject* pGmObj = VM->AllocUserObject( Text, GM_C_STRING );
		pGmObj->m_properties = GM_IS_NATIVE;
		return gmVariable( pGmObj );
	}
};

template<> struct CONVERT<GM_CONST_C_STRING,const char*>
{
	static inline const char* ToCpp( gmVariable& Var ){ return ConvertType<const char*>(&Var); }
	static inline gmVariable ToGm( const char* Text, gmMachine* VM, gmType )
	{
		gmUserObject* pGmObj = VM->AllocUserObject( (void*)Text, GM_C_STRING );
		pGmObj->m_properties = GM_IS_NATIVE | GM_IS_CONST;
		return gmVariable( pGmObj );
	}
};

/*======================================================
  Templates qui permettent la compilation conditionnelle
========================================================*/
class gmFuncProxy;

template<bool Condition> struct IF
{
	template<typename TYPE> static inline void Push(TYPE, gmThread*){}

	template<typename DUMMY, typename CLASS, typename RET, typename P1> static inline void CallOpSetInd( DUMMY, CLASS*, P1, gmVariable&){}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET> static inline gmFuncProxy* CreateProxy0( FUNCTION pFunc)
	{
		return new _gmFuncProxy<FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1> static inline gmFuncProxy* CreateProxy1( FUNCTION pFunc)
	{
		return new _gmFuncProxy<FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2> static inline gmFuncProxy* CreateProxy2( FUNCTION pFunc)
	{
		return new _gmFuncProxy<FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3> static inline gmFuncProxy* CreateProxy3( FUNCTION pFunc)
	{
		return new _gmFuncProxy<FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> static inline gmFuncProxy* CreateProxy4( FUNCTION pFunc)
	{
		return new _gmFuncProxy<FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> static inline gmFuncProxy* CreateProxy5( FUNCTION pFunc)
	{
		return new _gmFuncProxy<FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4,P5>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> static inline gmFuncProxy* CreateProxy6( FUNCTION pFunc)
	{
		return new _gmFuncProxy<FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4,P5,P6>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> static inline gmFuncProxy* CreateProxy7( FUNCTION pFunc)
	{
		return new _gmFuncProxy<FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4,P5,P6,P7>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> static inline gmFuncProxy* CreateProxy8( FUNCTION pFunc)
	{
		return new _gmFuncProxy<FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> static inline gmFuncProxy* CreateProxy9( FUNCTION pFunc)
	{
		return new _gmFuncProxy<FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> static inline gmFuncProxy* CreateProxy10( FUNCTION pFunc)
	{
		return new _gmFuncProxy<FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>( pFunc );
	}
};


template<> struct IF<true>
{
	template<typename TYPE> static inline void Push(TYPE Param, gmThread* pThread ){ pThread->Push( CONVERT< GET<TYPE>::StaticId, TYPE >::ToGm( Param, pThread->GetMachine(), IS_BASIC_TYPE( GET<TYPE>::StaticId ) ? GET<TYPE>::StaticId : GetRuntimeId<TYPE>()  ) ); }

	template<typename OPERATOR, typename CLASS, typename RET, typename P1> static inline void CallOpSetInd( OPERATOR pOp, CLASS* pObj, P1 Param1, gmVariable& Var ){ (pObj->*pOp)(Param1) = CONVERT< GET<REMOVE_REF<RET>::Type>::StaticId, REMOVE_REF<RET>::Type >::ToCpp( Var ); }

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET> static inline gmFuncProxy* CreateProxy0( FUNCTION pFunc)
	{
		return new _gmFuncProxy<-FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1> static inline gmFuncProxy* CreateProxy1( FUNCTION pFunc)
	{
		return new _gmFuncProxy<-FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2> static inline gmFuncProxy* CreateProxy2( FUNCTION pFunc)
	{
		return new _gmFuncProxy<-FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3> static inline gmFuncProxy* CreateProxy3( FUNCTION pFunc)
	{
		return new _gmFuncProxy<-FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> static inline gmFuncProxy* CreateProxy4( FUNCTION pFunc)
	{
		return new _gmFuncProxy<-FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> static inline gmFuncProxy* CreateProxy5( FUNCTION pFunc)
	{
		return new _gmFuncProxy<-FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4,P5>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> static inline gmFuncProxy* CreateProxy6( FUNCTION pFunc)
	{
		return new _gmFuncProxy<-FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4,P5,P6>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> static inline gmFuncProxy* CreateProxy7( FUNCTION pFunc)
	{
		return new _gmFuncProxy<-FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4,P5,P6,P7>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> static inline gmFuncProxy* CreateProxy8( FUNCTION pFunc)
	{
		return new _gmFuncProxy<-FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> static inline gmFuncProxy* CreateProxy9( FUNCTION pFunc)
	{
		return new _gmFuncProxy<-FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9>( pFunc );
	}

	template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> static inline gmFuncProxy* CreateProxy10( FUNCTION pFunc)
	{
		return new _gmFuncProxy<-FUNCTION_CAT,CONSTNESS,NumParam, FUNCTION, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>( pFunc );
	}
};


template<int FUNCTION_CAT> struct SWITCH
{	
	template<typename DUMMY, class CLASS, typename RET> static gmVariable Call0Param( DUMMY, CLASS*, gmThread*, const gmType* ){ return gmVariable::s_null; }

	template<typename DUMMY, class CLASS, typename RET, typename P1> static gmVariable Call1Param( DUMMY, CLASS*, gmThread*, const gmType* ){ return gmVariable::s_null; }

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2> static gmVariable Call2Param( DUMMY, CLASS*, gmThread*, const gmType* ){ return gmVariable::s_null; }

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3> static gmVariable Call3Param( DUMMY, CLASS*, gmThread*, const gmType* ){ return gmVariable::s_null; }

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> static gmVariable Call4Param( DUMMY, CLASS*, gmThread*, const gmType* ){ return gmVariable::s_null; }

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> static gmVariable Call5Param( DUMMY, CLASS*, gmThread*, const gmType* ){ return gmVariable::s_null; }
	
	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> static gmVariable Call6Param( DUMMY, CLASS*, gmThread*, const gmType* ){ return gmVariable::s_null; }
	
	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> static gmVariable Call7Param( DUMMY, CLASS*, gmThread*, const gmType* ){ return gmVariable::s_null; }
	
	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> static gmVariable Call8Param( DUMMY, CLASS*, gmThread*, const gmType* ){ return gmVariable::s_null; }
	
	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> static gmVariable Call9Param( DUMMY, CLASS*, gmThread*, const gmType* ){ return gmVariable::s_null; }

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> static gmVariable Call10Param( DUMMY, CLASS*, gmThread*, const gmType* ){ return gmVariable::s_null; }

	
	template<typename OPERATOR, class CLASS, typename RET> static inline gmVariable CallOperator0( OPERATOR, gmVariable*, gmMachine*, const gmType* ){ return gmVariable::s_null; }
	template<typename OPERATOR, class CLASS, typename RET, typename P1> static inline gmVariable CallOperator1( OPERATOR, gmVariable*, gmMachine*, const gmType* ){ return gmVariable::s_null; }
	template<typename OPERATOR, class CLASS, typename RET, typename P1, typename P2> static inline gmVariable CallOperator2( OPERATOR, gmVariable*, gmMachine*, const gmType* ){ return gmVariable::s_null; }
};


/*=============================
  Cas des appels ・des variable
===============================*/

template<> struct SWITCH<GET_SET_ACCESS>
{
	template<class CLASS, typename TYPE> static inline gmVariable Get( CLASS* pObj, TYPE CLASS::*, const TYPE CLASS::*, TYPE (CLASS::*pGet)(), gmMachine* VM, gmType TypeId )
	{
		return CONVERT< GET<TYPE>::StaticId, TYPE >::ToGm( (pObj->*pGet)(), VM, TypeId );
	}

	template<class CLASS, typename TYPE> static inline void Set( CLASS* pObj, TYPE CLASS::*, const TYPE CLASS::*, void (CLASS::*pSet)(TYPE), gmVariable& Value )
	{
		GM_ASSERT( pSet); //Setter cannot be null
		if( pSet )
			(pObj->*pSet)( CONVERT< GET<TYPE>::StaticId, TYPE >::ToCpp( Value ) );
	}
};

template<> struct SWITCH<DIRECT_ACCESS>
{
	template<class CLASS, typename TYPE> static inline gmVariable Get( CLASS* pObj, TYPE CLASS::*pVar, const TYPE CLASS::*, TYPE (CLASS::*)(), gmMachine* VM, gmType TypeId )
	{
		return CONVERT< GET<TYPE>::StaticId, TYPE >::ToGm( pObj->*pVar, VM, TypeId );
	}

	template<class CLASS, typename TYPE> static inline void Set( CLASS* pObj, TYPE CLASS::*pVar, const TYPE CLASS::*, void (CLASS::*)(TYPE), gmVariable& Value )
	{
		pObj->*pVar = CONVERT< GET<TYPE>::StaticId, TYPE >::ToCpp( Value );
	}
};

template<> struct SWITCH<DIRECT_ACCESS_READ_ONLY>
{
	template<class CLASS, typename TYPE> static inline gmVariable Get( CLASS* pObj, TYPE CLASS::*, const TYPE CLASS::*pConstVar, TYPE (CLASS::*)(), gmMachine* VM, gmType TypeId )
	{
		return CONVERT< GET<TYPE>::StaticId, TYPE >::ToGm( pObj->*pConstVar, VM, TypeId );
	}

	template<class CLASS, typename TYPE> static inline void Set( CLASS* pObj, TYPE CLASS::*, const TYPE CLASS::*, void (CLASS::*)(TYPE), gmVariable& Value )
	{
		GM_ASSERT(0);// Setdot:read only variable
	}
};

/*=========================================
  Cas des appels ・des fonctions classiques
===========================================*/


template<> struct SWITCH<FUNCTION_TYPE>
{
	template<typename FUNCTION, class CLASS, typename RET> static gmVariable Call0Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (*pFunc)(), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1> static gmVariable Call1Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (*pFunc)(Param1), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2> static gmVariable Call2Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (*pFunc)(Param1,Param2), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3> static gmVariable Call3Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (*pFunc)(Param1,Param2,Param3), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> static gmVariable Call4Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (*pFunc)(Param1,Param2,Param3,Param4), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> static gmVariable Call5Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (*pFunc)(Param1,Param2,Param3,Param4,Param5), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> static gmVariable Call6Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (*pFunc)(Param1,Param2,Param3,Param4,Param5,Param6), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> static gmVariable Call7Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (*pFunc)(Param1,Param2,Param3,Param4,Param5,Param6,Param7), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> static gmVariable Call8Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );
		
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (*pFunc)(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> static gmVariable Call9Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );
		P9 Param9 = CONVERT< GET<P9>::StaticId, P9 >::ToCpp( pThread->Param(8) );
		
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (*pFunc)(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8,Param9), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> static gmVariable Call10Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );
		P9 Param9 = CONVERT< GET<P9>::StaticId, P9 >::ToCpp( pThread->Param(8) );
		P10 Param10 = CONVERT< GET<P10>::StaticId, P10 >::ToCpp( pThread->Param(9) );
		
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (*pFunc)(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8,Param9,Param10), pThread->GetMachine(), TypeIds[1] );
	}
};

/*===========================================================
  Cas des appels ・des fonctions ne retournant pas de valeurs
=============================================================*/

template<> struct SWITCH<FUNCTION_NORET_TYPE>
{
	template<typename FUNCTION, class CLASS, typename RET> static gmVariable Call0Param( FUNCTION pFunc, CLASS*, gmThread*, const gmType* )
	{
		(*pFunc)();
		return gmVariable::s_null;
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1> static gmVariable Call1Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );

		(*pFunc)(Param1);
		return gmVariable::s_null;
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2> static gmVariable Call2Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );

		(*pFunc)(Param1,Param2);
		return gmVariable::s_null;
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3> static gmVariable Call3Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );

		(*pFunc)(Param1,Param2,Param3);
		return gmVariable::s_null;
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> static gmVariable Call4Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );

		(*pFunc)(Param1,Param2,Param3,Param4);
		return gmVariable::s_null;
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> static gmVariable Call5Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );

		(*pFunc)(Param1,Param2,Param3,Param4,Param5);
		return gmVariable::s_null;
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> static gmVariable Call6Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );

		(*pFunc)(Param1,Param2,Param3,Param4,Param5,Param6);
		return gmVariable::s_null;
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> static gmVariable Call7Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );

		(*pFunc)(Param1,Param2,Param3,Param4,Param5,Param6,Param7);
		return gmVariable::s_null;
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> static gmVariable Call8Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );

		(*pFunc)(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8);
		return gmVariable::s_null;
	}


	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> static gmVariable Call9Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );
		P9 Param9 = CONVERT< GET<P9>::StaticId, P9 >::ToCpp( pThread->Param(8) );

		(*pFunc)(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8,Param9);
		return gmVariable::s_null;
	}

	template<typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> static gmVariable Call10Param( FUNCTION pFunc, CLASS*, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );
		P9 Param9 = CONVERT< GET<P9>::StaticId, P9 >::ToCpp( pThread->Param(8) );
		P10 Param10 = CONVERT< GET<P10>::StaticId, P10 >::ToCpp( pThread->Param(9) );

		(*pFunc)(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8,Param9,Param10);
		return gmVariable::s_null;
	}
};

/*========================================
  Cas des appels ・des m騁hodes classiques
==========================================*/

template<> struct SWITCH<METHOD_TYPE>
{
	template<typename METHOD, class CLASS, typename RET> static gmVariable Call0Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* TypeIds )
	{
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (pObj->*pMeth)(), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename METHOD, class CLASS, typename RET, typename P1> static gmVariable Call1Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );

		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (pObj->*pMeth)(Param1), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2> static gmVariable Call2Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );

		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (pObj->*pMeth)(Param1,Param2), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3> static gmVariable Call3Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );

		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (pObj->*pMeth)(Param1,Param2,Param3), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> static gmVariable Call4Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );

		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (pObj->*pMeth)(Param1,Param2,Param3,Param4), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> static gmVariable Call5Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );

		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (pObj->*pMeth)(Param1,Param2,Param3,Param4,Param5), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> static  gmVariable Call6Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );

		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (pObj->*pMeth)(Param1,Param2,Param3,Param4,Param5,Param6), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> static gmVariable Call7Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );

		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (pObj->*pMeth)(Param1,Param2,Param3,Param4,Param5,Param6,Param7), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> static gmVariable Call8Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );

		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (pObj->*pMeth)(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> static gmVariable Call9Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );
		P9 Param9 = CONVERT< GET<P9>::StaticId, P9 >::ToCpp( pThread->Param(8) );

		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (pObj->*pMeth)(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8,Param9), pThread->GetMachine(), TypeIds[1] );
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> static gmVariable Call10Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );
		P9 Param9 = CONVERT< GET<P9>::StaticId, P9 >::ToCpp( pThread->Param(8) );
		P10 Param10 = CONVERT< GET<P10>::StaticId, P10 >::ToCpp( pThread->Param(9) );

		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (pObj->*pMeth)(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8,Param9,Param10), pThread->GetMachine(), TypeIds[1] );
	}
};

/*==========================================================
  Cas des appels ・des m騁hodes ne retournant pas de valeurs
============================================================*/

template<> struct SWITCH<METHOD_NORET_TYPE>
{
	template<typename METHOD, class CLASS, typename RET> static gmVariable Call0Param( METHOD pMeth, CLASS* pObj, gmThread*, const gmType* )
	{
		(pObj->*pMeth)();
		return gmVariable::s_null;
	}

	template<typename METHOD, class CLASS, typename RET, typename P1> static gmVariable Call1Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );

		(pObj->*pMeth)(Param1);
		return gmVariable::s_null;
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2> static gmVariable Call2Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );

		(pObj->*pMeth)(Param1,Param2);
		return gmVariable::s_null;
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3> static gmVariable Call3Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );

		(pObj->*pMeth)(Param1,Param2,Param3);
		return gmVariable::s_null;
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> static gmVariable Call4Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );

		(pObj->*pMeth)(Param1,Param2,Param3,Param4);
		return gmVariable::s_null;
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> static gmVariable Call5Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );

		(pObj->*pMeth)(Param1,Param2,Param3,Param4,Param5);
		return gmVariable::s_null;
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> static gmVariable Call6Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );

		(pObj->*pMeth)(Param1,Param2,Param3,Param4,Param5,Param6);
		return gmVariable::s_null;
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> static gmVariable Call7Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );

		(pObj->*pMeth)(Param1,Param2,Param3,Param4,Param5,Param6,Param7);
		return gmVariable::s_null;
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> static gmVariable Call8Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );

		(pObj->*pMeth)(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8);
		return gmVariable::s_null;
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> static gmVariable Call9Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );
		P9 Param9 = CONVERT< GET<P9>::StaticId, P9 >::ToCpp( pThread->Param(8) );

		(pObj->*pMeth)(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8,Param9);
		return gmVariable::s_null;
	}

	template<typename METHOD, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> static gmVariable Call10Param( METHOD pMeth, CLASS* pObj, gmThread* pThread, const gmType* )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );
		P9 Param9 = CONVERT< GET<P9>::StaticId, P9 >::ToCpp( pThread->Param(8) );
		P10 Param10 = CONVERT< GET<P10>::StaticId, P10 >::ToCpp( pThread->Param(9) );

		(pObj->*pMeth)(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8,Param9,Param10);
		return gmVariable::s_null;
	}
};

/*=======================================
  Cas des appels ・des op駻ateurs membres
=========================================*/

template<> struct SWITCH<METHOD_OPERATOR_TYPE>
{
	template<typename OPERATOR, class CLASS, typename RET> static inline gmVariable CallOperator0( OPERATOR pOp, gmVariable* operand, gmMachine* VM, const gmType* TypeIds )
	{
		CLASS* pObj = (CLASS*)((gmUserObject*)operand->m_value.m_ref)->m_user;
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (pObj->*pOp)(), VM, TypeIds[1] );
	}

	template<typename OPERATOR, class CLASS, typename RET, typename P1> static gmVariable CallOperator1( OPERATOR pOp, gmVariable* operand, gmMachine* VM, const gmType* TypeIds )
	{ 
		CLASS* pObj = (CLASS*)((gmUserObject*)operand->m_value.m_ref)->m_user;

		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( operand[1] );

		if( operand->m_type > 0 )
			return CONVERT< GET<RET>::StaticId, RET >::ToGm( (pObj->*pOp)(Param1), VM, TypeIds[1] );
		else  //SetInd
		{
			IF< GET<RET>::StaticId == GM_OBJ_REF >::CallOpSetInd<OPERATOR,CLASS,RET,P1>( pOp, pObj, Param1, operand[2] );

			return gmVariable::s_null;
		}
	}
};



/*===========================================
  Cas des appels ・des op駻ateurs non membres
=============================================*/

template<> struct SWITCH<FUNCTION_OPERATOR_TYPE>
{
	template<typename OPERATOR, class CLASS, typename RET, typename P1> static inline gmVariable CallOperator1( OPERATOR pOp, gmVariable* operand, gmMachine* VM, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( operand[0] );
		
		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (*pOp)(Param1), VM, TypeIds[1] );
	}

	template<typename OPERATOR, class CLASS, typename RET, typename P1, typename P2> static inline gmVariable CallOperator2( OPERATOR pOp, gmVariable* operand, gmMachine* VM, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( operand[0] );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( operand[1] );

		return CONVERT< GET<RET>::StaticId, RET >::ToGm( (*pOp)(Param1,Param2), VM, TypeIds[1] );
	}
};



/*==================================
  Cas des appels ・des constructeurs
====================================*/

template<> struct SWITCH<CONSTRUCTOR_TYPE>
{
	template<typename DUMMY, class CLASS, typename RET> static gmVariable Call0Param( DUMMY, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		CLASS* pObj = new CLASS;

		if( pObj )
		{
			gmUserObject* pGmObj = pThread->GetMachine()->AllocUserObject( pObj, TypeIds[0] );
			if( pGmObj )
			{
				pThread->GetMachine()->AdjustKnownMemoryUsed( sizeof(CLASS) );
				return gmVariable( pGmObj );
			}
			else
				delete pObj;
		}
		
		return gmVariable::s_null;
	}

	template<typename DUMMY, class CLASS, typename RET, typename P1> static gmVariable Call1Param( DUMMY, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );

		CLASS* pObj = new CLASS(Param1);

		if( pObj )
		{
			gmUserObject* pGmObj = pThread->GetMachine()->AllocUserObject( pObj, TypeIds[0] );
			if( pGmObj )
			{
				pThread->GetMachine()->AdjustKnownMemoryUsed( sizeof(CLASS) );
				return gmVariable( pGmObj );
			}
			else
				delete pObj;
		}
		
		return gmVariable::s_null;
	}

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2> static gmVariable Call2Param( DUMMY, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );

		CLASS* pObj = new CLASS(Param1,Param2);

		if( pObj )
		{
			gmUserObject* pGmObj = pThread->GetMachine()->AllocUserObject( pObj, TypeIds[0] );
			if( pGmObj )
			{
				pThread->GetMachine()->AdjustKnownMemoryUsed( sizeof(CLASS) );
				return gmVariable( pGmObj );
			}
			else
				delete pObj;
		}
		
		return gmVariable::s_null;
	}

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3> static gmVariable Call3Param( DUMMY, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );

		CLASS* pObj = new CLASS(Param1,Param2,Param3);

		if( pObj )
		{
			gmUserObject* pGmObj = pThread->GetMachine()->AllocUserObject( pObj, TypeIds[0] );
			if( pGmObj )
			{
				pThread->GetMachine()->AdjustKnownMemoryUsed( sizeof(CLASS) );
				return gmVariable( pGmObj );
			}
			else
				delete pObj;
		}
		
		return gmVariable::s_null;
	}

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> static gmVariable Call4Param( DUMMY, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );

		CLASS* pObj = new CLASS(Param1,Param2,Param3,Param4);

		if( pObj )
		{
			gmUserObject* pGmObj = pThread->GetMachine()->AllocUserObject( pObj, TypeIds[0] );
			if( pGmObj )
			{
				pThread->GetMachine()->AdjustKnownMemoryUsed( sizeof(CLASS) );
				return gmVariable( pGmObj );
			}
			else
				delete pObj;
		}
		
		return gmVariable::s_null;
	}

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> static gmVariable Call5Param( DUMMY, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );

		CLASS* pObj = new CLASS(Param1,Param2,Param3,Param4,Param5);

		if( pObj )
		{
			gmUserObject* pGmObj = pThread->GetMachine()->AllocUserObject( pObj, TypeIds[0] );
			if( pGmObj )
			{
				pThread->GetMachine()->AdjustKnownMemoryUsed( sizeof(CLASS) );
				return gmVariable( pGmObj );
			}
			else
				delete pObj;
		}
		
		return gmVariable::s_null;
	}

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> static gmVariable Call6Param( DUMMY, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );

		CLASS* pObj = new CLASS(Param1,Param2,Param3,Param4,Param5,Param6);

		if( pObj )
		{
			gmUserObject* pGmObj = pThread->GetMachine()->AllocUserObject( pObj, TypeIds[0] );
			if( pGmObj )
			{
				pThread->GetMachine()->AdjustKnownMemoryUsed( sizeof(CLASS) );
				return gmVariable( pGmObj );
			}
			else
				delete pObj;
		}
		
		return gmVariable::s_null;
	}

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> static gmVariable Call7Param( DUMMY, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );

		CLASS* pObj = new CLASS(Param1,Param2,Param3,Param4,Param5,Param6,Param7);

		if( pObj )
		{
			gmUserObject* pGmObj = pThread->GetMachine()->AllocUserObject( pObj, TypeIds[0] );
			if( pGmObj )
			{
				pThread->GetMachine()->AdjustKnownMemoryUsed( sizeof(CLASS) );
				return gmVariable( pGmObj );
			}
			else
				delete pObj;
		}
		
		return gmVariable::s_null;
	}

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> static gmVariable Call8Param( DUMMY, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );

		CLASS* pObj = new CLASS(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8);

		if( pObj )
		{
			gmUserObject* pGmObj = pThread->GetMachine()->AllocUserObject( pObj, TypeIds[0] );
			if( pGmObj )
			{
				pThread->GetMachine()->AdjustKnownMemoryUsed( sizeof(CLASS) );
				return gmVariable( pGmObj );
			}
			else
				delete pObj;
		}
		
		return gmVariable::s_null;
	}

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> static gmVariable Call9Param( DUMMY, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );
		P9 Param9 = CONVERT< GET<P9>::StaticId, P9 >::ToCpp( pThread->Param(8) );

		CLASS* pObj = new CLASS(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8,Param9);

		if( pObj )
		{
			gmUserObject* pGmObj = pThread->GetMachine()->AllocUserObject( pObj, TypeIds[0] );
			if( pGmObj )
			{
				pThread->GetMachine()->AdjustKnownMemoryUsed( sizeof(CLASS) );
				return gmVariable( pGmObj );
			}
			else
				delete pObj;
		}
		
		return gmVariable::s_null;
	}

	template<typename DUMMY, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> static gmVariable Call10Param( DUMMY, CLASS*, gmThread* pThread, const gmType* TypeIds )
	{
		P1 Param1 = CONVERT< GET<P1>::StaticId, P1 >::ToCpp( pThread->Param(0) );
		P2 Param2 = CONVERT< GET<P2>::StaticId, P2 >::ToCpp( pThread->Param(1) );
		P3 Param3 = CONVERT< GET<P3>::StaticId, P3 >::ToCpp( pThread->Param(2) );
		P4 Param4 = CONVERT< GET<P4>::StaticId, P4 >::ToCpp( pThread->Param(3) );
		P5 Param5 = CONVERT< GET<P5>::StaticId, P5 >::ToCpp( pThread->Param(4) );
		P6 Param6 = CONVERT< GET<P6>::StaticId, P6 >::ToCpp( pThread->Param(5) );
		P7 Param7 = CONVERT< GET<P7>::StaticId, P7 >::ToCpp( pThread->Param(6) );
		P8 Param8 = CONVERT< GET<P8>::StaticId, P8 >::ToCpp( pThread->Param(7) );
		P9 Param9 = CONVERT< GET<P9>::StaticId, P9 >::ToCpp( pThread->Param(8) );
		P10 Param10 = CONVERT< GET<P10>::StaticId, P10 >::ToCpp( pThread->Param(9) );

		CLASS* pObj = new CLASS(Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8,Param9,Param10);

		if( pObj )
		{
			gmUserObject* pGmObj = pThread->GetMachine()->AllocUserObject( pObj, TypeIds[0] );
			if( pGmObj )
			{
				pThread->GetMachine()->AdjustKnownMemoryUsed( sizeof(CLASS) );
				return gmVariable( pGmObj );
			}
			else
				delete pObj;
		}
		
		return gmVariable::s_null;
	}
};


/*=============================================
 Template definissant un destructeur par defaut
===============================================*/

void GM_CDECL gmExIntermediateDestructor( gmMachine* VM, gmUserObject* pGmObj );

template<class CLASS> void GM_CDECL gmExDefaultDestructor( gmMachine* VM, gmUserObject* pGmObj )
{
	delete (CLASS*)pGmObj->m_user;
	VM->AdjustKnownMemoryUsed( - (int)(sizeof(CLASS)) );
}

template<class CLASS> gmGCDestructCallback CreateDestructor( gmGCDestructCallback pFunc = NULL )
{
	if( pFunc )
		return pFunc;
	else
		return &gmExDefaultDestructor<CLASS>;
}



template<typename TYPE> struct IS
{
	static const int Const = ( GET<TYPE>::StaticId == GM_OBJ ? true : false );
};

template<typename TYPE> struct IS<TYPE*>
{
	static const int Const = false;
};

template<typename TYPE> struct IS<const TYPE*>
{
	static const int Const = true;
};

template<typename TYPE> struct IS<TYPE&>
{
	static const int Const = false;
};

template<typename TYPE> struct IS<const TYPE&>
{
	static const int Const = true;
};

template<class CLASS> struct ADJUST
{
	typedef CLASS* TYPE;
};

template<class CLASS> struct ADJUST<CLASS*>
{
	typedef CLASS* TYPE;
};

template<class CLASS> struct ADJUST<CLASS&>
{
	typedef CLASS* TYPE;
};

template<class CLASS> struct REMOVE_REF
{
	typedef CLASS Type;
};

template<class CLASS> struct REMOVE_REF<CLASS&>
{
	typedef CLASS Type;
};

template<class DUMMY, typename OPERATOR> bool isUnary( OPERATOR Op ) //la classe factice est la car VC++ est bugg・
{
	return _isUnary( Op );
};

template<typename RET, typename P1> bool _isUnary( RET (*)(P1) )
{
	return true;
};

template<typename RET, typename P1, typename P2> bool _isUnary( RET (*)(P1,P2) )
{
	return false;
};

template<class CLASS, typename RET> bool _isUnary( RET (CLASS::*)() )
{
	return true;
};

template<class CLASS, typename RET> bool _isUnary( RET (CLASS::*)() const )
{
	return true;
};

template<class CLASS, typename RET, typename P1> bool _isUnary( RET (CLASS::*)(P1) )
{
	return false;
};

template<class CLASS, typename RET, typename P1> bool _isUnary( RET (CLASS::*)(P1) const )
{
	return false;
};

#endif

