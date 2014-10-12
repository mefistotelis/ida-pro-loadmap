////////////////////////////////////////////////////////////////////////////////
/// @file MAPReader.h
///     MAP file analysis and loading routines.
/// @par Purpose:
///     Library for loading MAP file entries.
/// @author TQN <truong_quoc_ngan@yahoo.com>
/// @author TL <mefistotelis@gmail.com>
/// @date 2004.09.11 - 2011.09.13
/// @par  Copying and copyrights:
///     This program is free software; you can redistribute it and/or modify
///     it under the terms of the GNU General Public License as published by
///     the Free Software Foundation; either version 2 of the License, or
///     (at your option) any later version.
////////////////////////////////////////////////////////////////////////////////

#include  "MAPReader.h"

#include  <cstring>
#include  <cctype>
#include  <cassert>

#include "stdafx.h"

using namespace std;

namespace MapFile {

/// @name Strings used to identify start of symbol table in various MAP files.
/// @{
const char MSVC_HDR_START[]        = "Address         Publics by Value              Rva+Base     Lib:Object";
const char BCCL_HDR_NAME_START[]   = "Address         Publics by Name";
const char BCCL_HDR_VALUE_START[]  = "Address         Publics by Value";
const char WATCOM_MEMMAP_START[]   = "Address        Symbol";
const char WATCOM_MEMMAP_SKIP[]   = "=======        ======";
const char WATCOM_MEMMAP_COMMENT[] = "Module: ";
const char WATCOM_END_TABLE_HDR[]  = "+----------------------+";
const char MSVC_LINE_NUMBER[]      = "Line numbers for ";
const char MSVC_FIXUP[]            = "FIXUPS: ";
const char MSVC_EXPORTS[]          = " Exports";

/// @}

};

////////////////////////////////////////////////////////////////////////////////
/// @brief Open a map file and map the file content to virtual memory
/// @param lpszFileName  Path name of file to open.
/// @param dwSize Out variable to receive size of file.
/// @param lpMapAddr The pointer to memory address of mapped file
/// @return enum value of OPEN_FILE_ERROR
/// @author TQN
/// @date 2004.09.12
////////////////////////////////////////////////////////////////////////////////
MapFile::MAPResult MapFile::openMAP(const char * lpszFileName, char * &lpMapAddr, size_t &dwSize)
{
    // Set default values for output parameters
    lpMapAddr = NULL;
    dwSize = INVALID_FILE_SIZE;

    // Validate all input pointer parameters
    assert(NULL != lpszFileName);
    assert(FALSE == IsBadStringPtr(lpszFileName, (UINT_PTR) -1));
    if ((NULL == lpszFileName) || IsBadStringPtr(lpszFileName, (UINT_PTR) -1))
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return WIN32_ERROR;
    }

    // Open the file
    HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        return WIN32_ERROR;
    }

    dwSize = GetFileSize(hFile, NULL);
    if ((INVALID_FILE_SIZE == dwSize) || (0 == dwSize))
    {
        // File too large or empty
        WIN32CHECK(CloseHandle(hFile));
        return ((0 == dwSize) ? FILE_EMPTY_ERROR : WIN32_ERROR);
    }

    HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (NULL == hMap)
    {
        WIN32CHECK(CloseHandle(hFile));
        return WIN32_ERROR;
    }

    // Mapping creation successful, do not need file handle anymore
    WIN32CHECK(CloseHandle(hFile));

    lpMapAddr = (LPSTR) MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, dwSize);
    if (NULL == lpMapAddr)
    {
        WIN32CHECK(CloseHandle(hMap));
        return WIN32_ERROR;
    }

    // Map View successful, do not need the map handle anymore
    WIN32CHECK(CloseHandle(hMap));

    if (NULL != memchr(lpMapAddr, 0, dwSize))
    {
        // File is binary or Unicode file
        WIN32CHECK(UnmapViewOfFile(lpMapAddr));
        lpMapAddr = NULL;
        return FILE_BINARY_ERROR;
    }

    return OPEN_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Close memory map file which opened by MemMapFileOpen function.
