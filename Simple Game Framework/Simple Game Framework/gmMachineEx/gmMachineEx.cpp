#include "gmMachineEx.h"
#include "gmException.h"

#pragma warning ( disable: 4800 )
#pragma warning ( disable: 4244 )

std::map<gmType,gmClassDef*> gmMachineEx::ClassList;

std::multimap<int,gmThread*> gmMachineEx::_ThreadList_AtCreate;
std::multimap<int,gmThread*> gmMachineEx::_ThreadList_AtDestroy;

gmMachineEx::gmMachineEx( bool UseExtendedThreadFunction ) :
AT_CREATE(0) , AT_DESTROY(1) , AT_TIME(2) 
{
	if( UseExtendedThreadFunction )
	{
		RegisterLibraryFunction( "threadEx", gmThreadEx );
		s_machineCallback = MachineCallback;
	}
#ifdef _DEBUG
	s_userBreakCallback=userBreakCallBack;
#endif
	CreateUserType( "__unknown_type__" );
	CreateUserType( "__thread_launch_info__" );

	AtCreate  = AllocUserObject( &AT_CREATE,  GM_THREAD_LAUNCH_INFO );
	AtDestroy = AllocUserObject( &AT_DESTROY, GM_THREAD_LAUNCH_INFO );
	AtTime	  = AllocUserObject( &AT_TIME,	  GM_THREAD_LAUNCH_INFO );

	AtCreate->SetPersist( true );
	AtDestroy->SetPersist( true );
	AtTime->SetPersist( true );

	#if GM_GC_KEEP_PERSISTANT_SEPARATE
		m_gc->MakeObjectPersistant( AtCreate );
		m_gc->MakeObjectPersistant( AtDestroy );
		m_gc->MakeObjectPersistant( AtTime );
	#endif

	m_global->Set( this, "AT_CREATE",	gmVariable( AtCreate  ) );
	m_global->Set( this, "AT_DESTROY",	gmVariable( AtDestroy ) );
	m_global->Set( this, "AT_TIME",		gmVariable( AtTime    ) );

	gmType Type;
	gmClassDef* pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "any_ptr" );
		pDef->pTypeInfo = &typeid(void*);
		pDef->pConstTypeInfo = &typeid(const void*);
		ClassList.insert( std::make_pair( Type, pDef ) );
	}

	pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "c_string" );
		pDef->pTypeInfo = &typeid(char*);
		pDef->pConstTypeInfo = &typeid(const char*);
		ClassList.insert( std::make_pair( Type, pDef ) );
		RegisterUserCallbacks( Type, NULL, NULL, gmCStringAsStringCallback);
	}

	pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "bool_ptr" );
		pDef->pTypeInfo = &typeid(bool*);
		pDef->pConstTypeInfo = &typeid(const bool*);
		ClassList.insert( std::make_pair( Type, pDef ) );
		RegisterTypeLibraryFunction( Type, "Get", &gmGet );
		RegisterTypeLibraryFunction( Type, "Set", &gmSet );
		RegisterUserCallbacks( Type, NULL, &gmExDefaultDestructor<bool> );
	}

	pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "int_ptr" );
		pDef->pTypeInfo = &typeid(int*);
		pDef->pConstTypeInfo = &typeid(const int*);
		ClassList.insert( std::make_pair( Type, pDef ) );
		RegisterTypeLibraryFunction( Type, "Get", &gmGet );
		RegisterTypeLibraryFunction( Type, "Set", &gmSet );
		RegisterUserCallbacks( Type, NULL, &gmExDefaultDestructor<int> );
	}

	pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "short_ptr" );
		pDef->pTypeInfo = &typeid(short*);
		pDef->pConstTypeInfo = &typeid(const short*);
		ClassList.insert( std::make_pair( Type, pDef ) );
		RegisterTypeLibraryFunction( Type, "Get", &gmGet );
		RegisterTypeLibraryFunction( Type, "Set", &gmSet );
		RegisterUserCallbacks( Type, NULL, &gmExDefaultDestructor<short> );
	}

	pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "long_ptr" );
		pDef->pTypeInfo = &typeid(long*);
		pDef->pConstTypeInfo = &typeid(const long*);
		ClassList.insert( std::make_pair( Type, pDef ) );
		RegisterTypeLibraryFunction( Type, "Get", &gmGet );
		RegisterTypeLibraryFunction( Type, "Set", &gmSet );
		RegisterUserCallbacks( Type, NULL, &gmExDefaultDestructor<long> );
	}

	pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "longlong_ptr" );
		pDef->pTypeInfo = &typeid(long long*);
		pDef->pConstTypeInfo = &typeid(const long long*);
		ClassList.insert( std::make_pair( Type, pDef ) );
		RegisterTypeLibraryFunction( Type, "Get", &gmGet );
		RegisterTypeLibraryFunction( Type, "Set", &gmSet );
		RegisterUserCallbacks( Type, NULL, &gmExDefaultDestructor<long long> );
	}

	pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "uint_ptr" );
		pDef->pTypeInfo = &typeid(unsigned int*);
		pDef->pConstTypeInfo = &typeid(const unsigned int*);
		ClassList.insert( std::make_pair( Type, pDef ) );
		RegisterTypeLibraryFunction( Type, "Get", &gmGet );
		RegisterTypeLibraryFunction( Type, "Set", &gmSet );
		RegisterUserCallbacks( Type, NULL, &gmExDefaultDestructor<unsigned int> );
	}

	pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "ushort_ptr" );
		pDef->pTypeInfo = &typeid(unsigned short*);
		pDef->pConstTypeInfo = &typeid(const unsigned short*);
		ClassList.insert( std::make_pair( Type, pDef ) );
		RegisterTypeLibraryFunction( Type, "Get", &gmGet );
		RegisterTypeLibraryFunction( Type, "Set", &gmSet );
		RegisterUserCallbacks( Type, NULL, &gmExDefaultDestructor<unsigned short> );
	}

	pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "ulong_ptr" );
		pDef->pTypeInfo = &typeid(unsigned long*);
		pDef->pConstTypeInfo = &typeid(const unsigned long*);
		ClassList.insert( std::make_pair( Type, pDef ) );
		RegisterTypeLibraryFunction( Type, "Get", &gmGet );
		RegisterTypeLibraryFunction( Type, "Set", &gmSet );
		RegisterUserCallbacks( Type, NULL, &gmExDefaultDestructor<unsigned long> );
	}

	pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "ulonglong_ptr" );
		pDef->pTypeInfo = &typeid(unsigned long long*);
		pDef->pConstTypeInfo = &typeid(const unsigned long long*);
		ClassList.insert( std::make_pair( Type, pDef ) );
		RegisterTypeLibraryFunction( Type, "Get", &gmGet );
		RegisterTypeLibraryFunction( Type, "Set", &gmSet );
		RegisterUserCallbacks( Type, NULL, &gmExDefaultDestructor<unsigned long long> );
	}

	pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "float_ptr" );
		pDef->pTypeInfo = &typeid(float*);
		pDef->pConstTypeInfo = &typeid(const float*);
		ClassList.insert( std::make_pair( Type, pDef ) );
		RegisterTypeLibraryFunction( Type, "Get", &gmGet );
		RegisterTypeLibraryFunction( Type, "Set", &gmSet );
		RegisterUserCallbacks( Type, NULL, &gmExDefaultDestructor<float> );
	}

	pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "double_ptr" );
		pDef->pTypeInfo = &typeid(double*);
		pDef->pConstTypeInfo = &typeid(const double*);
		ClassList.insert( std::make_pair( Type, pDef ) );
		RegisterTypeLibraryFunction( Type, "Get", &gmGet );
		RegisterTypeLibraryFunction( Type, "Set", &gmSet );
		RegisterUserCallbacks( Type, NULL, &gmExDefaultDestructor<double> );
	}

	pDef = new gmClassDef;
	if( pDef )
	{
		Type = CreateUserType( "longdouble_ptr" );
		pDef->pTypeInfo = &typeid(long double*);
		pDef->pConstTypeInfo = &typeid(const long double*);
		ClassList.insert( std::make_pair( Type, pDef ) );
		RegisterTypeLibraryFunction( Type, "Get", &gmGet );
		RegisterTypeLibraryFunction( Type, "Set", &gmSet );
		RegisterUserCallbacks( Type, NULL, &gmExDefaultDestructor<long double> );
	}

	RegisterLibraryFunction( "const_cast", gmConstCast );

	_ProxyList.reserve( 4096 );
};

