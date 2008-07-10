#ifndef GM_FUNCTION_PROXY_H
#define GM_FUNCTION_PROXY_H

class gmFuncProxy
{
public:
	virtual ~gmFuncProxy(){}
	virtual bool isConst() = 0;
	virtual void UpdateTypeIds() = 0;
	virtual const gmType* GetTypeIds() const = 0;
	virtual int GetFunctionCat() const = 0;
	virtual bool CheckNumParams( int ParamsNumber ) const = 0;
	virtual gmVariable operator()(gmThread* pThread, void* pObj) const = 0;
	virtual gmVariable operator()(gmVariable* operand, gmMachine*) const = 0;
	
	gmFuncProxy* pNextOverloaded;
};


template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS = void, typename RET = void, typename P1 = void, typename P2 = void, typename P3 = void, typename P4 = void, typename P5 = void, typename P6 = void, typename P7 = void, typename P8 = void, typename P9 = void, typename P10 = void> class _gmFuncProxy : public gmFuncProxy
{
public:
	_gmFuncProxy( FUNCTION pFunc )
	{
		this->pFunc = pFunc;
		pNextOverloaded = NULL;

		for( int i = 0; i < NumParam+2; ++i )
			TypeId[i] = GM_UNKNOWN;
		UpdateTypeIds();//TODO: remove this shit
	}

	gmVariable operator()(gmThread* pThread, void* pObj) const
	{
		gmVariable Result = gmVariable::s_null;
		int ParamNumber = pThread->GetNumParams();

		switch( ParamNumber )
		{
		case 0:
			Result = SWITCH< NumParam == 0 ? ( ( FUNCTION_CAT != FUNCTION_OPERATOR_TYPE && FUNCTION_CAT != METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::Call0Param<FUNCTION,CLASS,RET>( pFunc, (CLASS*)pObj, pThread, TypeId ); break;
		case 1:
			Result = SWITCH< NumParam == 1 ? ( ( FUNCTION_CAT != FUNCTION_OPERATOR_TYPE && FUNCTION_CAT != METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::Call1Param<FUNCTION,CLASS,RET,P1>( pFunc, (CLASS*)pObj, pThread, TypeId ); break;
		case 2:
			Result = SWITCH< NumParam == 2 ? ( ( FUNCTION_CAT != FUNCTION_OPERATOR_TYPE && FUNCTION_CAT != METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::Call2Param<FUNCTION,CLASS,RET,P1,P2>( pFunc, (CLASS*)pObj, pThread, TypeId ); break;
		case 3:
			Result = SWITCH< NumParam == 3 ? ( ( FUNCTION_CAT != FUNCTION_OPERATOR_TYPE && FUNCTION_CAT != METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::Call3Param<FUNCTION,CLASS,RET,P1,P2,P3>( pFunc, (CLASS*)pObj, pThread, TypeId ); break;
		case 4:
			Result = SWITCH< NumParam == 4 ? ( ( FUNCTION_CAT != FUNCTION_OPERATOR_TYPE && FUNCTION_CAT != METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::Call4Param<FUNCTION,CLASS,RET,P1,P2,P3,P4>( pFunc, (CLASS*)pObj, pThread, TypeId ); break;
		case 5:
			Result = SWITCH< NumParam == 5 ? ( ( FUNCTION_CAT != FUNCTION_OPERATOR_TYPE && FUNCTION_CAT != METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::Call5Param<FUNCTION,CLASS,RET,P1,P2,P3,P4,P5>( pFunc, (CLASS*)pObj, pThread, TypeId ); break;
		case 6:
			Result = SWITCH< NumParam == 6 ? ( ( FUNCTION_CAT != FUNCTION_OPERATOR_TYPE && FUNCTION_CAT != METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::Call6Param<FUNCTION,CLASS,RET,P1,P2,P3,P4,P5,P6>( pFunc, (CLASS*)pObj, pThread, TypeId ); break;
		case 7:
			Result = SWITCH< NumParam == 7 ? ( ( FUNCTION_CAT != FUNCTION_OPERATOR_TYPE && FUNCTION_CAT != METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::Call7Param<FUNCTION,CLASS,RET,P1,P2,P3,P4,P5,P6,P7>( pFunc, (CLASS*)pObj, pThread, TypeId ); break;
		case 8:
			Result = SWITCH< NumParam == 8 ? ( ( FUNCTION_CAT != FUNCTION_OPERATOR_TYPE && FUNCTION_CAT != METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::Call8Param<FUNCTION,CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8>( pFunc, (CLASS*)pObj, pThread, TypeId ); break;
		case 9:
			Result = SWITCH< NumParam == 9 ? ( ( FUNCTION_CAT != FUNCTION_OPERATOR_TYPE && FUNCTION_CAT != METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::Call9Param<FUNCTION,CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9>( pFunc, (CLASS*)pObj, pThread, TypeId ); break;		
		case 10:
			Result = SWITCH< NumParam == 10 ? ( ( FUNCTION_CAT != FUNCTION_OPERATOR_TYPE && FUNCTION_CAT != METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::Call10Param<FUNCTION,CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>( pFunc, (CLASS*)pObj, pThread, TypeId );		
		}

		return Result;
	}

	gmVariable operator()(gmVariable* operand, gmMachine* VM) const
	{
		switch( NumParam )
		{
		case 0:
			return SWITCH< NumParam == 0 ? ( ( FUNCTION_CAT == FUNCTION_OPERATOR_TYPE || FUNCTION_CAT == METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::CallOperator0<FUNCTION,CLASS,RET>( pFunc, operand, VM, TypeId );
		case 1:
			return SWITCH< NumParam == 1 ? ( ( FUNCTION_CAT == FUNCTION_OPERATOR_TYPE || FUNCTION_CAT == METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::CallOperator1<FUNCTION,CLASS,RET,P1>( pFunc, operand, VM, TypeId );
		case 2:
			return SWITCH< NumParam == 2 ? ( ( FUNCTION_CAT == FUNCTION_OPERATOR_TYPE || FUNCTION_CAT == METHOD_OPERATOR_TYPE ) ? FUNCTION_CAT : 0 ) : 0 >::CallOperator2<FUNCTION,CLASS,RET,P1,P2>( pFunc, operand, VM, TypeId );
		}

		return gmVariable::s_null;
	}

	void UpdateTypeIds();
	bool isConst(){ return CONSTNESS; }
	const gmType* GetTypeIds() const { return &TypeId[2]; }
	int GetFunctionCat() const { return FUNCTION_CAT; }
	bool CheckNumParams( int ParamsNumber ) const { return ParamsNumber == NumParam; }

private:
	FUNCTION pFunc;
	gmType TypeId[1+1+NumParam]; //Class,Ret,Params
};


template<int FUNCTION_CAT, bool CONSTNESS, int NumParam, typename FUNCTION, class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> void _gmFuncProxy<FUNCTION_CAT,CONSTNESS,NumParam,FUNCTION,CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>::UpdateTypeIds()
{
	if( TypeId[0] == GM_UNKNOWN ) TypeId[0] = GetRuntimeId<CLASS>();
	if( TypeId[1] == GM_UNKNOWN ) TypeId[1] = IS_BASIC_TYPE( GET<RET>::StaticId ) ? GET<RET>::StaticId : GetRuntimeId<RET>();

	if( NumParam > 0 && TypeId[2] == GM_UNKNOWN )
	{
		TypeId[2] = IS_BASIC_TYPE( GET<P1>::StaticId ) ? GET<P1>::StaticId : GetRuntimeId<P1>();
		if( TypeId[2] != GM_UNKNOWN )
			TypeId[2] = IS<P1>::Const ? -TypeId[2] : TypeId[2];
	}

	if( NumParam > 1 && TypeId[3] == GM_UNKNOWN )
	{
		TypeId[3] = IS_BASIC_TYPE( GET<P2>::StaticId ) ? GET<P2>::StaticId : GetRuntimeId<P2>();
		if( TypeId[3] != GM_UNKNOWN )
			TypeId[3] = IS<P2>::Const ? -TypeId[3] : TypeId[3];
	}

	if( NumParam > 2 && TypeId[4] == GM_UNKNOWN )
	{
		TypeId[4] = IS_BASIC_TYPE( GET<P3>::StaticId ) ? GET<P3>::StaticId : GetRuntimeId<P3>();
		if( TypeId[4] != GM_UNKNOWN )
			TypeId[4] = IS<P3>::Const ? -TypeId[4] : TypeId[4];
	}

	if( NumParam > 3 && TypeId[5] == GM_UNKNOWN )
	{
		TypeId[5] = IS_BASIC_TYPE( GET<P4>::StaticId ) ? GET<P4>::StaticId : GetRuntimeId<P4>();
		if( TypeId[5] != GM_UNKNOWN )
			TypeId[5] = IS<P4>::Const ? -TypeId[5] : TypeId[5];
	}

	if( NumParam > 4 && TypeId[6] == GM_UNKNOWN )
	{
		TypeId[6] = IS_BASIC_TYPE( GET<P5>::StaticId ) ? GET<P5>::StaticId : GetRuntimeId<P5>();
		if( TypeId[6] != GM_UNKNOWN )
			TypeId[6] = IS<P5>::Const ? -TypeId[6] : TypeId[6];
	}

	if( NumParam > 5 && TypeId[7] == GM_UNKNOWN )
	{
		TypeId[7] = IS_BASIC_TYPE( GET<P6>::StaticId ) ? GET<P6>::StaticId : GetRuntimeId<P6>();
		if( TypeId[7] != GM_UNKNOWN )
			TypeId[7] = IS<P6>::Const ? -TypeId[7] : TypeId[7];
	}

	if( NumParam > 6 && TypeId[8] == GM_UNKNOWN )
	{
		TypeId[8] = IS_BASIC_TYPE( GET<P7>::StaticId ) ? GET<P7>::StaticId : GetRuntimeId<P7>();
		if( TypeId[8] != GM_UNKNOWN )
			TypeId[8] = IS<P7>::Const ? -TypeId[8] : TypeId[8];
	}

	if( NumParam > 7 && TypeId[9] == GM_UNKNOWN )
	{
		TypeId[9] = IS_BASIC_TYPE( GET<P8>::StaticId ) ? GET<P8>::StaticId : GetRuntimeId<P8>();
		if( TypeId[9] != GM_UNKNOWN )
			TypeId[9] = IS<P8>::Const ? -TypeId[9] : TypeId[9];
	}

	if( NumParam > 8 && TypeId[10] == GM_UNKNOWN )
	{
		TypeId[10] = IS_BASIC_TYPE( GET<P9>::StaticId ) ? GET<P9>::StaticId : GetRuntimeId<P9>();
		if( TypeId[10] != GM_UNKNOWN )
			TypeId[10] = IS<P9>::Const ? -TypeId[10] : TypeId[10];
	}

	if( NumParam > 9 && TypeId[11] == GM_UNKNOWN )
	{
		TypeId[11] = IS_BASIC_TYPE( GET<P10>::StaticId ) ? GET<P10>::StaticId : GetRuntimeId<P10>();
		if( TypeId[11] != GM_UNKNOWN )
			TypeId[11] = IS<P10>::Const ? -TypeId[11] : TypeId[11];
	}
}


template<typename FUNCTION> inline gmFuncProxy* CreateFunctionProxy( FUNCTION pFunc )	{ return _CreateFunctionProxy(pFunc); }
template<typename METHOD>  inline gmFuncProxy* CreateMethodProxy ( METHOD pMeth )		{ return _CreateMethodProxy(pMeth); }
template<typename OPERATOR> inline gmFuncProxy* CreateOperatorProxy( OPERATOR pOp )		{ return _CreateOperatorProxy(pOp); }

template<typename RET> inline gmFuncProxy* _CreateFunctionProxy( RET (__cdecl* pFunc)() )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy0<FUNCTION_TYPE,IS_NOT_CONST,0, RET(__cdecl*)(), void,RET>( pFunc ); }
template<typename RET> inline gmFuncProxy* _CreateFunctionProxy( RET (__stdcall* pFunc)() )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy0<FUNCTION_TYPE,IS_NOT_CONST,0, RET(__stdcall*)(), void,RET>( pFunc ); }

template<typename RET, typename P1> inline gmFuncProxy* _CreateFunctionProxy( RET (__cdecl* pFunc)(P1) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy1<FUNCTION_TYPE,IS_NOT_CONST,1, RET(__cdecl*)(P1), void,RET,P1>( pFunc ); }
template<typename RET, typename P1> inline gmFuncProxy* _CreateFunctionProxy( RET (__stdcall* pFunc)(P1) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy1<FUNCTION_TYPE,IS_NOT_CONST,1, RET(__stdcall*)(P1), void,RET,P1>( pFunc ); }

template<typename RET, typename P1, typename P2> inline gmFuncProxy* _CreateFunctionProxy( RET (__cdecl* pFunc)(P1,P2) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy2<FUNCTION_TYPE,IS_NOT_CONST,2, RET(__cdecl*)(P1,P2), void,RET,P1,P2>( pFunc ); }
template<typename RET, typename P1, typename P2> inline gmFuncProxy* _CreateFunctionProxy( RET (__stdcall* pFunc)(P1,P2) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy2<FUNCTION_TYPE,IS_NOT_CONST,2, RET(__stdcall*)(P1,P2), void,RET,P1,P2>( pFunc );  }

template<typename RET, typename P1, typename P2, typename P3> inline gmFuncProxy* _CreateFunctionProxy( RET (__cdecl* pFunc)(P1,P2,P3) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy3<FUNCTION_TYPE,IS_NOT_CONST,3, RET(__cdecl*)(P1,P2,P3), void,RET,P1,P2,P3>( pFunc ); }
template<typename RET, typename P1, typename P2, typename P3> inline gmFuncProxy* _CreateFunctionProxy( RET (__stdcall* pFunc)(P1,P2,P3) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy3<FUNCTION_TYPE,IS_NOT_CONST,3, RET(__stdcall*)(P1,P2,P3), void,RET,P1,P2,P3>( pFunc ); }

template<typename RET, typename P1, typename P2, typename P3, typename P4> inline gmFuncProxy* _CreateFunctionProxy( RET (__cdecl* pFunc)(P1,P2,P3,P4) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy4<FUNCTION_TYPE,IS_NOT_CONST,4, RET(__cdecl*)(P1,P2,P3,P4), void,RET,P1,P2,P3,P4>( pFunc ); }
template<typename RET, typename P1, typename P2, typename P3, typename P4> inline gmFuncProxy* _CreateFunctionProxy( RET (__stdcall* pFunc)(P1,P2,P3,P4) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy4<FUNCTION_TYPE,IS_NOT_CONST,4, RET(__stdcall*)(P1,P2,P3,P4), void,RET,P1,P2,P3,P4>( pFunc ); }

template<typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> inline gmFuncProxy* _CreateFunctionProxy( RET (__cdecl* pFunc)(P1,P2,P3,P4,P5) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy5<FUNCTION_TYPE,IS_NOT_CONST,5, RET(__cdecl*)(P1,P2,P3,P4,P5), void,RET,P1,P2,P3,P4,P5>( pFunc ); }
template<typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> inline gmFuncProxy* _CreateFunctionProxy( RET (__stdcall* pFunc)(P1,P2,P3,P4,P5) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy5<FUNCTION_TYPE,IS_NOT_CONST,5, RET(__stdcall*)(P1,P2,P3,P4,P5), void,RET,P1,P2,P3,P4,P5>( pFunc ); }

template<typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> inline gmFuncProxy* _CreateFunctionProxy( RET (__cdecl* pFunc)(P1,P2,P3,P4,P5,P6) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy6<FUNCTION_TYPE,IS_NOT_CONST,6, RET(__cdecl*)(P1,P2,P3,P4,P5,P6), void,RET,P1,P2,P3,P4,P5,P6>( pFunc ); }
template<typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> inline gmFuncProxy* _CreateFunctionProxy( RET (__stdcall* pFunc)(P1,P2,P3,P4,P5,P6) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy6<FUNCTION_TYPE,IS_NOT_CONST,6, RET(__stdcall*)(P1,P2,P3,P4,P5,P6), void,RET,P1,P2,P3,P4,P5,P6>( pFunc ); }

template<typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> inline gmFuncProxy* _CreateFunctionProxy( RET (__cdecl* pFunc)(P1,P2,P3,P4,P5,P6,P7) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy7<FUNCTION_TYPE,IS_NOT_CONST,7, RET(__cdecl*)(P1,P2,P3,P4,P5,P6,P7), void,RET,P1,P2,P3,P4,P5,P6,P7>( pFunc ); }
template<typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> inline gmFuncProxy* _CreateFunctionProxy( RET (__stdcall* pFunc)(P1,P2,P3,P4,P5,P6,P7) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy7<FUNCTION_TYPE,IS_NOT_CONST,7, RET(__stdcall*)(P1,P2,P3,P4,P5,P6,P7), void,RET,P1,P2,P3,P4,P5,P6,P7>( pFunc ); }

template<typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> inline gmFuncProxy* _CreateFunctionProxy( RET (__cdecl* pFunc)(P1,P2,P3,P4,P5,P6,P7,P8) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy8<FUNCTION_TYPE,IS_NOT_CONST,8, RET(__cdecl*)(P1,P2,P3,P4,P5,P6,P7,P8), void,RET,P1,P2,P3,P4,P5,P6,P7,P8>( pFunc ); }
template<typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> inline gmFuncProxy* _CreateFunctionProxy( RET (__stdcall* pFunc)(P1,P2,P3,P4,P5,P6,P7,P8) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy8<FUNCTION_TYPE,IS_NOT_CONST,8, RET(__stdcall*)(P1,P2,P3,P4,P5,P6,P7,P8), void,RET,P1,P2,P3,P4,P5,P6,P7,P8>( pFunc ); }

template<typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> inline gmFuncProxy* _CreateFunctionProxy( RET (__cdecl* pFunc)(P1,P2,P3,P4,P5,P6,P7,P8,P9) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy9<FUNCTION_TYPE,IS_NOT_CONST,9, RET(__cdecl*)(P1,P2,P3,P4,P5,P6,P7,P8,P9), void,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9>( pFunc ); }
template<typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> inline gmFuncProxy* _CreateFunctionProxy( RET (__stdcall* pFunc)(P1,P2,P3,P4,P5,P6,P7,P8,P9) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy9<FUNCTION_TYPE,IS_NOT_CONST,9, RET(__stdcall*)(P1,P2,P3,P4,P5,P6,P7,P8,P9), void,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9>( pFunc ); }

template<typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> inline gmFuncProxy* _CreateFunctionProxy( RET (__cdecl* pFunc)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10) )		{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy10<FUNCTION_TYPE,IS_NOT_CONST,10, RET(__cdecl*)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10), void,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>( pFunc ); }
template<typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> inline gmFuncProxy* _CreateFunctionProxy( RET (__stdcall* pFunc)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy10<FUNCTION_TYPE,IS_NOT_CONST,10, RET(__stdcall*)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10), void,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>( pFunc ); }



template<class CLASS, typename RET> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)() )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy0<METHOD_TYPE,IS_NOT_CONST,0, RET(__cdecl    CLASS::*)(), CLASS,RET>( pMeth ); }
template<class CLASS, typename RET> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)() )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy0<METHOD_TYPE,IS_NOT_CONST,0, RET(__stdcall  CLASS::*)(), CLASS,RET>( pMeth ); }
template<class CLASS, typename RET> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)() )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy0<METHOD_TYPE,IS_NOT_CONST,0, RET(__thiscall CLASS::*)(), CLASS,RET>( pMeth ); }

template<class CLASS, typename RET> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)() const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy0<METHOD_TYPE,IS_CONST,0, RET(__cdecl    CLASS::*)() const, CLASS,RET>( pMeth ); }
template<class CLASS, typename RET> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)() const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy0<METHOD_TYPE,IS_CONST,0, RET(__stdcall  CLASS::*)() const, CLASS,RET>( pMeth );}
template<class CLASS, typename RET> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)() const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy0<METHOD_TYPE,IS_CONST,0, RET(__thiscall CLASS::*)() const, CLASS,RET>( pMeth );}

template<class CLASS, typename RET, typename P1> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy1<METHOD_TYPE,IS_NOT_CONST,1, RET(__cdecl    CLASS::*)(P1), CLASS,RET,P1>( pMeth ); }
template<class CLASS, typename RET, typename P1> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy1<METHOD_TYPE,IS_NOT_CONST,1, RET(__stdcall  CLASS::*)(P1), CLASS,RET,P1>( pMeth ); }
template<class CLASS, typename RET, typename P1> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy1<METHOD_TYPE,IS_NOT_CONST,1, RET(__thiscall CLASS::*)(P1), CLASS,RET,P1>( pMeth ); }

template<class CLASS, typename RET, typename P1> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy1<METHOD_TYPE,IS_CONST,1, RET(__cdecl    CLASS::*)(P1) const, CLASS,RET,P1>( pMeth ); }
template<class CLASS, typename RET, typename P1> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy1<METHOD_TYPE,IS_CONST,1, RET(__stdcall  CLASS::*)(P1) const, CLASS,RET,P1>( pMeth ); }
template<class CLASS, typename RET, typename P1> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy1<METHOD_TYPE,IS_CONST,1, RET(__thiscall CLASS::*)(P1) const, CLASS,RET,P1>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy2<METHOD_TYPE,IS_NOT_CONST,2, RET(__cdecl    CLASS::*)(P1,P2), CLASS,RET,P1,P2>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy2<METHOD_TYPE,IS_NOT_CONST,2, RET(__stdcall  CLASS::*)(P1,P2), CLASS,RET,P1,P2>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy2<METHOD_TYPE,IS_NOT_CONST,2, RET(__thiscall CLASS::*)(P1,P2), CLASS,RET,P1,P2>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy2<METHOD_TYPE,IS_CONST,2, RET(__cdecl    CLASS::*)(P1,P2) const, CLASS,RET,P1,P2>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy2<METHOD_TYPE,IS_CONST,2, RET(__stdcall  CLASS::*)(P1,P2) const, CLASS,RET,P1,P2>( pMeth );  }
template<class CLASS, typename RET, typename P1, typename P2> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy2<METHOD_TYPE,IS_CONST,2, RET(__thiscall CLASS::*)(P1,P2) const, CLASS,RET,P1,P2>( pMeth );  }

