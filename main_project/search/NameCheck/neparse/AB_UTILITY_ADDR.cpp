/**
 *@file AB_UTIILTY_ADDR.c
 *@author bfzhang
 *@brief implement the public interface for parsing addr
 * create the file in 2007-07-02
 */
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

#include "abutility.h"
#include "HASHMAP.h"
#include "HASHSET.h"
#include "abstring.h"
#include "number.h"
#include "STRING_UTILITY.h"
#include "AB_UTILITY_ADDR.h"
#include "AB_UTILITY_WORDS.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "AB_UTILITY_WORDS_SEG_SL.h"
#include "AB_UTILITY_WORDS_UTILITY.h"
#include "AB_UTILITY_MAPTREE.h"
#include "AB_UTILITY_ADDR.h"
#include "AB_UTILITY_COMMON.h"
#include "AB_UTILITY_ADDR_RESOURCE.h"
#include "AB_UTILITY_MAPTREE.h"

#ifndef GB_LEN
#define GB_LEN 2
#endif

#define ADDR_LEN_MAX 256

static const int REMOVE_ENG_PUNCT_COUNT  = 25;
static const char *REMOVE_ENG_PUNCT[] = 
{
    "!",  "\"",  "#",  "$",  "%",
    "&",  "\\",  "'",  "*",  "+",
    ",",  ".",   "/",  ":",  ";",
    "<",  "=",   ">",  "?",  "@",
    "^",  "`",   "|",  "~",  "·"
};

static const int REMOVE_GB_PUNCT_COUNT = 13; 
static const char *REMOVE_GB_PUNCT[] = 
{
    "！",  "，",  "。",  "？",  "：",
    "；",  "＃",  "％",  "＆",  "＠",
    "￥",  "＊",  "＝"    
};

/*******************************************/
/*****privace data type declaration*********/
/*******************************************/
typedef struct _ADDR_RECOG_LOG_S
{
    char szProvince[32];
    int provinceLen;
    
    char szCity[32];
    int cityLen; 
    
    char szCounty[32];
    int countyLen;
    
    char szTown[ADDR_LEN_MAX];
    int townLen;
    
    char szStreet[ADDR_LEN_MAX];
    int streetLen;
    
    char szStreetNO[ADDR_LEN_MAX];
    int streetNOLen;
    
    char szAddrLeft[ADDR_LEN_MAX];
    int addrLeftLen;
}ADDR_RECOG_LOG_S;

/**********************************************/
/*****private interface declaration************/
/**********************************************/
static ABU_WORDS_DICT_S *createSegDict(const char **szDictArray,
                                       const uint32_t dictNumber);

static HashSet *createFeatWordSet();

static void ADDR_destroyLibContent(ABU_ADDR_LIB_S *stpAddrLib);

static void ADDR_preprocess(abstring * addrString);

static void ADDR_preprocess_replace(abstring *addrString);

static void ADDR_preprocess_removePunct(abstring *addrString);

static void ADDR_preprocess_cnum2num(abstring *addrString);

/**
 *@brief (stpSegResult->pWordArray + wordCovered)->szWord is PCC
 *@retval isPCC return TRUE else return FALSE
 */
static unsigned char ADDR_checkPCCNextFeat(
                        const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                        const int wordCovered);

static void ADDR_setProvince(ABU_ADDR_RECORD_S *stpAddrRecord,
                        const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                        const ABU_ADDR_LIB_S *stpAddrLib,
                        int *piWordCovered);

static void ADDR_trimProvince(ABU_ADDR_RECORD_S *stpAddrRecord,
                        const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                        const ABU_ADDR_LIB_S *stpAddrLib,
                        int *piWordCovered);

static void ADDR_setCity(ABU_ADDR_RECORD_S *stpAddrRecord,
                const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                const ABU_ADDR_LIB_S  *stpAddrLib,
                int *piWordCovered,
                const char *szCity);

static void ADDR_trimCity(ABU_ADDR_RECORD_S *stpAddrRecord,
                const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                const ABU_ADDR_LIB_S  *stpAddrLib,
                int *piWordCovered);

static void ADDR_setCounty(ABU_ADDR_RECORD_S *stpAddrRecord,
                    const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                    const ABU_ADDR_LIB_S *stpAddrLib, 
                    int *piWordCovered,
                    const char *szCity);

static void ADDR_trimCounty(ABU_ADDR_RECORD_S *stpAddrRecord,
                    const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                    const ABU_ADDR_LIB_S *stpAddrLib, 
                    int *piWordCovered);

static void ADDR_setBriefAddr(ABU_ADDR_RECORD_S *stpAddrRecord,
                    const char *szBriefAddr);

static void ADDR_setTown(ABU_ADDR_RECORD_S *stpAddrRecord,
                 const char *szAddr,
                 int *pStrLengthCovered);

static void ADDR_setStreetAndNumber(ABU_ADDR_RECORD_S *stpAddrRecord,
                            const char *szAddr,
                            int *pStrLengthCovered);

static void ADDR_setAddrLeft(ABU_ADDR_RECORD_S *stpAddrRecord,
                        const char *szAddr,
                        int *pStrLengthCovered);

static void ADDR_setDetailLevel(ABU_ADDR_RECORD_S *stpAddrRecord);

static void ADDR_postprocess(ABU_ADDR_RECORD_S *stpAddrRecord);

static void ADDR_checkRecord(ABU_ADDR_RECORD_S *stpAddrRecord);

static BOOL ADDR_sameProvince(const char *szProvinceLeft,
                              const char *szProvinceRight);

static BOOL ADDR_sameCity(const char *szCityLeft,
                          const char *szCityRight);

static BOOL ADDR_sameCounty(const char *szCountyLeft,
                            const char *szCountyRight);

static void ADDR_destroyRecordContent(ABU_ADDR_RECORD_S *stpAddrRecord);

static void ADDR_destroyContent(ABU_ADDR_S *stpAddr);

static char *ADDR_getFormatAddr(const ABU_ADDR_RECORD_S *stpAddrRecord,
                                const ABU_ADDR_LIB_S *stpAddrLib);

static ABU_ADDR_ATTACH_S * ADDR_getAttach(ABU_ADDR_S *stpAddr);

static void ADDR_destroyAttachContent(ABU_ADDR_ATTACH_S *stpAddrAttach);

static void ADDR_destroyAttach(ABU_ADDR_ATTACH_S *stpAddrAttach);

static unsigned char ADDR_endsWithProvinceFeat(const char *szAddr);

static unsigned char ADDR_endsWithCityFeat(const char *szAddr);

static unsigned char ADDR_endsWithCountyFeat(const char *szAddr);

static unsigned char ADDR_isAddrFeat(const char *szGBWord);

static unsigned char ADDR_isDirectFeat(const char *szGBWord);

static unsigned char ADDR_isProvinceFeat(const char *szGBWord);

static unsigned char ADDR_isCityFeat(const char *szGBWord);

static unsigned char ADDR_isCountyFeat(const char *szGBWord);

static unsigned int PCC_get4Char(const char * szField);

static void getWordAndPosTag(const char *szBuff,
                             char *szWord,char *szPosTag);
static void catField(char **szDst,const char *szSrc,const int srcLen);

static char *ADDR_minMatchNextTownFeat(const char *szAddr);

static char *ADDR_minMatchNextStreetFeat(const char *szAddr);

static char *ADDR_minMatchNextStreetNOFeat(const char *szAddr);

static uint16_t ADDR_getMinMatchNextTownFeatLen(const char *szAddr);

static uint16_t ADDR_getMinMatchNextStreetFeatLen(const char *szAddr);
/*********************************************************/
/***public interface implementation***********************/
/*********************************************************/
ABU_ADDR_LIB_S *ABU_ADDR_createLib(const char **szDictArray,
                                        const uint32_t uiDictCount)
{/*{{{*/
    ABU_ADDR_LIB_S *stpAddrLib = (ABU_ADDR_LIB_S *)ab_calloc(1,sizeof(ABU_ADDR_LIB_S));
       
    //step2: create big dict
    stpAddrLib->stpSegDict = createSegDict(szDictArray,uiDictCount);
    
    //step3:create seg lib
    stpAddrLib->stpSegLib = ABU_WORDS_SEG_createSegLib();
    
    //step4:create mapTree
    stpAddrLib->stpMapTree = ABU_MAPTREE_create();
    
    //step5: create addr feature word set
    stpAddrLib->stpFeatWordSet = createFeatWordSet();
    
    return stpAddrLib;    
}/*}}}*/


void ABU_ADDR_destroyLib(ABU_ADDR_LIB_S *stpAddrLib)                                        
{/*{{{*/
    if(stpAddrLib == NULL) return;

    ADDR_destroyLibContent(stpAddrLib);
    
    ABU_FREE(stpAddrLib);        
}/*}}}*/


ABU_ADDR_RECORD_S  *ABU_ADDR_parseRecord(const ABU_ADDR_LIB_S *stpAddrLib,
                                         const char *szAddr)
{/*{{{*/
    return ABU_ADDR_parseRecord_bycity(stpAddrLib, szAddr, NULL);
}/*}}}*/


