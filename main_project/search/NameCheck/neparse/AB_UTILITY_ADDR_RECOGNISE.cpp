/**
 *@file AB_UTILITY_ADDR_RECOGNISE.h
 *@author bfzhang
 *@brief recognise the addr
 * create the file in 2007-07-03
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "abutility.h"
#include "abstring.h"
#include "STRING_UTILITY.h"
#include "AB_UTILITY_ADDR.h"
#include "AB_UTILITY_ADDR_RESOURCE.h"
#include "AB_UTILITY_ADDR_RECOGNISE.h"
#include "AB_UTILITY_WORDS.h"
#include "AB_UTILITY_WORDS_UTILITY.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "AB_UTILITY_WORDS_SEG_SL.h"

#define GB_LEN 2
#define ASCII_LEN 1
/******************************************************************/
/******private interface declaration*******************************/
/******************************************************************/
/**
 * @desc: log the addr recognise information
 */
typedef struct _ADDR_RECOG_LOG_S
{
        //if exist addr feature word
    BOOL bExistFeatWord;
        //if exist addr( by segment--ns postag)
    BOOL bExistNSPosTag;
        //if end with company suffix
    BOOL bEndWithCmpSuffix;
        //if exist addr prefix word
    BOOL bExistPrefix;
        //if exist addr prefix flag
    BOOL bExistPrefixFlag;
        //loop exist addr feature word
    BOOL bLoopExistFeatWord;
        //loop exist addr(by segment--ns postag)
    BOOL bLoopExistNSPosTag;
        //addr detail level
    ADDR_DETAILLEVEL detailLevel;
        //sum rule weight
    int sumWeight;
        //word number(by segment)
    int wordSpan;
}ADDR_RECOG_LOG_S;

static void ADDR_RECOG_destroyContent(ABU_ADDR_RECOG_S *stpAddrRecog);


static void ADDR_RECOG_destroyLibContent(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib);

/**
 * @desc: create addr separate set
 */
static void ADDR_RECOG_createSeparSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib);

/**
 * @desc: create addr feature word
 */
static void ADDR_RECOG_createFeatWordSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib);

/**
 * @desc: create addr prefix set
 */
static void ADDR_RECOG_createPrefixSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib);

/**
 * @desc:create addr prefix flag set
 */
static void ADDR_RECOG_createPrefixFlagSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib);

/**
 * @desc: create addr index word
 */
static void ADDR_RECOG_createIndexWordSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib);

/**
 * @desc create suffix dict
 */
static  void ADDR_RECOG_createSuffixDict(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib);

/**
 * @desc: create company suffix hashset
 */
//static void ADDR_RECOG_createCmpSuffixSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib);

/**
 * @desc: create addr suffix set
 */
static void ADDR_RECOG_createAddrSuffixSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib);

/**
 * @desc: create rule weight map
 */
static void ADDR_RECOG_createRuleMap(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib);

/**
 * @desc: init ABU_ADDR_RECOG_S
 */
static ABU_ADDR_RECOG_S *ADDR_RECOG_init(uint32_t initSize);

/**
 * @desc:addr recognise by separator
 */
static void ADDR_RECOG_parseBySepa(const ABU_ADDR_RECOG_LIB_S  *stpAddrRecogLib,
                                   const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                   ABU_ADDR_RECOG_S *stpAddrRecog,
                                   int *piHasCover,
                                   const char *szSource,
                                   uint32_t *mapping);
/**
 * @desc: parse by addr separator: sum the addr rule weight
 */
static void ADDR_RECOG_parseBySepa_sumWeight(const ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib,
                                             const ABU_ADDR_RECORD_S *stpAddrRecord,
                                             ADDR_RECOG_LOG_S *stpAddrRecogLog);
                 
/**
 * @desc: addr recognise by structural information(prefix word)
 */
static void ADDR_RECOG_parseByPrefix(const ABU_ADDR_RECOG_LIB_S  *stpRecogLib,
                                     const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                     ABU_ADDR_RECOG_S *stpAddrRecog,
                                     int *piHasCover,
                                     const char *szSource,
                                     uint32_t *mapping);
/**
 * @desc: addr recognisation : add addr record
 */
static void ADDR_RECOG_parse_add(const ABU_ADDR_S *stpAddrRecord,
                                 ABU_ADDR_RECOG_S *stpAddrRecog);    

/**
 * @desc judge the addr end with cmp suffix 
 */
