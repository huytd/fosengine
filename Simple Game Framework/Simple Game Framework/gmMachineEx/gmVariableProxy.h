#ifndef GM_VARIABLE_PROXY_H
#define GM_VARIABLE_PROXY_H

class gmVarProxy
{
public:
	virtual ~gmVarProxy(){}
	virtual void UpdateTypeId() = 0;
	virtual gmType GetTypeId() const = 0;
	virtual void Set(void* pObj, gmVariable& Value) const = 0;
	virtual gmVariable Get(void* pObj, gmThread* pThread ) const = 0;
};

template<class CLASS, typename TYPE, int ACCESS_TYPE> class _gmVarProxy : public gmVarProxy
{
public:
	_gmVarProxy( TYPE CLASS::*pVar )
	{
		this->pVar = pVar;
		TypeId = GM_UNKNOWN;
	}

	_gmVarProxy( const TYPE CLASS::*pConstVar )
	{
		this->pConstVar = pConstVar;
		TypeId = GM_UNKNOWN;
	}

	_gmVarProxy( TYPE (CLASS::*pGet)(), void (CLASS::*pSet)(TYPE) )
	{
		pGetSet.pGet = pGet;
		pGetSet.pSet = pSet;
		TypeId = GM_UNKNOWN;
	}

	gmVariable Get(void* pObj, gmThread* pThread ) const
	{ 
		gmType TempId = TypeId > 0 ? TypeId : -TypeId;
		return SWITCH< ACCESS_TYPE >::Get<CLASS,TYPE>( (CLASS*)pObj, pVar, pConstVar, pGetSet.pGet, pThread->GetMachine(), TempId );
	}

	void Set(void* pObj, gmVariable& Value) const
	{
		SWITCH< ACCESS_TYPE >::Set<CLASS,TYPE>( (CLASS*)pObj, pVar, NULL, pGetSet.pSet, Value );
	}

	void UpdateTypeId()
	{ 
		if( TypeId == GM_UNKNOWN )
		{
			TypeId = IS_BASIC_TYPE( GET<TYPE>::StaticId ) ? GET<TYPE>::StaticId : GetRuntimeId<TYPE>();
			if( TypeId != GM_UNKNOWN )
				TypeId = IS<TYPE>::Const ? -TypeId : TypeId;
		}
	}

	gmType GetTypeId() const{ return TypeId; }

private:
	union
	{
		TYPE CLASS::*pVar;
		const TYPE CLASS::*pConstVar;
		struct
		{
			TYPE (CLASS::*pGet)();
			void (CLASS::*pSet)(TYPE);
		} pGetSet;
	};

	gmType TypeId;
};


template<class CLASS, typename TYPE> inline gmVarProxy* CreateVariableProxy( TYPE CLASS::*pVar, bool ReadOnly = false)
{
	if( ReadOnly )
		return new _gmVarProxy<CLASS,TYPE,DIRECT_ACCESS_READ_ONLY>( (const TYPE CLASS::*) pVar );
	else
		return new _gmVarProxy<CLASS,TYPE,DIRECT_ACCESS>( pVar );
}

template<class CLASS, typename TYPE> inline gmVarProxy* CreateVariableProxy( const TYPE CLASS::*pConstVar, bool = true )
{
	return new _gmVarProxy<CLASS,TYPE,DIRECT_ACCESS_READ_ONLY>( pConstVar );
}

template<class CLASS, typename TYPE> inline gmVarProxy* CreateGetSetVariableProxy( TYPE(CLASS::*pGet)(), void(CLASS::*pSet)(TYPE) = NULL )
{
	GM_ASSERT( pGet );// getter cannot be null
	if( pGet )
		return new _gmVarProxy<CLASS,TYPE,GET_SET_ACCESS>( pGet, pSet );
	else
		return NULL;
}

#endif