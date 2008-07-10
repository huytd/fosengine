/*===============================
*** Written By Seth/Nightmare ***
=================================*/

#ifndef GM_MACHINE_EX_H
#define GM_MACHINE_EX_H

#pragma warning( disable: 4127 )
#pragma warning( disable: 4244 )
#pragma warning( disable: 4800 )

#include <map>
#include <vector>
#include <typeinfo>
#include "../GameMonkey/gmThread.h"

template<typename TYPE> TYPE ConvertType( const gmVariable* );
template<> int ConvertType<int>( const gmVariable* );
template<> float ConvertType<float>( const gmVariable* );
template<> const char* ConvertType<const char*>( const gmVariable* );

#define IS_BASIC_TYPE(Type) ( Type >= GM_NULL && Type <= GM_USER )
#define GM_IS_NATIVE	( 1 << 1 )
#define GM_IS_CONST		( 1 << 2 )

#include "gmTmp.h"
//#include "gmCout.h"
//#include "gmScan.h"
#include "gmExVariable.h"
//#include "gmFileStream.h"
#include "gmExOperator.h"
#include "gmFunctionProxy.h"
#include "gmVariableProxy.h"

struct gmClassDef
{
	gmClassDef(){ pTypeInfo = NULL; pConstTypeInfo = NULL; BaseClass = NULL; BaseClassType = GM_NULL; }

	gmType BaseClassType;
	const gmClassDef* BaseClass;
	gmGCDestructCallback pDestructor;

	const type_info* pTypeInfo;
	const type_info* pConstTypeInfo;

	std::map<int,gmVarProxy*> VarList;
	std::map<gmOperator,gmFuncProxy*> OpList;
};

#define GLOBAL_NAMESPACE  false, NULL
#define NAMESPACE( Name ) false, Name
#define CLASS( Name )	  true,  Name, &typeid(Typename*), &typeid(const Typename*)
#define STRUCTURE( Name ) CLASS( Name )
#define UNION( Name )	  CLASS( Name )
#define END				  NULL,  END_VIRTUAL_TYPE

#define FUNCTION( Func, ... )			CreateFunctionProxy<__VA_ARGS__>( &Func ), FUNCTION_TYPE, #Func  //impossible de mettre FUNCTION_TYPE avant le proxy car VC++ est bugge
#define METHOD( Meth, ... )				CreateMethodProxy<__VA_ARGS__>( &Typename::Meth ), METHOD_TYPE, #Meth
#define VARIABLE( Var, ... )			CreateVariableProxy(&Typename::Var,__VA_ARGS__), VARIABLE_TYPE, #Var
#define VARIABLE_GETSET( Var, ... )		CreateGetSetVariableProxy(__VA_ARGS__), VARIABLE_TYPE, Var
#define CONSTRUCTOR( ... )				CreateConstructorProxy<Typename,__VA_ARGS__>(), CONSTRUCTOR_TYPE
#define DESTRUCTOR( ... )				CreateDestructor<Typename>(__VA_ARGS__), DESTRUCTOR_VIRTUAL_TYPE
#define AS_STRING( Callback )			Callback, ASSTRING_VIRTUAL_TYPE
#define OPERATOR( Op, ... )				CreateOperatorProxy<__VA_ARGS__>( &Typename::operator##Op ), METHOD_OPERATOR_TYPE, gmMachineEx::CppOpToGm(#Op, isUnary<void,__VA_ARGS__>(&Typename::operator##Op) )
#define NM_OPERATOR( Op, ... )			CreateOperatorProxy<__VA_ARGS__>( &operator##Op ), FUNCTION_OPERATOR_TYPE, gmMachineEx::CppOpToGm(#Op, isUnary<void,__VA_ARGS__>(&operator##Op) )
#define POOL( Get, Free )				CreateFunctionProxy( &Get ), POOL_VIRTUAL_TYPE, &Free
#define BASE_CLASS( Class )				&typeid(Class*), BASECLASS_VIRTUAL_TYPE    

/*!\brief An enhanced version of gmMachine
 * \see gmMachine
 * This machine has the ability to bind C++ class and function to gameMonkey
 * It also have extended thread function
 * For more information, refer to gmMachineEx.pdf
 */