bool gmMachineEx::OnCreateThread( const void* pContext )
{
	const gmVariable* pFunc = ((gmThread*)pContext)->GetFunction();
	if( pFunc )
	{
		const int Ref = pFunc->m_value.m_ref;
		std::multimap<int,gmThread*>::iterator first = gmMachineEx::_ThreadList_AtCreate.find(Ref);

		if( first != gmMachineEx::_ThreadList_AtCreate.end() )
		{
			std::multimap<int,gmThread*>::const_iterator i = first;
			std::multimap<int,gmThread*>::iterator last = gmMachineEx::_ThreadList_AtCreate.find(Ref+1);

			gmMachine* VM = NULL;

			while( i != last )
			{
				if( i->second )
				{
					VM = i->second->GetMachine();
					if( VM )
						VM->Sys_SwitchState( i->second, gmThread::RUNNING );
				}
					
				++i;
			}

			gmMachineEx::_ThreadList_AtCreate.erase( first, last );
		}
	}

	return false;
}

bool gmMachineEx::OnDestroyThread( const void* pContext )
{
	const gmVariable* pFunc = ((gmThread*)pContext)->GetFunction();
	if( pFunc )
	{
		const int Ref = pFunc->m_value.m_ref;
		std::multimap<int,gmThread*>::iterator first = gmMachineEx::_ThreadList_AtDestroy.find(Ref);

		if( first != gmMachineEx::_ThreadList_AtDestroy.end() )
		{
			std::multimap<int,gmThread*>::const_iterator i = first;
			std::multimap<int,gmThread*>::iterator last = gmMachineEx::_ThreadList_AtDestroy.find(Ref+1);

			gmMachine* VM = NULL;

			while( i != last )
			{
				if( i->second )
				{
					VM = i->second->GetMachine();
					if( VM )
						VM->Sys_SwitchState( i->second, gmThread::RUNNING );
				}
					
				++i;
			}

			gmMachineEx::_ThreadList_AtDestroy.erase( first, last );
		}
	}

	return false;
}

bool gmMachineEx::MachineCallback( gmMachine*, gmMachineCommand Command, const void* pContext)	
{
	if( pContext )
	{
		switch(Command)
		{
			case MC_THREAD_CREATE:
				return gmMachineEx::OnCreateThread( pContext );

			case MC_THREAD_DESTROY:
				return gmMachineEx::OnDestroyThread( pContext );
		}
	}

	return false;
}

int GM_CDECL gmMachineEx::gmThreadEx(gmThread* pThread)
{
	if( pThread )
	{
		gmMachine* VM = pThread->GetMachine();
		if( VM )
		{
			const int NumParams = pThread->GetNumParams();
			const int LaunchInfoIndex = NumParams-2;
			const int LaunchParamIndex = NumParams-1;
			
			scriptAssert( NumParams >= 3 ,"Expecting 3 params");
			scriptAssert( pThread->Param(0).m_type == GM_FUNCTION ,"Param 0 must be function");
			scriptAssert( pThread->Param(LaunchInfoIndex).m_type == GM_THREAD_LAUNCH_INFO ,"Invalid thread launch info");

			if( NumParams >= 3 && pThread->Param(0).m_type == GM_FUNCTION && pThread->Param(LaunchInfoIndex).m_type == GM_THREAD_LAUNCH_INFO )
			{
				gmThread* NewThread = VM->CreateThread();
				if( NewThread )
				{
					const gmVariable* pThis = pThread->GetThis();
					if( !pThis )
						pThis = &gmVariable::s_null;

					gmFunctionObject* pFunc = (gmFunctionObject*)pThread->Param(0).m_value.m_ref;
					if( pFunc )
					{
						NewThread->Push( *pThis );
						NewThread->PushFunction( pFunc );

						for(int i = 1; i < LaunchInfoIndex; ++i)
							NewThread->Push(pThread->Param(i));
						NewThread->PushStackFrame( NumParams - 3 );

						gmUserObject* pGmObj = (gmUserObject*)pThread->Param(LaunchInfoIndex).m_value.m_ref;

						GM_ASSERT( pGmObj && pGmObj->m_user );
						if( pGmObj && pGmObj->m_user )
						{
							int LaunchInfo = *(int*)(pGmObj->m_user);

							if( LaunchInfo == 2 ) //AT_TIME
							{
								GM_ASSERT( pThread->Param(LaunchParamIndex).m_type == GM_INT || pThread->Param(LaunchParamIndex).m_type == GM_FLOAT );
								if( pThread->Param(LaunchParamIndex).m_type == GM_FLOAT )
									pThread->Param(LaunchParamIndex).m_value.m_int = (int) (pThread->Param(LaunchParamIndex).m_value.m_float * 1000.0f);
								else if( pThread->Param(LaunchParamIndex).m_type == GM_INT )
									pThread->Param(LaunchParamIndex).m_value.m_int *= 1000;

								NewThread->Sys_SetTimeStamp( VM->GetTime() + pThread->Param(LaunchParamIndex).m_value.m_int );
								VM->Sys_SwitchState( NewThread , gmThread::SLEEPING );
							}
							else
							{
								GM_ASSERT( pThread->Param(LaunchParamIndex).m_type == GM_FUNCTION );
								VM->Sys_SwitchState( NewThread, gmThread::BLOCKED );
								std::pair<int,gmThread*> Pair = std::make_pair( pThread->Param(LaunchParamIndex).m_value.m_ref, NewThread );

								if( LaunchInfo == 0) //AT_CREATE
									gmMachineEx::_ThreadList_AtCreate.insert( Pair );	
								else //AT_DESTROY
									gmMachineEx::_ThreadList_AtDestroy.insert( Pair );
							}

							return GM_OK;
						}
					}
				}
			}
		}
	}

	return GM_EXCEPTION;
}

