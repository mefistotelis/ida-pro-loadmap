////////////////////////////////////////////////////////////////////////////////
/// @file stdafx.h
///     Include file for standard system include files.
/// @par Purpose:
///     Includes standard system headers, or project specific include files
///     that are used frequently, but are changed infrequently.
/// @author TQN <truong_quoc_ngan@yahoo.com>
/// @author TL <mefistotelis@gmail.com>
/// @date 2004.09.11 - 2011.09.13
/// @par  Copying and copyrights:
///     This program is free software; you can redistribute it and/or modify
///     it under the terms of the GNU General Public License as published by
///     the Free Software Foundation; either version 2 of the License, or
///     (at your option) any later version.
////////////////////////////////////////////////////////////////////////////////
#ifndef STDAFX_H_
#define STDAFX_H_

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _OBJC_NO_COM
//#define WINSHLWAPI
#define NOGDI
#include <windef.h>
#include <basetyps.h>
#include <cstdarg>
#include <winbase.h>
#include <winuser.h>
//#include <windows.h>

// C RTL Debug Support Header Files
//#include <crtdbg.h>

// Shell Lightweight API
#include <winreg.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

void pathExtensionSwitch(char * fname, const char * newext, size_t fnbuf_len);

#ifdef _DEBUG
    #define _VERIFY(x)  _ASSERTE(x)

    #define WIN32CHECK(x)   { \
        DWORD __dwErr__ = GetLastError(); \
        _ASSERTE(x); \
        SetLastError(__dwErr__); \
    };
#else
    #define _VERIFY(x)  (x)
    #define WIN32CHECK(x)   (x)
#endif

#endif //!STDAFX_H_
////////////////////////////////////////////////////////////////////////////////
