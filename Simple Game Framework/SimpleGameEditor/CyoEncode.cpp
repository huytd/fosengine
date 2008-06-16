
//////////////////////////////////////////////////////////////////////
//
// CyoEncode.cpp
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
// - CyoEncode.h
// - CyoDecode.h
// - CyoDecode.cpp
//
// History
// - September 22, 2004 - Created
// - November 04, 2004 - Not strictly ANSI compatible!
//
//////////////////////////////////////////////////////////////////////

#include "CyoEncode.h"

#include <stdexcept> //for std::runtime_error
#include <assert.h> //for assert()

typedef unsigned char BYTE, *LPBYTE;
typedef unsigned long DWORD;
typedef const char* LPCSTR;

////////////////////////////////////////
// Constants

namespace CyoEncode
{
    DWORD BASE16_INPUT = 1;
    DWORD BASE16_OUTPUT = 2;
    LPCSTR BASE16_TABLE = "0123456789ABCDEF";

    const char* c_pszErrorInvalidParameter = "Invalid parameter";
}

////////////////////////////////////////
// Base16 Encoding

unsigned long CyoEncode::Base16EncodeGetLength( unsigned long size )
{
    // output 2 bytes for every 1 input:
    //                1
    // 1 = ----1111 = 1111----
    // 2 = ----1111 = ----1111

    return (((size + BASE16_INPUT - 1) / BASE16_INPUT) * BASE16_OUTPUT);
}

unsigned long CyoEncode::Base16Encode( void* dest, const void* src, unsigned long size )
{
    if (!dest || !src)
        throw std::runtime_error( c_pszErrorInvalidParameter );

    ///

    LPBYTE pSrc = (LPBYTE)src;
    LPBYTE pDest = (LPBYTE)dest;
    DWORD dwSrcSize = size;
    DWORD dwDestSize = 0;

    while (dwSrcSize >= 1)
    {
        // 1 input...
        BYTE ch = *pSrc++;
        dwSrcSize -= BASE16_INPUT; //1

        // 2 outputs...
        *pDest++ = BASE16_TABLE[ (ch & 0xf0) >> 4 ];
        *pDest++ = BASE16_TABLE[ (ch & 0x0f)      ];
        dwDestSize += BASE16_OUTPUT; //2
    }

    return dwDestSize;
}