void GM_CDECL gmMachineEx::gmCStringAsStringCallback(gmUserObject* pObj, char * buffer, int bufferSize)
{
	if( pObj && buffer )
		strncpy( buffer, (const char*)pObj->m_user, bufferSize );
}

gmMachineEx::~gmMachineEx()
{
	m_gc->DestructAll();
	std::map<gmType,gmClassDef*>::const_iterator i = ClassList.begin();
	std::map<int,gmVarProxy*>::const_iterator k;

	gmClassDef* pDef = NULL;
	gmFuncProxy* TempProxy;
	gmFuncProxy* NextProxy;

	while( i != ClassList.end() )
	{
		pDef = i->second;

		if( pDef )
		{
			k = pDef->VarList.begin();

			while( k != pDef->VarList.end() )
			{
				if( k->second )
					delete k->second;

				++k;
			}

			pDef->VarList.clear();
			pDef->OpList.clear();
			delete pDef;
		}

		++i;
	}

	ClassList.clear();
//	_GmFuncList.clear();
	std::vector<gmFuncProxy*>::const_iterator l = _ProxyList.begin();

	while( l != _ProxyList.end() )
	{
		if( *l )
		{
			NextProxy = (*l)->pNextOverloaded;
			while( NextProxy )
			{
				TempProxy = NextProxy;
				NextProxy = NextProxy->pNextOverloaded;

				delete TempProxy;
			}

			delete *l;
		}

		++l;
	}
	
	_ProxyList.clear();
	_ThreadList_AtCreate.clear();
	_ThreadList_AtDestroy.clear();
}

