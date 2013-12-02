/**
 * @file name_util.h
 * @author bfzhang
 * @brief name parse trivial logic
 * @date 2008-09-02
 */
#include "ab_hmap.h"
#include "ab_hset.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "AB_UTILITY_NAME.h"

#ifndef NAME_UTIL_H
#define NAME_UTIL_H
#define NAME_LEN_MAX 128

extern const char *PROVINCE_FIELD;
extern const char *CITY_FIELD;
extern const char *COUNTY_FIELD;
extern const char *STEMPRE_FIELD;
extern const char *STEM_FIELD;
extern const char *STEMPOST_FIELD;
extern const char *TYPE_FIELD;
extern const char *ORG_FIELD;
extern const char *ASSOPREFIX_FIELD;
extern const char *ASSOSUFFIX_FIELD;
extern const char *APPENDIX_FIELD;
extern const char *MINSTEM_FIELD;
extern const char *MAXTYPE_FIELD;
extern const char *BRANCHPLACE_FIELD;
extern const char *MULTIPLESHOP_FIELD;

typedef struct _NAME_RECOG_LOG_S
{
    char szProvince[NAME_LEN_MAX + 1];
    char szCity[NAME_LEN_MAX + 1];
    char szCounty[NAME_LEN_MAX + 1];

    char szStemPre[NAME_LEN_MAX + 1];
    char szStem[NAME_LEN_MAX + 1];
    char szStemPost[NAME_LEN_MAX + 1];
    char szType[NAME_LEN_MAX + 1];
    char szOrg[NAME_LEN_MAX + 1];
    
    char szAssoPrefix[NAME_LEN_MAX + 1];
    char szAssoSuffix[NAME_LEN_MAX + 1];
    char szAppendix[NAME_LEN_MAX + 1];
    
    //special infoME_RECOG_LOG_S
    char szMinStem[NAME_LEN_MAX + 1];
    char szMaxType[NAME_LEN_MAX + 1];
    char szBestType[NAME_LEN_MAX + 1]; 
    char szMultipleShop[NAME_LEN_MAX + 1];
    char szBranchPlace[NAME_LEN_MAX + 1];
}NAME_RECOG_LOG_S;

typedef struct _NAME_PARAM_LOG_S
{
    unsigned char endWithOrg;
    unsigned char isSingle;
    int typeFeatCount;
    int nameStemCount;
    int orgCount;    
}NAME_PARAM_LOG_S;

ABU_WORDS_DICT_S *NAME_createSegDict(const char **szDictArray, const uint32_t uiDictCount);

ABU_WORDS_DICT_S *NAME_createSegDictNameType(const char **szDictArray, const uint32_t uiDictCount);
  
ab_hmap *NAME_createNameStemMap(const char ** szDictArray, const uint32_t uiDictCount);
    
ab_hmap *NAME_createNameTypeMap(const char ** szDictArray, const uint32_t uiDictCount);
    
ab_hset *NAME_createNameOrgSet(const char ** szDictArray, const uint32_t uiDictCount);

ab_hset *NAME_createNameStemOrgFeatSet(const char ** szDictArray, const uint32_t uiDictCount);

ab_hset *NAME_createNameTypeOrgFeatSet(const char ** szDictArray, const uint32_t uiDictCount);

ab_hset *NAME_createNamePlaceSet(const char **szDictArray, const uint32_t uiDictCount);
    
ab_hset *NAME_createAddrFeatWordSet();
 
ABU_NAME_ATTACH_S * NAME_getAttach(ABU_NAME_S *stpName);
 
void NAME_destroyLibContent(ABU_NAME_LIB_S *stpNameLib);
    
void NAME_destroyRecordContent(ABU_NAME_RECORD_S *stpNameRecord);
    
void NAME_destroyContent(ABU_NAME_S *stpName);

void NAME_destroyAttachContent(ABU_NAME_ATTACH_S *stpNameAttach);

void NAME_destroyAttach(ABU_NAME_ATTACH_S *stpNameAttach);

unsigned char NAME_endWithStemOrgFeat(const ABU_NAME_LIB_S *stpNameLib, const char *str); 

unsigned char NAME_endWithTypeOrgFeat(const ABU_NAME_LIB_S *stpNameLib, const char *str);

unsigned char NAME_endWithAddrFeat(const char *str);
 
unsigned char NAME_isEffectiveStem(const ABU_NAME_LIB_S *stpNameLib, const char *szStem);

unsigned char NAME_isPartOfMiniStem(const ABU_NAME_LIB_S *stpNameLib, const char *szMiniStem, const char *szWord);

unsigned char NAME_isEffectivePlace(const ABU_NAME_LIB_S *stpNameLib, const char *szPlace, const char *szCity);

ABU_NAME_RECORD_S *NAME_getRecord(const NAME_RECOG_LOG_S *stpSrc);

void NAME_printLog(const NAME_RECOG_LOG_S *stpNameLog, FILE *fp);

void NAME_printRecord(const ABU_NAME_RECORD_S *stpNameRecord,FILE *fp);

void NAME_printAttach(const ABU_NAME_ATTACH_S *stpNameAttach, FILE *fp);

void NAME_print(const ABU_NAME_S *stpName,FILE *fp);

void NAME_printLib(const ABU_NAME_LIB_S *stpNameLib, FILE *fp);

uint32_t PCC_get4Char(const char * szField);
#endif