ABU_ADDR_RECORD_S  *ABU_ADDR_parseRecord_bycity(const ABU_ADDR_LIB_S *stpAddrLib,
                                                const char *szAddr,
                                                const char *szCity)
{/*{{{*/
    ABU_ASSERT(stpAddrLib != NULL && szAddr != NULL);
 
    int addrLen = strlen(szAddr);   
    if(addrLen == 0 || addrLen > ADDR_LEN_MAX)
    {
#ifdef AB_DEBUG
        printf("<<<<<debug log(ABU_ADDR_parseRecord_bycity)\n");
        printf("addrLen == 0 || addrLen > 128\n");
        printf(">>>>>debug log(ABU_ADDR_parseRecord_bycity)\n");
#endif
        return NULL;
    }

#ifdef AB_DEBUG
    printf("<<<<<debug log(ABU_ADDR_parseRecord_bycity)\n");
    printf("origin:%s ", szAddr);
    if(szCity != NULL && szCity[0] != 0)
        printf("%s", szCity);
    printf("\n");
#endif

    char *szGivenCity = (char *)szCity;
    if(szGivenCity != NULL && szGivenCity[0] == 0)
        szGivenCity = NULL;

    //step0:preprocess
    abstring *addrString = abstring_new(szAddr);
    ADDR_preprocess(addrString);
#ifdef AB_DEBUG
    printf("preprocess:%s\n", addrString->str);
#endif

    char szAddrCopy[addrString->len + 1];
    memset(szAddrCopy,0,sizeof(szAddrCopy));
    strncpy(szAddrCopy,addrString->str,addrString->len);
    abstring_free(addrString,TRUE);
    
    ABU_ADDR_RECORD_S *stpAddrRecord = (ABU_ADDR_RECORD_S *)ab_calloc(1,sizeof(ABU_ADDR_RECORD_S));
    ABU_WORDS_SEGRESULT_SL_S *stpSegResult = NULL;
    
    addrLen = strlen(szAddrCopy);
    stpSegResult = ABU_WORDS_SEG_SL_getSegResultFromBuff
                        (szAddrCopy,addrLen,stpAddrLib->stpSegDict,stpAddrLib->stpSegLib);
    if(stpSegResult == NULL)
    {
        ABU_WORDS_SEG_SL_destroySegResult(stpSegResult);
        stpSegResult = NULL;
        ABU_ADDR_destroyRecord(stpAddrRecord);
        stpAddrRecord = NULL;
#ifdef AB_DEBUG
        printf(">>>>>debug log(ABU_ADDR_parseRecord_bycity)\n");
#endif
        return NULL;
    }
#ifdef AB_DEBUG
    printf("seg:");
    for(int i = 0;i < stpSegResult->dwWordNumber;i++)
    {
        char *szWord = (stpSegResult->pWordArray + i)->szWord;
        printf("%s-", szWord);
    }
    printf("\n");
#endif 
    
    int wordUnitCovered = 0;
    int strLengthCovered = 0;
    
    //step0: set province city county
    //while(1)
    //{
    //    int preWordUnitCovered = wordUnitCovered;

        ADDR_setProvince(stpAddrRecord,stpSegResult,stpAddrLib,&wordUnitCovered);
    
        ADDR_trimProvince(stpAddrRecord,stpSegResult,stpAddrLib,&wordUnitCovered);
    
        ADDR_setCity(stpAddrRecord,stpSegResult,stpAddrLib,
                     &wordUnitCovered, szGivenCity);
    
        ADDR_trimCity(stpAddrRecord,stpSegResult,stpAddrLib,&wordUnitCovered);
    
        ADDR_setCounty(stpAddrRecord,stpSegResult,stpAddrLib,
                       &wordUnitCovered, szGivenCity);
    
        ADDR_trimCounty(stpAddrRecord,stpSegResult,stpAddrLib,&wordUnitCovered);

   //     if(preWordUnitCovered == wordUnitCovered)
   //         break;
   // }
#ifdef AB_DEBUG
    printf("step0:set province city county\n");
    if(stpAddrRecord->szProvince != NULL)
        printf("    province:%s\n", stpAddrRecord->szProvince);
    if(stpAddrRecord->szCity != NULL)
        printf("    city:%s\n", stpAddrRecord->szCity);
    if(stpAddrRecord->szCounty != NULL)
        printf("    county:%s\n", stpAddrRecord->szCounty); 
#endif

    for(int i= 0;i < wordUnitCovered;i++)
        strLengthCovered += strlen( (stpSegResult->pWordArray + i)->szWord);
            
    //step1: set brief addr
    ADDR_setBriefAddr(stpAddrRecord,szAddrCopy + strLengthCovered);
#ifdef AB_DEBUG
    printf("step1:set brief addr\n");
    if(stpAddrRecord->szBriefAddr != NULL)
        printf("    briefAddr:%s\n", stpAddrRecord->szBriefAddr);
#endif
    
    //step2: set town
    ADDR_setTown(stpAddrRecord,szAddrCopy,&strLengthCovered);
#ifdef AB_DEBUG
    printf("step2:set town\n");
    if(stpAddrRecord->szTown != NULL)
        printf("    town:%s\n", stpAddrRecord->szTown); 
#endif
    
    //step3: set street and streetno
    ADDR_setStreetAndNumber(stpAddrRecord,szAddrCopy,&strLengthCovered);
#ifdef AB_DEBUG
    printf("step3:set street and streetno\n");
    if(stpAddrRecord->szStreet != NULL)
        printf("    street:%s\n", stpAddrRecord->szStreet);
    if(stpAddrRecord->szStreetNO != NULL)
    {
        printf("    streetno:%s\n", stpAddrRecord->szStreetNO);
        printf("    streetno(int):%d\n", stpAddrRecord->streetNO); 
    }
#endif
    
    //step4: set addr left
    ADDR_setAddrLeft(stpAddrRecord,szAddrCopy,&strLengthCovered);
#ifdef AB_DEBUG
    printf("step4:set addr left\n");
    if(stpAddrRecord->szAddrLeft != NULL)
        printf("    addrleft:%s\n", stpAddrRecord->szAddrLeft);    
#endif
    
    //step5:set addr detail level
    ADDR_setDetailLevel(stpAddrRecord);
#ifdef AB_DEBUG
    printf("step5:set detail level\n");
#endif

    //step6:addr postprocess
    ADDR_postprocess(stpAddrRecord);
#ifdef AB_DEBUG
    printf("step6:addr postprocess\n");
#endif

    //step7:check addr parse record
    //ADDR_checkRecord(stpAddrRecord);
#ifdef AB_DEBUG
    printf("step7:check addr parse record\n");
#endif
    
    //free memory
    ABU_WORDS_SEG_SL_destroySegResult(stpSegResult);
    stpSegResult = NULL;

#ifdef AB_DEBUG
    ABU_ADDR_printRecord(stpAddrRecord, stdout);
    printf(">>>>>debug log(ABU_ADDR_parseRecord_bycity)\n");
#endif
    
    return stpAddrRecord;

}/*}}}*/


void ABU_ADDR_copyRecord(ABU_ADDR_RECORD_S *stpDst,
                            const ABU_ADDR_RECORD_S *stpSrc)
{/*{{{*/
    ABU_ASSERT(stpDst != NULL && stpSrc != NULL);
    
    if(stpSrc->szProvince != NULL)
        stpDst->szProvince = abstru_strdup(stpSrc->szProvince);
   
    if(stpSrc->szCity != NULL)
        stpDst->szCity = abstru_strdup(stpSrc->szCity);
    
    if(stpSrc->szCounty != NULL)
        stpDst->szCounty = abstru_strdup(stpSrc->szCounty);
    
    if(stpSrc->szTown != NULL)
        stpDst->szTown = abstru_strdup(stpSrc->szTown);
    
    if(stpSrc->szStreet != NULL)
        stpDst->szStreet = abstru_strdup(stpSrc->szStreet);
    
    if(stpSrc->szStreetNO != NULL)
        stpDst->szStreetNO = abstru_strdup(stpSrc->szStreetNO);
    
    if(stpSrc->szAddrLeft != NULL)
        stpDst->szAddrLeft = abstru_strdup(stpSrc->szAddrLeft);
    
    if(stpSrc->szBriefAddr != NULL)
        stpDst->szBriefAddr = abstru_strdup(stpSrc->szBriefAddr);
    
    //detail level
    stpDst->detailLevel = stpSrc->detailLevel;
}/*}}}*/


int ABU_ADDR_joinRecord(const ABU_ADDR_RECORD_S *stpRecord,
                        char *szDst,
                        const unsigned int uiLen)
{/*{{{*/
    ABU_ASSERT(stpRecord != NULL && szDst != NULL);
    
    int totalLen = 0;
    
    //step1:join province
    if(stpRecord->szProvince != NULL)
    {
        int provinceLen = strlen(stpRecord->szProvince);
        strncpy(szDst + totalLen,stpRecord->szProvince,provinceLen);
        totalLen += provinceLen;
    }

    //step2:join city
    if(stpRecord->szCity != NULL)
    {
        if(stpRecord->szProvince == NULL ||
           isdgc(stpRecord->szProvince) == FALSE)
        {
            int cityLen = strlen(stpRecord->szCity);
            strncpy(szDst + totalLen,stpRecord->szCity,cityLen);
            totalLen += cityLen;
        }
    }
    
    //step3:join county
    if(stpRecord->szCounty != NULL)
    {
        int countyLen = strlen(stpRecord->szCounty);
        strncpy(szDst + totalLen,stpRecord->szCounty,countyLen);
        totalLen += countyLen;
    }
    
    //step4:join town
    if(stpRecord->szTown != NULL)
    {
        int townLen = strlen(stpRecord->szTown);
        strncpy(szDst + totalLen,stpRecord->szTown,townLen);
        totalLen += townLen;
    }

    //step5:join street
    if(stpRecord->szStreet != NULL)
    {
        int streetLen = strlen(stpRecord->szStreet);
        strncpy(szDst + totalLen,stpRecord->szStreet,streetLen);
        totalLen += streetLen;
    }
    
    //step6:join streetNO
    if(stpRecord->szStreetNO != NULL)
    {
        int streetNOLen = strlen(stpRecord->szStreetNO);
        strncpy(szDst + totalLen,stpRecord->szStreetNO,streetNOLen);
        totalLen += streetNOLen;
    }
    
    //step7:join addrLeft
    if(stpRecord->szAddrLeft != NULL)
    {
        int addrLeftLen = strlen(stpRecord->szAddrLeft);
        strncpy(szDst + totalLen,stpRecord->szAddrLeft,addrLeftLen);
        totalLen += addrLeftLen;
    }

    return (totalLen <= uiLen)?0:-1;
}/*}}}*/