gmType gmMachineEx::Bind( bool isClass, const char* ClassOrNamespace, ...)
{	
	gmType Type = GM_NULL;
	gmClassDef* pDef = NULL;
	gmTableObject* Table = m_global;

	va_list pArg;
	va_start( pArg, ClassOrNamespace );

	if( isClass )
	{
		Type = CreateUserType( ClassOrNamespace );
		m_types[Type].m_nativeOperators[O_EQ] = NULL;
		m_types[Type].m_nativeOperators[O_NEQ] = NULL;
		m_types[Type].m_nativeOperators[O_NOT] = NULL;

		pDef = new gmClassDef;
		if( !pDef ) return GM_NULL;

		pDef->pTypeInfo = va_arg( pArg, const type_info* );
		pDef->pConstTypeInfo = va_arg( pArg, const type_info* );
	}
	else
	{
		if( ClassOrNamespace )
		{
			Table = AllocTableObject();
			if( !Table ) return GM_NULL;

			m_global->Set( this, ClassOrNamespace, gmVariable( Table ) );
		}
	}

	gmAsStringCallback pStringCallback = NULL;
	gmFuncProxy* pTempOverloaded = NULL;
	gmFuncProxy* pNextOverloaded = NULL;
	gmFunctionObject* pFuncObj = NULL;
	gmStringObject* pStringObj = NULL;
	bool PoolFunctionDefined = false;
	bool ConstructorDefined = false;
	gmFuncProxy* pFuncProxy = NULL;
	gmVarProxy* pVarProxy = NULL;
	const char* Name = NULL;
	void* FirstParam = NULL;
	gmVariable TempVar;
	bool End = false;


	do
	{
		FirstParam = va_arg( pArg, void* ); //pointeur tampon utilisé avant de savoir à quoi on a affaire à cause de ce maudit VC++ buggé
		int ParamType = va_arg( pArg, int );

		switch( ParamType )
		{
			case CONSTRUCTOR_TYPE:
				GM_ASSERT( PoolFunctionDefined != true );
				ConstructorDefined = true;
			case FUNCTION_TYPE:
			{
				if( ParamType == FUNCTION_TYPE )
					Name = va_arg( pArg, const char* );
				else
					Name = ClassOrNamespace;

				pFuncProxy = (gmFuncProxy*) FirstParam;

				if( pFuncProxy )
				{
					TempVar = Table->Get( this, Name );

					if( TempVar.m_type != GM_FUNCTION )
					{
						if( isClass )
							RegisterLibraryFunction( Name, &FunctionDispatcher, NULL, pFuncProxy );
						else
							RegisterLibraryFunction( Name, &FunctionDispatcher, ClassOrNamespace, pFuncProxy );

						_ProxyList.push_back( pFuncProxy );
					}
					else
					{
						pFuncObj = ((gmFunctionObject*)TempVar.m_value.m_ref);
						if( pFuncObj && pFuncObj->m_cUserData )
						{
							pTempOverloaded = (gmFuncProxy*) pFuncObj->m_cUserData;
							while( pTempOverloaded )
							{
								pNextOverloaded = pTempOverloaded;
								pTempOverloaded = pTempOverloaded->pNextOverloaded;
							}
								
							pNextOverloaded->pNextOverloaded = pFuncProxy;
						}
					}
				}
			} break;

			case METHOD_TYPE:
			{
				Name = va_arg( pArg, const char* );
				pFuncProxy = (gmFuncProxy*) FirstParam;

				if( pFuncProxy  )
				{
					gmStringObject* pName = AllocPermanantStringObject( Name );
					if( pName )
					{
						TempVar = GetTypeVariable( Type, gmVariable( pName ) );

						if( TempVar.m_type != GM_FUNCTION )
						{
							RegisterTypeLibraryFunction( Type, Name, &FunctionDispatcher, pFuncProxy );
							_ProxyList.push_back( pFuncProxy );
						}
						else
						{
							pFuncObj = ((gmFunctionObject*)TempVar.m_value.m_ref);
							if( pFuncObj && pFuncObj->m_cUserData )
							{
								pTempOverloaded = (gmFuncProxy*) pFuncObj->m_cUserData;
								while( pTempOverloaded )
								{
									pNextOverloaded = pTempOverloaded;
									pTempOverloaded = pTempOverloaded->pNextOverloaded;
								}
									
								pNextOverloaded->pNextOverloaded = pFuncProxy;
							}
						}
					}
				}

			} break;

			case FUNCTION_OPERATOR_TYPE:
			case METHOD_OPERATOR_TYPE:
			{
				gmOperator Op = va_arg( pArg, gmOperator );
				pFuncProxy = (gmFuncProxy*) FirstParam;

				GM_ASSERT( Op != O_MAXOPERATORS ); // = operator not supported
				if( Op != O_MAXOPERATORS )
				{
					gmOperatorFunction OpFunc = GetTypeNativeOperator( Type, Op );
					if( OpFunc )
					{
						std::map<gmOperator,gmFuncProxy*>::const_iterator i = pDef->OpList.find( Op );

						if( i !=  pDef->OpList.end() )
						{
							pTempOverloaded = i->second;
							while( pTempOverloaded )
							{
								pNextOverloaded = pTempOverloaded;
								pTempOverloaded = pTempOverloaded->pNextOverloaded;
							}
										
							pNextOverloaded->pNextOverloaded = pFuncProxy;
						}
					}
					else
					{
						switch( Op )
						{
							case O_GETIND:
								RegisterTypeOperator( Type, O_GETIND, NULL, &gmOpGetInd );
								RegisterTypeOperator( Type, O_SETIND, NULL, &gmOpSetInd );
								pDef->OpList.insert( std::make_pair( O_SETIND, pFuncProxy ) ); break;
							case O_ADD:
								RegisterTypeOperator( Type, O_ADD, NULL, &gmOpAdd ); break;
							case O_SUB:
								RegisterTypeOperator( Type, O_SUB, NULL, &gmOpSub ); break;
							case O_MUL:
								RegisterTypeOperator( Type, O_MUL, NULL, &gmOpMul ); break;
							case O_DIV:
								RegisterTypeOperator( Type, O_DIV, NULL, &gmOpDiv ); break;
							case O_REM:
								RegisterTypeOperator( Type, O_REM, NULL, &gmOpRem); break;
							case O_BIT_OR:
								RegisterTypeOperator( Type, O_BIT_OR, NULL, &gmOpBitOr ); break;
							case O_BIT_XOR:
								RegisterTypeOperator( Type, O_BIT_XOR, NULL, &gmOpBitXor ); break;
							case O_BIT_AND:
								RegisterTypeOperator( Type, O_BIT_AND, NULL, &gmOpBitAnd ); break;
							case O_BIT_SHIFTLEFT:
								RegisterTypeOperator( Type, O_BIT_SHIFTLEFT, NULL, &gmOpBitShiftLeft ); break;
							case O_BIT_SHIFTRIGHT:
								RegisterTypeOperator( Type, O_BIT_SHIFTRIGHT, NULL, &gmOpBitShiftRight ); break;
							case O_BIT_INV:
								RegisterTypeOperator( Type, O_BIT_INV, NULL, &gmOpBitInv ); 
							case O_LT:
								RegisterTypeOperator( Type, O_LT, NULL, &gmOpLessThan ); break;
							case O_GT:
								RegisterTypeOperator( Type, O_GT, NULL, &gmOpGreaterThan ); break;
							case O_LTE:
								RegisterTypeOperator( Type, O_LTE, NULL, &gmOpLessThanOrEqual ); break;
							case O_GTE:
								RegisterTypeOperator( Type, O_GTE, NULL, &gmOpGreaterThanOrEqual ); break;
							case O_EQ:
								RegisterTypeOperator( Type, O_EQ, NULL, &gmOpEqual); break;
							case O_NEQ:
								RegisterTypeOperator( Type, O_NEQ, NULL, &gmOpNotEqual ); break;
							case O_NEG:
								RegisterTypeOperator( Type, O_NEG, NULL, &gmOpNeg ); break;
							case O_POS:
								RegisterTypeOperator( Type, O_POS, NULL, &gmOpPos ); break;
							case O_NOT:
								RegisterTypeOperator( Type, O_NOT, NULL, &gmOpNot ); break;
						}

						pDef->OpList.insert( std::make_pair( Op, pFuncProxy ) );
						_ProxyList.push_back( pFuncProxy );
					}
				}

			} break;

			case VARIABLE_TYPE:
			{
				Name = va_arg( pArg, const char* );
				pVarProxy = (gmVarProxy*) FirstParam;

				if( pVarProxy  )
				{
					pStringObj = AllocPermanantStringObject( Name );
					if( pStringObj )
					{
						std::pair<int,gmVarProxy*> VarPair = std::make_pair( pStringObj->GetRef(), pVarProxy );
						pDef->VarList.insert( VarPair );
					}
				}

			} break;

			case DESTRUCTOR_VIRTUAL_TYPE:
				pDef->pDestructor = (gmGCDestructCallback) FirstParam; break;

			case ASSTRING_VIRTUAL_TYPE:
				pStringCallback = (gmAsStringCallback) FirstParam; break;

			case BASECLASS_VIRTUAL_TYPE:
			{
				const type_info* pInfo = (const type_info*) FirstParam;
				if( pInfo )
				{
					gmType BaseType = GetRuntimeId( pInfo );

					GM_ASSERT( BaseType != GM_UNKNOWN );  //the base class MUST be bound BEFORE the derived one
					std::map<gmType,gmClassDef*>::const_iterator i = ClassList.find( BaseType );
					if( i != ClassList.end() )
					{
						pDef->BaseClass = i->second;
						pDef->BaseClassType = BaseType;
					}
				}

			} break;

			case POOL_VIRTUAL_TYPE:
			{
				GM_ASSERT( ConstructorDefined != true );
				PoolFunctionDefined = true;

				gmGCDestructCallback pFree = va_arg( pArg, gmGCDestructCallback );
				pFuncProxy = (gmFuncProxy*) FirstParam;

				if( pFuncProxy && pFree )
				{
					RegisterLibraryFunction( ClassOrNamespace, &gmIntermediateGet, NULL, pFuncProxy );
					pDef->pDestructor = pFree;
					_ProxyList.push_back( pFuncProxy );
				}
			} break;

			case END_VIRTUAL_TYPE:
			{ 
				End = true;

			} break;
		}

	} while( !End );

	va_end( pArg );

	if( isClass )
	{
		RegisterTypeOperator( Type, O_GETDOT, NULL, &gmMachineEx::OpGetDot );
		RegisterTypeOperator( Type, O_SETDOT, NULL, &gmMachineEx::OpSetDot );

		RegisterUserCallbacks( Type, NULL, gmExIntermediateDestructor, pStringCallback );

		gmTableObject* pThisTable = m_types[Type].m_variables;
		gmVariable TempVar = gmVariable::s_null;
		gmTableObject* pBaseTable = NULL;
		gmFunctionObject* pFunc = NULL;
		gmTableNode* pBaseNode = NULL;
		gmType BaseType = GM_UNKNOWN;
		const char* MethName = NULL;
		gmTableIterator it;

		bool LookForStringCallback = false;
		if( !GetUserAsStringCallback( Type ) )
			LookForStringCallback = true;

		const gmClassDef* BaseClass = pDef->BaseClass;

		while( BaseClass )
		{
			BaseType = GetRuntimeId( BaseClass->pTypeInfo );

			GM_ASSERT( BaseType != GM_UNKNOWN );
			if( BaseType != GM_UNKNOWN )
			{
				if( LookForStringCallback )
				{
					pStringCallback = GetUserAsStringCallback( BaseType );
					if( pStringCallback )
					{
						RegisterUserCallbacks( Type, NULL, gmExIntermediateDestructor, pStringCallback );
						LookForStringCallback = false;
					}
				}

				pBaseTable = m_types[BaseType].m_variables;
				it = -2;

				while( (pBaseNode = pBaseTable->GetNext(it) ) != NULL )
				{
					if( pBaseNode->m_value.m_type == GM_FUNCTION )
					{
						MethName = ((gmStringObject*)pBaseNode->m_key.m_value.m_ref)->GetString();
						TempVar = pThisTable->Get( this, MethName );

						if( TempVar.m_type != GM_FUNCTION )
						{
							pFunc = (gmFunctionObject*)pBaseNode->m_value.m_value.m_ref;

							if( pFunc && pFunc->m_cUserData)
							{
								gmFuncProxy* pFuncProxy = (gmFuncProxy*)pFunc->m_cUserData;
								RegisterTypeLibraryFunction( Type, MethName, &FunctionDispatcher, pFuncProxy );
							}
						}
					}
				}
			}

			gmOperator Op;
			gmOperatorFunction OpFunc;

			for( int i = 2; i < 25; ++i )
			{
				Op = static_cast<gmOperator>(i);
				if( !GetTypeNativeOperator( Type, Op ) )
				{
					if( (OpFunc = GetTypeNativeOperator( BaseType, Op )) != NULL )
					{
						RegisterTypeOperator( Type, Op, NULL, OpFunc );

						std::map<gmOperator,gmFuncProxy*>::const_iterator i = BaseClass->OpList.find( Op );
						pDef->OpList.insert( std::make_pair( Op, i->second ) );
					}
				}
			}
				
			BaseClass = BaseClass->BaseClass;
		}

		ClassList.insert( std::make_pair( Type, pDef ) );

	}

	return Type;
}


