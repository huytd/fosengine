#include "gmMachineEx.h"
#include "gmExOperator.h"

#pragma warning (disable: 4127)
// Hmm, I need to talk about this with Greg/Matt. Maybe disabling GC would be good enough:
// Quote from gmOperator.h:
// Note though, that if you are creating gmObjects from within one of these operators, you must call
// a_thread->SetTop() with the known top of stack before you allocate your gmObject such that the garbage
// collector can mark all objects properly.

void CallOperatorUnary( gmThread* pThread, gmVariable *operand, gmOperator Op ) 
{
	std::map<gmType,gmClassDef*>::const_iterator i = gmMachineEx::ClassList.find( operand->m_type );

	if( i != gmMachineEx::ClassList.end() && i->second )
	{
		std::map<gmOperator,gmFuncProxy*>::const_iterator j = i->second->OpList.find( Op );

		if( j != i->second->OpList.end() )
		{
			gmFuncProxy* FuncProxy = NULL;
			gmFuncProxy* TempProxy = j->second;

			gmUserObject* pGmObj = (gmUserObject*)operand->m_value.m_ref;
			int isConst = pGmObj->m_properties & GM_IS_CONST;
			int TotalCoeff, BestCoeff = 0;
			const gmType* TypeId;

			while( TempProxy )
			{
				TotalCoeff = 3;
				TypeId = TempProxy->GetTypeIds();

				if( TempProxy->GetFunctionCat() == FUNCTION_OPERATOR_TYPE )
				{
					gmType TempId = isConst ? -(operand->m_type) : operand->m_type;
					TotalCoeff = CompareParam( TempId, *TypeId );
				}
				else
				{
					if( isConst && !TempProxy->isConst() )
						TotalCoeff = 0;
				}

				if( TotalCoeff == 3 )
				{
					FuncProxy = TempProxy;
					break;
				}
				else if( TotalCoeff > BestCoeff )
				{
					BestCoeff = TotalCoeff;
					FuncProxy = TempProxy;
				}
						
				TempProxy = TempProxy->pNextOverloaded;
			}

			if( FuncProxy )
				*operand = (*FuncProxy)(operand,pThread->GetMachine());
			else
				pThread->GetMachine()->GetLog().LogEntry( "Could not find any matching overloaded operator: '%s'", gmGetOperatorName( Op ) );
		}
	}
}