static BOOL ADDR_RECOG_isEndWithCmpSuffix(const char *szAddr,
                const ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib);                

/**
 * @desc stpAddrRecord is province || city || county
 */
static BOOL ADDR_RECOG_isPCC(const ABU_ADDR_RECORD_S *stpAddrRecord);
            
/**
 * @desc normalize credit weight
 */
static int ADDR_RECOG_normalizeWeight(const int weight)
{
    return (weight >  MAX_ADDR_CREDIT_WEIGHT)?
            MAX_ADDR_CREDIT_WEIGHT:weight;
}            
/**********public interface implementation***********************/
/**
 * @desc: create addr recognise lib
 */
ABU_ADDR_RECOG_LIB_S *ABU_ADDR_RECOG_createLib(
                const char **szDictArray,const uint32_t uiCount)
{
    ABU_ASSERT(szDictArray != NULL);
    
    ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib = (ABU_ADDR_RECOG_LIB_S *)ab_calloc(1,sizeof(ABU_ADDR_RECOG_LIB_S));
    
    stpAddrRecogLib->stpAddrLib = ABU_ADDR_createLib(szDictArray,uiCount);
    
    ADDR_RECOG_createAddrSuffixSet(stpAddrRecogLib);
    
    ADDR_RECOG_createSuffixDict(stpAddrRecogLib);
    
    ADDR_RECOG_createFeatWordSet(stpAddrRecogLib);
    
    ADDR_RECOG_createIndexWordSet(stpAddrRecogLib);
    
    ADDR_RECOG_createPrefixFlagSet(stpAddrRecogLib);
    
    ADDR_RECOG_createPrefixSet(stpAddrRecogLib);
    
    ADDR_RECOG_createRuleMap(stpAddrRecogLib);
        
    ADDR_RECOG_createSeparSet(stpAddrRecogLib);
    
    return stpAddrRecogLib;
}

/**
 * @desc: destroy addr recognise lib
 */
void ABU_ADDR_RECOG_destroyLib(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib)
{
    ABU_ASSERT(stpAddrRecogLib != NULL);

    ADDR_RECOG_destroyLibContent(stpAddrRecogLib);
    
    ABU_FREE(stpAddrRecogLib);
}

/**
 * @desc:parse string to recognise addr result
 */
ABU_ADDR_RECOG_S *ABU_ADDR_RECOG_parse(const ABU_ADDR_RECOG_LIB_S *stpRecogLib,
                                       const char *szBuff)
{
    ABU_ASSERT(stpRecogLib != NULL && szBuff != NULL);
    
    ABU_WORDS_SEGRESULT_SL_S *stpSegResult = 
        ABU_WORDS_SEG_SL_getSegResultFromBuff(szBuff,strlen(szBuff),
                                 stpRecogLib->stpAddrLib->stpSegDict,
                                 stpRecogLib->stpAddrLib->stpSegLib);
    if(stpSegResult == NULL)
    {
        //fprintf(stderr, "Seg Addr Failed. with Addr: %s\n", szBuff);
        return NULL;
    }

    //source:source string targe:segment string
    //mapping(source->target)
    uint32_t sourceLen = strlen(szBuff);
    char szSource[sourceLen + 1];
    memset(szSource, 0, sizeof(szSource));
    memcpy(szSource, szBuff, sourceLen);

    uint32_t mapping[sourceLen];
    memset(mapping, 0, sizeof(mapping));
    //uint32_t *mapping = ab_calloc(sourceLen, sizeof(uint32_t));
    for(int i = 0;i < sourceLen;i++)
        mapping[i] = i;
    getMapping(szSource, mapping);

    uint32_t initSize = 1024;
    ABU_ADDR_RECOG_S *stpAddrRecog = ADDR_RECOG_init(initSize);
    
    for(int hasCover = 0;hasCover < stpSegResult->dwWordNumber;)
    {
        //recognise by addr separator=>
        ADDR_RECOG_parseBySepa(stpRecogLib,stpSegResult,stpAddrRecog,&hasCover, szBuff, mapping);
        
        //recognise by addr prefix=>get structured add  
        ADDR_RECOG_parseByPrefix(stpRecogLib,stpSegResult,stpAddrRecog,&hasCover, szBuff, mapping);    
    }
    
    ABU_WORDS_SEG_SL_destroySegResult(stpSegResult);

    return stpAddrRecog;
}

  
/**
 * @desc:destroy recognise addr result
 */