class gmMachineEx : public gmMachine
{
public:
	gmMachineEx( bool UseExtendedThreadFunction = false );
	virtual ~gmMachineEx();
	
	/*!\brief The command where all the magic begin
	 * For more information, refer to gmMachineEx.pdf
	 */
	gmType Bind( bool isClass, const char* Name, ...);

	/// \brief Update all type ids. Remember to call it after a lot of class binds
	void UpdateTypeIds();
	inline gmExVariable Call(gmFunctionObject* Func)																																																																{ return _CallFunc<0,int,int,int,int,int,int,int,int,int,int>( Func, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, gmVariable::s_null ); }
	template<typename P1> inline gmExVariable Call(gmFunctionObject* Func,P1 Param1)																																																								{ return _CallFunc<1,P1,int,int,int,int,int,int,int,int,int>( Func, Param1, 0, 0, 0, 0, 0, 0, 0, 0, 0, gmVariable::s_null ); }
	template<typename P1, typename P2> inline gmExVariable Call(gmFunctionObject* Func, P1 Param1, P2 Param2)																																																		{ return _CallFunc<2,P1,P2,int,int,int,int,int,int,int,int>( Func, Param1, Param2, 0, 0, 0, 0, 0, 0, 0, 0, gmVariable::s_null ); }
	template<typename P1, typename P2, typename P3> inline gmExVariable Call(gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3)																																												{ return _CallFunc<3,P1,P2,P3,int,int,int,int,int,int,int>( Func, Param1, Param2, Param3, 0, 0, 0, 0, 0, 0, 0, gmVariable::s_null ); }
	template<typename P1, typename P2, typename P3, typename P4> inline gmExVariable Call(gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4)																																						{ return _CallFunc<4,P1,P2,P3,P4,int,int,int,int,int,int>( Func, Param1, Param2, Param3, Param4, 0, 0, 0, 0, 0, 0, gmVariable::s_null ); }
	template<typename P1, typename P2, typename P3, typename P4, typename P5> inline gmExVariable Call(gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5)																																{ return _CallFunc<5,P1,P2,P3,P4,P5,int,int,int,int,int>( Func, Param1, Param2, Param3, Param4, Param5, 0, 0, 0, 0, 0, gmVariable::s_null ); }
	template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> inline gmExVariable Call(gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5, P6 Param6)																										{ return _CallFunc<6,P1,P2,P3,P4,P5,P6,int,int,int,int>( Func, Param1, Param2, Param3, Param4, Param5, Param6, 0, 0, 0, 0, gmVariable::s_null ); }
	template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> inline gmExVariable Call(gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5, P6 Param6, P7 Param7)																				{ return _CallFunc<7,P1,P2,P3,P4,P5,P6,P7,int,int,int>( Func, Param1, Param2, Param3, Param4, Param5, Param6, Param7, 0, 0, 0, gmVariable::s_null ); }
	template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> inline gmExVariable Call(gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5, P6 Param6, P7 Param7, P8 Param8)														{ return _CallFunc<8,P1,P2,P3,P4,P5,P6,P7,P8,int,int>( Func, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, 0, 0, gmVariable::s_null ); }
	template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> inline gmExVariable Call(gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5, P6 Param6, P7 Param7, P8 Param8, P9 Param9)								{ return _CallFunc<9,P1,P2,P3,P4,P5,P6,P7,P8,P9,int>( Func, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, 0, gmVariable::s_null ); }
	template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> inline gmExVariable Call(gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5, P6 Param6, P7 Param7, P8 Param8, P9 Param9, P10 Param10)	{ return _CallFunc<10,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>( Func, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, gmVariable::s_null ); }