void ABU_ADDR_destroyRecord(ABU_ADDR_RECORD_S *stpAddrRecord)
{/*{{{*/
    if(stpAddrRecord == NULL) return;
    
    ADDR_destroyRecordContent(stpAddrRecord);
        
    ABU_FREE(stpAddrRecord);
}/*}}}*/


ABU_ADDR_S *ABU_ADDR_parse(const ABU_ADDR_LIB_S *stpAddrLib,const char *szAddr)
{/*{{{*/
    return ABU_ADDR_parse_bycity(stpAddrLib,szAddr,NULL);  
}/*}}}*/


ABU_ADDR_S *ABU_ADDR_parse_bycity(const ABU_ADDR_LIB_S *stpAddrLib,
                           const char *szAddr,const char *szCity)    
{/*{{{*/
    ABU_ASSERT(stpAddrLib != NULL && szAddr != NULL);
    
    int addrLen = strlen(szAddr);
    if(addrLen == 0) return NULL;
    if(addrLen > ADDR_LEN_MAX) 
    {
        fprintf(stderr,
                "!!!can't parse this addr(len > %d bytes)\n",ADDR_LEN_MAX);
        fprintf(stderr,"\t%s\n",szAddr);
        return NULL;
    }    

    ABU_ADDR_S *stpAddr = (ABU_ADDR_S *)ab_calloc(1,sizeof(ABU_ADDR_S));

    //step1:set szOriginAddr
    stpAddr->szOriginAddr = abstru_strndup(szAddr, addrLen);

    //step2:parse addr record        
    stpAddr->stpAddrRecord = ABU_ADDR_parseRecord_bycity(stpAddrLib,szAddr,szCity);
    if(stpAddr->stpAddrRecord == NULL)
    {
        ABU_ADDR_destroy(stpAddr);
        return NULL;
    }
    
    //step3:parse addr attach
    stpAddr->stpAddrAttach = ADDR_getAttach(stpAddr);
    if(stpAddr->stpAddrAttach == NULL)
    {
        ABU_ADDR_destroy(stpAddr);
        return NULL;
    }

    //step4:set format addr
    stpAddr->szFormatAddr = ADDR_getFormatAddr(stpAddr->stpAddrRecord,stpAddrLib);

    //step5: how to set addr credit weight;
    stpAddr->iCredWeight = MAX_ADDR_CREDIT_WEIGHT;

    return stpAddr;
}/*}}}*/


void ABU_ADDR_destroy(void *stpAddr) {/*{{{*/
    if(stpAddr == NULL) return;
    ABU_ADDR_S *p = (ABU_ADDR_S *)stpAddr;
    ADDR_destroyContent(p);
    ABU_FREE(p);
}/*}}}*/

void ABU_ADDR_sign(const void *_p, char *p_sign_buf, size_t *p_buf_size) {/*{{{*/
    ABU_ADDR_S *p = (ABU_ADDR_S *)_p;
    size_t copy_buf_len = MIN(strlen(p->szOriginAddr), *p_buf_size);
    memcpy(p_sign_buf, p->szOriginAddr, copy_buf_len);
    *p_buf_size = copy_buf_len;
}/*}}}*/

void ABU_ADDR_printRecord(const ABU_ADDR_RECORD_S *stpAddrRecord,FILE *fp)
{/*{{{*/
    ABU_ASSERT(stpAddrRecord != NULL && fp != NULL);
    
    fprintf(fp,"ABU_ADDR_RECORD_S structrue:\n");
    
    if(stpAddrRecord->szProvince != NULL)
        fprintf(fp,"    szProvince:%s\n",stpAddrRecord->szProvince);
    
    if(stpAddrRecord->szCity != NULL)
        fprintf(fp,"    szCity:%s\n",stpAddrRecord->szCity);
        
    if(stpAddrRecord->szCounty != NULL)
        fprintf(fp,"    szCounty:%s\n",stpAddrRecord->szCounty);

    if(stpAddrRecord->szTown != NULL)
        fprintf(fp,"    szTown:%s\n",stpAddrRecord->szTown);
    
    if(stpAddrRecord->szStreet != NULL)
        fprintf(fp,"    szStreet:%s\n",stpAddrRecord->szStreet);
        
    if(stpAddrRecord->szStreetNO != NULL)
    {
        fprintf(fp,"    szStreetNo:%s\n",stpAddrRecord->szStreetNO);
        fprintf(fp,"    streetNO:%d\n", stpAddrRecord->streetNO);
    }
        
    if(stpAddrRecord->szAddrLeft != NULL)
        fprintf(fp,"    szAddrLeft:%s\n",stpAddrRecord->szAddrLeft);
        
    if(stpAddrRecord->szBriefAddr != NULL)
        fprintf(fp,"    szBriefAddr:%s\n",stpAddrRecord->szBriefAddr);
    
    fprintf(fp,"    addrDetailLevel:");
    switch(stpAddrRecord->detailLevel)
    {
        case PROVINCE_LEVEL:
            fprintf(fp,"province_level");break;
        case CITY_LEVEL:
            fprintf(fp,"city_level");break;
        case COUNTY_LEVEL:
            fprintf(fp,"county_level");break;
        case TOWN_LEVEL:
            fprintf(fp,"county_town");break;
        case STREET_LEVEL:
            fprintf(fp,"street_level");break;
        case STREETNO_LEVEL:
            fprintf(fp,"streetno_level");break;
        case UNKNOWN_LEVEL:
            fprintf(fp,"unknown_level");break;
    }

    fprintf(fp,"\n");
}/*}}}*/


void ABU_ADDR_printAttach(const ABU_ADDR_ATTACH_S *stpAddrAttach, FILE *fp)
{/*{{{*/
    ABU_ASSERT(stpAddrAttach != NULL && fp != NULL);
    fprintf(fp, "ABU_ADDR_ATTACH_S structure:\n");

    /*
    fprintf(fp, "    gbOriginAddrLen:%u gbOriginAddr:", stpAddrAttach->gbOriginAddrLen);
    for(uint16_t i = 0;i < stpAddrAttach->gbOriginAddrLen;i++)
    {
        fprintf(fp, "%u", stpAddrAttach->gbOriginAddr[i]);
        if(i != (stpAddrAttach->gbOriginAddrLen - 1))
        fprintf(fp, "-");
    }
    fprintf(fp,"\n");
    */
    
    fprintf(fp, "    province4Char:%u\n", stpAddrAttach->province4Char);
    fprintf(fp, "    city4Char:%u\n", stpAddrAttach->city4Char);
    fprintf(fp, "    county4Char:%u\n", stpAddrAttach->county4Char);

    fprintf(fp, "    gbTownLen:%u gbTown:", stpAddrAttach->gbTownLen);
    for(uint16_t i = 0;i < stpAddrAttach->gbTownLen;i++)
    {
        fprintf(fp, "%u", stpAddrAttach->gbTown[i]);
        if(i != (stpAddrAttach->gbTownLen - 1))
            fprintf(fp, "-");
    }
    fprintf(fp,"\n");

    fprintf(fp, "    gbStreetLen:%u gbStreet:", stpAddrAttach->gbStreetLen);
    for(uint16_t i = 0;i < stpAddrAttach->gbStreetLen;i++)
    {
        fprintf(fp, "%u", stpAddrAttach->gbStreet[i]);
        if(i != (stpAddrAttach->gbStreetLen - 1))
            fprintf(fp, "-");
    }
    fprintf(fp,"\n");

    fprintf(fp, "    gbAddrLeftLen:%u gbAddrLeft:", stpAddrAttach->gbAddrLeftLen);
    for(uint16_t i = 0;i < stpAddrAttach->gbAddrLeftLen;i++)
    {
        fprintf(fp, "%u", stpAddrAttach->gbAddrLeft[i]);
        if(i != (stpAddrAttach->gbAddrLeftLen - 1))
            fprintf(fp, "-");
    }
    fprintf(fp,"\n");

    /*
    fprintf(fp, "    gbBriefAddrLen:%u gbBriefAddr:", stpAddrAttach->gbBriefAddrLen);
    for(uint16_t i = 0;i < stpAddrAttach->gbBriefAddrLen;i++)
    {
        fprintf(fp, "%u", stpAddrAttach->gbBriefAddr[i]);
        if(i != (stpAddrAttach->gbBriefAddrLen - 1))
            fprintf(fp, "-");
    }
    fprintf(fp,"\n");
    */
}/*}}}*/


void ABU_ADDR_print(const ABU_ADDR_S *stpAddr,FILE *fp)
{/*{{{*/
    ABU_ASSERT(stpAddr != NULL);
    
    fprintf(fp,"ABU_ADDR_S structure:\n");    
    fprintf(fp,"szOriginAddr: %s\n",stpAddr->szOriginAddr);

    fprintf(fp,"szFormatAddr: %s\n",stpAddr->szFormatAddr);
    
    ABU_ADDR_printRecord(stpAddr->stpAddrRecord,fp);

    ABU_ADDR_printAttach(stpAddr->stpAddrAttach,fp);
    
    fprintf(fp,"creditWeight:%u\n",stpAddr->iCredWeight);
}/*}}}*/


/******************************************************/
/*******the private interface implementation***********/
/******************************************************/

