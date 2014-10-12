// stdafx.cpp : source file that includes just the standard includes
// LoadMap.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include <cstring>

#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////
/// @brief Changes extension in given file name buffer
/// @return void
/// @author TL
/// @date 2011.08.07
 ////////////////////////////////////////////////////////////////////////////////
void pathExtensionSwitch(char * fname, const char * newext, size_t fnbuf_len)
{
    size_t len,extlen;
    char * target;
    char * mintarget;
    len = std::strlen(fname);
    target = std::strrchr(fname,'.');
    mintarget = std::strpbrk(fname,":\\/");
    if ( (target == NULL) || (target <= mintarget) )
        target = fname+len;
    extlen = std::strlen(newext);
    // If end of the buffer
    if (target+extlen+1 >= fname+fnbuf_len)
        return;
    strncpy(target, newext, extlen+1);
}

