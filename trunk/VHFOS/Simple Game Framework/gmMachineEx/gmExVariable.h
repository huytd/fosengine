#ifndef GM_VARIABLE_EX_H
#define GM_VARIABLE_EX_H

/*====================================================================================================
  Une extension de la structure gmVariable de base plus simple à utiliser grâce aux opérateurs de cast
======================================================================================================*/

struct gmExVariable : public gmVariable
{
	gmExVariable(){ m_type=GM_NULL; }
	explicit gmExVariable(int iVal) : gmVariable( iVal ){}
	explicit gmExVariable(float fVal) : gmVariable( fVal ){}
	explicit gmExVariable(gmStringObject* String) : gmVariable( String ){}
	explicit gmExVariable(gmTableObject* pTable) : gmVariable( pTable ){}
	explicit gmExVariable(gmFunctionObject* pFunc) : gmVariable( pFunc ){}
	explicit gmExVariable(gmUserObject* pGmObj) : gmVariable( pGmObj ){}

	bool operator==( gmType Type ) const { return m_type == Type; }
	bool operator!=( gmType Type ) const { return m_type != Type; }
	bool operator<=( gmType Type ) const { return m_type <= Type; }
	bool operator>=( gmType Type ) const { return m_type >= Type; }
	bool operator< ( gmType Type ) const { return m_type < Type; }
	bool operator> ( gmType Type ) const { return m_type > Type; }

	char* toString() const;
	int toInt()								const {	return ConvertType<int>( this ); }
	float toFloat()							const {	return ConvertType<float>( this ); }
	const char* toConstString()				const { return ConvertType<const char*>( this ); }	
	gmStringObject* toStringObj()			const { if( m_type == GM_STRING	)	return (gmStringObject*)m_value.m_ref				; else return NULL; }
	gmFunctionObject* toFuncObj()			const { if( m_type == GM_FUNCTION ) return (gmFunctionObject*)m_value.m_ref				; else return NULL; }
	gmTableObject* toTableObj()				const { if( m_type == GM_TABLE )	return (gmTableObject*)m_value.m_ref				; else return NULL; }
	gmUserObject* toUserObj()				const { if( m_type > GM_USER )		return (gmUserObject*)m_value.m_ref					; else return NULL; }
	template<class T> T toUser()			const { if( m_type > GM_USER )		return (T)((gmUserObject*)m_value.m_ref)->m_user	; else return NULL; }
	template<class T> void toUser( T& Obj ) const { if( m_type > GM_USER )		Obj = (T)((gmUserObject*)m_value.m_ref)->m_user; }

	operator int()					const { return toInt(); }
	operator float()				const { return toFloat(); }
	operator const char*()			const { return toConstString(); }
	operator char*()				const { return toString(); }
	operator gmStringObject*()		const { return toStringObj(); }
	operator gmFunctionObject*()	const { return toFuncObj(); }
	operator gmTableObject*()		const { return toTableObj(); }
	operator gmUserObject*()		const { return toUserObj(); }
	template<class T> operator T*() const { return toUser(); }
};

#endif