static ABU_WORDS_DICT_S *createSegDict(const char **szDictArray,const uint32_t dictNumber)
{/*{{{*/
    ABU_ASSERT(szDictArray != NULL);
    
    char szBuff[1024];
    char szWord[512];
    char szPosTag[512];
    
    //step1: load dict into hashmap
    HashMap *stpWordMap = createHashMap(1024 * 128);
    for(int i = 0;i < dictNumber;i++)
    {
        FILE *fp = ab_fopen(szDictArray[ i ],"r");
                
        while( !feof(fp))
        {
            memset(szBuff,0,sizeof(szBuff));
            
            fgets(szBuff,sizeof(szBuff),fp);
            
            abstru_rstrip(szBuff);
                    
            ABU_WORDS_PREPROCESS_all(szBuff);
            
            if(strlen(szBuff) == 0) continue;
                
            memset(szWord,0,sizeof(szWord));
            memset(szPosTag,0,sizeof(szPosTag));
            
            getWordAndPosTag(szBuff,szWord,szPosTag);
            
            insertHashMapNode(stpWordMap,szWord,szPosTag);
        }

        ab_fclose(fp); 
    }
    
    //step2:load cmp suffix 
    for(int i = 0;i < CMP_SUFFIX_WORD_COUNT;i++)
    {
        const char *szPrePosTag = getValueHashMap(stpWordMap,CMP_SUFFIX_WORD[ i ]);
        
        if(szPrePosTag == NULL)
        {
            insertHashMapNode(stpWordMap,CMP_SUFFIX_WORD[ i ],CMP_SUFFIX_POSTAG);
        }
        else
        {
            memset(szPosTag,0,sizeof(szPrePosTag));
            strcat(szPosTag,szPrePosTag);//create new postag
            strcat(szPosTag," ");
            strcat(szPosTag,CMP_SUFFIX_POSTAG);
            changeHashMapNode(stpWordMap,CMP_SUFFIX_WORD[ i ],CMP_SUFFIX_POSTAG);
        }
    }
    
    //step3: create word item array
    char **szWordItemArray = (char**)ab_calloc(stpWordMap->dwNumberHashMapNode,sizeof(char*));
    int wordCount = 0;
    for(int i = 0;i < stpWordMap->dwBucketSize;i++)
    {
        if( (stpWordMap->pBucketArray + i)->key == NULL)
        {
            continue;
        }
        else
        {
            HashMapNodePtr currPtr= stpWordMap->pBucketArray + i;
            while(currPtr != NULL && currPtr->key != NULL)
            {
                memset(szBuff,0,sizeof(szBuff));
                strcat(szBuff,currPtr->key);
                strcat(szBuff," ");
                strcat(szBuff,currPtr->value);
                
                int wordLen =strlen(szBuff);
                szWordItemArray[wordCount] = abstru_strndup(szBuff, wordLen);
                wordCount++;
                
                currPtr = currPtr->next;
            }
        }
    }
    
    //step4: create dict
    ABU_WORDS_DICT_S *stpDict = ABU_WORDS_DICT_loadDictFromArray((const char **)szWordItemArray,wordCount);
    
    //step5: free memory
    destroyHashMap(stpWordMap);
    for(int i  = 0;i < wordCount;i++)
    {
        ABU_FREE(szWordItemArray[ i ]);
    }
    ABU_FREE(szWordItemArray);
    
    return stpDict;
}/*}}}*/


static HashSet *createFeatWordSet()
{/*{{{*/
    HashSet *stpFeatWordSet = createHashSet(ADDR_FEAT_WORD_COUNT);
    
    for(int i = 0;i < ADDR_FEAT_WORD_COUNT;i++)
        insertHashSetNode(stpFeatWordSet,ADDR_FEAT_WORD[ i ]);
    
    return stpFeatWordSet;
}/*}}}*/


static void ADDR_destroyLibContent(ABU_ADDR_LIB_S *stpAddrLib)
{/*{{{*/
    if(stpAddrLib == NULL) return;
        
    //step1: destroy seg dict
    ABU_WORDS_DICT_destroyDict(stpAddrLib->stpSegDict);
    
    //step2: destroy seg lib
    ABU_WORDS_SEG_destroySegLib(stpAddrLib->stpSegLib);
    
    //step3: destroy mapTree
    ABU_MAPTREE_destroy(stpAddrLib->stpMapTree);
    
    //step4: destroy feature wod
    destroyHashSet(stpAddrLib->stpFeatWordSet);
    
    return;
}/*}}}*/


static void ADDR_preprocess(abstring *addrString)
{/*{{{*/
    ABU_ASSERT(addrString != NULL);
    //step0:remove half gb code
    abstring_remove_badcode_gb(addrString);

    //step1:replace bracket
    ADDR_preprocess_replace(addrString);

    //step2;remove punct
    ADDR_preprocess_removePunct(addrString);

    //step3:sbc case -> dbc case
    abstring_sbc2dbc(addrString);

    //step4:remove white space
    abstring_remove_gb(addrString," ");
    abstring_remove_gb(addrString,"\t");
    abstring_remove_gb(addrString,"\n");
    abstring_remove_gb(addrString,"\r");

    //step5:remove 市辖区
    //abstring_lstrip_str(addrString,"中国");
    //abstring_lstrip_str(addrString,"中华人民共和国");
    abstring_remove_gb(addrString,"中国");
    abstring_remove_gb(addrString,"中华人民共和国");
    abstring_remove_gb(addrString,"市辖区");
    abstring_remove_gb(addrString,"辖区");
    abstring_remove_gb(addrString,"地区");
    
    //step6:add "号"
    if(addrString->len < 1)
        return;
    if(addrString->str[addrString->len - 1] >= '0' &&
       addrString->str[addrString->len - 1] <= '9')
        abstring_append(addrString,STREETNO_FEAT_WORD);

    
    //step7:cnum2num
    ADDR_preprocess_cnum2num(addrString);
}/*}}}*/


static void ADDR_preprocess_cnum2num(abstring *addrString)
{/*{{{*/
    ABU_ASSERT(addrString != NULL);
    
    //step1:get cnum
    char *q = abstru_strstr_gb(addrString->str,STREETNO_FEAT_WORD);
    if(q == NULL)
        return;

    char *p = q;  
    while( (p - GB_LEN) >= addrString->str)
    {
        int pos = -1;
        for(int i = 0;i < GB_NUMBER_COUNT;i++)
        {
            if(strncmp(p - GB_LEN,GB_NUMBER[i],GB_LEN) == 0)
            {
                pos = i;
                break;
            }
        } 
        if(pos == -1)
            break;
        p -= GB_LEN;
    }
    
    if(p == q)
        return;

    int pos = p - addrString->str;
    int len_cnum = q - p;
    char cnum[len_cnum + 1];
    memset(cnum,0,sizeof(cnum));
    strncpy(cnum,p,len_cnum);
    
    long long num = cnum2num(cnum);
    if(num == -1)
        return;

    char strnum[16];
    memset(strnum,0,sizeof(strnum));
    sprintf(strnum,"%lld",num);

    abstring_erase(addrString,pos,len_cnum);
    abstring_insert_len(addrString,pos,strnum,strlen(strnum));  
}/*}}}*/


static void ADDR_preprocess_replace(abstring *addrString)
{/*{{{*/
    ABU_ASSERT(addrString != NULL);
    abstring_replace_gb(addrString,"[","(");
    abstring_replace_gb(addrString,"{","(");
    abstring_replace_gb(addrString,"]",")");
    abstring_replace_gb(addrString,"}",")");
    abstring_replace_gb(addrString,"（","(");
    abstring_replace_gb(addrString,"［","(");
    abstring_replace_gb(addrString,"｛","(");
    abstring_replace_gb(addrString,"）",")");
    abstring_replace_gb(addrString,"］",")");
    abstring_replace_gb(addrString,"｝",")");
    abstring_replace_gb(addrString,"－","-");
}/*}}}*/


static void ADDR_preprocess_removePunct(abstring *addrString)
{/*{{{*/

    ABU_ASSERT(addrString != NULL);
    for(int i = 0;i < REMOVE_ENG_PUNCT_COUNT;i++)
        abstring_remove(addrString,REMOVE_ENG_PUNCT[i]);

    for(int i = 0 ;i < REMOVE_GB_PUNCT_COUNT;i++)
        abstring_remove_gb(addrString,REMOVE_GB_PUNCT[i]);
}/*}}}*/


static unsigned char ADDR_checkPCCNextFeat(
                        const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                        const int wordCovered)
{/*{{{*/
    ABU_ASSERT(stpSegResult != NULL);
    
    if((wordCovered + 1) >= stpSegResult->dwWordNumber)
        return TRUE;

    ABU_WORDS_WORD_S *stpWord = NULL;
    stpWord = stpSegResult->pWordArray + wordCovered;
    
    char szGBWord[GB_LEN + 1]; 
    memset(szGBWord,0,sizeof(szGBWord));
    int len = strlen(stpWord->szWord);
    strncpy(szGBWord,stpWord->szWord + len - GB_LEN,GB_LEN);
    if(ADDR_isProvinceFeat(szGBWord) == TRUE ||
      ADDR_isCityFeat(szGBWord) == TRUE ||
      ADDR_isCountyFeat(szGBWord) == TRUE)
        return TRUE; 

    //check next word(addr feat)
    ABU_WORDS_WORD_S *stpNWord = NULL;
    stpNWord = stpSegResult->pWordArray + wordCovered + 1;
    if(stpNWord->szWord[0] > 0)
        return TRUE;
    if(ADDR_isAddrFeat(stpNWord->szWord) == TRUE)
        return FALSE;

    //check next word and next'next word(direct and addr feat)
    if((wordCovered + 2) >= stpSegResult->dwWordNumber)
        return TRUE;
    ABU_WORDS_WORD_S *stpNNWord = NULL;
    stpNNWord = stpSegResult->pWordArray + wordCovered + 2;
    if(stpNNWord->szWord[0] > 0)
        return TRUE;
    if(ADDR_isDirectFeat(stpNWord->szWord) == TRUE && 
       ADDR_isAddrFeat(stpNNWord->szWord) == TRUE)
        return FALSE;

    return TRUE;        
}/*}}}*/