void ABU_ADDR_RECOG_destroy(ABU_ADDR_RECOG_S *stpAddrRecog)
{
    if(stpAddrRecog == NULL) return;
    
    ADDR_RECOG_destroyContent(stpAddrRecog);
    
    ABU_FREE(stpAddrRecog);
}

//#ifndef AB_DEBUG
void ABU_ADDR_RECOG_print(const ABU_ADDR_RECOG_S *stpAddrRecog,
                              const BOOL bVerbose,FILE *fp)
{
    ABU_ASSERT(stpAddrRecog != NULL);
    
    fprintf(fp,"total %u addr recog result\n",stpAddrRecog->uiAddrCount);
    
    if(bVerbose == TRUE)
    {
        for(int i = 0;i < stpAddrRecog->uiAddrCount;i++)
        {
            fprintf(fp,"\n");
            ABU_ADDR_print(stpAddrRecog->stpAddrArray[i],fp);
        }
    }
    else
    {
        fprintf(fp,"format    <addr,creditWeigth>\n");
        for(int i = 0;i < stpAddrRecog->uiAddrCount;i++)
        {
            fprintf(fp,"<%s,%d>\n",(stpAddrRecog->stpAddrArray[i])->szOriginAddr,
                               (stpAddrRecog->stpAddrArray[i])->iCredWeight);
        }    
    }
}
//#endif

/******private interface implementation******************************/
static void ADDR_RECOG_destroyContent(ABU_ADDR_RECOG_S *stpAddrRecog)
{
    //step1:free each stpAddr
    for(int i = 0;i < stpAddrRecog->uiAddrCount;i++)
    {
        ABU_ADDR_destroy(stpAddrRecog->stpAddrArray[i]);
    }
    
    //step2: free array
    ABU_FREE(stpAddrRecog->stpAddrArray);
}

/**
 * @desc: destroy the content of addr recogise lib
 */
static void ADDR_RECOG_destroyLibContent(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib)
{
    ABU_ASSERT(stpAddrRecogLib != NULL);
 
    destroyHashSet(stpAddrRecogLib->stpSeparSet);
 
    destroyHashMap(stpAddrRecogLib->stpRuleWeightMap);
 
    destroyHashSet(stpAddrRecogLib->stpPrefixSet);
 
    destroyHashSet(stpAddrRecogLib->stpPrefixFlagSet);

    destroyHashSet(stpAddrRecogLib->stpIndexWordSet);

    ABU_WORDS_DICT_destroyDict(stpAddrRecogLib->stpCmpSuffixDict);

    destroyHashSet(stpAddrRecogLib->stpFeatWordSet);

    destroyHashSet(stpAddrRecogLib->stpSuffixSet);    

    ABU_ADDR_destroyLib(stpAddrRecogLib->stpAddrLib);
}
/**
 * @desc: create addr separate set
 */
static void ADDR_RECOG_createSeparSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib)
{
    ABU_ASSERT(stpAddrRecogLib != NULL);
    
    stpAddrRecogLib->stpSeparSet = createHashSet(ADDR_SEPARATOR_COUNT);    
    if(stpAddrRecogLib->stpSeparSet == NULL) return;
    
    for(int i = 0;i < ADDR_SEPARATOR_COUNT;i++)
    {
        insertHashSetNode(stpAddrRecogLib->stpSeparSet,ADDR_SEPARATOR[ i ]);
    }
}

/**
 * @desc: create addr feature word
 */
static void ADDR_RECOG_createFeatWordSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib)
{
    ABU_ASSERT(stpAddrRecogLib);

    stpAddrRecogLib->stpFeatWordSet = createHashSet(ADDR_FEAT_WORD_COUNT);
    if(stpAddrRecogLib->stpFeatWordSet == NULL) return;
    
    for(int i = 0;i < ADDR_FEAT_WORD_COUNT;i++)
    {
        insertHashSetNode(stpAddrRecogLib->stpFeatWordSet,ADDR_FEAT_WORD[ i ]);
    }
}

/**
 * @desc: create addr prefix set
 */
static void ADDR_RECOG_createPrefixSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib)
{
    ABU_ASSERT(stpAddrRecogLib != NULL);
    
    stpAddrRecogLib->stpPrefixSet = createHashSet(ADDR_PREFIX_COUNT);
    if(stpAddrRecogLib->stpPrefixSet == NULL) return;
    
    for(int i = 0;i < ADDR_PREFIX_COUNT;i++)
    {
        insertHashSetNode(stpAddrRecogLib->stpPrefixSet,ADDR_PREFIX[ i ]);
    }
}