void CallOperatorBinary( gmThread* pThread, gmVariable *operand, gmOperator Op )
{
	int isConst = false;
	bool SkipMethodOperators;
	gmType Type1, Type2, TempIdType1, TempIdType2;
	const gmType *TypeId;

	gmMachine* VM = pThread->GetMachine();

	if( VM )
	{
		gmUserObject* pGmObj;

		if( operand[0].m_type <= GM_USER )
			TempIdType1 = operand[0].m_type;
		else
		{
			pGmObj = (gmUserObject*)operand[0].m_value.m_ref;
			isConst = pGmObj->m_properties & GM_IS_CONST;
			TempIdType1 = isConst ? -(operand[0].m_type) : operand[0].m_type;
		}

		if( operand[1].m_type <= GM_USER )
			TempIdType2 = operand[1].m_type;
		else
		{
			pGmObj = (gmUserObject*)operand[1].m_value.m_ref;
			TempIdType2 = ( pGmObj->m_properties & GM_IS_CONST ) ? -(operand[1].m_type) : operand[1].m_type;
		}

		if( operand[0].m_type > GM_USER && VM->GetTypeNativeOperator( operand[0].m_type, Op ) )
		{
			Type1 = operand[0].m_type;
			Type2 = operand[1].m_type;

			SkipMethodOperators = false;
		}
		else 
		{
			Type1 = operand[1].m_type;
			Type2 = operand[0].m_type;

			SkipMethodOperators = true;
		}

		#ifdef _DEBUG
			bool Ambiguity = false;
		#endif

		int TotalCoeff, TempCoeff, BestCoeff = 0;
		std::map<gmType,gmClassDef*>::const_iterator i = gmMachineEx::ClassList.find( Type1 );

		if( i != gmMachineEx::ClassList.end() && i->second )
		{
			std::map<gmOperator,gmFuncProxy*>::const_iterator j = i->second->OpList.find( Op );

			if( j != i->second->OpList.end() )
			{
				gmFuncProxy* FuncProxy = NULL;
				gmFuncProxy* TempProxy = j->second;

				while( TempProxy )
				{
					TotalCoeff = 0;
					TypeId = TempProxy->GetTypeIds();

					if( TempProxy->GetFunctionCat() == FUNCTION_OPERATOR_TYPE )
					{
						TotalCoeff = CompareParam( TempIdType1, *TypeId );
						if( TotalCoeff )
						{
							TempCoeff = CompareParam( TempIdType2, TypeId[1] );
							if(TempCoeff)
								TotalCoeff += TempCoeff;
							else
								TotalCoeff = 0;
						}
					}
					else
					{
						if( !SkipMethodOperators )
						{
							if( isConst )
							{
								if( TempProxy->isConst() )
									TotalCoeff = 3;
							}
							else
								TotalCoeff = 3;
							
							if( TotalCoeff )
							{
								TempCoeff = CompareParam( TempIdType2, *TypeId );
								if(TempCoeff)
									TotalCoeff += TempCoeff;
								else
									TotalCoeff = 0;
							}
						}
					}

					if( TotalCoeff == 6 )
					{
						FuncProxy = TempProxy;

						#ifdef _DEBUG
							Ambiguity = false;
						#endif

						break;
					}
					else if( TotalCoeff > BestCoeff )
					{
						BestCoeff = TotalCoeff;
						FuncProxy = TempProxy;

						#ifdef _DEBUG
							Ambiguity = false;
						#endif
					}
					#ifdef _DEBUG
					else if( TotalCoeff != 0 && TotalCoeff == BestCoeff )
						Ambiguity = true;
					#endif
						
					TempProxy = TempProxy->pNextOverloaded;

					if( !TempProxy )
					{
						if( Type2 != Type1 && Type2 > GM_USER && VM->GetTypeNativeOperator(Type2,Op) )
						{
							i = gmMachineEx::ClassList.find( Type2 );
							if( i != gmMachineEx::ClassList.end() && i->second )
							{
								j = i->second->OpList.find( Op );
								if( j != i->second->OpList.end() )
									TempProxy = j->second;
							}

							Type2 = GM_NULL;
						}
						else
							break;
					}
				}

				#ifdef _DEBUG
					if( Ambiguity )
						VM->GetLog().LogEntry( "Ambiguous call to an overloaded operator: '%s'", gmGetOperatorName( Op ) );
				#endif

				if( FuncProxy )
				{
					if( Op == O_SETIND )
						operand->m_type = -operand->m_type;

					*operand = (*FuncProxy)(operand,VM);
				}
				else
				{
					VM->GetLog().LogEntry( "Could not find any matching overloaded operator: '%s'", gmGetOperatorName( Op ) );
					operand->Nullify();
				}
			}
		}
	}
}


void GM_CDECL gmOpGetInd(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_GETIND );
};

void GM_CDECL gmOpSetInd(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_SETIND );
};

void GM_CDECL gmOpAdd(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_ADD );
};

void GM_CDECL gmOpSub(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_SUB );
};

void GM_CDECL gmOpMul(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_MUL );
};

void GM_CDECL gmOpDiv(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_DIV );
};

void GM_CDECL gmOpRem(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_REM );
};

void GM_CDECL gmOpBitOr(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_BIT_OR );
};

void GM_CDECL gmOpBitXor(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_BIT_XOR );
};

void GM_CDECL gmOpBitAnd(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_BIT_AND );
};

void GM_CDECL gmOpBitShiftLeft(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_BIT_SHIFTLEFT );
};

void GM_CDECL gmOpBitShiftRight(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_BIT_SHIFTRIGHT );
};

void GM_CDECL gmOpBitInv(gmThread* pThread, gmVariable *operand)
{
	CallOperatorUnary( pThread, operand, O_BIT_INV );
};

void GM_CDECL gmOpLessThan(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_LT );
};

void GM_CDECL gmOpGreaterThan(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_GT );
};

void GM_CDECL gmOpLessThanOrEqual(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_LTE );
};

void GM_CDECL gmOpGreaterThanOrEqual(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_GTE );
};

void GM_CDECL gmOpEqual(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_EQ );
};

void GM_CDECL gmOpNotEqual(gmThread* pThread, gmVariable *operand)
{
	CallOperatorBinary( pThread, operand, O_NEQ );
};

void GM_CDECL gmOpNeg(gmThread* pThread, gmVariable *operand)
{
	CallOperatorUnary( pThread, operand, O_NEG );
};

void GM_CDECL gmOpPos(gmThread* pThread, gmVariable *operand)
{
	CallOperatorUnary( pThread, operand, O_POS );
};

void GM_CDECL gmOpNot(gmThread* pThread, gmVariable *operand)
{
	CallOperatorUnary( pThread, operand, O_NOT );
};


#pragma warning (default: 4127)