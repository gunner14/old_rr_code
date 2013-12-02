#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#include "AB_UTILITY_HASH.h"

static 	UINT32 CRC32Table[256];
static 	UINT64 CRC64Table[256];

static 	pthread_once_t		initonce32b = PTHREAD_ONCE_INIT;
static 	pthread_once_t	 	initonce64b = PTHREAD_ONCE_INIT;

int 	ABU_HASH_Init32TableOnce(void);
int		ABU_HASH_Init64TableOnce(void);

void 	ABU_HASH_InitCRC32Table(void);
void 	ABU_HASH_InitCRC64Table(void);

/******************************************************************************/
int ABU_HASH_create32bSign(char * sBuf, UINT32 wBufSize, UINT32* dwSign)
{
    UINT32 crc = INIT32CRC;

	ABU_HASH_Init32TableOnce();
	
    while(wBufSize > 0)
    {
        crc = CRC32Table[(crc ^ *sBuf++) & 0xff] ^ (crc >> 8);
        
        wBufSize -= 1;
    }
    
    *dwSign = crc;

    return 0;
}
/******************************************************************************/
int ABU_HASH_create64bSign(char * sBuf, UINT32 wBufSize, UINT32* dwSign1/*high 32 bit*/, UINT32 * dwSign2/*low 32 bit*/)
{
    UINT64 crc = INIT64CRC;

	ABU_HASH_Init64TableOnce();

    while(wBufSize > 0)
    {
        crc = CRC64Table[(crc ^ *sBuf++) & 0xff] ^ (crc >> 8);
        
        wBufSize -= 1;
    }
    
    *dwSign2 = crc & 0xffffffff;
    *dwSign1 = ( crc >> 32 ) & 0xffffffff;

    return 0;
}

/**
@
@
*/
int ABU_HASH_Init32TableOnce(void)
{
	return pthread_once(&initonce32b, ABU_HASH_InitCRC32Table);
}

int ABU_HASH_Init64TableOnce(void)
{
	return pthread_once(&initonce64b, ABU_HASH_InitCRC64Table);
}

/**
@ Use Table Driven CRC to Improve the Speed,
@Precompute the Hash Value and Store them in Tables
*/
void ABU_HASH_InitCRC32Table(void)
{
    int part;
    int i, j;


    for (i = 0; i < 256; i++)
    {
        part = i;
        for (j = 0; j < 8; j++)
        {
            if (part & 1)
	        part = (part >> 1) ^ POLY32REV;
	    else
	        part >>= 1;
	}
	    
        CRC32Table[i] = part;
    }
}

/******************************************************************************/
void ABU_HASH_InitCRC64Table(void)
{
    UINT64 part;
    int i, j;

    for (i = 0; i < 256; i++)
    {
        part = i;
        for (j = 0; j < 8; j++)
        {
            if (part & 1)
	        part = (part >> 1) ^ POLY64REV;
	    else
	        part >>= 1;
        }
	    
        CRC64Table[i] = part;
    }
}
