#pragma once

//-------------------------------------------------------------------------------------------------
#ifndef IMPORT
#define IMPORT __declspec(dllimport)
#endif


//-------------------------------------------------------------------------------------------------
#ifndef EXPORT
#define EXPORT __declspec(dllexport)
#endif


//-------------------------------------------------------------------------------------------------
#ifndef API
#define API __stdcall
#endif


//-------------------------------------------------------------------------------------------------
#ifndef interface
#define interface struct
#endif


//-------------------------------------------------------------------------------------------------
#ifndef _rect_
#define _rect_
struct rect{ int x0, y0, x1, y1; };
#endif


//-------------------------------------------------------------------------------------------------
#ifndef byte
typedef unsigned char byte;
#endif


//-------------------------------------------------------------------------------------------------
#ifndef word
typedef unsigned short word;
#endif



//-------------------------------------------------------------------------------------------------
#ifndef API_CODE
#define API_CODE
#define API_OK      0 // success
#define API_FAIL    1 // fail
#define API_PARERR  2 // parameter error
#define API_MEMERR  3 // memory error
#endif