static void ADDR_setProvince(ABU_ADDR_RECORD_S *stpAddrRecord,
                        const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                        const ABU_ADDR_LIB_S *stpAddrLib,
                        int *piWordCovered) 
{/*{{{*/
    ABU_ASSERT(stpAddrRecord != NULL && stpSegResult != NULL);
    ABU_ASSERT(stpAddrLib != NULL && piWordCovered != NULL);
    
    if(stpAddrRecord->szProvince != NULL)
        return;

    if(*piWordCovered >= stpSegResult->dwWordNumber) 
        return;

    ABU_WORDS_WORD_S *stpWord = stpSegResult->pWordArray + *piWordCovered;
    if(ABU_WORDS_UTILITY_isProvince(stpWord) == FALSE) 
        return;
    
    if(ADDR_checkPCCNextFeat(stpSegResult,*piWordCovered) == FALSE)
        return;    
   
    stpAddrRecord->szProvince = abstru_strdup(stpWord->szWord); 
    *piWordCovered += 1;
}/*}}}*/


static void ADDR_trimProvince(ABU_ADDR_RECORD_S *stpAddrRecord,
                        const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                        const ABU_ADDR_LIB_S *stpAddrLib,
                        int *piWordCovered)
{/*{{{*/
    ABU_ASSERT(stpAddrRecord != NULL && stpSegResult != NULL);
    ABU_ASSERT(stpAddrLib != NULL && piWordCovered != NULL);
 
    if(stpAddrRecord->szProvince == NULL)
        return;

    ABU_WORDS_WORD_S *stpWord = NULL;
    while(TRUE) 
    {
        if( *piWordCovered >= stpSegResult->dwWordNumber) 
            break;
        
        stpWord = stpSegResult->pWordArray + *piWordCovered;
        
        if(ABU_WORDS_UTILITY_isProvince(stpWord) == FALSE) 
            break;
        
        if(ADDR_sameProvince(stpAddrRecord->szProvince,stpWord->szWord) == FALSE) 
            break;

        unsigned char isProvince = FALSE;
        
        if(ADDR_endsWithProvinceFeat(stpWord->szWord))
        {
            isProvince = TRUE;
        }
        else
        {
            if((*piWordCovered + 1) < stpSegResult->dwWordNumber)
            {
                stpWord = stpSegResult->pWordArray + *piWordCovered + 1;
                //stpWord->szWord is province city county
                if((ABU_WORDS_UTILITY_isProvince(stpWord) && 
                    ADDR_sameProvince(stpAddrRecord->szProvince,stpWord->szWord)) ||
                   (ABU_WORDS_UTILITY_isCity(stpWord) &&
                    ABU_MAPTREE_isParent(stpAddrLib->stpMapTree,
                            stpAddrRecord->szProvince,stpWord->szWord)) ||
                    (ABU_WORDS_UTILITY_isCounty(stpWord) &&
                    ABU_MAPTREE_isParent(stpAddrLib->stpMapTree,
                            stpAddrRecord->szProvince,stpWord->szWord))
                  )
                {
                    isProvince = TRUE;
                }
            }
        }

        if(isProvince)
            *piWordCovered += 1;
        else
            break;
    }
}/*}}}*/


static void ADDR_setCity(ABU_ADDR_RECORD_S *stpAddrRecord,
                const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                const ABU_ADDR_LIB_S  *stpAddrLib,
                int *piWordCovered,
                const char *szCity)
{/*{{{*/
    ABU_ASSERT(stpAddrRecord != NULL && stpSegResult != NULL);
    ABU_ASSERT(stpAddrLib != NULL && piWordCovered != NULL);
    
    if(stpAddrRecord->szCity != NULL)
        return;

    if(*piWordCovered >= stpSegResult->dwWordNumber) 
        return;
    
    ABU_WORDS_WORD_S *stpWord = NULL;
    stpWord = stpSegResult->pWordArray + *piWordCovered;
    
    if(ABU_WORDS_UTILITY_isCity(stpWord) == FALSE)
        return;

    if(ADDR_checkPCCNextFeat(stpSegResult,*piWordCovered) == FALSE)
        return;

    char *szProvince = NULL;
    if(szProvince == NULL && szCity != NULL && isdgc(szCity))
        szProvince = (char *)szCity;
    else
        szProvince = stpAddrRecord->szProvince;

    if(ABU_MAPTREE_isRightPCC(stpAddrLib->stpMapTree, szProvince, stpWord->szWord, NULL) == FALSE)
        return;

    stpAddrRecord->szCity = abstru_strdup(stpWord->szWord);
    *piWordCovered += 1;
}/*}}}*/


static void ADDR_trimCity(ABU_ADDR_RECORD_S *stpAddrRecord,
                const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                const ABU_ADDR_LIB_S  *stpAddrLib,
                 int *piWordCovered)
{/*{{{*/    
    ABU_ASSERT(stpAddrRecord != NULL && stpSegResult != NULL);
    ABU_ASSERT(stpAddrLib != NULL && piWordCovered != NULL);
    
    if(stpAddrRecord->szCity == NULL) 
        return;

    ABU_WORDS_WORD_S *stpWord = NULL;
    while(TRUE)
    {
        if(*piWordCovered >= stpSegResult->dwWordNumber) 
            break;
        
        stpWord = stpSegResult->pWordArray + *piWordCovered;
        if(ABU_WORDS_UTILITY_isCity(stpWord) == FALSE)
            break;

        if(ADDR_sameCity(stpAddrRecord->szCity,stpWord->szWord) == FALSE) 
            break;
  
        unsigned char isCity = FALSE; 
        if(ADDR_endsWithCityFeat(stpWord->szWord))
        {
            isCity = TRUE;
        }
        else
        {
            if((*piWordCovered + 1) < stpSegResult->dwWordNumber)
            {
                stpWord = stpSegResult->pWordArray + *piWordCovered + 1;
                //stpWord->szWord is province city county
                if((ABU_WORDS_UTILITY_isCity(stpWord) && 
                    ADDR_sameCity(stpAddrRecord->szCity,stpWord->szWord)) ||
                    (ABU_WORDS_UTILITY_isCounty(stpWord) &&
                    ABU_MAPTREE_isParent(stpAddrLib->stpMapTree,
                                        stpAddrRecord->szCity,stpWord->szWord))
                  )
                {
                    isCity = TRUE;
                }
            }
        }

        if(isCity)
            *piWordCovered += 1;
        else
            break;
   }
}/*}}}*/


static void ADDR_setCounty(ABU_ADDR_RECORD_S *stpAddrRecord,
                    const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                    const ABU_ADDR_LIB_S *stpAddrLib, 
                    int *piWordCovered,
                    const char *szCity)
{/*{{{*/
    ABU_ASSERT(stpAddrRecord != NULL && stpSegResult != NULL);
    ABU_ASSERT(stpAddrLib != NULL && piWordCovered != NULL);
        
    if(stpAddrRecord->szCounty != NULL)
        return;

    if(*piWordCovered >= stpSegResult->dwWordNumber) 
        return;
    
    ABU_WORDS_WORD_S *stpWord = NULL;
    stpWord = stpSegResult->pWordArray + *piWordCovered;
    
    if(ABU_WORDS_UTILITY_isCounty(stpWord) == FALSE)
        return;

    if(ADDR_checkPCCNextFeat(stpSegResult,*piWordCovered) == FALSE)
        return;

    char *szProvince = NULL;
    if(szProvince == NULL && szCity != NULL && isdgc(szCity))
        szProvince = (char *)szCity;
    else
        szProvince = stpAddrRecord->szProvince;

    if(ABU_MAPTREE_isRightPCC(stpAddrLib->stpMapTree, szProvince, stpAddrRecord->szCity, stpWord->szWord) == FALSE)
       return; 
   
    stpAddrRecord->szCounty = abstru_strdup(stpWord->szWord);
    *piWordCovered += 1;   
    
    if((*piWordCovered + 1) >= stpSegResult->dwWordNumber) 
        return;
    
    //try to find county feat word 房山县->房山区
    //add county feat import
    stpWord = stpSegResult->pWordArray + *piWordCovered;
    if(ADDR_isCountyFeat(stpWord->szWord))
    {
        if(!abstru_endswith(stpAddrRecord->szCounty, COUNTY_FEAT_WORD_QU) &&
           !abstru_endswith(stpAddrRecord->szCounty, COUNTY_FEAT_WORD_SHI) &&
           !abstru_endswith(stpAddrRecord->szCounty, COUNTY_FEAT_WORD_XIAN))
        {
            catField(&(stpAddrRecord->szCounty),stpWord->szWord,
                 strlen(stpWord->szWord));
            *piWordCovered += 1;
        }
    }
}/*}}}*/


static void ADDR_trimCounty(ABU_ADDR_RECORD_S *stpAddrRecord,
                    const ABU_WORDS_SEGRESULT_SL_S *stpSegResult,
                    const ABU_ADDR_LIB_S *stpAddrLib, 
                    int *piWordCovered)
{/*{{{*/
    ABU_ASSERT(stpAddrRecord != NULL && stpSegResult != NULL);
    ABU_ASSERT(stpAddrLib != NULL && piWordCovered != NULL);
 
    if(stpAddrRecord->szCounty == NULL)
        return;

    ABU_WORDS_WORD_S *stpWord = NULL;
    while(1)    
    {
        if(*piWordCovered >= stpSegResult->dwWordNumber) 
            break;
        
        stpWord = stpSegResult->pWordArray + *piWordCovered;
        
        if(ABU_WORDS_UTILITY_isCounty(stpWord) == FALSE)
            break;
 
        if(ADDR_sameCounty(stpAddrRecord->szCounty,stpWord->szWord) == FALSE) 
            break;

        if(ADDR_endsWithCountyFeat(stpWord->szWord))
        {
            *piWordCovered += 1;
        }
        else
        {
            break;
        }
    }
}/*}}}*/


static void ADDR_setBriefAddr(ABU_ADDR_RECORD_S *stpAddrRecord,const char *szBriefAddr)
{/*{{{*/
    ABU_ASSERT(stpAddrRecord != NULL && szBriefAddr != NULL);
    if(szBriefAddr[0] == 0)
        return;
    
    stpAddrRecord->szBriefAddr = abstru_strdup(szBriefAddr);
}/*}}}*/                    


