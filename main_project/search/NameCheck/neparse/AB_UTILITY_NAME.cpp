/**
 * @file AB_UTILITY_NAME.c
 * @author dairui.cui 
 * @brief name parse core algorithm
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "abstring.h"
#include "ab_hmap.h"
#include "ab_hset.h"
#include "abutility.h"
#include "name_util.h"
#include "AB_UTILITY_COMMON.h"
#include "AB_UTILITY_MAPTREE.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "AB_UTILITY_WORDS_SEG_SL.h"
#include "AB_UTILITY_WORDS_UTILITY.h"
#include "AB_UTILITY_ADDR_RESOURCE.h"
#include "AB_UTILITY_NAME_RESOURCE.h"
#include "AB_UTILITY_NAME.h"
  
#define GB_LEN 2
/*****************************************/
/******private interface delaration*******/
/*****************************************/
static void NAME_preprocess(const ABU_NAME_LIB_S *stpNameLib, char *szName);
    
static void NAME_postprocess(const ABU_NAME_LIB_S *stpNameLib, NAME_RECOG_LOG_S *stpNameLog);

static unsigned char NAME_isSingle(const ABU_NAME_LIB_S *stpNameLib,
                                   const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                   const int32_t wordUnitStart,
                                   const int32_t wordUnitEnd,
                                   const char *szCity,
                                   NAME_PARAM_LOG_S *stpParamLog);

static unsigned char NAME_isSinglePattern(const ABU_NAME_LIB_S *stpNameLib,
                                          const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                          const int32_t wordUnitStart,
                                          const int32_t wordUnitEnd,
                                          const char *szCity);

static unsigned char NAME_isAssociatePattern(const ABU_NAME_LIB_S *stpNameLib,
                                          const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                          const int32_t wordUnitStart,
                                          const int32_t wordUnitEnd,
                                          const char *szCity);

