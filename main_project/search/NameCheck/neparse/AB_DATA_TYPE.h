/**********************************************************
 *Filename: AB_DATA_TYPE.h
 *Author: Alan
 *Date: 2007-04-30
 *Desc: define the data structure in AB_DATA
 * **********************************************************/

#ifndef AB_DATA_TYPE_H_
#define AB_DATA_TYPE_H_

#include "AB_UTILITY_COMMON.h"

/* the macro defination*/
#define BATCH_NUMBER 10000

#ifdef MAX_LINE_LENGTH
#undef MAX_LINE_LENGTH
#endif

#define MAX_LINE_LENGTH 1024*1024

extern const char *TXT_FIELD[];

const static char TXT_START[] = "###";
const static char TXT_FIELD_SEPARATOR[] = "|||";

const static UINT32 TXT_FIELD_NUMBER = 38;

/***
 * @desc the struct that indicate every field's position
 ***/
typedef struct _MerchantPos_S
{
    //begin with >= 1
	unsigned char idPos;
	unsigned char namePos;
	unsigned char addrPos;
	unsigned char provincePos;
	unsigned char cityPos;
	unsigned char countyPos;
	unsigned char addDetailNumberPos;
	unsigned char telPos;
	unsigned char tagPos;
	unsigned char categoryPos;
	unsigned char pricePos;
	unsigned char scorePos;
    unsigned char officialSitePos;
	unsigned char reviewCountPos;//new added
    unsigned char allReviewCountPos;
    unsigned char rankingCatePos;
    unsigned char rankingTagsPos;
	unsigned char timePos;
	unsigned char isBranchPos;
	unsigned char rootIDPos;
	unsigned char rootNamePos;
	unsigned char mapDescPos;
    unsigned char mapXPos;
    unsigned char mapYPos;
    unsigned char mapGridPos;
    unsigned char accuracyPos;
    unsigned char effectLevelPos;        //effect 22
    unsigned char descPos;        // desc 23-26
    unsigned char descSourcePos;
    unsigned char creatorPos;
    unsigned char sourcePos;        
    unsigned char imageURLPos; //media 27 28
    unsigned char imageCountPos;
    unsigned char snapshotPos;
    unsigned char statusPos;        //indicators 29-31
    unsigned char reservedPos;
    unsigned char operatorPos;
    unsigned char extraPos;
}MerchantPos_S;

#endif 