	template<typename THIS> inline gmExVariable Call(THIS This, gmFunctionObject* Func)																																																														{ return _CallFunc<0,int,int,int,int,int,int,int,int,int,int>( Func, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CONVERT< GET<THIS>::Type, THIS >::ToGm(This,this, ( GET<THIS>::Type < GM_USER ) ? GET<THIS>::Type : GetTypeId<THIS>() ) ); }
	template<typename THIS, typename P1> inline gmExVariable Call(THIS This, gmFunctionObject* Func,P1 Param1)																																																								{ return _CallFunc<1,P1,int,int,int,int,int,int,int,int,int>( Func, Param1, 0, 0, 0, 0, 0, 0, 0, 0, 0, CONVERT< GET<THIS>::Type, THIS >::ToGm(This,this, ( GET<THIS>::Type < GM_USER ) ? GET<THIS>::Type : GetTypeId<THIS>() ) ); }
	template<typename THIS, typename P1, typename P2> inline gmExVariable Call(THIS This, gmFunctionObject* Func, P1 Param1, P2 Param2)																																																		{ return _CallFunc<2,P1,P2,int,int,int,int,int,int,int,int>( Func, Param1, Param2, 0, 0, 0, 0, 0, 0, 0, 0, CONVERT< GET<THIS>::Type, THIS >::ToGm(This,this, ( GET<THIS>::Type < GM_USER ) ? GET<THIS>::Type : GetTypeId<THIS>() ) ); }
	template<typename THIS, typename P1, typename P2, typename P3> inline gmExVariable Call(THIS This, gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3)																																												{ return _CallFunc<3,P1,P2,P3,int,int,int,int,int,int,int>( Func, Param1, Param2, Param3, 0, 0, 0, 0, 0, 0, 0, CONVERT< GET<THIS>::Type, THIS >::ToGm(This,this, ( GET<THIS>::Type < GM_USER ) ? GET<THIS>::Type : GetTypeId<THIS>() ) ); }
	template<typename THIS, typename P1, typename P2, typename P3, typename P4> inline gmExVariable Call(THIS This, gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4)																																						{ return _CallFunc<4,P1,P2,P3,P4,int,int,int,int,int,int>( Func, Param1, Param2, Param3, Param4, 0, 0, 0, 0, 0, 0, CONVERT< GET<THIS>::Type, THIS >::ToGm(This,this, ( GET<THIS>::Type < GM_USER ) ? GET<THIS>::Type : GetTypeId<THIS>() ) ); }
	template<typename THIS, typename P1, typename P2, typename P3, typename P4, typename P5> inline gmExVariable Call(THIS This, gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5)																																{ return _CallFunc<5,P1,P2,P3,P4,P5,int,int,int,int,int>( Func, Param1, Param2, Param3, Param4, Param5, 0, 0, 0, 0, 0, CONVERT< GET<THIS>::Type, THIS >::ToGm(This,this, ( GET<THIS>::Type < GM_USER ) ? GET<THIS>::Type : GetTypeId<THIS>() ) ); }
	template<typename THIS, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> inline gmExVariable Call(THIS This, gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5, P6 Param6)																										{ return _CallFunc<6,P1,P2,P3,P4,P5,P6,int,int,int,int>( Func, Param1, Param2, Param3, Param4, Param5, Param6, 0, 0, 0, 0, CONVERT< GET<THIS>::Type, THIS >::ToGm(This,this, ( GET<THIS>::Type < GM_USER ) ? GET<THIS>::Type : GetTypeId<THIS>() ) ); }
	template<typename THIS, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> inline gmExVariable Call(THIS This, gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5, P6 Param6, P7 Param7)																				{ return _CallFunc<7,P1,P2,P3,P4,P5,P6,P7,int,int,int>( Func, Param1, Param2, Param3, Param4, Param5, Param6, Param7, 0, 0, 0, CONVERT< GET<THIS>::Type, THIS >::ToGm(This,this, ( GET<THIS>::Type < GM_USER ) ? GET<THIS>::Type : GetTypeId<THIS>() ) ); }
	template<typename THIS, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> inline gmExVariable Call(THIS This, gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5, P6 Param6, P7 Param7, P8 Param8)														{ return _CallFunc<8,P1,P2,P3,P4,P5,P6,P7,P8,int,int>( Func, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, 0, 0, CONVERT< GET<THIS>::Type, THIS >::ToGm(This,this, ( GET<THIS>::Type < GM_USER ) ? GET<THIS>::Type : GetTypeId<THIS>() ) ); }
	template<typename THIS, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> inline gmExVariable Call(THIS This, gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5, P6 Param6, P7 Param7, P8 Param8, P9 Param9)								{ return _CallFunc<9,P1,P2,P3,P4,P5,P6,P7,P8,P9,int>( Func, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, 0, CONVERT< GET<THIS>::Type, THIS >::ToGm(This,this, ( GET<THIS>::Type < GM_USER ) ? GET<THIS>::Type : GetTypeId<THIS>() ) ); }
	template<typename THIS, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> inline gmExVariable Call(THIS This, gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5, P6 Param6, P7 Param7, P8 Param8, P9 Param9, P10 Param10)	{ return _CallFunc<10,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>( Func, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, CONVERT< GET<THIS>::Type, THIS >::ToGm(This,this, ( GET<THIS>::Type < GM_USER ) ? GET<THIS>::Type : GetTypeId<THIS>() ) ); }