/**
 * @desc:create addr prefix flag set
 */
static void ADDR_RECOG_createPrefixFlagSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib)
{
    ABU_ASSERT(stpAddrRecogLib != NULL);
    
    stpAddrRecogLib->stpPrefixFlagSet = createHashSet(ADDR_PREFIX_FLAG_COUNT);
    if(stpAddrRecogLib ->stpPrefixFlagSet == NULL) return;
    
    for(int i =0 ;i < ADDR_PREFIX_FLAG_COUNT;i++)
    {
        insertHashSetNode(stpAddrRecogLib->stpPrefixFlagSet,ADDR_PREFIX_FLAG[i]);
    }
}

/**
 * @desc: create addr index word
 */
static void ADDR_RECOG_createIndexWordSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib)
{
    ABU_ASSERT(stpAddrRecogLib != NULL);
    
    stpAddrRecogLib->stpIndexWordSet = createHashSet(ADDR_INDEX_WORD_COUNT);
    if(stpAddrRecogLib->stpIndexWordSet == NULL) return;
    
    for(int i = 0;i < ADDR_INDEX_WORD_COUNT;i++)
    {
        insertHashSetNode(stpAddrRecogLib->stpIndexWordSet,ADDR_INDEX_WORD[ i ]);
    }
}

/**
 * @desc create suffix dict
 */
static  void ADDR_RECOG_createSuffixDict(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib)
{
    ABU_ASSERT(stpAddrRecogLib != NULL);
    
    //step1: load cmp_suffix into array
    char **szWordItemArray = (char**)ab_calloc(CMP_SUFFIX_WORD_COUNT,sizeof(char *));
    char szBuff[128];
    int wordCount = 0;
    for(int i = 0;i < CMP_SUFFIX_WORD_COUNT;i++)
    {
        memset(szBuff,0,sizeof(szBuff));
        strcat(szBuff,CMP_SUFFIX_WORD[ i ]);
        strcat(szBuff," ");
        strcat(szBuff,CMP_SUFFIX_POSTAG);
        
        int len = strlen(szBuff);
        szWordItemArray[ wordCount ] = (char*)ab_calloc(len + 1,sizeof(char));
        strncpy(szWordItemArray[ wordCount ],szBuff,len);
        wordCount++; 
    }
    
    //step2:create cmp suffix dict
    stpAddrRecogLib->stpCmpSuffixDict = ABU_WORDS_DICT_loadDictFromArray((const char**)szWordItemArray,wordCount);
    
    //step3: free memory
    for(int i = 0;i < wordCount;i++)
    {
        ABU_FREE(szWordItemArray[ i ]);
    }
    ABU_FREE(szWordItemArray);
    
}


/**
 * @desc: create rule weight map
 */
static void ADDR_RECOG_createRuleMap(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib)
{
    ABU_ASSERT(stpAddrRecogLib != NULL);
    
    uint32_t initSize = 10;
    stpAddrRecogLib->stpRuleWeightMap = createHashMap(initSize);
    if(stpAddrRecogLib->stpRuleWeightMap == NULL) return;
    
    //positive rule 
    insertHashMapNode(stpAddrRecogLib->stpRuleWeightMap,
                        ADDR_RULE_PROVINCE,ADDR_RULE_PROVINCE_WEIGHT);
    
    insertHashMapNode(stpAddrRecogLib->stpRuleWeightMap,
                        ADDR_RULE_CITY,ADDR_RULE_CITY_WEIGHT);
                        
    insertHashMapNode(stpAddrRecogLib->stpRuleWeightMap,
                        ADDR_RULE_COUNTY,ADDR_RULE_COUNTY_WEIGHT);
    
    insertHashMapNode(stpAddrRecogLib->stpRuleWeightMap,
                        ADDR_RULE_TOWN,ADDR_RULE_TOWN_WEIGHT);
                        
    insertHashMapNode(stpAddrRecogLib->stpRuleWeightMap,
                        ADDR_RULE_STREET,ADDR_RULE_STREET_WEIGHT);
                        
    insertHashMapNode(stpAddrRecogLib->stpRuleWeightMap,
                        ADDR_RULE_STREETNO,ADDR_RULE_STREETNO_WEIGHT);
    
    insertHashMapNode(stpAddrRecogLib->stpRuleWeightMap,
                        ADDR_RULE_ABA,ADDR_RULE_ABA_WEIGHT);
                        
    insertHashMapNode(stpAddrRecogLib->stpRuleWeightMap,
                        ADDR_RULE_FEATWORD,ADDR_RULE_FEATWORD_WEIGHT);
                        
        //addr suffix word
    insertHashMapNode(stpAddrRecogLib->stpRuleWeightMap,
                        ADDR_RULE_SUFFIXWORD,ADDR_RULE_SUFFIXWORD_WEIGHT);
    
    //negative rule
    insertHashMapNode(stpAddrRecogLib->stpRuleWeightMap,
                        ADDR_RULE_CMPSUFFIX,ADDR_RULE_CMPSUFFIX_WEIGHT);
        
    return;
}