static void ADDR_setTown(ABU_ADDR_RECORD_S *stpAddrRecord,
                 const char *szAddr,
                 int *pStrLengthCovered)
{/*{{{*/
    ABU_ASSERT(stpAddrRecord != NULL && szAddr != NULL && pStrLengthCovered);
    
    char *prePos = (char *)szAddr + *pStrLengthCovered;
    char *currPos = NULL;
    
    int addrLen = strlen(szAddr);
    char szTown[addrLen + 1];
    memset(szTown,0,addrLen + 1);
    
    while((currPos = ADDR_minMatchNextTownFeat(prePos)))
    {
        char *nextStreetPos = ADDR_minMatchNextStreetFeat(prePos);
        if(nextStreetPos != NULL && nextStreetPos < currPos)
            break;
        char *nextStreetNoPos = ADDR_minMatchNextStreetNOFeat(prePos);
        if(nextStreetNoPos != NULL && nextStreetNoPos < currPos)
            break;

        int len = currPos - prePos + ADDR_getMinMatchNextTownFeatLen(prePos);
        strncat(szTown,prePos,len);
        prePos += len;
    }  
    
    if(szTown[0] == 0)
        return;
    
    int townLen = strlen(szTown);
    stpAddrRecord->szTown = abstru_strndup(szTown, townLen);
    *pStrLengthCovered += townLen;
}/*}}}*/


static void ADDR_setStreetAndNumber(ABU_ADDR_RECORD_S *stpAddrRecord,
                            const char *szAddr,
                            int *pStrLengthCovered)
{/*{{{*/
    ABU_ASSERT(stpAddrRecord != NULL && szAddr != NULL);
    ABU_ASSERT(pStrLengthCovered != NULL);
    
    char *prePos = (char *)szAddr + *pStrLengthCovered;
    char *currPos = NULL;
            
    ADDR_RECOG_LOG_S addrRecogLog;
    memset(&addrRecogLog,0,sizeof(ADDR_RECOG_LOG_S));
    
    //get street
    while((currPos = ADDR_minMatchNextStreetFeat(prePos)))
    {
        char *nextStreetNoPos = ADDR_minMatchNextStreetNOFeat(prePos);
        if(nextStreetNoPos != NULL && nextStreetNoPos < currPos)
            break;
        
        char *leftBracketPos = strstr(prePos, "(");
        if(leftBracketPos != NULL && leftBracketPos < currPos)
            break;
           
        int len = currPos - prePos + ADDR_getMinMatchNextStreetFeatLen(prePos);
        strncat(addrRecogLog.szStreet, prePos, len);
        addrRecogLog.streetLen += len;
        
        prePos += len;
    }
    
    //get streetno
    currPos = STRU_CHIN_strstr(prePos,STREETNO_FEAT_WORD);
    if(currPos != NULL && *(currPos -1) >= '0' && *(currPos - 1) <= '9')
    {
        //get street and number
        char *tempCurrPos = currPos - 1;
        while(prePos < tempCurrPos)
        {
            if( (*tempCurrPos >= '0' && *tempCurrPos <= '9') || *tempCurrPos == '-') 
            {
                tempCurrPos--;
            }
            else
            {
                break;
            }
        }
        
        if(*tempCurrPos < '0' || *tempCurrPos > '9')    tempCurrPos++;
        
        //alter szStreet
        if(tempCurrPos >  prePos)
        {
            strncat(addrRecogLog.szStreet,prePos,tempCurrPos - prePos);
            addrRecogLog.streetLen += tempCurrPos - prePos;
        }
        prePos = tempCurrPos;
        
        if(currPos != prePos)
        {
            int streetNOLen = strlen(STREETNO_FEAT_WORD);
            strncat(addrRecogLog.szStreetNO,prePos,currPos - prePos);
            strncat(addrRecogLog.szStreetNO,STREETNO_FEAT_WORD,streetNOLen);
            
            addrRecogLog.streetNOLen += currPos - prePos;
            addrRecogLog.streetNOLen += streetNOLen;
            
            prePos += addrRecogLog.streetNOLen;
        }
    }

    //normalize street and streetno
    if(addrRecogLog.streetLen != 0 && addrRecogLog.streetNOLen != 0)
    {   
        for(uint32_t i = 0;i < STREETNO_PREFIX_COUNT;i++)
        {
            if(abstru_endswith(addrRecogLog.szStreet, STREETNO_PREFIX[i]))
            {
                char tmpStreet[addrRecogLog.streetLen + 1];
                memset(tmpStreet, 0, sizeof(tmpStreet));
                strncpy(tmpStreet, addrRecogLog.szStreet, addrRecogLog.streetLen);

                abstru_rstrip_str(addrRecogLog.szStreet, STREETNO_PREFIX[i]);
                int prefixLen = addrRecogLog.streetLen - strlen(addrRecogLog.szStreet);
                addrRecogLog.streetLen -= prefixLen;
                memmove(addrRecogLog.szStreetNO + prefixLen, addrRecogLog.szStreetNO, addrRecogLog.streetNOLen);
                memcpy(addrRecogLog.szStreetNO, tmpStreet + addrRecogLog.streetLen, prefixLen);
                addrRecogLog.streetNOLen += prefixLen;
                break;
            }
        }
    }

    if(addrRecogLog.streetLen != 0)
    {
        stpAddrRecord->szStreet = abstru_strndup(addrRecogLog.szStreet,
                                             addrRecogLog.streetLen);
        *pStrLengthCovered += addrRecogLog.streetLen;
    }
    
    if(addrRecogLog.streetNOLen != 0)
    {
        stpAddrRecord->szStreetNO = abstru_strndup(addrRecogLog.szStreetNO,
                                               addrRecogLog.streetNOLen);
        *pStrLengthCovered += addrRecogLog.streetNOLen;
    }

    if(addrRecogLog.streetNOLen != 0)
    {
        char streetNO[addrRecogLog.streetNOLen + 1];
        memset(streetNO, 0, sizeof(streetNO));
        unsigned char firstDigit = FALSE;
        for(uint32_t i = 0; i < addrRecogLog.streetNOLen;i++)
        {
            if(addrRecogLog.szStreetNO[i] < '0' || addrRecogLog.szStreetNO[i] > '9')
            {
                if(!firstDigit)
                    continue;
                else
                    break;
            }
            if(!firstDigit)
                firstDigit = TRUE;
            strncat(streetNO, addrRecogLog.szStreetNO + i, 1);
        }
        if(streetNO[0] != 0)
            stpAddrRecord->streetNO = strtol(streetNO, NULL, 10);        
    } 
}/*}}}*/


static void ADDR_setAddrLeft(ABU_ADDR_RECORD_S *stpAddrRecord,
                        const char *szAddr,
                        int *pStrLengthCovered)
{/*{{{*/
    ABU_ASSERT(stpAddrRecord != NULL && pStrLengthCovered != NULL)
    
    int addrLen = strlen(szAddr);    
    if(*pStrLengthCovered >= addrLen)
        return;

    int len =addrLen - *pStrLengthCovered;
    if(len > 0)
        stpAddrRecord->szAddrLeft = abstru_strndup(szAddr + *pStrLengthCovered, len);
    *pStrLengthCovered += len;
}/*}}}*/


static void ADDR_setDetailLevel(ABU_ADDR_RECORD_S *stpAddrRecord)
{/*{{{*/
    stpAddrRecord->detailLevel = UNKNOWN_LEVEL;
    if(stpAddrRecord->szStreetNO != NULL)
    {
        stpAddrRecord->detailLevel = STREETNO_LEVEL;
        return;
    }
    
    if(stpAddrRecord->szStreet != NULL)
    {
        stpAddrRecord->detailLevel = STREET_LEVEL;
        return;
    }
    
    if(stpAddrRecord->szTown != NULL)
    {
        stpAddrRecord->detailLevel = TOWN_LEVEL;
        return;
    }
    
    if(stpAddrRecord->szCounty != NULL)
    {
        stpAddrRecord->detailLevel = COUNTY_LEVEL;
        return;
    }
    
    if(stpAddrRecord->szCity != NULL)
    {
        stpAddrRecord->detailLevel = CITY_LEVEL;
        return;
    }
    
    if(stpAddrRecord->szProvince != NULL)
    {
        stpAddrRecord->detailLevel = PROVINCE_LEVEL;
        return;
    }
}/*}}}*/


static void ADDR_postprocess(ABU_ADDR_RECORD_S *stpAddrRecord)
{/*{{{*/
    ABU_ASSERT(stpAddrRecord != NULL);
    if(stpAddrRecord->szProvince != NULL && 
       isdgc(stpAddrRecord->szProvince) == TRUE && stpAddrRecord->szCity == NULL)
    {
        stpAddrRecord->szCity = abstru_strdup(stpAddrRecord->szProvince);
    }
}/*}}}*/

static void ADDR_checkRecord(ABU_ADDR_RECORD_S *stpAddrRecord)
{/*{{{*/
    ABU_ASSERT(stpAddrRecord != NULL);
    if(stpAddrRecord->szProvince != NULL && isdgc(stpAddrRecord->szProvince) == TRUE)
    {
        if(stpAddrRecord->szCity == NULL)
        {
            fprintf(stderr, "ADDR_checkRecord error:province:%s city is null", stpAddrRecord->szProvince);
        }
        if(stpAddrRecord->szCity != NULL && strcmp(stpAddrRecord->szProvince, stpAddrRecord->szCity) != 0)
        {
            fprintf(stderr, "ADDR_checkRecord error:province:%s city:%s", stpAddrRecord->szProvince, 
                                                                           stpAddrRecord->szCity);
        }            
    }
}/*}}}*/

static BOOL ADDR_sameProvince(const char *szProvinceLeft,const char *szProvinceRight)
{/*{{{*/
    ABU_ASSERT(szProvinceLeft != NULL && szProvinceRight != NULL);
    if(STRU_CHIN_strstr(szProvinceLeft,szProvinceRight) != NULL ||
       STRU_CHIN_strstr(szProvinceRight,szProvinceLeft) != NULL)
    {
        return TRUE; 
    }    
    
    return FALSE;
}/*}}}*/