template<class CLASS, typename RET, typename P1, typename P2, typename P3> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy3<METHOD_TYPE,IS_NOT_CONST,3, RET(__cdecl    CLASS::*)(P1,P2,P3), CLASS,RET,P1,P2,P3>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy3<METHOD_TYPE,IS_NOT_CONST,3, RET(__stdcall  CLASS::*)(P1,P2,P3), CLASS,RET,P1,P2,P3>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy3<METHOD_TYPE,IS_NOT_CONST,3, RET(__thiscall CLASS::*)(P1,P2,P3), CLASS,RET,P1,P2,P3>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy3<METHOD_TYPE,IS_CONST,3, RET(__cdecl    CLASS::*)(P1,P2,P3) const, CLASS,RET,P1,P2,P3>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy3<METHOD_TYPE,IS_CONST,3, RET(__stdcall  CLASS::*)(P1,P2,P3) const, CLASS,RET,P1,P2,P3>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy3<METHOD_TYPE,IS_CONST,3, RET(__thiscall CLASS::*)(P1,P2,P3) const, CLASS,RET,P1,P2,P3>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy4<METHOD_TYPE,IS_NOT_CONST,4, RET(__cdecl    CLASS::*)(P1,P2,P3,P4), CLASS,RET,P1,P2,P3,P4>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy4<METHOD_TYPE,IS_NOT_CONST,4, RET(__stdcall  CLASS::*)(P1,P2,P3,P4), CLASS,RET,P1,P2,P3,P4>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy4<METHOD_TYPE,IS_NOT_CONST,4, RET(__thiscall CLASS::*)(P1,P2,P3,P4), CLASS,RET,P1,P2,P3,P4>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy4<METHOD_TYPE,IS_CONST,4, RET(__cdecl    CLASS::*)(P1,P2,P3,P4) const, CLASS,RET,P1,P2,P3,P4>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy4<METHOD_TYPE,IS_CONST,4, RET(__stdcall  CLASS::*)(P1,P2,P3,P4) const, CLASS,RET,P1,P2,P3,P4>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy4<METHOD_TYPE,IS_CONST,4, RET(__thiscall CLASS::*)(P1,P2,P3,P4) const, CLASS,RET,P1,P2,P3,P4>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4,P5) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy5<METHOD_TYPE,IS_NOT_CONST,5, RET(__cdecl    CLASS::*)(P1,P2,P3,P4,P5), CLASS,RET,P1,P2,P3,P4,P5>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4,P5) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy5<METHOD_TYPE,IS_NOT_CONST,5, RET(__stdcall  CLASS::*)(P1,P2,P3,P4,P5), CLASS,RET,P1,P2,P3,P4,P5>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4,P5) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy5<METHOD_TYPE,IS_NOT_CONST,5, RET(__thiscall CLASS::*)(P1,P2,P3,P4,P5), CLASS,RET,P1,P2,P3,P4,P5>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4,P5) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy5<METHOD_TYPE,IS_CONST,5, RET(__cdecl    CLASS::*)(P1,P2,P3,P4,P5) const, CLASS,RET,P1,P2,P3,P4,P5>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4,P5) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy5<METHOD_TYPE,IS_CONST,5, RET(__stdcall  CLASS::*)(P1,P2,P3,P4,P5) const, CLASS,RET,P1,P2,P3,P4,P5>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4,P5) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy5<METHOD_TYPE,IS_CONST,5, RET(__thiscall CLASS::*)(P1,P2,P3,P4,P5) const, CLASS,RET,P1,P2,P3,P4,P5>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4,P5,P6) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy6<METHOD_TYPE,IS_NOT_CONST,6, RET(__cdecl    CLASS::*)(P1,P2,P3,P4,P5,P6), CLASS,RET,P1,P2,P3,P4,P5,P6>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4,P5,P6) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy6<METHOD_TYPE,IS_NOT_CONST,6, RET(__stdcall  CLASS::*)(P1,P2,P3,P4,P5,P6), CLASS,RET,P1,P2,P3,P4,P5,P6>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4,P5,P6) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy6<METHOD_TYPE,IS_NOT_CONST,6, RET(__thiscall CLASS::*)(P1,P2,P3,P4,P5,P6), CLASS,RET,P1,P2,P3,P4,P5,P6>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4,P5,P6) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy6<METHOD_TYPE,IS_CONST,6, RET(__cdecl    CLASS::*)(P1,P2,P3,P4,P5,P6) const, CLASS,RET,P1,P2,P3,P4,P5,P6>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4,P5,P6) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy6<METHOD_TYPE,IS_CONST,6, RET(__stdcall  CLASS::*)(P1,P2,P3,P4,P5,P6) const, CLASS,RET,P1,P2,P3,P4,P5,P6>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4,P5,P6) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy6<METHOD_TYPE,IS_CONST,6, RET(__thiscall CLASS::*)(P1,P2,P3,P4,P5,P6) const, CLASS,RET,P1,P2,P3,P4,P5,P6>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy7<METHOD_TYPE,IS_NOT_CONST,7, RET(__cdecl    CLASS::*)(P1,P2,P3,P4,P5,P6,P7), CLASS,RET,P1,P2,P3,P4,P5,P6,P7>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy7<METHOD_TYPE,IS_NOT_CONST,7, RET(__stdcall  CLASS::*)(P1,P2,P3,P4,P5,P6,P7), CLASS,RET,P1,P2,P3,P4,P5,P6,P7>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy7<METHOD_TYPE,IS_NOT_CONST,7, RET(__thiscall CLASS::*)(P1,P2,P3,P4,P5,P6,P7), CLASS,RET,P1,P2,P3,P4,P5,P6,P7>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy7<METHOD_TYPE,IS_CONST,7, RET(__cdecl    CLASS::*)(P1,P2,P3,P4,P5,P6,P7) const, CLASS,RET,P1,P2,P3,P4,P5,P6,P7>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy7<METHOD_TYPE,IS_CONST,7, RET(__stdcall  CLASS::*)(P1,P2,P3,P4,P5,P6,P7) const, CLASS,RET,P1,P2,P3,P4,P5,P6,P7>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy7<METHOD_TYPE,IS_CONST,7, RET(__thiscall CLASS::*)(P1,P2,P3,P4,P5,P6,P7) const, CLASS,RET,P1,P2,P3,P4,P5,P6,P7>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8) )		{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy8<METHOD_TYPE,IS_NOT_CONST,8, RET(__cdecl    CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8), CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8) )		{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy8<METHOD_TYPE,IS_NOT_CONST,8, RET(__stdcall  CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8), CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8) )		{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy8<METHOD_TYPE,IS_NOT_CONST,8, RET(__thiscall CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8), CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy8<METHOD_TYPE,IS_CONST,8, RET(__cdecl    CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8) const, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy8<METHOD_TYPE,IS_CONST,8, RET(__stdcall  CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8) const, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy8<METHOD_TYPE,IS_CONST,8, RET(__thiscall CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8) const, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8,P9) )		{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy9<METHOD_TYPE,IS_NOT_CONST,9, RET(__cdecl    CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9), CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8,P9) )		{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy9<METHOD_TYPE,IS_NOT_CONST,9, RET(__stdcall  CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9), CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8,P9) )		{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy9<METHOD_TYPE,IS_NOT_CONST,9, RET(__thiscall CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9), CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8,P9) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy9<METHOD_TYPE,IS_CONST,9, RET(__cdecl    CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9) const, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8,P9) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy9<METHOD_TYPE,IS_CONST,9, RET(__stdcall  CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9) const, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8,P9) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy9<METHOD_TYPE,IS_CONST,9, RET(__thiscall CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9) const, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy10<METHOD_TYPE,IS_NOT_CONST,10, RET(__cdecl    CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10), CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy10<METHOD_TYPE,IS_NOT_CONST,10, RET(__stdcall  CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10), CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10) )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy10<METHOD_TYPE,IS_NOT_CONST,10, RET(__thiscall CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10), CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>( pMeth ); }