static void NAME_parseRecord_setAppendix(const ABU_NAME_LIB_S *stpNameLib,
                                         const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                         int32_t *wordUnitStart,
                                         int32_t *wordUnitEnd,
                                         const char *szCity,
                                         NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_resetStemAndType(const ABU_NAME_LIB_S *stpNameLib,
                                              NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_setProvince(const ABU_NAME_LIB_S *stpNameLib,
                                         const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                         int32_t *wordUnitStart,
                                         int32_t *wordUnitEnd,
                                         const char *szCity, 
                                         NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_setCity(const ABU_NAME_LIB_S *stpNameLib,
                                     const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                     int32_t *wordUnitStart,
                                     int32_t *wordUnitEnd,
                                     const char *szCity, 
                                     NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_setCounty(const ABU_NAME_LIB_S *stpNameLib,
                                       const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                       int32_t *wordUnitStart,
                                       int32_t *wordUnitEnd,
                                       const char *szCity, 
                                       NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_setBestType(const ABU_NAME_LIB_S *stpNameLib,
                                         const char *szName,
                                         const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                         NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_getBestType(const ABU_NAME_LIB_S *stpNameLib,
                                         const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                         NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_single(const ABU_NAME_LIB_S *stpNameLib,
                                    const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                    int32_t *wordUnitStart,
                                    int32_t *wordUnitEnd,
                                    const char *szCity,
                                    NAME_PARAM_LOG_S *stpParamLog,
                                    NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_single_setAppendix(const ABU_NAME_LIB_S *stpNameLib,
                                                const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                int32_t *wordUnitStart,
                                                int32_t *wordUnitEnd, 
                                                const char *szCity,
                                                NAME_PARAM_LOG_S *stpParamLog,
                                                NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_single_setStemPre(const ABU_NAME_LIB_S *stpNameLib,
                                               const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                               int32_t *wordUnitStart,
                                               int32_t *wordUnitEnd, 
                                               const char *szCity,
                                               NAME_PARAM_LOG_S *stpParamLog,
                                               NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_single_setStem(const ABU_NAME_LIB_S *stpNameLib,
                                            const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                            int32_t *wordUnitStart,
                                            int32_t *wordUnitEnd, 
                                            const char *szCity,
                                            NAME_PARAM_LOG_S *stpParamLog,
                                            NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_single_setStemPost(const ABU_NAME_LIB_S *stpNameLib,
                                                const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                int32_t *wordUnitStart,
                                                int32_t *wordUnitEnd,
                                                const char *szCity,
                                                NAME_PARAM_LOG_S *stpParamLog,
                                                NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_single_setType(const ABU_NAME_LIB_S *stpNameLib,
                                            const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                            int32_t *wordUnitStart,
                                            int32_t *wordUnitEnd,
                                            const char *szCity,
                                            NAME_PARAM_LOG_S *stpParamLog,
                                            NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_single_setOrg(const ABU_NAME_LIB_S *stpNameLib,
                                           const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                           int32_t *wordUnitStart,
                                           int32_t *wordUnitEnd, 
                                           const char *szCity,
                                           NAME_PARAM_LOG_S *stpParamLog,
                                           NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_single_resetAppendix(const ABU_NAME_LIB_S *stpNameLib,
                                                  const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                  int32_t *wordUnitStart,
                                                  int32_t *wordUnitEnd,
                                                  const char *szCity,
                                                  NAME_PARAM_LOG_S *stpParamLog,
                                                  NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_associate(const ABU_NAME_LIB_S *stpNameLib,
                                       const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                       int32_t *wordUnitStart,
                                       int32_t *wordUnitEnd, 
                                       const char *szCity,
                                       NAME_PARAM_LOG_S *stpParamLog,
                                       NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_associate_setStemPre(const ABU_NAME_LIB_S *stpNameLib,
                                                  const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                  int32_t *wordUnitStart,
                                                  int32_t *wordUnitEnd, 
                                                  const char *szCity,
                                                  NAME_PARAM_LOG_S *stpParamLog,
                                                  NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_associate_setStem(const ABU_NAME_LIB_S *stpNameLib,
                                               const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                               int32_t *wordUnitStart,
                                               int32_t *wordUnitEnd, 
                                               const char *szCity,
                                               NAME_PARAM_LOG_S *stpParamLog,
                                               NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_associate_setStemPost(const ABU_NAME_LIB_S *stpNameLib,
                                                   const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                   int32_t *wordUnitStart,
                                                   int32_t *wordUnitEnd, 
                                                   const char *szCity,
                                                   NAME_PARAM_LOG_S *stpParamLog,
                                                   NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_associate_setType(const ABU_NAME_LIB_S *stpNameLib,
                                               const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                               int32_t *wordUnitStart,
                                               int32_t *wordUnitEnd, 
                                               const char *szCity,
                                               NAME_PARAM_LOG_S *stpParamLog,
                                               NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_associate_setOrg(const ABU_NAME_LIB_S *stpNameLib,
                                              const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                              int32_t *wordUnitStart,
                                              int32_t *wordUnitEnd,
                                              const char *szCity, 
                                              NAME_PARAM_LOG_S *stpParamLog,
                                              NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_associate_setAssoPrefix(const ABU_NAME_LIB_S *stpNameLib,
                                                  const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                  int32_t *wordUnitStart,
                                                  int32_t *wordUnitEnd, 
                                                  const char *szCity,
                                                  NAME_PARAM_LOG_S *stpParamLog,
                                                  NAME_RECOG_LOG_S *stpNameLog);

static void NAME_parseRecord_associate_setAssoSuffix(const ABU_NAME_LIB_S *stpNameLib,
                                                     const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                     int32_t *wordUnitStart,
                                                     int32_t *wordUnitEnd, 
                                                     const char *szCity,
                                                     NAME_PARAM_LOG_S *stpParamLog,
                                                     NAME_RECOG_LOG_S *stpNameLog);
/*****************************************/
/*****public interface implementation*****/
/*****************************************/
    
ABU_NAME_LIB_S *ABU_NAME_createLib(const char *szDictDir,
                                   const char **szDictArray,
                                   const uint32_t uiDictCount)
{/*{{{*/
    ABU_ASSERT(szDictArray != NULL);
    if(uiDictCount != 5)
    {
        fprintf(stderr, "init name parse,you need 5 dict file\n");
        fprintf(stderr, "dict list:\n");
        fprintf(stderr, "    basicDict.txt\n");
        fprintf(stderr, "    groupDict.txt\n");
        fprintf(stderr, "    merchant.stem.multiple\n");
        fprintf(stderr, "    merchant.feat\n");
        fprintf(stderr, "    place.dict\n");
        exit(-1);
    }

    char szBasicDict[strlen(szDictDir) + 1 + strlen(szDictArray[0]) + 1];
    char szGroupDict[strlen(szDictDir) + 1 + strlen(szDictArray[1]) + 1];
    char szStemDict[strlen(szDictDir) + 1 + strlen(szDictArray[2]) + 1];
    char szFeatDict[strlen(szDictDir) + 1 + strlen(szDictArray[3]) + 1];
    char szPlaceDict[strlen(szDictDir) + 1 + strlen(szDictArray[4]) + 1];
    sprintf(szBasicDict, "%s/%s", szDictDir, szDictArray[0]);
    sprintf(szGroupDict, "%s/%s", szDictDir, szDictArray[1]);
    sprintf(szStemDict, "%s/%s", szDictDir, szDictArray[2]);
    sprintf(szFeatDict, "%s/%s", szDictDir, szDictArray[3]);
    sprintf(szPlaceDict, "%s/%s", szDictDir, szDictArray[4]);
    const char *szDictArrayFull[5] = {szBasicDict, szGroupDict, szStemDict, szFeatDict, szPlaceDict};

    ABU_NAME_LIB_S *stpNameLib = (ABU_NAME_LIB_S *)ab_calloc(1,sizeof(ABU_NAME_LIB_S));

    stpNameLib->stpSegLib = ABU_WORDS_SEG_createSegLib();
    stpNameLib->stpSegDict = NAME_createSegDict(szDictArrayFull, uiDictCount);
    stpNameLib->stpSegDictNameType = NAME_createSegDictNameType(szDictArrayFull, uiDictCount);
    
    stpNameLib->stpNameStemMap = NAME_createNameStemMap(szDictArrayFull, uiDictCount);
    stpNameLib->stpNameTypeMap = NAME_createNameTypeMap(szDictArrayFull, uiDictCount);
    stpNameLib->stpNameOrgSet = NAME_createNameOrgSet(szDictArrayFull, uiDictCount);
    stpNameLib->stpNameStemOrgFeatSet = NAME_createNameStemOrgFeatSet(szDictArrayFull, uiDictCount);
    stpNameLib->stpNameTypeOrgFeatSet = NAME_createNameTypeOrgFeatSet(szDictArrayFull, uiDictCount);
    stpNameLib->stpNamePlaceSet = NAME_createNamePlaceSet(szDictArrayFull, uiDictCount);
    stpNameLib->stpAddrFeatWordSet = NAME_createAddrFeatWordSet();
    
    stpNameLib->stpMapTree = ABU_MAPTREE_create();
    stpNameLib->szDictDir = (char *)ab_calloc(strlen(szDictDir)+1, sizeof(char));
    strcpy(stpNameLib->szDictDir, szDictDir);
    return stpNameLib;
}/*}}}*/
    
    
void ABU_NAME_destroyLib(ABU_NAME_LIB_S *stpNameLib)
{/*{{{*/
    if(stpNameLib == NULL) return;
    
    NAME_destroyLibContent(stpNameLib);
    
    ABU_FREE(stpNameLib);   
}/*}}}*/
    
    
ABU_NAME_RECORD_S *ABU_NAME_parseRecord(const ABU_NAME_LIB_S *stpNameLib,
                                        const char *szName)
{/*{{{*/
    return ABU_NAME_parseRecord_bycity(stpNameLib, szName, NULL);
}/*}}}*/
    
    
ABU_NAME_RECORD_S *ABU_NAME_parseRecord_bycity(const ABU_NAME_LIB_S *stpNameLib,
                                               const char *szName,
                                               const char *szCity)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && szName != NULL);

    int nameLen = strlen(szName);
    if(nameLen == 0 || nameLen > NAME_LEN_MAX)
    {
#ifdef AB_DEBUG
        printf("<<<<<debug log(ABU_NAME_parseRecord_bycity)\n");
        printf("nameLen == 0 || nameLen > 128\n");
        printf(">>>>>debug log(ABU_NAME_parseRecord_bycity)\n");
#endif 
        return NULL;
    }

#ifdef AB_DEBUG
    printf("<<<<<debug log(ABU_NAME_parseRecord_bycity)\n");
    printf("origin:%s ", szName);
    if(szCity != NULL && szCity[0] != 0)
        printf("%s", szCity);
    printf("\n");
#endif
    
    char *szGivenCity = (char *)szCity;
    if(szGivenCity != NULL && szGivenCity[0] == 0)
        szGivenCity = NULL;
   
    char szNameCopy[nameLen + 1];
    memset(szNameCopy,0,nameLen + 1);
    strncpy(szNameCopy,szName,nameLen);
    
    //name preprocess
    NAME_preprocess(stpNameLib, szNameCopy);
    if(szNameCopy[0] == 0)
    {
#ifdef AB_DEBUG
        printf(">>>>>debug log(ABU_NAME_parseRecord_bycity)\n");
#endif
        return NULL;
    }
#ifdef AB_DEBUG
    printf("preprocess:%s\n", szNameCopy);
#endif
 
    //get seg result
    nameLen = strlen(szNameCopy);   
    ABU_WORDS_SEGRESULT_SL_S *stpSegResult = NULL;
    stpSegResult = ab_segmenter_seg(szNameCopy,nameLen,stpNameLib->stpSegDict,stpNameLib->stpSegLib);
    if(stpSegResult == NULL)
    {
        ab_segmenter_free(stpSegResult);
        stpSegResult = NULL;
#ifdef AB_DEBUG
        printf(">>>>>debug log(ABU_NAME_parseRecord_bycity)\n");
#endif
        return NULL;
    }
#ifdef AB_DEBUG
    printf("seg:");
    for(int32_t i = 0;i < stpSegResult->dwWordNumber;i++)
    {
        char *szWord = (stpSegResult->pWordArray + i)->szWord;
        printf("%s-", szWord);
    }
    printf("\n");
#endif
 
    int wordUnitStart = 0;
    int wordUnitEnd = stpSegResult->dwWordNumber;

    //step0:set appendix
    NAME_RECOG_LOG_S nameLog;
    memset(&nameLog, 0 , sizeof(NAME_RECOG_LOG_S));
    NAME_PARAM_LOG_S paramLog;
    memset(&paramLog, 0, sizeof(NAME_PARAM_LOG_S));
    NAME_parseRecord_setAppendix(stpNameLib, stpSegResult,
                                 &wordUnitStart, &wordUnitEnd, szGivenCity, &nameLog);
#ifdef AB_DEBUG
    printf("step0:set appendix\n");
    if(nameLog.szAppendix[0])
        printf("    appendix:%s\n", nameLog.szAppendix);
    if(nameLog.szBranchPlace[0])
        printf("    branchplace:%s\n", nameLog.szBranchPlace);
#endif 

    //step1: set province
    NAME_parseRecord_setProvince(stpNameLib, stpSegResult, 
                                 &wordUnitStart, &wordUnitEnd, szGivenCity, &nameLog);
#ifdef AB_DEBUG
    printf("step1:set province\n");
    printf("    province:%s\n", nameLog.szProvince);
#endif    
    
    //step2:set city
    NAME_parseRecord_setCity(stpNameLib, stpSegResult, 
                            &wordUnitStart, &wordUnitEnd, szGivenCity, &nameLog);
#ifdef AB_DEBUG
    printf("step2:set city\n");
    printf("    city:%s\n", nameLog.szCity);
#endif
    
    //step3:set county
    NAME_parseRecord_setCounty(stpNameLib, stpSegResult, 
                               &wordUnitStart, &wordUnitEnd, szGivenCity, &nameLog);
#ifdef AB_DEBUG
    printf("step3:set county\n");
    printf("    county:%s\n", nameLog.szCounty);
#endif

    //step4:parse left
#ifdef AB_DEBUG
    printf("step4:parse left\n");
#endif
    if(NAME_isSingle(stpNameLib, stpSegResult, wordUnitStart, wordUnitEnd, szGivenCity, &paramLog))
        NAME_parseRecord_single(stpNameLib, stpSegResult, 
                                &wordUnitStart, &wordUnitEnd, szGivenCity, &paramLog, &nameLog);
    else
        NAME_parseRecord_associate(stpNameLib, stpSegResult, 
                                &wordUnitStart, &wordUnitEnd, szGivenCity, &paramLog, &nameLog);

    //step5:reset ministem
    NAME_parseRecord_resetStemAndType(stpNameLib, &nameLog);
#ifdef AB_DEBUG
    printf("step5:reconstruct minStem and MaxType\n");
    printf("    minstem:%s\n", nameLog.szMinStem);
    printf("    maxType:%s\n", nameLog.szMaxType);    
#endif

    //step6:set besttype
    NAME_parseRecord_setBestType(stpNameLib, szName, stpSegResult, &nameLog);
#ifdef AB_DEBUG
    printf("step6:set besttype\n");
    printf("    bestType:%s\n", nameLog.szBestType);
#endif    
    
    //postprocess
    NAME_postprocess(stpNameLib, &nameLog);
#ifdef AB_DEBUG
    printf("postprocess:\n");
    NAME_printLog(&nameLog, stdout);
#endif

#ifdef AB_DEBUG
    printf("check pattern\n");
    if((paramLog.isSingle && nameLog.szAssoSuffix[0]) ||
       (!paramLog.isSingle && !nameLog.szAssoSuffix[0]))
        printf("pattern check error\n");        
#endif    
    
    ABU_NAME_RECORD_S *stpNameRecord = NAME_getRecord(&nameLog);
#ifdef AB_DEBUG
    printf("name record:\n");
    NAME_printRecord(stpNameRecord, stdout);    
#endif    
                                     
    //free memory
    ab_segmenter_free(stpSegResult);
    stpSegResult = NULL;
#ifdef AB_DEBUG
    printf(">>>>>debug log(ABU_NAME_parseRecord_bycity)\n");
#endif
    return stpNameRecord;
 
}/*}}}*/
    
    
ABU_NAME_S *ABU_NAME_parse(const ABU_NAME_LIB_S *stpNameLib,
                           const char *szName)
{/*{{{*/
    return ABU_NAME_parse_bycity(stpNameLib, szName, NULL);
}/*}}}*/
    
    
ABU_NAME_S *ABU_NAME_parse_bycity(const ABU_NAME_LIB_S *stpNameLib,
                                  const char *szName,
                                  const char *szCity)
{/*{{{*/
    if(stpNameLib == NULL && szName == NULL) return NULL;
    if(szName[0] == '\0') return NULL;
    
    int nameLen = strlen(szName);
    if(nameLen > NAME_LEN_MAX)
    {
        fprintf(stderr,
                "!!!can't parse this name(len > %d bytes)\n",NAME_LEN_MAX);
        fprintf(stderr,"\t%s\n",szName);
        return NULL;        
    }
    
    ABU_NAME_S *stpName = (ABU_NAME_S *)ab_calloc(1,sizeof(ABU_NAME_S));
 
    //step1:set szOriginName
    stpName->szOriginName = (char*)ab_calloc(nameLen + 1,sizeof(char));
    strncpy(stpName->szOriginName,szName,nameLen);
   
    //step2:parse name record
    stpName->stpNameRecord = ABU_NAME_parseRecord_bycity(stpNameLib,szName,szCity);
    if(stpName->stpNameRecord == NULL)
    {
        ABU_NAME_destroy(stpName);
        return NULL;
    }
    
    //step3:get name attach
    stpName->stpNameAttach = NAME_getAttach(stpName); 
    if(stpName->stpNameAttach == NULL)
    {
        ABU_NAME_destroy(stpName);
        return NULL;
    }
    
    return stpName;
}/*}}}*/
    
 
void ABU_NAME_destroyRecord(ABU_NAME_RECORD_S *stpNameRecord) {/*{{{*/
    if(stpNameRecord == NULL)
        return;
    NAME_destroyRecordContent(stpNameRecord);
    ABU_FREE(stpNameRecord);   
}/*}}}*/
    
void ABU_NAME_destroy(void *stpName) {/*{{{*/
    ABU_NAME_S *p = (ABU_NAME_S *)stpName;
    if(p == NULL) 
        return;
    NAME_destroyContent(p);
    ABU_FREE(p);
}/*}}}*/
    
void ABU_NAME_sign(const void *_p, char *p_sign_buf, size_t *p_buf_size) {/*{{{*/
    ABU_NAME_S *p = (ABU_NAME_S *)_p;
    size_t copy_buf_len = MIN(strlen(p->szOriginName), *p_buf_size);
    memcpy(p_sign_buf, p->szOriginName, copy_buf_len);
    *p_buf_size = copy_buf_len;
}/*}}}*/

/*********************************************/
/******private interface implementation*******/
/*********************************************/
static void NAME_preprocess(const ABU_NAME_LIB_S *stpNameLib, char *szName)
{/*{{{*/
    ABU_ASSERT(szName != NULL && stpNameLib != NULL);
    if(szName[0] == 0)
        return;
    
    abstring *string = abstring_new(szName);
    //abstring_remove_whitespace(string);
    abstring_replace_gb(string, "（", "(");
    abstring_replace_gb(string, "）", ")");
    abstring_replace_gb(string, "·", ".");
   
    //remove punct
    /*
    abstring_remove_gbpunct(string);
    for(int i = 0;i < ASCII_PUNCT_COUNT;i++)
    {
        if(ASCII_PUNCT[i] == '(' || ASCII_PUNCT[i] == ')')
            continue;
        abstring_remove_c(string, ASCII_PUNCT[i]);
    }
    */

    strncpy(szName, string->str, string->len);
    szName[string->len] = 0;
    
    abstring_free(string, TRUE);
    string = NULL;
}/*}}}*/
    
    
static void NAME_postprocess(const ABU_NAME_LIB_S *stpNameLib, NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLog != NULL && stpNameLib != NULL);
    
    //case1:szStem == NULL, reset szStem szMinStem
    if(!stpNameLog->szStem[0])
    {
        char szStem[NAME_LEN_MAX + 1];
        memset(szStem, 0, sizeof(szStem));
    
        if(szStem[0] == 0 && stpNameLog->szCounty[0])
        {
            strcat(szStem, stpNameLog->szCounty);
            memset(stpNameLog->szCounty, 0, sizeof(stpNameLog->szCounty));
        }
        if(szStem[0] == 0 && stpNameLog->szCity[0])
        {
            strcat(szStem, stpNameLog->szCity);
            memset(stpNameLog->szCity, 0, sizeof(stpNameLog->szCity));
        }
        if(szStem[0] == 0 && stpNameLog->szProvince[0])
        {
            strcat(szStem, stpNameLog->szProvince);
            memset(stpNameLog->szProvince, 0, sizeof(stpNameLog->szProvince));
        }
    
        int valid = 0;
        if(valid == 0 && stpNameLog->szStemPost[0])
        {
            strcat(szStem, stpNameLog->szStemPost);
            memset(stpNameLog->szStemPost, 0, sizeof(stpNameLog->szStemPost));
            valid++;
        }
        if(valid == 0 && stpNameLog->szType[0])
        {
            strcat(szStem, stpNameLog->szType);
            memset(stpNameLog->szType, 0, sizeof(stpNameLog->szType));
            valid++;
        }
        if(valid == 0 && stpNameLog->szOrg[0])
        {
            strcat(szStem, stpNameLog->szOrg);
            memset(stpNameLog->szOrg, 0, sizeof(stpNameLog->szOrg));
            valid++;
        }
        if(valid == 0 && stpNameLog->szAssoPrefix[0])
        {
            strcat(szStem, stpNameLog->szAssoPrefix);
            memset(stpNameLog->szAssoPrefix, 0, sizeof(stpNameLog->szAssoPrefix));
            valid++;
        }
        if(valid == 0 && stpNameLog->szAssoSuffix[0])
        {
            strcat(szStem, stpNameLog->szAssoSuffix);
            memset(stpNameLog->szAssoSuffix, 0, sizeof(stpNameLog->szAssoSuffix));
            valid++;
        }
        if(valid == 0 && stpNameLog->szAppendix[0])
        {
            strcat(szStem, stpNameLog->szAppendix);
            memset(stpNameLog->szAppendix, 0, sizeof(stpNameLog->szAppendix));
            valid++;        
        }
    
        strcpy(stpNameLog->szStem, szStem); 
        memset(stpNameLog->szMinStem, 0, sizeof(stpNameLog->szMinStem));
        strcpy(stpNameLog->szMinStem, szStem); 
    }
   
    //case 2:stem too short, need reset stem and minstem
    //stem is single chinese character 
    if(strlen(stpNameLog->szStem) == 2 && stpNameLog->szStem[0] < 0)
    {
        char szStem[NAME_LEN_MAX + 1];
        memset(szStem, 0, sizeof(szStem));
            
        int valid = 0;
        if(valid == 0 && stpNameLog->szType[0])
        {
            strcat(szStem, stpNameLog->szStem);
            strcat(szStem, stpNameLog->szType);
            memset(stpNameLog->szType, 0, sizeof(stpNameLog->szType));
            valid++;
        }
        if(valid == 0 && stpNameLog->szOrg[0])
        {
            strcat(szStem, stpNameLog->szStem);
            strcat(szStem, stpNameLog->szOrg);
            memset(stpNameLog->szOrg, 0, sizeof(stpNameLog->szOrg));
            valid++;
        }
        if(valid == 0 && stpNameLog->szAssoPrefix[0])
        {
            strcat(szStem, stpNameLog->szStem);
            strcat(szStem, stpNameLog->szAssoPrefix);
            memset(stpNameLog->szAssoPrefix, 0, sizeof(stpNameLog->szAssoPrefix));
            valid++;
        }
        if(valid == 0 && stpNameLog->szAssoSuffix[0])
        {
            strcat(szStem, stpNameLog->szStem);
            strcat(szStem, stpNameLog->szAssoSuffix);
            memset(stpNameLog->szAssoSuffix, 0, sizeof(stpNameLog->szAssoSuffix));
            valid++;
        }
        if(valid == 0 && stpNameLog->szAppendix[0])
        {
            strcat(szStem, stpNameLog->szStem);
            strcat(szStem, stpNameLog->szAppendix);
            memset(stpNameLog->szAppendix, 0, sizeof(stpNameLog->szAppendix));
            valid++;        
        }
        
        if(valid == 0 && stpNameLog->szCounty[0])
        {
            strcat(szStem, stpNameLog->szCounty);
            strcat(szStem, stpNameLog->szStem);
            memset(stpNameLog->szCounty, 0, sizeof(stpNameLog->szCounty));
            valid++;
        }
        if(valid == 0 && stpNameLog->szCity[0])
        {
            strcat(szStem, stpNameLog->szCity);
            strcat(szStem, stpNameLog->szStem);
            memset(stpNameLog->szCity, 0, sizeof(stpNameLog->szCity));
            valid++;
        }
        if(valid == 0 && stpNameLog->szProvince[0])
        {
            strcat(szStem, stpNameLog->szProvince);
            strcat(szStem, stpNameLog->szStem);
            memset(stpNameLog->szProvince, 0, sizeof(stpNameLog->szProvince));
            valid++;
        }
     
        if(szStem[0])
        { 
            memset(stpNameLog->szStem, 0, sizeof(stpNameLog->szStem));
            strcpy(stpNameLog->szStem, szStem);            
            memset(stpNameLog->szMinStem, 0, sizeof(stpNameLog->szMinStem));
            strcpy(stpNameLog->szMinStem, szStem);            
        }
    }
    
    if(ab_hmap_haskey(stpNameLib->stpNameTypeMap, stpNameLog->szStem) 
            && strcmp(ab_hmap_get(stpNameLib->stpNameTypeMap, stpNameLog->szStem), "1") == 0)        
    {
        char szStem[NAME_LEN_MAX + 1];
        memset(szStem, 0, sizeof(szStem));
    
        if(szStem[0] == 0 && stpNameLog->szCounty[0])
        {
            strcat(szStem, stpNameLog->szCounty);
            memset(stpNameLog->szCounty, 0, sizeof(stpNameLog->szCounty));
        }
        if(szStem[0] == 0 && stpNameLog->szCity[0])
        {
            strcat(szStem, stpNameLog->szCity);
            memset(stpNameLog->szCity, 0, sizeof(stpNameLog->szCity));
        }
        if(szStem[0] == 0 && stpNameLog->szProvince[0])
        {
            strcat(szStem, stpNameLog->szProvince);
            memset(stpNameLog->szProvince, 0, sizeof(stpNameLog->szProvince));
        }
        strcat(szStem, stpNameLog->szStem);

        memset(stpNameLog->szStem, 0, sizeof(stpNameLog->szStem));
        strcpy(stpNameLog->szStem, szStem);        
        memset(stpNameLog->szMinStem, 0, sizeof(stpNameLog->szMinStem));
        strcpy(stpNameLog->szMinStem, szStem);
    }

    if(stpNameLog->szProvince[0] && isdgc(stpNameLog->szProvince) && 
       stpNameLog->szCity[0] == 0)
        strcpy(stpNameLog->szCity, stpNameLog->szProvince);                      
}/*}}}*/
    
    
static void NAME_parseRecord_associate(const ABU_NAME_LIB_S *stpNameLib,
                                       const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                       int32_t *wordUnitStart,
                                       int32_t *wordUnitEnd,
                                       const char *szCity,
                                       NAME_PARAM_LOG_S *stpParamLog,
                                       NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL && stpParamLog != NULL);
    if(*wordUnitStart >= *wordUnitEnd)
        return;
    //step1:try to get stem type
#ifdef AB_DEBUG
    printf("  set appendix\n");
    printf("    appendix:%s\n", stpNameLog->szAppendix);
#endif

    NAME_parseRecord_associate_setStemPre(stpNameLib, stpSegResult,
                                          wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  set stemPre\n");
    printf("    stemPre:%s\n", stpNameLog->szStemPre);
#endif   

    NAME_parseRecord_associate_setStem(stpNameLib, stpSegResult,
                                       wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  set stem\n");
    printf("    stem:%s\n", stpNameLog->szStem);
    printf("    minStem:%s\n", stpNameLog->szMinStem);    
#endif

    NAME_parseRecord_associate_setStemPost(stpNameLib, stpSegResult,
                                           wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  set stemPost\n");
    printf("    stemPost:%s\n", stpNameLog->szStemPost);
#endif   

    //step2:try to get type
    NAME_parseRecord_associate_setType(stpNameLib, stpSegResult,
                                       wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  set Type\n");
    printf("    type:%s\n", stpNameLog->szType);
#endif

    //step3:try to get org
    NAME_parseRecord_associate_setOrg(stpNameLib, stpSegResult,
                                       wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  set stemOrg\n");
    printf("    org:%s\n", stpNameLog->szOrg);
#endif

    //step4:try to get associatePrefix
    NAME_parseRecord_associate_setAssoPrefix(stpNameLib, stpSegResult,
                                             wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  reset associatePrefix\n");
    printf("    assoPrefix:%s\n", stpNameLog->szAssoPrefix);
#endif

    //step5:try to get associateSuffix
    NAME_parseRecord_associate_setAssoSuffix(stpNameLib, stpSegResult,
                                             wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  reset associateSuffix\n");
    printf("    assoSuffix:%s\n", stpNameLog->szAssoSuffix);
#endif
}/*}}}*/


static void NAME_parseRecord_single(const ABU_NAME_LIB_S *stpNameLib,
                                    const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                    int32_t *wordUnitStart,
                                    int32_t *wordUnitEnd,
                                    const char *szCity,
                                    NAME_PARAM_LOG_S *stpParamLog, 
                                    NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL && stpParamLog != NULL)
    if(*wordUnitStart >= *wordUnitEnd)
        return;
   
    NAME_parseRecord_single_setAppendix(stpNameLib, stpSegResult, 
                                        wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  set appendix\n");
    printf("    appendix:%s\n", stpNameLog->szAppendix);
#endif

    NAME_parseRecord_single_setStemPre(stpNameLib, stpSegResult, 
                                       wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  set stemPre\n");
    printf("    stemPre:%s\n", stpNameLog->szStemPre);
#endif   

    NAME_parseRecord_single_setStem(stpNameLib, stpSegResult, 
                                    wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  set stem\n");
    printf("    stem:%s\n", stpNameLog->szStem);
    printf("    minStem:%s\n", stpNameLog->szMinStem);    
#endif

    NAME_parseRecord_single_setStemPost(stpNameLib, stpSegResult, 
                                        wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  set stemPost\n");
    printf("    stemPost:%s\n", stpNameLog->szStemPost);
#endif   

    NAME_parseRecord_single_setType(stpNameLib, stpSegResult, 
                                    wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  set stemType\n");
    printf("    stemType:%s\n", stpNameLog->szType);
#endif

    NAME_parseRecord_single_setOrg(stpNameLib, stpSegResult, 
                                   wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  set stemOrg\n");
    printf("    org:%s\n", stpNameLog->szOrg);
#endif

    NAME_parseRecord_single_resetAppendix(stpNameLib, stpSegResult, 
                                          wordUnitStart, wordUnitEnd, szCity, stpParamLog, stpNameLog);
#ifdef AB_DEBUG
    printf("  reset appendix\n");
    printf("    appendix:%s\n", stpNameLog->szAppendix);
#endif
}/*}}}*/
    
 
static void NAME_parseRecord_setAppendix(const ABU_NAME_LIB_S *stpNameLib,
                                         const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                         int32_t *wordUnitStart,
                                         int32_t *wordUnitEnd,
                                         const char *szCity,
                                         NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL);
    if(*wordUnitStart >= *wordUnitEnd)
        return;

    char *lastWord_0 = (stpSegResult->pWordArray + *wordUnitEnd -1)->szWord;
    int32_t startPos = -1;
    if(strcmp(lastWord_0, ")") == 0)
    {
        for(int32_t i = *wordUnitEnd - 1;i >= *wordUnitStart;i--)
        {
            char *szWord = (stpSegResult->pWordArray + i)->szWord;
            if(strcmp(szWord, "(") == 0)
                startPos = i;
        }
    }

    if(startPos != -1)
    {
        char szAppendix[NAME_LEN_MAX + 1];
        memset(szAppendix, 0, sizeof(szAppendix));
        for(int32_t i = startPos;i < *wordUnitEnd;i++)
            strcat(szAppendix, (stpSegResult->pWordArray + i)->szWord);
        strcpy(stpNameLog->szAppendix, szAppendix);        
        
        if((*wordUnitEnd -2) >= *wordUnitStart)
        {
            char *lastWord_1 = (stpSegResult->pWordArray + *wordUnitEnd -2)->szWord;
            char *lastWord_2 = (stpSegResult->pWordArray + *wordUnitEnd -3)->szWord;
   
            if((abstru_endswith(lastWord_1, "店") == TRUE || abstru_endswith(lastWord_1, "公司") == TRUE) &&  
               ab_hset_haskey(stpNameLib->stpNamePlaceSet, lastWord_2))                    
            {
                strcpy(stpNameLog->szBranchPlace, lastWord_2);                
            }
        }

        *wordUnitEnd = startPos; 
    }
}/*}}}*/
   

static void NAME_parseRecord_setProvince(const ABU_NAME_LIB_S *stpNameLib,
                             const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                             int32_t *wordUnitStart,
                             int32_t *wordUnitEnd,
                             const char *szCity,
                             NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL);
    if(*wordUnitStart >= *wordUnitEnd)
        return;

    unsigned char iseffective = FALSE;                                 
    char *szWord = (stpSegResult->pWordArray + *wordUnitStart)->szWord;
    if(ABU_MAPTREE_isFullProvince(stpNameLib->stpMapTree, szWord))
    {
        iseffective = TRUE;
    }
    else if (ABU_MAPTREE_isAliasProvince(stpNameLib->stpMapTree, szWord))
    {
        iseffective = TRUE;        
        if(szCity && isdgc(szCity) && !abstru_startswith(szWord, szCity) && !abstru_startswith(szCity, szWord))
            iseffective = FALSE;
        if(szCity && !isdgc(szCity) && !ABU_MAPTREE_isRightPCC(stpNameLib->stpMapTree, szWord, szCity, NULL))
                iseffective = FALSE;                        
    }

    if(iseffective)
    {
        strcpy(stpNameLog->szProvince, szWord);        
        *wordUnitStart += 1;
    }    
}/*}}}*/
    
    
static void NAME_parseRecord_setCity(const ABU_NAME_LIB_S *stpNameLib,
                         const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                         int32_t *wordUnitStart,
                         int32_t *wordUnitEnd,
                         const char *szCity,
                         NAME_RECOG_LOG_S *stpNameLog)

{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL);
    if(*wordUnitStart >= *wordUnitEnd)
        return;    

    unsigned char iseffective = FALSE;        
    char *szWord = (stpSegResult->pWordArray + *wordUnitStart)->szWord;
    if(ABU_MAPTREE_isFullCity(stpNameLib->stpMapTree, szWord))
    {
        iseffective = TRUE;
    }    
    else if(ABU_MAPTREE_isAliasCity(stpNameLib->stpMapTree, szWord))
    {
        iseffective = TRUE;
        if(iseffective && stpNameLog->szProvince[0] &&
           !ABU_MAPTREE_isParent(stpNameLib->stpMapTree, stpNameLog->szProvince, szWord))
            iseffective = FALSE;
        if(iseffective && szCity != NULL)
        {
            if(isdgc(szCity))
            {
                if(!ABU_MAPTREE_isParent(stpNameLib->stpMapTree, szCity, szWord))
                    iseffective = FALSE;                    
            }
            else
            {
                if(!abstru_startswith(szCity, szWord) && !abstru_startswith(szWord, szCity))
                    iseffective = FALSE;                
            }            
        }
    }

    if(iseffective)
    {
        strcpy(stpNameLog->szCity, szWord);        
        *wordUnitStart += 1;
    }    
}/*}}}*/
    
    
static void NAME_parseRecord_setCounty(const ABU_NAME_LIB_S *stpNameLib,
                         const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                         int32_t *wordUnitStart,
                         int32_t *wordUnitEnd,
                         const char *szCity,
                         NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL);
    if(*wordUnitStart >= *wordUnitEnd)
        return;     

    unsigned char iseffective = FALSE;    
    char *szWord = (stpSegResult->pWordArray + *wordUnitStart)->szWord;
    if(ABU_MAPTREE_isFullCounty(stpNameLib->stpMapTree, szWord))
    {
        iseffective = TRUE;        
    }
    else if(ABU_MAPTREE_isAliasCounty(stpNameLib->stpMapTree, szWord))
    {
        iseffective = TRUE;
        if(iseffective && stpNameLog->szProvince[0] &&
           !ABU_MAPTREE_isParent(stpNameLib->stpMapTree, stpNameLog->szProvince, szWord))
            iseffective = FALSE;
        if(iseffective && stpNameLog->szCity[0] &&
           !ABU_MAPTREE_isParent(stpNameLib->stpMapTree, stpNameLog->szCity, szWord))
            iseffective = FALSE;
        if(iseffective && szCity != NULL &&
           !ABU_MAPTREE_isParent(stpNameLib->stpMapTree, szCity, szWord))
            iseffective = FALSE;        
    }
    
    if(iseffective)
    {
        strcpy(stpNameLog->szCounty, szWord); 
        *wordUnitStart += 1;
    }      
}/*}}}*/


static void NAME_parseRecord_resetStemAndType(const ABU_NAME_LIB_S *stpNameLib,
                                              NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL);
#ifdef AB_DEBUG
    printf("<<<<<reconstruct MinStem and MaxType\n");
#endif

    char szStemType[NAME_LEN_MAX + 1];
    memset(szStemType, 0, sizeof(szStemType));
    strcat(szStemType, stpNameLog->szStem);
    strcat(szStemType, stpNameLog->szType);    

    ABU_WORDS_SEGRESULT_SL_S *stpSegResult = NULL;
    stpSegResult = ab_segmenter_seg(
            szStemType, strlen(szStemType),
            stpNameLib->stpSegDictNameType,stpNameLib->stpSegLib);

    if(stpSegResult != NULL)
    {
#ifdef AB_DEBUG
    printf("seg:");
    for(int i = 0;i < stpSegResult->dwWordNumber;i++)
    {
        printf("%s-", (stpSegResult->pWordArray + i)->szWord);    
    }
    printf("\n");
#endif
        memset(stpNameLog->szMinStem, 0, sizeof(stpNameLog->szMinStem));
        memset(stpNameLog->szMaxType, 0, sizeof(stpNameLog->szMaxType));        
        int pos = 0;
        for(;pos < stpSegResult->dwWordNumber;pos++)
        {
            char *szWord = (stpSegResult->pWordArray + pos)->szWord;
            if(NAME_isPartOfMiniStem(stpNameLib, stpNameLog->szMinStem, szWord))
                strcat(stpNameLog->szMinStem, szWord);
            else
                break;                
        }

        for(;pos < stpSegResult->dwWordNumber;pos++)
        {
            char *szWord = (stpSegResult->pWordArray + pos)->szWord;
            strcat(stpNameLog->szMaxType, szWord);
        }

        if(!stpNameLog->szMaxType[0]) 
        {
            for(int i = stpSegResult->dwWordNumber - 1;i >= 0;i--)
            {
                char *szWord = (stpSegResult->pWordArray + i)->szWord;
                if(!ab_hmap_haskey(stpNameLib->stpNameTypeMap, szWord))
                    break;
                memmove(stpNameLog->szMaxType  + strlen(szWord), stpNameLog->szMaxType, strlen(stpNameLog->szMaxType));
                memcpy(stpNameLog->szMaxType, szWord, strlen(szWord));                
            } 
        }
    } 
   
    ab_segmenter_free(stpSegResult);
    stpSegResult = NULL;
#ifdef AB_DEBUG
    printf("minStem:%s maxType:%s\n", stpNameLog->szMinStem, stpNameLog->szMaxType); 
    printf("reconstruct minStem and maxType>>>>>\n");
#endif
}/*}}}*/


static void NAME_parseRecord_setBestType(const ABU_NAME_LIB_S *stpNameLib,
                                         const char *szName,        
                                         const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                         NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && szName != NULL && stpSegResult != NULL && stpNameLog != NULL);
    if(stpNameLog->szAssoSuffix[0] && ab_hmap_haskey(stpNameLib->stpNameTypeMap, stpNameLog->szAssoSuffix) 
            && strcmp(ab_hmap_get(stpNameLib->stpNameTypeMap, stpNameLog->szAssoSuffix), "1") == 0)
    {
        strcpy(stpNameLog->szBestType, stpNameLog->szAssoSuffix); 
    }
    else if(stpNameLog->szMaxType[0] && ab_hmap_haskey(stpNameLib->stpNameTypeMap, stpNameLog->szMaxType)
            && strcmp(ab_hmap_get(stpNameLib->stpNameTypeMap, stpNameLog->szMaxType), "1") == 0)
    {
        strcpy(stpNameLog->szBestType, stpNameLog->szMaxType);
    }
    else if(stpNameLog->szType[0] && ab_hmap_haskey(stpNameLib->stpNameTypeMap, stpNameLog->szType)
            && strcmp(ab_hmap_get(stpNameLib->stpNameTypeMap, stpNameLog->szType), "1") == 0)
    {
        strcpy(stpNameLog->szBestType, stpNameLog->szType);        
    }
    else
    {
        NAME_parseRecord_getBestType(stpNameLib, stpSegResult, stpNameLog);
    }

    if(!stpNameLog->szBestType[0])
    {
        ABU_WORDS_SEGRESULT_SL_S *newSegResult = NULL;
        newSegResult = ab_segmenter_seg(szName, strlen(szName), stpNameLib->stpSegDictNameType, stpNameLib->stpSegLib);
        NAME_parseRecord_getBestType(stpNameLib, newSegResult, stpNameLog);
        ab_segmenter_free(newSegResult);
        newSegResult = NULL;
    }
}/*}}}*/

unsigned char NAME_isShortBestType(const ABU_NAME_LIB_S *stpNameLib,
                                 const char *szWord)
{
    ABU_ASSERT(stpNameLib != NULL && szWord != NULL);
    unsigned char ucFlag = FALSE;
    if(ab_hmap_haskey(stpNameLib->stpNameTypeMap, szWord) && 
       strcmp(ab_hmap_get(stpNameLib->stpNameTypeMap, szWord), "0") == 0) 
    {
        ucFlag = TRUE;
    }
    return ucFlag;
}

static void NAME_parseRecord_getBestType(const ABU_NAME_LIB_S *stpNameLib,
                                         const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                         NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpSegResult != NULL && stpNameLog != NULL);
    for(int i = stpSegResult->dwWordNumber - 1;i >= 0;i--) 
    {
        char *szWord = (stpSegResult->pWordArray + i)->szWord;
        if(ab_hmap_haskey(stpNameLib->stpNameTypeMap, szWord) && strcmp(ab_hmap_get(stpNameLib->stpNameTypeMap, szWord), "1") == 0)
        {
            memmove(stpNameLog->szBestType + strlen(szWord), stpNameLog->szBestType, strlen(stpNameLog->szBestType));
            memcpy(stpNameLog->szBestType, szWord, strlen(szWord));
            break;//alter a new version for cluster            
        }
        else
        {
            if(stpNameLog->szBestType[0])
                break;
            else
                continue;                
        }            
    }            
}/*}}}*/

static unsigned char NAME_isSingle(const ABU_NAME_LIB_S *stpNameLib,
                                   const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                   const int32_t wordUnitStart,
                                   const int32_t wordUnitEnd,
                                   const char *szCity,
                                   NAME_PARAM_LOG_S *stpParamLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpSegResult != NULL);
    if(wordUnitStart >= wordUnitEnd)
        return TRUE; 

#ifdef AB_DEBUG
    printf("<<<<<define is single pattern\n");
#endif

    int typeFeatCount = 0, nameStemCount = 0, orgCount = 0;   
    unsigned char endWithOrg = FALSE, isSingle =FALSE;    

    unsigned char effectivePos[wordUnitEnd - wordUnitStart + 1];
    memset(effectivePos, TRUE, sizeof(effectivePos));
    int leftPos = -1, rightPos = -1;
    for(int i = wordUnitStart;i < wordUnitEnd;i++)
    {
        char *szWord = (stpSegResult->pWordArray + i)->szWord;
        if(strcmp(szWord, "(") == 0)
            leftPos = i;
        if(strcmp(szWord, ")") == 0)
            rightPos = i;
        if(leftPos != -1 && rightPos != -1)
        {
            for(int j = leftPos;j <= rightPos;j++)
                effectivePos[j] = FALSE;
            leftPos = -1;
            rightPos = 1;            
        }            
    }    


    for(int i = wordUnitStart;i < wordUnitEnd;i++)
    {
        if(!effectivePos[i - wordUnitStart])
            continue; 
        char *szWord = (stpSegResult->pWordArray + i)->szWord;        
        if(ab_hmap_haskey(stpNameLib->stpNameTypeMap, szWord) && 
           strcmp(ab_hmap_get(stpNameLib->stpNameTypeMap, szWord), "1") == 0)
        {
            typeFeatCount++;
            nameStemCount++;
            if(NAME_endWithStemOrgFeat(stpNameLib, szWord))
                typeFeatCount++;
        }

        /*            
        if(ab_hset_haskey(stpNameLib->stpNamePlaceSet, szWord)) 
        {
            if(NAME_endWithTypeOrgFeat(szWord))
                typeFeatCount++;
        }
        */

        if(ab_hset_haskey(stpNameLib->stpNameOrgSet, szWord))
        {
            orgCount++;
            if(i == (wordUnitEnd - 1))
                endWithOrg = TRUE;
        }
    }

    if(NAME_isSinglePattern(stpNameLib, stpSegResult, 
                            wordUnitStart, wordUnitEnd, szCity))
    {
#ifdef AB_DEBUG
    printf("match single pattern\n");        
#endif        
        isSingle = TRUE; 
    }
    else if(NAME_isAssociatePattern(stpNameLib, stpSegResult,
                                    wordUnitStart, wordUnitEnd, szCity))
    {
#ifdef AB_DEBUG
    printf("match associate pattern\n");        
#endif
        isSingle = FALSE; 
    }
    else
    {   
        if(orgCount > 1 || nameStemCount > 1 || typeFeatCount > 1 || //(orgCount + typeFeatCount) > 1 
           (orgCount == 1 && endWithOrg == FALSE))
            isSingle = FALSE;            
        else
            isSingle = TRUE;
    }
    stpParamLog->isSingle = isSingle;
    stpParamLog->endWithOrg = endWithOrg;
    stpParamLog->typeFeatCount = typeFeatCount;
    stpParamLog->orgCount = orgCount;
    stpParamLog->nameStemCount = nameStemCount;    

#ifdef AB_DEBUG
    printf("<<<<<define is single pattern\n");
    printf("  isSingle:%s\n", stpParamLog->isSingle ? "True" : "False");
    printf("  endWithOrg:%s\n", stpParamLog->endWithOrg ? "True" : "False");
    printf("  typeFeatCount:%d\n", stpParamLog->typeFeatCount);
    printf("  orgCount:%d\n", stpParamLog->orgCount);
    printf("  nameStemCount:%d\n", stpParamLog->nameStemCount);
    if(isSingle)
        printf("is single pattern\n");
    else
        printf("is associate pattern\n");
                    
    printf("define is single pattern>>>>>\n");
#endif
    
    return isSingle; 
}/*}}}*/


static unsigned char NAME_isSinglePattern(const ABU_NAME_LIB_S *stpNameLib,
                                          const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                          const int32_t wordUnitStart,
                                          const int32_t wordUnitEnd,
                                          const char *szCity)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpSegResult != NULL);
    BOOL isSingle = FALSE;

    
    if(wordUnitStart < wordUnitEnd)
    {
        char *lastWord_0 = (stpSegResult->pWordArray + wordUnitEnd - 1)->szWord;
        if(strcmp(lastWord_0, "店") == 0||
           abstru_endswith(lastWord_0, "分店") ||
           abstru_endswith(lastWord_0, "分部") ||
           abstru_endswith(lastWord_0, "分公司"))
            isSingle = TRUE;
    }
    if(isSingle)
        return isSingle;
    

    int32_t orgCount = 0;
    for(int32_t i = wordUnitStart;i < wordUnitEnd;i++)
    {
        ABU_WORDS_WORD_S *stpWord = stpSegResult->pWordArray + i;
        if(ab_hset_haskey(stpNameLib->stpNameOrgSet, stpWord->szWord))            
            orgCount++;
    }

    if(orgCount >= 1 && (wordUnitStart + 1) < wordUnitEnd)
    {
        char *lastWord_0 = (stpSegResult->pWordArray + wordUnitEnd - 1)->szWord;
        char *lastWord_1 = (stpSegResult->pWordArray + wordUnitEnd -2)->szWord;
        if(abstru_endswith(lastWord_0, "店") && ab_hset_haskey(stpNameLib->stpNamePlaceSet, lastWord_1))
            isSingle = TRUE;
    }

    return isSingle;
}/*}}}*/


static unsigned char NAME_isAssociatePattern(const ABU_NAME_LIB_S *stpNameLib,
                                          const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                          const int32_t wordUnitStart,
                                          const int32_t wordUnitEnd,
                                          const char *szCity)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpSegResult != NULL);
    if(wordUnitStart >= wordUnitEnd)
        return FALSE;

    const char JIUDIAN[ ] = "酒店";
    const char FANDIAN[ ] = "饭店";
    
    int32_t jiudian_pos = -1;
    int32_t fandian_pos = -1;
    unsigned char jiudian_hasgbk = FALSE;
    unsigned char fandian_hasgbk = FALSE;    
    for(int32_t i = wordUnitStart;i < wordUnitEnd;i++)
    {
        char *szWord = (stpSegResult->pWordArray + i)->szWord;
        if(strstr(szWord, JIUDIAN) != NULL)
            jiudian_pos = i;
        if(strstr(szWord, FANDIAN) != NULL)
            fandian_pos = i;
    }
    for(int32_t i = wordUnitStart;i < wordUnitEnd;i++)
    {
        char *szWord = (stpSegResult->pWordArray + i)->szWord;
        if(jiudian_pos != -1 && i > jiudian_pos && szWord[0] < 0)
            jiudian_hasgbk = TRUE;
        if(fandian_pos != -1 && i > fandian_pos && szWord[0] < 0)
            fandian_hasgbk = TRUE;            
    }    
     
    if((jiudian_pos != -1 && jiudian_pos != (wordUnitEnd - 1) && jiudian_hasgbk) ||
       (fandian_pos != -1 && fandian_pos != (wordUnitEnd - 1) && fandian_hasgbk))
        return TRUE;
    
    return  FALSE;
}/*}}}*/


static void NAME_parseRecord_single_setAppendix(const ABU_NAME_LIB_S *stpNameLib,
                                                const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                int32_t *wordUnitStart,
                                                int32_t *wordUnitEnd,
                                                const char *szCity,
                                                NAME_PARAM_LOG_S *stpParamLog,
                                                NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL && stpParamLog != NULL);
    if((*wordUnitStart + 2) > *wordUnitEnd)
        return;

    char *lastWord_0 = (stpSegResult->pWordArray + *wordUnitEnd - 1)->szWord;
    char *lastWord_1 = (stpSegResult->pWordArray + *wordUnitEnd - 2)->szWord;
    if((abstru_endswith(lastWord_0, "店") || abstru_endswith(lastWord_0, "公司"))
       && !ab_hmap_haskey(stpNameLib->stpNameTypeMap, lastWord_0)
       && !ab_hset_haskey(stpNameLib->stpNameOrgSet, lastWord_0)
       && NAME_isEffectivePlace(stpNameLib, lastWord_1, szCity)) 
    {
        char szAppendix[NAME_LEN_MAX + 1];
        memset(szAppendix, 0, sizeof(szAppendix));
        strcat(szAppendix, lastWord_1);
        strcat(szAppendix, lastWord_0);
        if(stpNameLog->szAppendix[0])            
            strcat(szAppendix, stpNameLog->szAppendix);

        memset(stpNameLog->szAppendix, 0, sizeof(stpNameLog->szAppendix));
        strcpy(stpNameLog->szAppendix, szAppendix); 
        
        *wordUnitEnd -= 2;
        if(ab_hset_haskey(stpNameLib->stpNamePlaceSet, lastWord_1) && stpNameLog->szBranchPlace[0] == 0)
            strcpy(stpNameLog->szBranchPlace, lastWord_1);
    }
}/*}}}*/


static void NAME_parseRecord_single_setStemPre(const ABU_NAME_LIB_S *stpNameLib,
                                               const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                               int32_t *wordUnitStart,
                                               int32_t *wordUnitEnd,
                                               const char *szCity,
                                               NAME_PARAM_LOG_S *stpParamLog, 
                                               NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL && stpParamLog != NULL);
    if(*wordUnitStart >= *wordUnitEnd)
        return;

    char *szWordStart = (stpSegResult->pWordArray + *wordUnitStart)->szWord;
    if(strcmp(szWordStart, ")") == 0)
    {        
        strcpy(stpNameLog->szStemPre, szWordStart);
        *wordUnitStart += 1; 
    }

    if(strcmp(szWordStart, "(") == 0)
    {
        strcat(stpNameLog->szStemPre, szWordStart);
        *wordUnitStart += 1;
        unsigned char hasright = FALSE;        
        for(int i = *wordUnitStart;i < *wordUnitEnd;i++)
        {
            char *szWord = (stpSegResult->pWordArray + i)->szWord;
            if(strcmp(szWord, ")") == 0)
                hasright = TRUE;            
        }            
        if(hasright)
        {
            for(int i = *wordUnitStart;i < *wordUnitEnd;i++)
            {
                char *szWord = (stpSegResult->pWordArray + i)->szWord;
                strcat(stpNameLog->szStemPre, szWord);
                *wordUnitStart += 1;                    
                if(strcmp(szWord, ")") == 0)
                    break;
            }
        }        
    }
}/*}}}*/


static void NAME_parseRecord_single_setStem(const ABU_NAME_LIB_S *stpNameLib,
                                            const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                            int32_t *wordUnitStart,
                                            int32_t *wordUnitEnd,
                                            const char *szCity,
                                            NAME_PARAM_LOG_S *stpParamLog, 
                                            NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL && stpParamLog != NULL);

    unsigned char stemHasTypeFeat = FALSE;    
    for(int i = *wordUnitStart;i < *wordUnitEnd;i++)
    {
        char *szWord = (stpSegResult->pWordArray + i)->szWord;
        if(NAME_endWithAddrFeat(stpNameLog->szStem) == FALSE &&
           ((ab_hmap_haskey(stpNameLib->stpNameTypeMap, szWord) && 
             strcmp(ab_hmap_get(stpNameLib->stpNameTypeMap, szWord), "1") == 0)|| 
            ab_hset_haskey(stpNameLib->stpNameOrgSet, szWord))) 
               break;
        if(strcmp(szWord, "(") == 0 || strcmp(szWord, ")") == 0)
            break;            
        
        strcat(stpNameLog->szStem, szWord);
        *wordUnitStart += 1;
       
        if(ab_hmap_haskey(stpNameLib->stpNameStemMap, szWord))
        {
            strcpy(stpNameLog->szMultipleShop, szWord);
            if(NAME_endWithStemOrgFeat(stpNameLib, szWord))
            {
                stemHasTypeFeat = TRUE;
            }
            break;            
        }
        if(ab_hset_haskey(stpNameLib->stpNamePlaceSet, szWord)) 
        {
            if(NAME_endWithTypeOrgFeat(stpNameLib, szWord))
            {                
                stemHasTypeFeat = TRUE;
                break;
            }
        }
        if(abstru_isletters(szWord) && strlen(stpNameLog->szStem) >= 2)
        {
            if(*wordUnitStart == *wordUnitEnd)
                break;
            if(*wordUnitStart < *wordUnitEnd &&
               (stpSegResult->pWordArray + *wordUnitStart)->szWord[0] < 0)
                break;
        }
    }
    
    if(NAME_isEffectiveStem(stpNameLib, stpNameLog->szStem) == FALSE)
    {
        for(int i = *wordUnitStart;i < *wordUnitEnd;i++)
        {
            char *szWord = (stpSegResult->pWordArray + i)->szWord;
            if(ab_hset_haskey(stpNameLib->stpNameOrgSet, szWord) || 
               (ab_hmap_haskey(stpNameLib->stpNameTypeMap, szWord) && 
                strcmp(ab_hmap_get(stpNameLib->stpNameTypeMap, szWord), "1") == 0))
                break;
            if(strcmp(szWord, "(") == 0 || strcmp(szWord, ")") == 0)
                break;
           
            strcat(stpNameLog->szStem, szWord);
            *wordUnitStart += 1;
        }
    }

    if(NAME_isEffectiveStem(stpNameLib, stpNameLog->szStem) == FALSE)
    {
        for(int i = *wordUnitStart;i < *wordUnitEnd;i++)
        {
            char *szWord = (stpSegResult->pWordArray + i)->szWord;
            if(ab_hset_haskey(stpNameLib->stpNameOrgSet, szWord)) 
                break;
            if(strcmp(szWord, "(") == 0 || strcmp(szWord, ")") == 0)
                break;
 
            strcat(stpNameLog->szStem, szWord);
            *wordUnitStart += 1;

            if(ab_hmap_haskey(stpNameLib->stpNameTypeMap, szWord) &&
               strcmp(ab_hmap_get(stpNameLib->stpNameTypeMap, szWord), "1") == 0)
                stemHasTypeFeat = TRUE;
            if(stemHasTypeFeat == TRUE)
                break;
        }
    }
}/*}}}*/


static void NAME_parseRecord_single_setStemPost(const ABU_NAME_LIB_S *stpNameLib,
                                                const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                int32_t *wordUnitStart,
                                                int32_t *wordUnitEnd,
                                                const char *szCity,
                                                NAME_PARAM_LOG_S *stpParamLog, 
                                                NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL && stpParamLog != NULL);
    if(*wordUnitStart >= *wordUnitEnd)
        return;

    char *szWordStart = (stpSegResult->pWordArray + *wordUnitStart)->szWord;
    if(strcmp(szWordStart, "(") == 0)
    {
        strcat(stpNameLog->szStemPost, szWordStart);
        *wordUnitStart += 1;
        unsigned char hasright = FALSE;        
        for(int i = *wordUnitStart;i < *wordUnitEnd;i++)
        {
            char *szWord = (stpSegResult->pWordArray + i)->szWord;
            if(strcmp(szWord, ")") == 0)
                hasright = TRUE;            
        }            
        if(hasright)
        {
            for(int i = *wordUnitStart;i < *wordUnitEnd;i++)
            {
                char *szWord = (stpSegResult->pWordArray + i)->szWord;
                strcat(stpNameLog->szStemPost, szWord);
                *wordUnitStart += 1;                    
                if(strcmp(szWord, ")") == 0)
                    break;
            }
        }        
    }
    if((*wordUnitStart + 1) < *wordUnitEnd)
    {
        char *szWord = (stpSegResult->pWordArray + *wordUnitStart)->szWord;
        char *szNextWord = (stpSegResult->pWordArray + *wordUnitStart + 1)->szWord;
        if(NAME_isEffectivePlace(stpNameLib, szWord, szCity) &&
           ab_hmap_haskey(stpNameLib->stpNameTypeMap, szNextWord) && 
           strcmp(ab_hmap_get(stpNameLib->stpNameTypeMap, szNextWord), "1") == 0)
        {
            strcat(stpNameLog->szStemPost, szWord);
            *wordUnitStart += 1; 
        }
    } 

}/*}}}*/


static void NAME_parseRecord_single_setType(const ABU_NAME_LIB_S *stpNameLib,
                                            const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                            int32_t *wordUnitStart,
                                            int32_t *wordUnitEnd,
                                            const char *szCity,
                                            NAME_PARAM_LOG_S *stpParamLog, 
                                            NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL && stpParamLog != NULL);

    unsigned char hasOrg = FALSE;
    if(stpParamLog->orgCount >= 1)
        hasOrg = TRUE;        

    for(int i = *wordUnitStart;i < *wordUnitEnd;i++)
    {
        char *szWord = (stpSegResult->pWordArray + i)->szWord;
        if(!hasOrg && !ab_hmap_haskey(stpNameLib->stpNameTypeMap, szWord))
            break;            

        if(ab_hset_haskey(stpNameLib->stpNameOrgSet, szWord)) 
            break;
        //中国邮政广州市邮政局 
        if(i == (*wordUnitEnd - 1) && ab_hmap_haskey(stpNameLib->stpNameTypeMap, szWord) &&
                strcmp(ab_hmap_get(stpNameLib->stpNameTypeMap, szWord), "1") == 0 &&
           !stpParamLog->isSingle)
            break; 
                
        strcat(stpNameLog->szType, szWord);
        *wordUnitStart += 1;

        if(ab_hmap_haskey(stpNameLib->stpNameTypeMap, szWord) && 
           strcmp(ab_hmap_get(stpNameLib->stpNameTypeMap, szWord), "1") == 0 &&
           (!hasOrg || !stpParamLog->isSingle))
            break;            
    }
    /*
    if(hasorg)
    {
        for(int i = *wordUnitStart;i < *wordUnitEnd;i++)
        {
            char *szWord = (stpSegResult->pWordArray + i)->szWord;
            if(ab_hset_haskey(stpNameLib->stpNameOrgSet, szWord))            
                break;
            strcat(nameLog.szType, szWord);
            *wordUnitStart += 1;
        }               
    } 
    */    
}/*}}}*/


static void NAME_parseRecord_single_setOrg(const ABU_NAME_LIB_S *stpNameLib,
                                           const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                           int32_t *wordUnitStart,
                                           int32_t *wordUnitEnd,
                                           const char *szCity,
                                           NAME_PARAM_LOG_S *stpParamLog, 
                                           NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL && stpParamLog != NULL);

    if(*wordUnitStart < *wordUnitEnd)
    {
        char *szWord = (stpSegResult->pWordArray + *wordUnitStart)->szWord;
        char *szPreWord = NULL;
        if((*wordUnitStart - 1) >= 0)
            szPreWord = (stpSegResult->pWordArray + *wordUnitStart - 1)->szWord;        
        if(ab_hset_haskey(stpNameLib->stpNameOrgSet, szWord))            
        {
            //百达新材料科技苏州有限公司
            if(szPreWord != NULL && szCity != NULL && abstru_endswith(stpNameLog->szType, szPreWord) && 
               (ABU_MAPTREE_isProvince(stpNameLib->stpMapTree, szPreWord) || 
                ABU_MAPTREE_isCity(stpNameLib->stpMapTree, szPreWord)) &&
               (abstru_startswith(szCity, szPreWord) || abstru_startswith(szPreWord, szCity)))
            {
                abstru_rstrip_str(stpNameLog->szType, szPreWord);
                strcat(stpNameLog->szOrg, szPreWord);                
            }
                        
            strcat(stpNameLog->szOrg, szWord);
            *wordUnitStart += 1;
        }
    }
}/*}}}*/


static void NAME_parseRecord_single_resetAppendix(const ABU_NAME_LIB_S *stpNameLib,
                                                  const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                  int32_t *wordUnitStart,
                                                  int32_t *wordUnitEnd,
                                                  const char *szCity,
                                                  NAME_PARAM_LOG_S *stpParamLog,
                                                  NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL && stpParamLog != NULL);

    char szAppendix[NAME_LEN_MAX + 1];
    memset(szAppendix, 0, sizeof(szAppendix));    
    for(int i = *wordUnitStart;i < *wordUnitEnd;i++)
    {
        char *szWord = (stpSegResult->pWordArray + i)->szWord;
        strcat(szAppendix, szWord);        
    }
    strcat(szAppendix, stpNameLog->szAppendix);
    memset(stpNameLog->szAppendix, 0, sizeof(stpNameLog->szAppendix));
    strcpy(stpNameLog->szAppendix, szAppendix);

    //single gb code
    while(strlen(stpNameLog->szAppendix) == 2 && stpNameLog->szAppendix[0] < 0)
    {
        if(stpNameLog->szAssoSuffix[0])
        {
            strcat(stpNameLog->szAssoSuffix, stpNameLog->szAppendix);
            memset(stpNameLog->szAppendix, 0, sizeof(stpNameLog->szAppendix));
            break;            
        }
        else if(stpNameLog->szAssoPrefix[0])
        {
            break;            
        }
        else if(stpNameLog->szOrg[0])
        {
            break;
        }   
        else if(stpNameLog->szType[0])
        {
            strcat(stpNameLog->szType, stpNameLog->szAppendix);
            memset(stpNameLog->szAppendix, 0, sizeof(stpNameLog->szAppendix));
            break;
        }
        else if(stpNameLog->szStem[0])
        {
            strcat(stpNameLog->szStem, stpNameLog->szAppendix);
            memset(stpNameLog->szAppendix, 0, sizeof(stpNameLog->szAppendix));
            break;
        }
        else
        {
            break;
        }        
    }
}/*}}}*/


static void NAME_parseRecord_associate_setStemPre(const ABU_NAME_LIB_S *stpNameLib,
                                                  const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                  int32_t *wordUnitStart,
                                                  int32_t *wordUnitEnd,
                                                  const char *szCity,
                                                  NAME_PARAM_LOG_S *stpParamLog,
                                                  NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    NAME_parseRecord_single_setStemPre(stpNameLib, stpSegResult, wordUnitStart, 
                                       wordUnitEnd, szCity, stpParamLog, stpNameLog);    
}/*}}}*/
  

static void NAME_parseRecord_associate_setStem(const ABU_NAME_LIB_S *stpNameLib,
                                               const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                               int32_t *wordUnitStart,
                                               int32_t *wordUnitEnd,
                                               const char *szCity,
                                               NAME_PARAM_LOG_S *stpParamLog,
                                               NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    NAME_parseRecord_single_setStem(stpNameLib, stpSegResult, wordUnitStart, 
                                    wordUnitEnd, szCity, stpParamLog, stpNameLog);
}/*}}}*/


static void NAME_parseRecord_associate_setStemPost(const ABU_NAME_LIB_S *stpNameLib,
                                                   const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                   int32_t *wordUnitStart,
                                                   int32_t *wordUnitEnd,
                                                   const char *szCity,
                                                   NAME_PARAM_LOG_S *stpParamLog, 
                                                   NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    NAME_parseRecord_single_setStemPost(stpNameLib, stpSegResult, wordUnitStart, 
                                        wordUnitEnd, szCity, stpParamLog, stpNameLog);
}/*}}}*/


static void NAME_parseRecord_associate_setType(const ABU_NAME_LIB_S *stpNameLib,
                                               const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                               int32_t *wordUnitStart,
                                               int32_t *wordUnitEnd,
                                               const char *szCity,
                                               NAME_PARAM_LOG_S *stpParamLog, 
                                               NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    NAME_parseRecord_single_setType(stpNameLib, stpSegResult, wordUnitStart, 
                                    wordUnitEnd, szCity, stpParamLog, stpNameLog);
    /*    
    unsigned char stemHasTypeFeat = FALSE;
    if(NAME_endWithTypeOrgFeat(stpNameLog->szStem, stpNameLib))
        stemHasTypeFeat = TRUE;        

    for(int i = *wordUnitStart;i < *wordUnitEnd;i++)
    {
        if(stemHasTypeFeat == TRUE)
            break;
        char *szWord = (stpSegResult->pWordArray + i)->szWord;
        if(ab_hset_haskey(stpNameLib->stpNameOrgSet, szWord) || ab_hmap_haskey(stpNameLib->stpNameStemMap, szWord))                        
            break;

        strcat(stpNameLog->szType, szWord);
        *wordUnitStart += 1;

        if(NAME_endWithTypeOrgFeat(szWord, stpNameLib))
            break;
    }
    */
}/*}}}*/


static void NAME_parseRecord_associate_setOrg(const ABU_NAME_LIB_S *stpNameLib,
                                              const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                              int32_t *wordUnitStart,
                                              int32_t *wordUnitEnd,
                                              const char *szCity,
                                              NAME_PARAM_LOG_S *stpParamLog, 
                                              NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    NAME_parseRecord_single_setOrg(stpNameLib, stpSegResult, wordUnitStart, 
                                   wordUnitEnd, szCity, stpParamLog, stpNameLog); 
}/*}}}*/


static void NAME_parseRecord_associate_setAssoPrefix(const ABU_NAME_LIB_S *stpNameLib,
                                                  const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                  int32_t *wordUnitStart,
                                                  int32_t *wordUnitEnd,
                                                  const char *szCity,
                                                  NAME_PARAM_LOG_S *stpParamLog, 
                                                  NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL && stpParamLog != NULL);

    for(int i = *wordUnitStart;i < *wordUnitEnd - 1;i++)
    {
        char *szWord = (stpSegResult->pWordArray + i)->szWord;
        strcat(stpNameLog->szAssoPrefix, szWord);
        *wordUnitStart += 1;
    }
}/*}}}*/



static void NAME_parseRecord_associate_setAssoSuffix(const ABU_NAME_LIB_S *stpNameLib,
                                                     const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                                     int32_t *wordUnitStart,
                                                     int32_t *wordUnitEnd,
                                                     const char *szCity,
                                                     NAME_PARAM_LOG_S *stpParamLog, 
                                                     NAME_RECOG_LOG_S *stpNameLog)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && stpNameLog != NULL && stpSegResult != NULL);
    ABU_ASSERT(wordUnitStart != NULL && wordUnitEnd != NULL && stpParamLog != NULL);
    for(int i = *wordUnitStart;i < *wordUnitEnd;i++)
    {
        char *szWord = (stpSegResult->pWordArray + i)->szWord;
        strcat(stpNameLog->szAssoSuffix, szWord);
    } 
}/*}}}*/
