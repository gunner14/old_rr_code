/*********************************************************
*Filename: AB_DATA_UTILITY.h
*Author: Alan
*Date: 2007-04-30
*Desc: define some public interface and private interface
       in AB_DATA Module 
**********************************************************/
#ifndef AB_DATA_UTILITY_H_
#define AB_DATA_UTILITY_H_

#include "AB_UTILITY_COMMON.h"
#include "AB_DATA_TYPE.h"
#include "ARRAYLIST.h"

/***
 * a struct to contains a data record
 ***/
typedef struct _AB_DATA_Record
{
        //id-0
    unsigned long long id;
        //name-1
    char *name;
        //addr-2,3,4,5
    char *addr;
    char *province;
    char *city;
    char *county;
        //addrDetailNumber 6
    UINT8 addrLevel;
        //tel-7
    char *tel;
        //tag-8
    char *tag;
        //category-9
    char *category;
        //price-10
    char *price;
        //score-11
    char *score;
    char *officialSite;
        //reviewCount-12
    char *reviewCount;
    char *allReviewCount;
    char *rankingCate;
    char *rankingTags;
        //time -13
    char *time;
        //isBranch-14-15-16
    char *isBranch;

    char *rootID;

    char *rootName;
        //map 17-21
    char *mapDesc;
    char *mapX;
    char *mapY;
    char *mapGrid;
    int accuracy;
        //effect 22
    char *effectLevel;
        // desc 23-26
    char *desc;
    char *descSource;
    char *creator;
    char *source;
        //media 27 28
    char *imageURL;
    char *imageCount;
    char *snapshot;
        //indicators 29-31
    char *status;
    char *reserved;
    char *operator;
    char *extra;

} AB_DATA_Record;


void *xcalloc (size_t number, size_t size);

/***
 *@desc load fields' posistions from a format line
 ***/
int AB_DATA_loadFormat(MerchantPos_S *pFormatPos,const char *szFormatLine);

/***
 * @desc print a format line, normally on the head of a file
 ***/
void AB_DATA_printFormat(const MerchantPos_S *pFormatPos,FILE *fp);

/***
 * @desc print a record's original record format
 ***/
void AB_DATA_printLine(const ArrayList *pCmpInfoList,const MerchantPos_S *pFormatPos,FILE *fp);

/***
 * @desc print a line contains all the fields, give format seprator for blank fields
 ***/
void AB_DATA_printFullLine(const ArrayList *pCmpInfoList,const MerchantPos_S *pFormatPos,FILE *fp);

/***
 * @desc read a line into a record in AB_DATA_Record.
 * @return a struct when read a valid line
 *         NULL when invalid line.
 ***/
AB_DATA_Record *AB_DATA_getRecord(char *line, const MerchantPos_S *pFormatPos);

/***
 * @desc destroy a record
 ***/
void AB_DATA_freeRecord(AB_DATA_Record *record);

/***
 * @desc print a record in 
 ***/
void AB_DATA_printRecord(const AB_DATA_Record *record, const MerchantPos_S *pFormatPos, FILE *fp);


#endif /*AB_DATA_UTILITY_H_*/