template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> inline gmFuncProxy* _CreateMethodProxy( RET (__cdecl    CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy10<METHOD_TYPE,IS_CONST,10, RET(__cdecl    CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10) const, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> inline gmFuncProxy* _CreateMethodProxy( RET (__stdcall  CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy10<METHOD_TYPE,IS_CONST,10, RET(__stdcall  CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10) const, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>( pMeth ); }
template<class CLASS, typename RET, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> inline gmFuncProxy* _CreateMethodProxy( RET (__thiscall CLASS::*pMeth)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10) const )	{ return IF< GET<RET>::StaticId == GM_NULL >::CreateProxy10<METHOD_TYPE,IS_CONST,10, RET(__thiscall CLASS::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10) const, CLASS,RET,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>( pMeth ); }


template<class CLASS> inline gmFuncProxy* CreateConstructorProxy()																																		{ return new _gmFuncProxy<CONSTRUCTOR_TYPE,IS_NOT_CONST,0, void*,CLASS,void>(NULL); }
template<class CLASS, typename P1> inline gmFuncProxy* CreateConstructorProxy()																															{ return new _gmFuncProxy<CONSTRUCTOR_TYPE,IS_NOT_CONST,1, void*,CLASS,void,P1>(NULL); }
template<class CLASS, typename P1, typename P2> inline gmFuncProxy* CreateConstructorProxy()																											{ return new _gmFuncProxy<CONSTRUCTOR_TYPE,IS_NOT_CONST,2, void*,CLASS,void,P1,P2>(NULL); }
template<class CLASS, typename P1, typename P2, typename P3> inline gmFuncProxy* CreateConstructorProxy()																								{ return new _gmFuncProxy<CONSTRUCTOR_TYPE,IS_NOT_CONST,3, void*,CLASS,void,P1,P2,P3>(NULL); }
template<class CLASS, typename P1, typename P2, typename P3, typename P4> inline gmFuncProxy* CreateConstructorProxy()																					{ return new _gmFuncProxy<CONSTRUCTOR_TYPE,IS_NOT_CONST,4, void*,CLASS,void,P1,P2,P3,P4>(NULL); }
template<class CLASS, typename P1, typename P2, typename P3, typename P4, typename P5> inline gmFuncProxy* CreateConstructorProxy()																		{ return new _gmFuncProxy<CONSTRUCTOR_TYPE,IS_NOT_CONST,5, void*,CLASS,void,P1,P2,P3,P4,P5>(NULL); }
template<class CLASS, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6> inline gmFuncProxy* CreateConstructorProxy()														{ return new _gmFuncProxy<CONSTRUCTOR_TYPE,IS_NOT_CONST,6, void*,CLASS,void,P1,P2,P3,P4,P5,P6>(NULL); }
template<class CLASS, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7> inline gmFuncProxy* CreateConstructorProxy()											{ return new _gmFuncProxy<CONSTRUCTOR_TYPE,IS_NOT_CONST,7, void*,CLASS,void,P1,P2,P3,P4,P5,P6,P7>(NULL); }
template<class CLASS, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8> inline gmFuncProxy* CreateConstructorProxy()								{ return new _gmFuncProxy<CONSTRUCTOR_TYPE,IS_NOT_CONST,8, void*,CLASS,void,P1,P2,P3,P4,P5,P6,P7,P8>(NULL); }
template<class CLASS, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9> inline gmFuncProxy* CreateConstructorProxy()					{ return new _gmFuncProxy<CONSTRUCTOR_TYPE,IS_NOT_CONST,9, void*,CLASS,void,P1,P2,P3,P4,P5,P6,P7,P8,P9>(NULL); }
template<class CLASS, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10> inline gmFuncProxy* CreateConstructorProxy()	{ return new _gmFuncProxy<CONSTRUCTOR_TYPE,IS_NOT_CONST,10,void*,CLASS,void,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>(NULL); }

template<typename RET, typename P1> static inline gmFuncProxy* _CreateOperatorProxy( RET (__cdecl* pOp)(P1) )		{ return new _gmFuncProxy<FUNCTION_OPERATOR_TYPE,IS_NOT_CONST,1, RET(__cdecl*)(P1), void,RET,P1>( pOp ); }
template<typename RET, typename P1> static inline gmFuncProxy* _CreateOperatorProxy( RET (__stdcall* pOp)(P1) )		{ return new _gmFuncProxy<FUNCTION_OPERATOR_TYPE,IS_NOT_CONST,1, RET(__stdcall*)(P1), void,RET,P1>( pOp ); }

template<typename RET, typename P1, typename P2> static inline gmFuncProxy* _CreateOperatorProxy( RET (__cdecl* pOp)(P1,P2) )		{ return new _gmFuncProxy<FUNCTION_OPERATOR_TYPE,IS_NOT_CONST,2, RET(__cdecl*)(P1,P2), void,RET,P1,P2>( pOp ); }
template<typename RET, typename P1, typename P2> static inline gmFuncProxy* _CreateOperatorProxy( RET (__stdcall* pOp)(P1,P2) )		{ return new _gmFuncProxy<FUNCTION_OPERATOR_TYPE,IS_NOT_CONST,2, RET(__stdcall*)(P1,P2), void,RET,P1,P2>( pOp ); }

template<class CLASS, typename RET> inline gmFuncProxy* _CreateOperatorProxy( RET (__cdecl    CLASS::*pOp)() )	{ return new _gmFuncProxy<METHOD_OPERATOR_TYPE,IS_NOT_CONST,0, RET(__cdecl    CLASS::*)(), CLASS,RET>( pOp ); }
template<class CLASS, typename RET> inline gmFuncProxy* _CreateOperatorProxy( RET (__stdcall  CLASS::*pOp)() )	{ return new _gmFuncProxy<METHOD_OPERATOR_TYPE,IS_NOT_CONST,0, RET(__stdcall  CLASS::*)(), CLASS,RET>( pOp ); }
template<class CLASS, typename RET> inline gmFuncProxy* _CreateOperatorProxy( RET (__thiscall CLASS::*pOp)() )	{ return new _gmFuncProxy<METHOD_OPERATOR_TYPE,IS_NOT_CONST,0, RET(__thiscall CLASS::*)(), CLASS,RET>( pOp ); }

template<class CLASS, typename RET> inline gmFuncProxy* _CreateOperatorProxy( RET (__cdecl    CLASS::*pOp)() const )	{ return new _gmFuncProxy<METHOD_OPERATOR_TYPE,IS_CONST,0, RET(__cdecl    CLASS::*)() const, CLASS,RET>( pOp ); }
template<class CLASS, typename RET> inline gmFuncProxy* _CreateOperatorProxy( RET (__stdcall  CLASS::*pOp)() const )	{ return new _gmFuncProxy<METHOD_OPERATOR_TYPE,IS_CONST,0, RET(__stdcall  CLASS::*)() const, CLASS,RET>( pOp ); }
template<class CLASS, typename RET> inline gmFuncProxy* _CreateOperatorProxy( RET (__thiscall CLASS::*pOp)() const )	{ return new _gmFuncProxy<METHOD_OPERATOR_TYPE,IS_CONST,0, RET(__thiscall CLASS::*)() const, CLASS,RET>( pOp ); }

template<class CLASS, typename RET, typename P1> inline gmFuncProxy* _CreateOperatorProxy( RET (__cdecl    CLASS::*pOp)(P1) )	{ return new _gmFuncProxy<METHOD_OPERATOR_TYPE,IS_NOT_CONST,1, RET(__cdecl    CLASS::*)(P1), CLASS,RET,P1>( pOp ); }
template<class CLASS, typename RET, typename P1> inline gmFuncProxy* _CreateOperatorProxy( RET (__stdcall  CLASS::*pOp)(P1) )	{ return new _gmFuncProxy<METHOD_OPERATOR_TYPE,IS_NOT_CONST,1, RET(__stdcall  CLASS::*)(P1), CLASS,RET,P1>( pOp ); }
template<class CLASS, typename RET, typename P1> inline gmFuncProxy* _CreateOperatorProxy( RET (__thiscall CLASS::*pOp)(P1) )	{ return new _gmFuncProxy<METHOD_OPERATOR_TYPE,IS_NOT_CONST,1, RET(__thiscall CLASS::*)(P1), CLASS,RET,P1>( pOp ); }

template<class CLASS, typename RET, typename P1> inline gmFuncProxy* _CreateOperatorProxy( RET (__cdecl    CLASS::*pOp)(P1) const )	{ return new _gmFuncProxy<METHOD_OPERATOR_TYPE,IS_CONST,1, RET(__cdecl    CLASS::*)(P1) const, CLASS,RET,P1>( pOp ); }
template<class CLASS, typename RET, typename P1> inline gmFuncProxy* _CreateOperatorProxy( RET (__stdcall  CLASS::*pOp)(P1) const )	{ return new _gmFuncProxy<METHOD_OPERATOR_TYPE,IS_CONST,1, RET(__stdcall  CLASS::*)(P1) const, CLASS,RET,P1>( pOp ); }
template<class CLASS, typename RET, typename P1> inline gmFuncProxy* _CreateOperatorProxy( RET (__thiscall CLASS::*pOp)(P1) const )	{ return new _gmFuncProxy<METHOD_OPERATOR_TYPE,IS_CONST,1, RET(__thiscall CLASS::*)(P1) const, CLASS,RET,P1>( pOp ); }

#endif