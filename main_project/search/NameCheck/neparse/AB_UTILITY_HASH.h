#ifndef _AB_UTILITY_HASH_H_
#define _AB_UTILITY_HASH_H_
/********************************************************************************/
//AB_UTILITY_HASH Header File							
//Author: yhwu
//Date: 07-01-03
//
//Des:
//
/******************************************************************************/


#include "AB_UTILITY_COMMON.h"


#define POLY64REV	0x95AC9329AC4BC9B5ULL //CRC64 PolyGenerator
#define INIT64CRC	0xFFFFFFFFFFFFFFFFULL //CRC64 InitValue

#define POLY32REV	0x04C11DB7UL //CRC32 PolyGenerator
#define INIT32CRC	0xFFFFFFFFUL //CRC32 InitValue


//AB_UTILITY_HASH Provide 2 API
int ABU_HASH_create32bSign(char * sBuf, UINT32 wBufSize, UINT32* dwSign); 
int ABU_HASH_create64bSign(char * sBuf, UINT32 wBufSize, UINT32* dwSign1, UINT32 * dwSign2); 
#endif //_AB_UTILITY_HASH_H_