/**
 * @desc: create company suffix hashset
 */
//static void ADDR_RECOG_createCmpSuffixSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib)
//{
//    ABU_ASSERT(stpAddrRecogLib != NULL);
//    
//    stpAddrRecogLib->stpCmpSuffixSet = createHashSet(CMP_SUFFIX_COUNT);
//    if(stpAddrRecogLib->stpCmpSuffixSet == NULL) return;
//    
//    int i = 0;
//    for( i = 0;i < CMP_SUFFIX_COUNT;i++)
//    {
//        insertHashSetNode(stpAddrRecogLib->stpCmpSuffixSet,CMP_SUFFIX[ i ]);
//    }
//}
/**
 * @desc: create addr suffix word hashset
 */
static void ADDR_RECOG_createAddrSuffixSet(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib)
{
    ABU_ASSERT(stpAddrRecogLib != NULL);
    
    stpAddrRecogLib->stpSuffixSet = createHashSet(ADDR_SUFFIX_COUNT);
    if(stpAddrRecogLib->stpSuffixSet == NULL) return;
    
    int i = 0;
    for( i = 0;i < ADDR_SUFFIX_COUNT;i++)
    {
        insertHashSetNode(stpAddrRecogLib->stpSuffixSet,ADDR_SUFFIX[ i ]);
    }
}

/**
 * @desc: init ABU_ADDR_RECOG_S
 */
static ABU_ADDR_RECOG_S *ADDR_RECOG_init(uint32_t initSize)
{
    ABU_ASSERT(initSize != 0);
    
    ABU_ADDR_RECOG_S *stpAddrRecog = (ABU_ADDR_RECOG_S *)ab_calloc(1,sizeof(ABU_ADDR_RECOG_S));
    
    stpAddrRecog->stpAddrArray = (ABU_ADDR_S**)ab_calloc(initSize,sizeof(ABU_ADDR_S));
    
    stpAddrRecog->uiAddrCount = 0;
    
    stpAddrRecog->uiAddrPoolSize = initSize;
    
    return stpAddrRecog;
}
/**
 * @desc:addr recognise by separator
 */