void gmMachineEx::UpdateTypeIds()
{
	std::map<gmType,gmClassDef*>::const_iterator i = ClassList.begin();
	std::map<int,gmVarProxy*>::const_iterator j;
	while( i != ClassList.end() )
	{
		if( i->second )
		{
			j = i->second->VarList.begin();
			while( j != i->second->VarList.end() )
			{
				if( j->second )
					j->second->UpdateTypeId();
					++j;
			}
		}
			++i;
	}
	std::vector<gmFuncProxy*>::const_iterator l = _ProxyList.begin();
	while( l != _ProxyList.end() )
	{
		if( *l )
		{
			(*l)->UpdateTypeIds();
			gmFuncProxy* NextProxy = (*l)->pNextOverloaded;
			while( NextProxy )
			{
				NextProxy->UpdateTypeIds();
				NextProxy = NextProxy->pNextOverloaded;
			}
		}
		++l;
	}
}

int GM_CDECL gmMachineEx::FunctionDispatcher( gmThread* pThread )
{
	if( pThread )
	{
		#ifdef DEBUG
			for( int i =0; i < pThread->GetNumParams(); ++i )
				GM_ASSERT( pThread->ParamType(i) != GM_NULL );
		#endif

		const gmFunctionObject* pFunc = pThread->GetFunctionObject();       
		
		if( pFunc && pFunc->m_cUserData )
		{
			gmFuncProxy* pProxy = NULL, *TempProxy = NULL;
			int NumParam = pThread->GetNumParams();
			int ParamCoeff = NumParam * 3;
			int TotalCoeff, BestCoeff = 0, TempCoeff;
			gmUserObject* pGmObj = NULL;
			const gmType *TypeId;
			int isConst = false;
			gmType TempId;

			#ifdef _DEBUG
				bool Ambiguity = false;
			#endif

			TempProxy = (gmFuncProxy*)pFunc->m_cUserData;
			int FunctionCat = TempProxy->GetFunctionCat();

			void* pThisObj= NULL;
			if( FunctionCat == METHOD_TYPE || FunctionCat == METHOD_NORET_TYPE )
			{	
				const gmVariable* pThis = pThread->GetThis();
				pThisObj = ((gmUserObject*)pThis->m_value.m_ref)->m_user;
				isConst = ((gmUserObject*)pThis->m_value.m_ref)->m_properties & GM_IS_CONST;
			}
			//pProxy=TempProxy;
			while( TempProxy )
			{
				TotalCoeff = 0;

				if( isConst && !TempProxy->isConst() )
				{
					TempProxy = TempProxy->pNextOverloaded;
					continue;
				}

				if( TempProxy->CheckNumParams( NumParam ) )
				{
					TypeId = TempProxy->GetTypeIds();
					for( int x = 0; x < NumParam; ++x )
					{
						if( pThread->Param(x).m_type <= GM_USER )
							TempId = pThread->Param(x).m_type;
						else
						{
							pGmObj = (gmUserObject*)pThread->Param(x).m_value.m_ref;
							TempId = ( pGmObj->m_properties & GM_IS_CONST ) ? -(pThread->Param(x).m_type) : pThread->Param(x).m_type;
						}

						TempCoeff = CompareParam( TempId, TypeId[x] );

						if( TempCoeff )
							TotalCoeff += TempCoeff;
						else
						{
							TotalCoeff = -1;
							break;
						}
					}
				}
				else
					TotalCoeff = -1;

				if( TotalCoeff == ParamCoeff )
				{
					pProxy = TempProxy;

					#ifdef _DEBUG
						Ambiguity = false;
					#endif

					break;
				}
				else if( TotalCoeff > BestCoeff )
				{
					BestCoeff = TotalCoeff;
					pProxy = TempProxy;

					#ifdef _DEBUG
						Ambiguity = false;
					#endif
				}
				#ifdef _DEBUG
				else if( TotalCoeff != 0 && TotalCoeff == BestCoeff )
					Ambiguity = true;
				#endif

				TempProxy = TempProxy->pNextOverloaded;
			}


			if( !pProxy 
				#ifdef _DEBUG
					|| Ambiguity
				#endif
			)
			{
				gmTableObject* pTable = NULL;
				gmMachine* VM = pThread->GetMachine();

				switch( FunctionCat )
				{
					case METHOD_TYPE:
					case METHOD_NORET_TYPE: pTable = VM->GetTypeTable( pThread->GetThis()->m_type );
					case FUNCTION_TYPE:
					case FUNCTION_NORET_TYPE:
					{
						if(!pTable)
							pTable = VM->GetGlobals();

						gmTableIterator it;
						const char* FuncName = NULL;
						gmTableNode* pNode = pTable->GetFirst( it );

						while( pNode )
						{
							if( pNode->m_value.m_type == GM_FUNCTION )
								if( pNode->m_value.m_value.m_ref == pFunc->GetRef() )
								{
									FuncName = pNode->m_key.GetCStringSafe();
									break;
								}
							pNode = pTable->GetNext( it );
						}

						if( FunctionCat == FUNCTION_TYPE || FunctionCat == FUNCTION_NORET_TYPE )
						{
							#ifdef _DEBUG
								if( Ambiguity )
									VM->GetLog().LogEntry( "Ambiguous call to an overloaded function: '%s'", FuncName );
								else
							#endif
									VM->GetLog().LogEntry( "Could not find any matching overloaded function: '%s'", FuncName );
						}
						else
						{
							#ifdef _DEBUG
								if( Ambiguity )
									VM->GetLog().LogEntry( "Ambiguous call to an overloaded method: '%s'", FuncName );
								else
							#endif
									VM->GetLog().LogEntry( "Could not find any matching overloaded method: '%s'", FuncName );
						}

					} break;
				}
			}

			if( pProxy )
			{
				gmVariable ret;
				if(pThread->GetMachine()->GetDebugMode())//if in debug mode
				{
					try
					{//handle exception
						ret=(*pProxy)(pThread, pThisObj );
					}
					catch(std::exception& e)
					{
						printf("Standard exception: %s \n",e.what());
						pThread->Push( ret );// to prevent a stack fault
						return GM_EXCEPTION;
					}
					catch(...)
					{
						printf("Unknown exception\n");
						pThread->Push( ret );// to prevent a stack fault
						return GM_EXCEPTION;
					}
				}
				else
					ret=(*pProxy)(pThread, pThisObj );

				pThread->Push( ret );
				return GM_OK;
			}
		}
	}

	return GM_EXCEPTION;
}

