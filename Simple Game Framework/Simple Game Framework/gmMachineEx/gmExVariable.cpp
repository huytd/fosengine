#include "gmMachineEx.h"

char* gmExVariable::toString() const
{
	const char* Temp = ConvertType<const char*>( this );
	
	if( Temp )
	{
		size_t Length = strlen( Temp );
		char* NewString = new char[ Length + 1 ];
		if( NewString )
		{
			strcpy( NewString, Temp );
			NewString[ Length ] = '\0';
			return NewString;
		}
	}
			
	return NULL;
}