/// @param lpAddr: Pointer to memory return by MemMapFileOpen.
/// @author TQN
/// @date 2004.09.12
////////////////////////////////////////////////////////////////////////////////
void MapFile::closeMAP(const void * lpAddr)
{
    WIN32CHECK(UnmapViewOfFile(lpAddr));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Seek to non space character at the beginning of a memory buffer.
/// @param  lpStart Pointer to start of buffer
/// @param  lpEnd Pointer to end of buffer
/// @return Pointer to first non space character at the beginning of buffer
/// @author TQN
/// @date 2004.09.12
////////////////////////////////////////////////////////////////////////////////
const char * MapFile::skipSpaces(const char * pStart, const char * pEnd)
{
    assert(pStart != NULL);
    assert(pEnd != NULL);
    assert(pStart <= pEnd);

    const char * p = pStart;
    while ((p < pEnd) && isspace(*p))
    {
        p++;
    }

    return p;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Find the EOL character '\r' or '\n' in a memory buffer
/// @param  lpStart LPSTR Pointer to start of buffer
/// @param  lpEnd LPSTR Pointer to end of buffer
/// @return LPSTR Pointer to first EOL character in the buffer
/// @author TQN
/// @date 2004.09.12
////////////////////////////////////////////////////////////////////////////////
const char * MapFile::findEOL(const char * pStart, const char * pEnd)
{
    assert(pStart != NULL);
    assert(pEnd != NULL);
    assert(pStart <= pEnd);

    const char * p = pStart;
    while ((p < pEnd) && ('\r' != *p) && ('\n' != *p))
    {
        p++;
    }

    return p;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Checks if a line is the starting line of a section to be analyzed.
/// @param  pLine Pointer to start of buffer
/// @param  lineLen Length of the current line
/// @return Type of the new section, or NO_SECTION
/// @author TL
/// @date 2011.09.10
////////////////////////////////////////////////////////////////////////////////
MapFile::SectionType MapFile::recognizeSectionStart(const char *pLine, size_t lineLen)
{
    if (strncasecmp(pLine, MSVC_HDR_START, lineLen) == 0)
        return MapFile::MSVC_MAP;
    if (strncasecmp(pLine, BCCL_HDR_NAME_START, lineLen) == 0)
        return MapFile::BCCL_NAM_MAP;
    if (strncasecmp(pLine, BCCL_HDR_VALUE_START, lineLen) == 0)
        return MapFile::BCCL_VAL_MAP;
    if (strncasecmp(pLine, WATCOM_MEMMAP_START, lineLen) == 0)
        return MapFile::WATCOM_MAP;
    return MapFile::NO_SECTION;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Checks if a line is the ending line of a section we analyzed.
/// @param secType Type of the opened section.
/// @param  pLine Pointer to start of buffer
/// @param  lineLen Length of the current line
/// @return Type of the new section, or value of secType if no change
/// @author TL
/// @date 2011.09.10
////////////////////////////////////////////////////////////////////////////////
MapFile::SectionType MapFile::recognizeSectionEnd(MapFile::SectionType secType, const char *pLine, size_t lineLen)
{
    if (strncmp(pLine, MSVC_LINE_NUMBER, std::strlen(MSVC_LINE_NUMBER)) == 0)
        return MapFile::NO_SECTION;
    if (strncmp(pLine, MSVC_FIXUP, std::strlen(MSVC_FIXUP)) == 0)
        return MapFile::NO_SECTION;
    if (strncmp(pLine, MSVC_EXPORTS, std::strlen(MSVC_EXPORTS)) == 0)
        return MapFile::NO_SECTION;
    if (strncmp(pLine, WATCOM_END_TABLE_HDR, std::strlen(WATCOM_END_TABLE_HDR)) == 0)
        return MapFile::NO_SECTION;
    return secType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Reads one entry of Ms-like MAP file.
/// @param sym Target  buffer for symbol data.
/// @param  pLine Pointer to start of buffer
/// @param  lineLen Length of the current line
/// @param  minLineLen Minimal accepted length of line
/// @param numOfSegs Number of segments in IDA, used to verify segment number range
/// @return Result of the parsing
/// @author TL
/// @date 2011.09.10
////////////////////////////////////////////////////////////////////////////////
MapFile::ParseResult MapFile::parseMsSymbolLine(MapFile::MAPSymbol &sym, const char *pLine, size_t lineLen, size_t minLineLen, size_t numOfSegs)
{
    // Get segment number, address, name, by pass spaces at beginning,
    // between ':' character, between address and name
    long lineCut = lineLen;
    if (lineCut > MAXNAMELEN + minLineLen)
        lineCut = MAXNAMELEN + minLineLen;
    char * dupLine = (char *)std::malloc(lineCut+1);
    strncpy(dupLine,pLine,lineCut);
    dupLine[lineCut] = '\0';
    if (strncasecmp(dupLine, ";", 1) == 0)
    {
        strncpy(sym.name,dupLine+1,MAXNAMELEN-1);
        sym.name[MAXNAMELEN] = '\0';
        std::free(dupLine);
        return MapFile::COMMENT_LINE;
    }
    int ret = sscanf(dupLine, " %04X : %08X %[^\t\n ;]", &sym.seg, &sym.addr, sym.name);
    std::free(dupLine);
    if (3 != ret)
    {
        // we have parsed to end of value/name symbols table or reached EOF
        return MapFile::FINISHING_LINE;
    }
    else if ((0 == sym.seg) || (--sym.seg >= numOfSegs) ||
            (BADADDR == sym.addr) || (std::strlen(sym.name) == 0) )
    {
        return MapFile::INVALID_LINE;
    }
    // Ensure name is NULL terminated
    sym.name[MAXNAMELEN] = '\0';
    return MapFile::SYMBOL_LINE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Reads one entry of Watcom-like MAP file.
/// @param sym Target  buffer for symbol data.
/// @param  pLine Pointer to start of buffer
/// @param  lineLen Length of the current line
/// @param  minLineLen Minimal accepted length of line
/// @param numOfSegs Number of segments in IDA, used to verify segment number range
/// @return Result of the parsing
/// @author TL
/// @date 2011.09.10
////////////////////////////////////////////////////////////////////////////////
MapFile::ParseResult MapFile::parseWatcomSymbolLine(MapFile::MAPSymbol &sym, const char *pLine, size_t lineLen, size_t minLineLen, size_t numOfSegs)
{
    // Get segment number, address, name, by pass spaces at beginning,
    // between ':' character, between address and name
    long lineCut = lineLen;
    if (lineCut > MAXNAMELEN + minLineLen)
        lineCut = MAXNAMELEN + minLineLen;
    char * dupLine = (char *)std::malloc(lineCut+1);
    strncpy(dupLine,pLine,lineCut);
    dupLine[lineCut] = '\0';
    if (strncasecmp(dupLine, ";", 1) == 0)
    {
        strncpy(sym.name,dupLine+1,MAXNAMELEN-1);
        sym.name[MAXNAMELEN] = '\0';
        std::free(dupLine);
        return MapFile::COMMENT_LINE;
    }
    if (strncasecmp(dupLine, WATCOM_MEMMAP_SKIP, std::strlen(WATCOM_MEMMAP_SKIP)) == 0)
    {
        std::free(dupLine);
        return MapFile::SKIP_LINE;
    }
    if (strncasecmp(dupLine, WATCOM_MEMMAP_COMMENT, std::strlen(WATCOM_MEMMAP_COMMENT)) == 0)
    {
        strncpy(sym.name,dupLine+std::strlen(WATCOM_MEMMAP_COMMENT),MAXNAMELEN-1);
        sym.name[MAXNAMELEN] = '\0';
        std::free(dupLine);
        return MapFile::COMMENT_LINE;
    }
    int ret = sscanf(dupLine, " %04X : %08X%*c %[^\t\n;]", &sym.seg, &sym.addr, sym.name);
    std::free(dupLine);
    if (3 != ret)
    {
        // we have parsed to end of value/name symbols table or reached EOF
        return MapFile::FINISHING_LINE;
    }
    else if ((0 == sym.seg) || (--sym.seg >= numOfSegs) ||
            (BADADDR == sym.addr) || (std::strlen(sym.name) == 0) )
    {
        return MapFile::INVALID_LINE;
    }
    // Ensure name is NULL terminated
    sym.name[MAXNAMELEN] = '\0';
    return MapFile::SYMBOL_LINE;
}

////////////////////////////////////////////////////////////////////////////////