template<> int ConvertType<int>( const gmVariable* pVar )
{
	if( pVar )
	{
		switch( pVar->m_type )
		{
		case GM_INT:
			return pVar->m_value.m_int;

		case GM_FLOAT:
			return (int)pVar->m_value.m_float;

		case GM_C_STRING:
			{
				const char* String = (const char*)((gmUserObject*)pVar->m_value.m_ref)->m_user;
				if( String )
					return atoi( String );

			} break;

		case GM_STRING:
			{
				gmStringObject* pStringObj = (gmStringObject*)pVar->m_value.m_ref;
				if( pStringObj )
				{
					const char* String = pStringObj->GetString();
					if( String )
						return atoi( String );
				}
			}
		}
	}
	
	return 0;
}

template<> float ConvertType<float>( const gmVariable* pVar )
{
	if( pVar )
	{
		switch( pVar->m_type )
		{
		case GM_INT:
			return (float)pVar->m_value.m_int;

		case GM_FLOAT:
			return pVar->m_value.m_float;

		case GM_C_STRING:
			{
				const char* String = (const char*)((gmUserObject*)pVar->m_value.m_ref)->m_user;
				if( String )
					return (float)atof( String );

			} break;

		case GM_STRING:
			{
				gmStringObject* pStringObj = (gmStringObject*)pVar->m_value.m_ref;
				if( pStringObj )
				{
					const char* String = pStringObj->GetString();
					if( String )
						return (float)atof( String );
				}
			}
		}
	}

	return 0.0f;
}

template<> const char* ConvertType<const char*>( const gmVariable* pVar )
{
	if( pVar )
	{
		switch( pVar->m_type )
		{
		case GM_INT:
			{
				static char Buffer[64];
				sprintf( Buffer, "%d", pVar->m_value.m_int );
				return Buffer;
			}

		case GM_FLOAT:
			{
				static char Buffer[64];
				sprintf( Buffer, "%f", pVar->m_value.m_float );
				return Buffer;
			}

		case GM_C_STRING:
			{
				const char* String = (const char*)((gmUserObject*)pVar->m_value.m_ref)->m_user;
				if( String )
					return String;

			} break;

		case GM_STRING:
			{
				gmStringObject* pStringObj = (gmStringObject*)pVar->m_value.m_ref;
				if( pStringObj )
				{
					const char* String = pStringObj->GetString();
					if( String )
						return String;
				}
			}
		}
	}
	
	return "";
}

void GM_CDECL gmMachineEx::OpGetDot(gmThread* pThread, gmVariable* Operand)
{
	if( pThread && Operand )
	{
		GM_ASSERT( Operand->m_type > GM_USER );
		std::map<gmType,gmClassDef*>::const_iterator i = gmMachineEx::ClassList.find(Operand->m_type);

		if( i != gmMachineEx::ClassList.end() )
		{
			gmUserObject* pGmObj = (gmUserObject*)Operand->m_value.m_ref;
			if( pGmObj )
			{
				void* pObj = pGmObj->m_user;
				if( pObj )
				{
					GM_ASSERT( Operand[1].m_type == GM_STRING );
					if( i->second )
					{
						std::map<int,gmVarProxy*>::const_iterator j = i->second->VarList.find( Operand[1].m_value.m_ref );
						gmVarProxy* pProxy = NULL;

						if( j == i->second->VarList.end() )
						{
							const gmClassDef* BaseClass = i->second->BaseClass;

							while( BaseClass )
							{
								j = BaseClass->VarList.find( Operand[1].m_value.m_ref );
								if( j != BaseClass->VarList.end() )
								{
									pProxy = j->second;
									break;
								}

								BaseClass = BaseClass->BaseClass;
							}
						}
						else
							pProxy = j->second;

						if( pProxy )
						{
							*Operand = pProxy->Get( pObj, pThread );
							return;	
						}
					}
				}
			}
		}
		gmAssert(0,"Getdot:variable must exist. the variable has not been registered or wrongly typed\n");
	}
}

void GM_CDECL gmMachineEx::OpSetDot(gmThread* pThread, gmVariable* Operand)
{
	if( pThread && Operand )
	{
		GM_ASSERT( Operand->m_type > GM_USER );
		std::map<gmType,gmClassDef*>::const_iterator i = gmMachineEx::ClassList.find(Operand->m_type);

		if( i != gmMachineEx::ClassList.end() )
		{
			gmUserObject* pGmObj = (gmUserObject*)Operand->m_value.m_ref;
			if( pGmObj )
			{
				if( pGmObj->m_properties & GM_IS_CONST )
				{
					pThread->GetMachine()->GetLog().LogEntry( "L-value specifies const object" );
					return;
				}

				void* pObj = pGmObj->m_user;
				if( pObj )
				{	
					GM_ASSERT( Operand[2].m_type == GM_STRING );
					if( i->second )
					{
						std::map<int,gmVarProxy*>::const_iterator j = i->second->VarList.find( Operand[2].m_value.m_ref );
						gmVarProxy* pProxy = NULL;

						if( j == i->second->VarList.end() )
						{
							const gmClassDef* BaseClass = i->second->BaseClass;

							while( BaseClass )
							{
								j = BaseClass->VarList.find( Operand[2].m_value.m_ref );
								if( j != BaseClass->VarList.end() )
								{
									pProxy = j->second;
									break;
								}

								BaseClass = BaseClass->BaseClass;
							}
						}
						else
							pProxy = j->second;

						if( pProxy )
						{ 
							GM_ASSERT( Operand[1].m_type != GM_NULL );
							gmType TempId;

							if( Operand[1].m_type <= GM_USER )
								TempId = Operand[1].m_type;
							else
							{
								gmUserObject* pGmObj = (gmUserObject*)Operand[1].m_value.m_ref;
								TempId = ( pGmObj->m_properties & GM_IS_CONST ) ? -(Operand[1].m_type) : Operand[1].m_type;
							}

							if( CompareParam( TempId, pProxy->GetTypeId() ) )//pProxy->CompareType( TempId ) )
							{
								pProxy->Set( pObj, Operand[1] );
								return;
							}
						}
					}
				}
			}
		}

		gmAssert(0,"Setdot:variable must exist. the variable has not been registered or wrongly typed\n");
	}
}




