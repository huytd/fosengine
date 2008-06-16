
//////////////////////////////////////////////////////////////////////
//
// CyoDecode.cpp
//
// Developed by Graham Bull for Cyotec Systems Limited.
// http://www.cyotec.com
//
// Copyright (c) 2004 by Cyotec Systems Limited. All Rights Reserved.
//
// This source code may be used, modified, compiled, duplicated, and/or
// distributed without restriction provided this copyright notice remains intact.
// Cyotec Systems Limited and/or its employees cannot be held responsible for any
// direct or indirect damage or loss of any kind that may result from using this
// code, and provides no warranty, guarantee, or support.
//
// Associated Files
// - CyoDecode.h
// - CyoEncode.h
// - CyoEncode.cpp
//
// History
// - September 22, 2004 - Created
// - November 04, 2004 - Not strictly ANSI compatible!
//
//////////////////////////////////////////////////////////////////////

#include "CyoDecode.h"

#include <stdexcept> //for std::runtime_error
#include <assert.h> //for assert()

typedef unsigned char BYTE, *LPBYTE;
typedef unsigned long DWORD;

////////////////////////////////////////
// Constants

namespace CyoDecode
{
    DWORD BASE16_INPUT = 2;
    DWORD BASE16_OUTPUT = 1;
    BYTE BASE16_TABLE[ 0x80 ] = {
        /*00-07*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*08-0f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*10-17*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*18-1f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*20-27*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*28-2f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*30-37*/ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, //8 = '0'-'7'
        /*38-3f*/ 0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //2 = '8'-'9'
        /*40-47*/ 0xFF, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xFF, //6 = 'A'-'F'
        /*48-4f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*50-57*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*58-5f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*60-67*/ 0xFF, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xFF, //6 = 'a'-'f' (same as 'A'-'F')
        /*68-6f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*70-77*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /*78-7f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };

	const char* c_pszErrorInvalidParameter = "Invalid parameter";
	const char* c_pszErrorNotMultipleOf2 = "Invalid source, size is not a multiple of 2";}

////////////////////////////////////////
// Base16 Decoding

unsigned long CyoDecode::Base16DecodeGetLength( unsigned long size )
{
    // output 1 byte for every 2 input:
    //                1
    // 1 = ----1111 = 1111----
    // 2 = ----1111 = ----1111

    if (size % BASE16_INPUT != 0)
        throw std::runtime_error( c_pszErrorNotMultipleOf2 );

    return (((size + BASE16_INPUT - 1) / BASE16_INPUT) * BASE16_OUTPUT);
}

unsigned long CyoDecode::Base16Decode( void* dest, const void* src, unsigned long size )
{
    if ((dest == NULL) || (src == NULL))
        throw std::runtime_error( c_pszErrorInvalidParameter );

    if (size % BASE16_INPUT != 0)
        throw std::runtime_error( c_pszErrorNotMultipleOf2 );

    ///

    LPBYTE pSrc = (LPBYTE)src;
    LPBYTE pDest = (LPBYTE)dest;
    DWORD dwSrcSize = size;
    DWORD dwDestSize = 0;

    while (dwSrcSize >= 1)
    {
        // 2 inputs...
        BYTE in1 = *pSrc++;
        BYTE in2 = *pSrc++;
        dwSrcSize -= BASE16_INPUT; //2

        // Validate ascii...
        assert( 0 <= in1 && in1 <= 0x7f );
        assert( 0 <= in2 && in2 <= 0x7f );

        // Convert ascii to base16...
        in1 = BASE16_TABLE[ in1 ];
        in2 = BASE16_TABLE[ in2 ];

        // Validate base16...
        assert( in1 != 0xff );
        assert( 0 <= in1 && in1 <= 15 );
        assert( 0 <= in2 && in2 <= 15 );

        // 1 output...
        *pDest++ = ((in1 << 4) | in2);
        dwDestSize += BASE16_OUTPUT; //1
    }

    return dwDestSize;
}