	static std::map<gmType,gmClassDef*> ClassList;
	static int GM_CDECL FunctionDispatcher( gmThread* );

	static std::multimap<int,gmThread*> _ThreadList_AtCreate;
	static std::multimap<int,gmThread*> _ThreadList_AtDestroy;

	static int  GM_CDECL gmThreadEx(gmThread* pthread);
	static bool OnCreateThread( const void* pContext );
	static bool OnDestroyThread( const void* pContext );
	static bool MachineCallback( gmMachine*, gmMachineCommand Command, const void* pContext);

	static void GM_CDECL OpGetDot(gmThread*, gmVariable*);
	static void GM_CDECL OpSetDot(gmThread*, gmVariable*);

	static void GM_CDECL gmCStringAsStringCallback(gmUserObject * a_userObj, char * a_buffer, int a_bufferSize);
	static gmOperator CppOpToGm( const char*, bool );

private:
	template<int NumParam, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> inline gmExVariable _CallFunc( gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5, P6 Param6, P7 Param7, P8 Param8, P9 Param9, P10 Param10, const gmVariable& This );
	gmThread* _BeginFuncCall( gmFunctionObject* func, const gmVariable& This );
	gmExVariable _EndFuncCall( gmThread* pThread, int ParamNumber );
	std::vector<gmFuncProxy*> _ProxyList;
	int AT_CREATE, AT_DESTROY, AT_TIME;
	gmUserObject *AtCreate, *AtDestroy, *AtTime;

};

int GM_CDECL gmGet( gmThread* );
int GM_CDECL gmSet( gmThread* );
int GM_CDECL gmConstCast( gmThread* );
gmType GetRuntimeId( const type_info* );
int GM_CDECL gmDelete( gmThread* pThread );
int GM_CDECL gmIntermediateGet( gmThread* pThread );
inline int CompareParam( gmType GmParamType, gmType CppParamType );

template<class CLASS> inline gmType GetRuntimeId()
{
	const type_info* pThisInfo = &typeid( ADJUST<CLASS>::TYPE );
	return GetRuntimeId( pThisInfo );
};

template<int NumParam, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> inline gmExVariable gmMachineEx::_CallFunc( gmFunctionObject* Func, P1 Param1, P2 Param2, P3 Param3, P4 Param4, P5 Param5, P6 Param6, P7 Param7, P8 Param8, P9 Param9, P10 Param10, const gmVariable& This )
{
	gmThread* pThread = _BeginFuncCall( Func, This );

	if( pThread )
	{
		IF< ( NumParam > 0 ) >::Push<P1>( Param1, pThread );
		IF< ( NumParam > 1 ) >::Push<P2>( Param2, pThread );
		IF< ( NumParam > 2 ) >::Push<P3>( Param3, pThread );
		IF< ( NumParam > 3 ) >::Push<P4>( Param4, pThread );
		IF< ( NumParam > 4 ) >::Push<P5>( Param5, pThread );
		IF< ( NumParam > 5 ) >::Push<P6>( Param6, pThread );
		IF< ( NumParam > 6 ) >::Push<P7>( Param7, pThread );
		IF< ( NumParam > 7 ) >::Push<P8>( Param8, pThread );
		IF< ( NumParam > 8 ) >::Push<P9>( Param9, pThread );
		IF< ( NumParam > 9 ) >::Push<P10>( Param10, pThread );
	}

	return _EndFuncCall( pThread, NumParam );
}

#pragma warning( default: 4127 )
#pragma warning( default: 4244 )
#pragma warning( default: 4800 )

#endif