static void ADDR_RECOG_parseBySepa(const ABU_ADDR_RECOG_LIB_S  *stpAddrRecogLib,
                                   const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                                   ABU_ADDR_RECOG_S *stpAddrRecog,
                                   int *piHasCover,
                                   const char *szSource,
                                   uint32_t *mapping)
{
    ABU_ASSERT(stpAddrRecog != NULL && stpAddrRecogLib != NULL);
    ABU_ASSERT(stpSegResult != NULL && piHasCover != NULL);
    ABU_ASSERT(szSource != NULL && mapping != NULL);
   
    ADDR_RECOG_LOG_S addrRecogLog;
    memset(&addrRecogLog,0,sizeof(ADDR_RECOG_LOG_S));
 
    int preTargetPos = (stpSegResult->pWordArray + *piHasCover)->dwOffset;
    //step1: get addr
    while(*piHasCover < stpSegResult->dwWordNumber)
    {
        ABU_WORDS_WORD_S *stpWord = stpSegResult->pWordArray + *piHasCover;
        
        //match addr prefix word
        if(searchHashSetNode(stpAddrRecogLib->stpPrefixSet,stpWord->szWord) == 0)
        {
            return;
        }
        
        //self is addr separator
        if(searchHashSetNode(stpAddrRecogLib->stpSeparSet,stpWord->szWord) == 0)
        {
            *piHasCover += 1;
            //continue;
            break;
        }
        
        *piHasCover += 1;
        
        addrRecogLog.wordSpan += 1;
        
        if(stpWord->isNextNeSepa == TRUE) 
            break;
    }
    
    int postTargetPos = 0;
    if(*piHasCover == 0)
        postTargetPos = (stpSegResult->pWordArray + *piHasCover)->dwOffset;
    else
        postTargetPos = (stpSegResult->pWordArray + *piHasCover - 1)->dwOffset + 
                         strlen((stpSegResult->pWordArray + *piHasCover -1)->szWord) - 1;
    if(preTargetPos == postTargetPos)
        return;

    int sourceLen = strlen(szSource);
    char szAddr[sourceLen + 1];
    memset(szAddr,0,sizeof(szAddr));
    int preSourcePos = mapping[preTargetPos];
    int postSourcePos = mapping[postTargetPos];
    int partSourceLen = postSourcePos - preSourcePos;
    if(szSource[postSourcePos] == -93 ||
            ((postSourcePos + 1) < sourceLen && 
             szSource[postSourcePos] == -95 && szSource[postSourcePos + 1] == -95))
    {
        partSourceLen += GB_LEN;
    }
    else
    {
        partSourceLen += ASCII_LEN;
    }
    strncpy(szAddr, szSource + preSourcePos, partSourceLen); 

    if(szAddr[0] == STRING_END) 
        return;
         
     //step2: filter noise
    if(ADDR_RECOG_isEndWithCmpSuffix(szAddr,stpAddrRecogLib) == TRUE)
        return;
    if(abstru_hasgb(szAddr) == FALSE)
        return;   

    //step3:parse addr
    ABU_ADDR_S *stpAddr = ABU_ADDR_parse(stpAddrRecogLib->stpAddrLib,szAddr);
    if(stpAddr== NULL) return;             
                                       
    //step4:get sum weight
    ADDR_RECOG_parseBySepa_sumWeight(stpAddrRecogLib,stpAddr->stpAddrRecord,&addrRecogLog);
    stpAddr->iCredWeight = MIN(addrRecogLog.sumWeight, MAX_ADDR_CREDIT_WEIGHT);
    
    //step5:try to add this addr record 
    if(addrRecogLog.bExistNSPosTag == TRUE || addrRecogLog.bExistFeatWord == TRUE)
    {
        ADDR_RECOG_parse_add(stpAddr, stpAddrRecog);
    }
    else
    {
        ABU_ADDR_destroy(stpAddr);
    }
}
/**
 * @desc:addr recognise by separator: sum rule weight
 */
static void ADDR_RECOG_parseBySepa_sumWeight(const ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib,
                                            const ABU_ADDR_RECORD_S *stpAddrRecord,
                                            ADDR_RECOG_LOG_S *stpAddrRecogLog)
{
    ABU_ASSERT(stpAddrRecord != NULL && stpAddrRecogLog != NULL);
    
    //if exist province
    if(stpAddrRecord->szProvince != NULL)
    {
        stpAddrRecogLog->sumWeight += atoi(getValueHashMap(stpAddrRecogLib->stpRuleWeightMap,
                                        ADDR_RULE_PROVINCE));
        stpAddrRecogLog->bExistNSPosTag = TRUE;
    }
    //if exist city
    if(stpAddrRecord->szCity != NULL)
    {
        stpAddrRecogLog->sumWeight += atoi(getValueHashMap(stpAddrRecogLib->stpRuleWeightMap,
                                        ADDR_RULE_CITY));
        stpAddrRecogLog->bExistNSPosTag = TRUE;
    }
    //if exist county
    if(stpAddrRecord->szCounty != NULL)
    {
        stpAddrRecogLog->sumWeight += atoi(getValueHashMap(stpAddrRecogLib->stpRuleWeightMap,
                                        ADDR_RULE_COUNTY));
        stpAddrRecogLog->bExistNSPosTag = TRUE;    
    }
    //if exist town
    if(stpAddrRecord->szTown != NULL)
    {
        stpAddrRecogLog->sumWeight += atoi(getValueHashMap(stpAddrRecogLib->stpRuleWeightMap,
                                        ADDR_RULE_TOWN));
        stpAddrRecogLog->bExistFeatWord = TRUE;
    }
    //if exist street
    if(stpAddrRecord->szStreet != NULL)
    {
        stpAddrRecogLog->sumWeight += atoi(getValueHashMap(stpAddrRecogLib->stpRuleWeightMap,
                                        ADDR_RULE_STREET));
        stpAddrRecogLog->bExistFeatWord = TRUE;
    }
    //if exist streetNO
    if(stpAddrRecord->szStreetNO != NULL)
    {
        stpAddrRecogLog->sumWeight += atoi(getValueHashMap(stpAddrRecogLib->stpRuleWeightMap,
                                        ADDR_RULE_STREETNO));
        stpAddrRecogLog->bExistFeatWord = TRUE;
    }
    //if exist addr left
    if(stpAddrRecord->szAddrLeft != NULL)
    {
        int i =0;
        for(i = 0;i < ADDR_FEAT_WORD_COUNT;i++)
        {
            if(strstr(stpAddrRecord->szAddrLeft,ADDR_FEAT_WORD[ i ]) != NULL)
            {
                stpAddrRecogLog->sumWeight += atoi(getValueHashMap(stpAddrRecogLib->stpRuleWeightMap,
                                            ADDR_RULE_FEATWORD));
                stpAddrRecogLog->bExistFeatWord = TRUE;
            }                        
        }
    }
    
    //alter sumWeight
    if(ADDR_RECOG_isPCC(stpAddrRecord))
    {
        stpAddrRecogLog->sumWeight = MAX_ADDR_CREDIT_WEIGHT;
        stpAddrRecogLog->bExistNSPosTag = TRUE;
    }
}