gmOperator gmMachineEx::CppOpToGm( const char* Op, bool isUnary )
{
	if( Op )
	{
		if( !strcmp( Op, "+" ) )
		{
			if( isUnary )
				return O_POS;
			else
				return O_ADD;
		}
		else if( !strcmp( Op, "-" ) )
		{
			if( isUnary )
				return O_NEG;
			else
				return O_SUB;
		}
		else if( !strcmp( Op, "*" ) )
			return O_MUL;
		else if( !strcmp( Op, "/" ) )
			return O_DIV;
		else if( !strcmp( Op, "%" ) )
			return O_REM;
		else if( !strcmp( Op, "|" ) )
			return O_BIT_OR;
		else if( !strcmp( Op, "^" ) )
			return O_BIT_XOR;
		else if( !strcmp( Op, "&" ) )
			return O_BIT_AND;
		else if( !strcmp( Op, "<<" ) )
			return O_BIT_SHIFTLEFT;
		else if( !strcmp( Op, ">>" ) )
			return O_BIT_SHIFTRIGHT;
		else if( !strcmp( Op, "~" ) )
			return O_BIT_INV;
		else if( !strcmp( Op, "<" ) )
			return O_LT;
		else if( !strcmp( Op, ">" ) )
			return O_GT;
		else if( !strcmp( Op, "<=" ) )
			return O_LTE;
		else if( !strcmp( Op, ">=" ) )
			return O_GTE;
		else if( !strcmp( Op, "==" ) )
			return O_EQ;
		else if( !strcmp( Op, "!=" ) )
			return O_NEQ;
		else if( !strcmp( Op, "!" ) )
			return O_NOT;
		else if( !strcmp( Op, "[]" ) )
			return O_GETIND;
	}

	return O_MAXOPERATORS;
}

gmThread* gmMachineEx::_BeginFuncCall( gmFunctionObject* func, const gmVariable& This )
{
	if( func )
	{
		gmThread* pThread = CreateThread();
		if( pThread )
		{
			pThread->Push(This);
			pThread->PushFunction( func );
				return pThread;
		}
	}
	return NULL;
}

gmExVariable gmMachineEx::_EndFuncCall( gmThread* pThread, int ParamNumber )
{
	gmExVariable ResultVar;

	if( pThread )
	{
		pThread->PushStackFrame( ParamNumber );
		GM_ASSERT( ParamNumber <= pThread->GetFunctionObject()->GetNumParams() );
		pThread->Sys_Execute(&ResultVar);
	}
		
	return ResultVar;
}

gmType GetRuntimeId( const type_info* pInfo )
{
	if( pInfo )
	{
		std::map<gmType,gmClassDef*>::const_iterator i = gmMachineEx::ClassList.begin();

		while( i != gmMachineEx::ClassList.end() )
		{
			if( i->second )
			{
				if( *pInfo == *(i->second->pTypeInfo) || *pInfo == *(i->second->pConstTypeInfo) )
					return i->first;
			}
		
			++i;
		}
	}

	return GM_UNKNOWN;
};

int GM_CDECL gmConstCast( gmThread* pThread )
{
	scriptAssert( pThread->GetNumParams() == 1 ,"Wrong number of Param");
	scriptAssert( pThread->ParamType(0) > GM_USER ,"Invalid type");

	gmUserObject* pGmObj = (gmUserObject*)pThread->Param(0).m_value.m_ref;
	if( pGmObj )
	{
		gmUserObject* pNewObj = pThread->GetMachine()->AllocUserObject( pGmObj->m_user, pGmObj->m_userType );

		if( pGmObj->m_properties & GM_IS_CONST )
			pNewObj->m_properties = GM_IS_NATIVE;
		else
			pNewObj->m_properties = GM_IS_NATIVE | GM_IS_CONST;

		pThread->PushUser( pNewObj );
		return GM_OK;
	}

	return GM_EXCEPTION;
}

void GM_CDECL gmExIntermediateDestructor( gmMachine* VM, gmUserObject* pGmObj )
{
	if( pGmObj->m_user && !( pGmObj->m_properties & GM_IS_NATIVE ) )
	{
		std::map<gmType,gmClassDef*>::const_iterator i = gmMachineEx::ClassList.find( pGmObj->m_userType );

		if( i != gmMachineEx::ClassList.end() && i->second )
		{
			if( i->second->pDestructor )
				(*i->second->pDestructor)( VM, pGmObj );
		}
	}
}


int GM_CDECL gmIntermediateGet( gmThread* pThread )
{
	const gmFunctionObject* pFunc = pThread->GetFunctionObject();
	if( pFunc && pFunc->m_cUserData )
	{
		gmFuncProxy* pProxy = (gmFuncProxy*)pFunc->m_cUserData;
		if( pProxy )
		{
			gmVariable Ret = (*pProxy)( pThread, NULL );

			scriptAssert( Ret.m_type > GM_USER ,"Invalid type");
			if( Ret.m_type > GM_USER )
			{
				gmUserObject* pGmObj = (gmUserObject*)Ret.m_value.m_ref;
				if( pGmObj )
				{
					pGmObj->m_properties ^= GM_IS_NATIVE;
					pThread->Push( Ret );
					return GM_OK;
				}
			}
		}
	}

	return GM_EXCEPTION;
}

int GM_CDECL gmDelete( gmThread* pThread )
{
	scriptAssert( pThread->GetNumParams() == 1 ,"Wrong number of param");
	scriptAssert( pThread->ParamType(0) > GM_USER ,"Invalid type");

	gmUserObject* pGmObj = (gmUserObject*)pThread->Param(0).m_value.m_ref;
	
	if( pGmObj->m_properties & GM_IS_NATIVE )
		pGmObj->m_properties ^= GM_IS_NATIVE;

	return GM_OK;
}



int GM_CDECL gmGet( gmThread* pThread )
{
	if( pThread )
	{
		scriptAssert( pThread->GetThis() != NULL ,"\"this\" cannot be null");
		scriptAssert( pThread->GetNumParams() == 0 ,"Wrong number of param");

		const gmVariable* pThis = pThread->GetThis();
		if( pThis )
		{
			void* pObj = ((gmUserObject*)pThis->m_value.m_ref)->m_user;
			gmVariable Ret;

			switch( pThis->m_type )
			{
				case GM_BOOL_PTR:
					Ret = gmVariable( (int)*((bool*)pObj) ); break;

				case GM_INT_PTR:
					Ret = gmVariable( *((int*)pObj) ); break;

				case GM_SHORT_PTR:
					Ret = gmVariable( *((short*)pObj) ); break;

				case GM_LONG_PTR:
					Ret = gmVariable( *((long*)pObj) ); break;

				case GM_LONGLONG_PTR:
					Ret = gmVariable( (int)*((long long*)pObj) ); break;
				
				case GM_UINT_PTR:
					Ret = gmVariable( (int)*((unsigned int*)pObj) ); break;
				
				case GM_USHORT_PTR:
					Ret = gmVariable( (int)*((unsigned short*)pObj) ); break;
				
				case GM_ULONG_PTR:
					Ret = gmVariable( (int)*((unsigned long*)pObj) ); break;
				
				case GM_ULONGLONG_PTR:
					Ret = gmVariable( (int)*((unsigned long long*)pObj) ); break;

				case GM_FLOAT_PTR:
					Ret = gmVariable( *((float*)pObj) ); break;

				case GM_DOUBLE_PTR:
					Ret = gmVariable( (float)*((double*)pObj) ); break;

				case GM_LONGDOUBLE_PTR:
					Ret = gmVariable( (float)*((long double*)pObj) ); break;
			}

			pThread->Push( Ret );
			return GM_OK;
		}
	}

	return GM_EXCEPTION;
}