static BOOL ADDR_sameCity(const char *szCityLeft,const char *szCityRight)
{/*{{{*/
    ABU_ASSERT(szCityLeft != NULL && szCityRight != NULL);
    
    if(STRU_CHIN_strstr(szCityLeft,szCityRight) != NULL ||
        STRU_CHIN_strstr(szCityRight,szCityLeft) != NULL)
    {
        return TRUE;
    }
    
    return FALSE;
}/*}}}*/
 

static BOOL ADDR_sameCounty(const char *szCountyLeft,const char *szCountyRight)
{/*{{{*/
    ABU_ASSERT(szCountyLeft != NULL && szCountyRight != NULL);
    
    if(STRU_CHIN_strstr(szCountyLeft,szCountyRight) != NULL ||
        STRU_CHIN_strstr(szCountyRight,szCountyLeft) != NULL)
    {
        return TRUE;
    }
    
    return FALSE;
}/*}}}*/


static void ADDR_destroyRecordContent(ABU_ADDR_RECORD_S *stpAddrRecord)
{/*{{{*/
    if(stpAddrRecord == NULL) return;
    
    ABU_FREE(stpAddrRecord->szProvince);
    ABU_FREE(stpAddrRecord->szCity);
    ABU_FREE(stpAddrRecord->szCounty);
    ABU_FREE(stpAddrRecord->szTown);
    ABU_FREE(stpAddrRecord->szStreet);
    ABU_FREE(stpAddrRecord->szStreetNO);
    ABU_FREE(stpAddrRecord->szAddrLeft);
    ABU_FREE(stpAddrRecord->szBriefAddr);
}/*}}}*/


static void ADDR_destroyContent(ABU_ADDR_S *stpAddr)
{/*{{{*/
    if(stpAddr == NULL) return;
    
    //destroy addr record
    ABU_ADDR_destroyRecord(stpAddr->stpAddrRecord);

    //destroy addr attach
    ADDR_destroyAttach(stpAddr->stpAddrAttach);
    
    //free originAddr
    ABU_FREE(stpAddr->szOriginAddr);

    //free formatAddr
    ABU_FREE(stpAddr->szFormatAddr);
}/*}}}*/

char * ADDR_getFormatAddr(const ABU_ADDR_RECORD_S *stpAddrRecord,
                          const ABU_ADDR_LIB_S *stpAddrLib)
{/*{{{*/
    if(stpAddrRecord == NULL)
        return NULL;
    
    int len = 0;
    
    //normalize province
    char *szProvince = NULL;
    if(stpAddrRecord->szProvince != NULL)
        szProvince = (char *)ABU_MAPTREE_getFullProvince(
                                            stpAddrLib->stpMapTree,
                                            stpAddrRecord->szProvince);
    if(szProvince == NULL)
        szProvince = stpAddrRecord->szProvince;

    int provinceLen = (szProvince == NULL)?0:strlen(szProvince);
    len += provinceLen;

    //normalize city
    char *szCity = NULL;
    if(stpAddrRecord->szCity != NULL && 
       isdgc(stpAddrRecord->szCity) == FALSE)
    {   
        if(stpAddrRecord->szCity != NULL)
            szCity = (char *)ABU_MAPTREE_getFullCity(
                                            stpAddrLib->stpMapTree,
                                            stpAddrRecord->szCity,
                                            stpAddrRecord->szProvince);
        if(szCity == NULL)
            szCity = stpAddrRecord->szCity;
    }

    int cityLen = (szCity == NULL)?0:strlen(szCity);
    len += cityLen;
 
    //county
    char *szCounty = stpAddrRecord->szCounty;
    int countyLen = (szCounty == NULL)?0:strlen(szCounty);
    len += countyLen;

    //town
    char *szTown = stpAddrRecord->szTown;
    int townLen = (szTown == NULL)?0:strlen(szTown);
    len += townLen;

    //street
    char *szStreet = stpAddrRecord->szStreet;
    int streetLen = (szStreet == NULL)?0:strlen(szStreet);
    len += streetLen;

    //streetno
    char *szStreetNO = stpAddrRecord->szStreetNO;
    int streetNOLen = (szStreetNO == NULL)?0:strlen(szStreetNO);
    len += streetNOLen;

    //streetleft
    char *szAddrLeft = stpAddrRecord->szAddrLeft;
    int addrLeftLen = (szAddrLeft == NULL)?0:strlen(szAddrLeft);
    len += addrLeftLen;

    char *p = (char*)ab_calloc(len + 1,sizeof(char));
    
    if(provinceLen > 0)
        strncat(p,szProvince,provinceLen);
    
    if(cityLen > 0)
        strncat(p,szCity,cityLen);

    if(countyLen > 0)
        strncat(p,szCounty,countyLen);

    if(townLen > 0)
        strncat(p,szTown,townLen);

    if(streetLen > 0)
        strncat(p,szStreet,streetLen);

    if(streetNOLen > 0)
        strncat(p,szStreetNO,streetNOLen);

    if(addrLeftLen > 0)
        strncat(p,szAddrLeft,addrLeftLen);

    return p;
}/*}}}*/


static unsigned char ADDR_isAddrFeat(const char *szGBWord)
{/*{{{*/
    ABU_ASSERT(szGBWord != NULL);

    unsigned char isAddrFeat = FALSE;
    
    for(int i = 0;i < ADDR_FEAT_WORD_COUNT;i++)
    {
        if(strcmp(ADDR_FEAT_WORD[i],szGBWord) == 0)
        {
            isAddrFeat = TRUE;
            break;
        }
    }
    
    return isAddrFeat;
}/*}}}*/


static unsigned char ADDR_isDirectFeat(const char *szGBWord)
{/*{{{*/
    ABU_ASSERT(szGBWord != NULL);

    unsigned char isDirectFeat = FALSE;

    for(int i = 0;i < DIRECT_FEAT_WORD_COUNT;i++)
    {
        if(strcmp(DIRECT_FEAT_WORD[i],szGBWord) == 0)
        {
            isDirectFeat = TRUE;
            break;
        }
    }

    return isDirectFeat;
}/*}}}*/


static unsigned char ADDR_isProvinceFeat(const char *szGBWord)
{/*{{{*/
    ABU_ASSERT(szGBWord != NULL);

    unsigned char isProvinceFeat = FALSE;

    for(int i = 0;i < PROVINCE_FEAT_WORD_COUNT;i++)
    {
        if(strcmp(PROVINCE_FEAT_WORD[i],szGBWord) == 0)
        {
            isProvinceFeat = TRUE;
            break;
        }
    }

    return isProvinceFeat;
}/*}}}*/


static unsigned char ADDR_isCityFeat(const char *szGBWord)
{/*{{{*/
    ABU_ASSERT(szGBWord != NULL);

    unsigned char isCityFeat = FALSE;

    for(int i = 0;i < CITY_FEAT_WORD_COUNT;i++)
    {
        if(strcmp(CITY_FEAT_WORD[i],szGBWord) == 0)
        {
            isCityFeat = TRUE;
            break;
        }
    }

    return isCityFeat;
}/*}}}*/


static unsigned char ADDR_isCountyFeat(const char *szGBWord)
{/*{{{*/
    ABU_ASSERT(szGBWord != NULL);
   
    unsigned char isCountyFeat = FALSE;

    for(int i = 0;i < COUNTY_FEAT_WORD_COUNT;i++)
    {
        if(strcmp(COUNTY_FEAT_WORD[i], szGBWord) == 0)
        {
            isCountyFeat = TRUE;
            break;
        }
    }

    return isCountyFeat;
}/*}}}*/

/*
static unsigned char ADDR_endsWithAddrFeat(const char *szAddr)
{
    ABU_ASSERT(szAddr != NULL);

    unsigned char endWithAddrFeat = FALSE;
    for(int i = 0;i < ADDR_FEAT_WORD_COUNT;i++)
    {
        if(abstru_endswith(szAddr, ADDR_FEAT_WORD[i]))
        {
            endWithAddrFeat = TRUE;
            break;
        }
    }

    return endWithAddrFeat;
}


static unsigned char ADDR_endsWithDirectFeat(const char *szAddr)
{
    ABU_ASSERT(szAddr != NULL);

    unsigned char endWithDirectFeat = FALSE;

    for(int i = 0;i < DIRECT_FEAT_WORD_COUNT;i++)
    {
        if(abstru_endswith(szAddr, DIRECT_FEAT_WORD[i]))
        {
            endWithDirectFeat = TRUE;
            break;
        }
    }

    return endWithDirectFeat;
}
*/

static unsigned char ADDR_endsWithProvinceFeat(const char *szAddr)
{/*{{{*/
    ABU_ASSERT(szAddr != NULL);

    unsigned char endWithProvinceFeat = FALSE;

    for(int i = 0;i < PROVINCE_FEAT_WORD_COUNT;i++)
    {
        if(abstru_endswith(szAddr, PROVINCE_FEAT_WORD[i]))
        {
            endWithProvinceFeat = TRUE;
            break;
        }
    }

    return endWithProvinceFeat;
}/*}}}*/


static unsigned char ADDR_endsWithCityFeat(const char *szAddr)
{/*{{{*/
    ABU_ASSERT(szAddr != NULL);

    unsigned char endWithCityFeat = FALSE;

    for(int i = 0;i < CITY_FEAT_WORD_COUNT;i++)
    {
        if(abstru_endswith(szAddr, CITY_FEAT_WORD[i]))
        {
            endWithCityFeat = TRUE;
            break;
        }
    }

    return endWithCityFeat;
}/*}}}*/