/**
 * @desc: addr recognisation by separator: add addr parse result
 */
static void ADDR_RECOG_parse_add(const ABU_ADDR_S *stpAddr,
                                 ABU_ADDR_RECOG_S *stpAddrRecog)
{
    ABU_ASSERT(stpAddr != NULL && stpAddrRecog != NULL);
    
    //need realloc memory
    if((stpAddrRecog->uiAddrCount + 1) > stpAddrRecog->uiAddrPoolSize)
    {
        stpAddrRecog->stpAddrArray = (ABU_ADDR_S**)ab_realloc(stpAddrRecog->stpAddrArray,
                            2 * stpAddrRecog->uiAddrPoolSize * sizeof(ABU_ADDR_S));
        
        stpAddrRecog->uiAddrPoolSize *= 2;
        for(int i = stpAddrRecog->uiAddrCount;i < stpAddrRecog->uiAddrPoolSize;i++)
            stpAddrRecog->stpAddrArray[i] = NULL;
    }
   
    stpAddrRecog->stpAddrArray[stpAddrRecog->uiAddrCount] = (ABU_ADDR_S *)stpAddr; 
     
    stpAddrRecog->uiAddrCount += 1; 
}
/**
 * @desc: addr recognise by structural information(prefix word)
 */
static void ADDR_RECOG_parseByPrefix(const ABU_ADDR_RECOG_LIB_S  *stpRecogLib,
                            const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                            ABU_ADDR_RECOG_S *stpAddrRecog,
                            int *piHasCover,
                            const char *szSource,
                            uint32_t *mapping)
{
    ABU_ASSERT(stpRecogLib != NULL && stpSegResult != NULL);
    ABU_ASSERT(stpAddrRecog != NULL && piHasCover != NULL);
    ABU_ASSERT(szSource != NULL && mapping != NULL);
    
    if(*piHasCover >= stpSegResult->dwWordNumber) 
        return;
    
    ADDR_RECOG_LOG_S addrRecogLog;
    memset(&addrRecogLog,0,sizeof(ADDR_RECOG_LOG_S));

    ABU_WORDS_WORD_S *stpWord = stpSegResult->pWordArray + *piHasCover;
    
    //step1: try to  match addr prefix pattern    
    if(searchHashSetNode(stpRecogLib->stpPrefixSet,stpWord->szWord) != 0) 
        return;
    
    addrRecogLog.bExistPrefix = TRUE;
    *piHasCover += 1;
    

    //step2: try to match addr prefix flag pattern
    while(1)
    {
        if(*piHasCover >= stpSegResult->dwWordNumber) return;
            
        stpWord = stpSegResult->pWordArray + *piHasCover;
        
        if(searchHashSetNode(stpRecogLib->stpPrefixFlagSet,stpWord->szWord) != 0 &&
            strcmp(stpWord->szWordPosTag,POS_W) != 0)
        {
            break;
        }
    
        *piHasCover += 1;

        addrRecogLog.bExistPrefixFlag = TRUE;
    }
    if(addrRecogLog.bExistPrefixFlag == TRUE) 
        return;
    
    if(*piHasCover >= stpSegResult->dwWordNumber) 
        return;

    //step3: try to get szAddr
    int preTargetPos = (stpSegResult->pWordArray + *piHasCover)->dwOffset;
    while(*piHasCover < stpSegResult->dwWordNumber)
    {
        stpWord = stpSegResult->pWordArray + *piHasCover;
 
        *piHasCover += 1;
       
        if(stpWord->isNextNeSepa == TRUE) break;
    }
    int postTargetPos = 0;
    if(*piHasCover == 0)
        postTargetPos = (stpSegResult->pWordArray + *piHasCover)->dwOffset;
    else
        postTargetPos = (stpSegResult->pWordArray + *piHasCover - 1)->dwOffset + 
                        strlen((stpSegResult->pWordArray + *piHasCover -1)->szWord) - 1;
    if(preTargetPos == postTargetPos)
        return;

    int sourceLen = strlen(szSource);
    char szAddr[sourceLen + 1];
    memset(szAddr,0,sizeof(szAddr));
    int preSourcePos = mapping[preTargetPos];
    int postSourcePos = mapping[postTargetPos];
    int partSourceLen = postSourcePos - preSourcePos;
    if(szSource[postSourcePos] == -93 ||
            ((postSourcePos + 1) < sourceLen && 
             szSource[postSourcePos] == -95 && szSource[postSourcePos + 1] == -95))
    {
        partSourceLen += GB_LEN;
    }
    else
    {
        partSourceLen += ASCII_LEN;
    }
    strncpy(szAddr, szSource + preSourcePos, partSourceLen); 
    //step4: parse addr
    if(szAddr[0] == STRING_END)
        return;
    ABU_ADDR_S *stpAddr = ABU_ADDR_parse(stpRecogLib->stpAddrLib,szAddr);
    if(stpAddr == NULL)
        return;
    stpAddr->iCredWeight = MAX_ADDR_CREDIT_WEIGHT;
    
    //step5: try to add this addr record
    ADDR_RECOG_parse_add(stpAddr, stpAddrRecog);
    
}