int GM_CDECL gmSet( gmThread* pThread )
{
	if( pThread )
	{
		scriptAssert( pThread->GetThis() != NULL ,"\"this\" cannot be null");
		scriptAssert( pThread->GetNumParams() == 1 ,"Wrong number of Param");

		const gmVariable* pThis = pThread->GetThis();
		if( pThis )
		{
			void* pObj = ((gmUserObject*)pThis->m_value.m_ref)->m_user;
			int isConst = ((gmUserObject*)pThis->m_value.m_ref)->m_properties & GM_IS_CONST;

			switch( pThis->m_type )
			{
				case GM_BOOL_PTR:
				{
					int Val = ConvertType<int>( &pThread->Param(0) );
					if( !isConst )
						*(bool*)pObj = Val;

				} break;

				case GM_INT_PTR:
				{
					int Val = ConvertType<int>( &pThread->Param(0) );
					if( !isConst )
						*(int*)pObj = Val;

				} break;

				case GM_SHORT_PTR:
				{
					int Val = ConvertType<int>( &pThread->Param(0) );
					if( !isConst )
						*(short*)pObj = Val;

				} break;

				case GM_LONG_PTR:
				{
					int Val = ConvertType<int>( &pThread->Param(0) );
					if( !isConst )
						*(long*)pObj = Val;

				} break;

				case GM_LONGLONG_PTR:
				{
					int Val = ConvertType<int>( &pThread->Param(0) );
					if( !isConst )
						*(long long*)pObj = Val;

				} break;
				
				case GM_UINT_PTR:
				{
					int Val = ConvertType<int>( &pThread->Param(0) );
					if( !isConst )
						*(unsigned int*)pObj = Val;

				} break;
				
				case GM_USHORT_PTR:
				{
					int Val = ConvertType<int>( &pThread->Param(0) );
					if( !isConst )
						*(unsigned short*)pObj = Val;

				} break;
				
				case GM_ULONG_PTR:
				{
					int Val = ConvertType<int>( &pThread->Param(0) );
					if( !isConst )
						*(unsigned long*)pObj = Val;

				} break;
				
				case GM_ULONGLONG_PTR:
				{
					int Val = ConvertType<int>( &pThread->Param(0) );
					if( !isConst )
						*(unsigned long long*)pObj = Val;

				} break;

				case GM_FLOAT_PTR:
				{
					float Val = ConvertType<float>( &pThread->Param(0) );
					if( !isConst )
						*(float*)pObj = Val;

				} break;

				case GM_DOUBLE_PTR:
				{
					float Val = ConvertType<float>( &pThread->Param(0) );
					if( !isConst )
						*(double*)pObj = Val;

				} break;

				case GM_LONGDOUBLE_PTR:
				{
					float Val = ConvertType<float>( &pThread->Param(0) );
					if( !isConst )
						*(long double*)pObj = Val;
				} break;
			}


			#ifdef _DEBUG
			if( isConst )
				pThread->GetMachine()->GetLog().LogEntry( "You cannot assign to a variable that is const" );
			#endif

			return GM_OK;
		}
	}

	return GM_EXCEPTION;
}



inline int CompareParam( gmType GmParamType, gmType CppParamType )
{
	if( CppParamType == GmParamType )
		return 3;

	switch( GmParamType )
	{
		case GM_INT:
		{
			if( CppParamType == GM_FLOAT )
				return 2;
			else if( CppParamType == GM_C_STRING || CppParamType == GM_CONST_C_STRING || CppParamType == GM_STRING )
				return 1;

		} break;

		case GM_FLOAT:
		{
			if( CppParamType == GM_INT )
				return 2;
			else if( CppParamType == GM_C_STRING || CppParamType == GM_CONST_C_STRING || CppParamType == GM_STRING )
				return 1;

		} break;

		case GM_STRING:
		{
			if( CppParamType == GM_C_STRING || CppParamType == GM_CONST_C_STRING )
				return 2;
			else if( CppParamType == GM_ANY_PTR || CppParamType == GM_CONST_ANY_PTR || CppParamType == GM_INT || CppParamType == GM_FLOAT )
				return 1;

		} break;

		case GM_CONST_C_STRING:
		{
			if( CppParamType == GM_CONST_ANY_PTR || CppParamType == GM_INT || CppParamType == GM_FLOAT )
				return 1;
			else if( CppParamType == GM_STRING )
				return 2;

		} break;

		case GM_C_STRING:
		{
			if( CppParamType == GM_CONST_C_STRING )
				return 3;
			else if( CppParamType == GM_ANY_PTR || CppParamType == GM_CONST_ANY_PTR || CppParamType == GM_INT || CppParamType == GM_FLOAT )
				return 1;	
			else if( CppParamType == GM_STRING )
				return 2;		

		} break;
				
		case GM_TABLE:
		case GM_FUNCTION:
		{
			if( CppParamType == GM_ANY_PTR || CppParamType == GM_CONST_ANY_PTR )
				return 1;

		} break;
			
		default:
		{
			static std::map<gmType,gmClassDef*>::const_iterator i;

			if( GmParamType > 0 ) //GmParamType est non const
			{
				if( CppParamType == (-GmParamType) )
					return 3;
				else if( CppParamType == GM_ANY_PTR || CppParamType == GM_CONST_ANY_PTR )
					return 1;
				else
					i = gmMachineEx::ClassList.find( GmParamType );
			}
			else //Type est const ptr
			{
				if( CppParamType == (-GmParamType) || CppParamType == GM_ANY_PTR )
					return 0;
				else if( CppParamType == GM_CONST_ANY_PTR )
					return 1;
				else
					i = gmMachineEx::ClassList.find( -GmParamType );
			}

			if( i != gmMachineEx::ClassList.end() )
			{
				static const gmClassDef* pDef;
				pDef = i->second;
				while( pDef )
				{
					gmType TempId = CppParamType > 0 ? CppParamType : -CppParamType;
					if( pDef->BaseClassType == TempId )
					{
						if( GmParamType > 0 )
							return 2;
						else
						{
							if( CppParamType < 0 )
								return 2;
							else
								return 0;
						}
					}

					pDef = pDef->BaseClass;
				}
			}
		}
	}
	
	return 0;
}


#pragma warning ( default: 4800 )
#pragma warning ( default: 4244 )