static unsigned char ADDR_endsWithCountyFeat(const char *szAddr)
{/*{{{*/
    ABU_ASSERT(szAddr != NULL);

    unsigned char endWithCountyFeat = FALSE;

    for(int i = 0;i < COUNTY_FEAT_WORD_COUNT;i++)
    {
        if(abstru_endswith(szAddr, COUNTY_FEAT_WORD[i]))
        {
            endWithCountyFeat = TRUE;
            break;
        }
    }

    return endWithCountyFeat;
}/*}}}*/


static ABU_ADDR_ATTACH_S * ADDR_getAttach(ABU_ADDR_S *stpAddr)
{/*{{{*/
    ABU_ASSERT(stpAddr != NULL);
    ABU_ADDR_ATTACH_S *stpAddrAttach = (ABU_ADDR_ATTACH_S *)ab_calloc(1, sizeof(ABU_ADDR_ATTACH_S));
    
    if(stpAddr->szOriginAddr != NULL)
    {
        //stpAddrAttach->originAddrLen = strlen(stpAddr->szOriginAddr);
        stpAddrAttach->originAddrSign = abstru_hash(stpAddr->szOriginAddr);
        
        //abgbstring * gbstring = abgbstring_new(stpAddr->szOriginAddr);
        //stpAddrAttach->gbOriginAddrLen = gbstring->len;
        //stpAddrAttach->gbOriginAddr = ab_calloc(gbstring->len, sizeof(unsigned short));
        //memcpy(stpAddrAttach->gbOriginAddr, gbstring->gb_str, gbstring->len * sizeof(unsigned short));
        //abgbstring_free(gbstring, TRUE);
    }

    if(stpAddr->stpAddrRecord == NULL)
    {
        fprintf(stderr, "ADDR_getAttach error:no stpAddrRecord\n");
        exit(-1);        
    }

    ABU_ADDR_RECORD_S *stpAddrRecord = stpAddr->stpAddrRecord;
    
    //province
    if(stpAddrRecord->szProvince != NULL)
        stpAddrAttach->province4Char = PCC_get4Char(stpAddrRecord->szProvince);

    //city
    if(stpAddrRecord->szCity != NULL)
        stpAddrAttach->city4Char = PCC_get4Char(stpAddrRecord->szCity);
    
    //county
    if(stpAddrRecord->szCounty != NULL)
        stpAddrAttach->county4Char = PCC_get4Char(stpAddrRecord->szCounty);
    
    //town
    if(stpAddrRecord->szTown != NULL && stpAddrRecord->szTown[0] != STRING_END)
    {
        abgbstring * gbstring = abgbstring_new(stpAddrRecord->szTown);
        stpAddrAttach->gbTownLen = gbstring->len;
        stpAddrAttach->gbTown = (unsigned short *)ab_calloc(gbstring->len, sizeof(unsigned short));
        memcpy(stpAddrAttach->gbTown, gbstring->gb_str, gbstring->len * sizeof(unsigned short));
        abgbstring_free(gbstring, TRUE);
    }

    //street
    if(stpAddrRecord->szStreet != NULL && stpAddrRecord->szStreet[0] != STRING_END)
    {
        abgbstring * gbstring = abgbstring_new(stpAddrRecord->szStreet);
        stpAddrAttach->gbStreetLen = gbstring->len;
        stpAddrAttach->gbStreet = (unsigned short*)ab_calloc(gbstring->len, sizeof(unsigned short));
        memcpy(stpAddrAttach->gbStreet, gbstring->gb_str, gbstring->len * sizeof(unsigned short));
        abgbstring_free(gbstring, TRUE);
    }
    
    //addr left
    if(stpAddrRecord->szAddrLeft != NULL && stpAddrRecord->szAddrLeft[0] != STRING_END)
    {
        abgbstring * gbstring = abgbstring_new(stpAddrRecord->szAddrLeft);
        stpAddrAttach->gbAddrLeftLen = gbstring->len;
        stpAddrAttach->gbAddrLeft = (unsigned short*)ab_calloc(gbstring->len, sizeof(unsigned short));
        memcpy(stpAddrAttach->gbAddrLeft, gbstring->gb_str, gbstring->len * sizeof(unsigned short));
        abgbstring_free(gbstring, TRUE);
    }

    //brief addr
    /*
    if(stpAddrRecord->szBriefAddr != NULL && stpAddrRecord->szBriefAddr[0] != STRING_END)
    {
        abgbstring * gbstring = abgbstring_new(stpAddrRecord->szBriefAddr);
        stpAddrAttach->gbBriefAddrLen = gbstring->len;
        stpAddrAttach->gbBriefAddr = ab_calloc(gbstring->len, sizeof(unsigned short));
        memcpy(stpAddrAttach->gbBriefAddr, gbstring->gb_str, gbstring->len * sizeof(unsigned short));
        abgbstring_free(gbstring, TRUE);
    }
    */

    return stpAddrAttach;
}/*}}}*/


static void ADDR_destroyAttachContent(ABU_ADDR_ATTACH_S *stpAddrAttach)
{/*{{{*/
    if(stpAddrAttach == NULL)
        return;

    //ABU_FREE(stpAddrAttach->gbOriginAddr);
    ABU_FREE(stpAddrAttach->gbTown);
    ABU_FREE(stpAddrAttach->gbStreet);
    ABU_FREE(stpAddrAttach->gbAddrLeft);
    //ABU_FREE(stpAddrAttach->gbBriefAddr);
}/*}}}*/


static void ADDR_destroyAttach(ABU_ADDR_ATTACH_S *stpAddrAttach)
{/*{{{*/
    if(stpAddrAttach == NULL)
        return;

    ADDR_destroyAttachContent(stpAddrAttach);
    ABU_FREE(stpAddrAttach);
}/*}}}*/


static void getWordAndPosTag(const char *szBuff,char *szWord,char *szPosTag)
{/*{{{*/
    ABU_ASSERT(szBuff != NULL && szWord != NULL && szPosTag != NULL);
    
    int len = strlen(szBuff);
    char *p = strchr(szBuff,SPACE);
    int wordLen = p - szBuff;
    
    strncpy(szWord,szBuff, wordLen);
    strncpy(szPosTag,szBuff + wordLen + 1,len - wordLen - 1);
}/*}}}*/


static unsigned int PCC_get4Char(const char * szField)
{/*{{{*/
    ABU_ASSERT(szField != NULL);
    
    int len = strlen(szField);
    unsigned int sign = 0;
    for(int i = 0;i < 4 && i < len;i++)
    {
        sign = sign << 8 | (unsigned char)szField[i];        
    }
    return sign;
}/*}}}*/


static char *ADDR_minMatchNextTownFeat(const char *szAddr)
{/*{{{*/
    if(szAddr == NULL)
        return NULL;
    char * szNextTownFeat = NULL;

    for(int i = 0; i < TOWN_FEAT_WORD_COUNT;i++)
    {
        char *nextPos = STRU_CHIN_strstr(szAddr,TOWN_FEAT_WORD[i]);
        if(nextPos == NULL)
            continue;
        if(szNextTownFeat == NULL || nextPos < szNextTownFeat)
            szNextTownFeat = nextPos;
    }
    return szNextTownFeat;
}/*}}}}*/


static char *ADDR_minMatchNextStreetFeat(const char *szAddr)
{/*{{{*/
    if(szAddr == NULL)
        return NULL;
    char * szNextStreetFeat = NULL;

    for(int i = 0; i < STREET_FEAT_WORD_COUNT;i++)
    {
        char *nextPos = STRU_CHIN_strstr(szAddr,STREET_FEAT_WORD[i]);
        if(nextPos == NULL)
            continue;
        if(szNextStreetFeat == NULL || nextPos < szNextStreetFeat)
            szNextStreetFeat = nextPos;
    }
    return szNextStreetFeat;
}/*}}}*/


static char *ADDR_minMatchNextStreetNOFeat(const char *szAddr)
{/*{{{*/
    return STRU_CHIN_strstr(szAddr, STREETNO_FEAT_WORD);
}/*}}}*/


static uint16_t ADDR_getMinMatchNextTownFeatLen(const char *szAddr)
{/*{{{*/
    if(szAddr == NULL)
        return 0;
    char * szNextTownFeat = NULL;
    uint16_t nextTownFeatLen = 0;

    for(int i = 0; i < TOWN_FEAT_WORD_COUNT;i++)
    {
        char *nextPos = STRU_CHIN_strstr(szAddr,TOWN_FEAT_WORD[i]);
        if(nextPos == NULL)
            continue;
        if(szNextTownFeat == NULL || nextPos < szNextTownFeat)
        {
            szNextTownFeat = nextPos;
            nextTownFeatLen = strlen(TOWN_FEAT_WORD[i]);
        }
    }
    return nextTownFeatLen;
}/*}}}}*/


static uint16_t ADDR_getMinMatchNextStreetFeatLen(const char *szAddr)
{/*{{{*/
    if(szAddr == NULL)
        return 0;
    char * szNextStreetFeat = NULL;
    uint16_t nextStreetFeatLen = 0;

    for(int i = 0; i < STREET_FEAT_WORD_COUNT;i++)
    {
        char *nextPos = STRU_CHIN_strstr(szAddr,STREET_FEAT_WORD[i]);
        if(nextPos == NULL)
            continue;
        if(szNextStreetFeat == NULL || nextPos < szNextStreetFeat)
        {
            szNextStreetFeat = nextPos;
            nextStreetFeatLen = strlen(STREET_FEAT_WORD[i]);
        }
    }
    return nextStreetFeatLen;
}/*}}}*/


static void catField(char **szDst,const char *szSrc,const int srcLen)
{/*{{{*/
    ABU_ASSERT(szSrc != NULL);
    if(srcLen == 0)
        return;
    
    int preLen = 0;
    if(*szDst != NULL)
        preLen = strlen(*szDst);
    *szDst = (char*)ab_realloc(*szDst,(preLen + srcLen + 1) * sizeof(char));
    memset(*szDst + preLen, 0, srcLen + 1);
    strncpy(*szDst + preLen,szSrc,srcLen);
}/*}}}*/