/**
 * @desc judge the addr end with cmp suffix 
 */
static BOOL ADDR_RECOG_isEndWithCmpSuffix(const char *szAddr,
                const ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib)
{
    ABU_ASSERT(szAddr != NULL && stpAddrRecogLib != NULL);
    
    //step1: get seg result
    ABU_WORDS_SEGRESULT_SL_S *stpSegResult = 
        ABU_WORDS_SEG_SL_getSegResultFromBuff(szAddr,strlen(szAddr),
        stpAddrRecogLib->stpCmpSuffixDict,stpAddrRecogLib->stpAddrLib->stpSegLib);
    if(stpSegResult == NULL) 
    {
        return FALSE;
    }
    
    //step2: judge is endwith cmp suffix 
    ABU_WORDS_WORD_S *stpLastWord = stpSegResult->pWordArray + 
                                        stpSegResult->dwWordNumber - 1;
    if(strcmp(stpLastWord->szWordPosTag,CMP_SUFFIX_POSTAG) == 0)
    {
        ABU_WORDS_SEG_SL_destroySegResult(stpSegResult);
        return TRUE;
    }
    for(int i = 0;i < CMP_SUFFIX_HANZI_COUNT;i++)
    {
        if(STRU_endWith(stpLastWord->szWord,CMP_SUFFIX_HANZI[ i ]))
        {
            ABU_WORDS_SEG_SL_destroySegResult(stpSegResult);
            return TRUE;
        }
    }
    
    //step3: free memory
    ABU_WORDS_SEG_SL_destroySegResult(stpSegResult);
    return FALSE;
}

/**
 * @desc stpAddrRecord is province || city || county
 */
static BOOL ADDR_RECOG_isPCC(const ABU_ADDR_RECORD_S *stpAddrRecord)
{
    ABU_ASSERT(stpAddrRecord != NULL);
    if(stpAddrRecord->szTown != NULL || stpAddrRecord->szStreet != NULL ||
       stpAddrRecord->szStreetNO != NULL || stpAddrRecord->szAddrLeft != NULL)
    {
        return FALSE;
    }
    
    if(stpAddrRecord->szProvince != NULL || stpAddrRecord->szCity != NULL ||
       stpAddrRecord->szCounty != NULL)
    {
        return TRUE;
    }
    
    return FALSE;
}
