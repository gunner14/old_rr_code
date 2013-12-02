/**
 * @file name_util.c
 * @author bfzhang
 * @brief name parse trivial logic
 * @date 2008-09-02
 */
#include "ab_hmap.h"
#include "ab_hset.h"
#include "name_util.h"
#include "abstring.h"
#include "abutility.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "AB_UTILITY_NAME_RESOURCE.h"
#include "AB_UTILITY_ADDR_RESOURCE.h"

const char *PROVINCE_FIELD = "province";
const char *CITY_FIELD = "city";
const char *COUNTY_FIELD = "county";
const char *STEMPRE_FIELD = "stemPre";
const char *STEM_FIELD = "stem";
const char *STEMPOST_FIELD = "stemPost";
const char *TYPE_FIELD = "type";
const char *ORG_FIELD = "org";
const char *ASSOPREFIX_FIELD = "assoPrefix";
const char *ASSOSUFFIX_FIELD = "assoSuffix";
const char *APPENDIX_FIELD = "appendix";
const char *MINSTEM_FIELD = "minStem";
const char *MAXTYPE_FIELD = "maxType";
const char *BRANCHPLACE_FIELD = "branchPlace";
const char *MULTIPLESHOP_FIELD = "multipleShop";

static const char SEPA[ ] = " ";
#define  MAX_LINE_LENGTH  1024

ABU_WORDS_DICT_S *NAME_createSegDict(const char **szDictArray, const uint32_t uiDictCount)
{/*{{{*/
    ABU_ASSERT(szDictArray != NULL);
    
    char szBuff[1024];
    ab_hmap *stpWordMap = ab_hmap_init(1024 * 128);
    
    //step1:load file
    for(int i = 0;i < uiDictCount;i++)
    {
        FILE *fp = ab_fopen(szDictArray[i],"r");
        
        while(!feof(fp))
        {
            memset(szBuff, 0, sizeof(szBuff));
            fgets(szBuff, sizeof(szBuff), fp);
            abstru_strip(szBuff);
            if(szBuff[0] == 0)
                break;
            if(abstru_startswith(szBuff, "#"))
                continue; 
            
            char *szWord = strtok(szBuff, SEPA);
            char *szPostag = strtok(NULL, SEPA);
            /*if((strstr(szDictArray[i], AIBANG_BASIC_DICT) != NULL ||
                strstr(szDictArray[i], AIBANG_GROUP_DICT) != NULL))
               && strstr(szPostag, "ns") == NULL) 
                continue;     modified by cuidairui 2012-04-17*/
            ab_hmap_insert(stpWordMap, szWord, szPostag);
        }
    
        ab_fclose(fp); 
    }
    
    //step2:create word item
    char **szWordArray = (char**)ab_calloc(stpWordMap->len, sizeof(char*));
    int wordCount = 0;
    for(int i = 0;i < stpWordMap->size;i++)
    {
        if((stpWordMap->bucket + i)->key == NULL)
            continue;
        ab_hmap_node *currNode= stpWordMap->bucket + i;
        while(currNode != NULL && currNode->key != NULL)
        {
            int keyLen = strlen(currNode->key);
            int valueLen = strlen(currNode->value);
         
            szWordArray[wordCount] = (char*)ab_calloc(keyLen + valueLen + 1 + 1,sizeof(char));
            strncat(szWordArray[wordCount], currNode->key, keyLen);
            strncat(szWordArray[wordCount], " ", 1);
            strncat(szWordArray[wordCount], currNode->value, valueLen);                
            wordCount++;
            currNode = currNode->next; 
        }
    }       
    
    ABU_WORDS_DICT_S *stpDict = ABU_WORDS_DICT_loadDictFromArray((const char **)szWordArray,wordCount);
    
    ab_hmap_destroy(stpWordMap);
    for(int i  = 0;i < wordCount;i++)
    {
        ABU_FREE(szWordArray[ i ]);
    }
    ABU_FREE(szWordArray);
    
    return stpDict;
}/*}}}*/


ABU_WORDS_DICT_S *NAME_createSegDictNameType(const char **szDictArray, const uint32_t uiDictCount)
{/*{{{*/
    ABU_ASSERT(szDictArray != NULL);
    char szBuff[1024];
    ab_hmap *stpWordMap = ab_hmap_init(1024 * 128);
    
    //step1:load file
    for(int i = 0;i < uiDictCount;i++)
    {
        if(strstr(szDictArray[i], NAME_TYPE_DICT) == NULL)
            continue;
        FILE *fp = ab_fopen(szDictArray[i],"r");
        
        while(!feof(fp))
        {
            memset(szBuff, 0, sizeof(szBuff));
            fgets(szBuff, sizeof(szBuff), fp);
            abstru_strip(szBuff);
            if(szBuff[0] == 0)
                break;
            if(abstru_startswith(szBuff, "#"))
                continue;
            
            char *szWord = strtok(szBuff, SEPA);
            char *szPostag = strtok(NULL, SEPA);
            abstru_strip(szPostag);
            abstru_rstrip_str(szPostag, "-1");
            abstru_rstrip_str(szPostag, "-0");
            if(strcmp(szPostag, NAME_TYPE) != 0)
                continue; 
            ab_hmap_insert(stpWordMap, szWord, szPostag);
        }
    
        ab_fclose(fp); 
    }
    
    //step2:create word item
    char **szWordArray = (char**)ab_calloc(stpWordMap->len, sizeof(char*));
    int wordCount = 0;
    for(int i = 0;i < stpWordMap->size;i++)
    {
        if((stpWordMap->bucket + i)->key == NULL)
            continue;
        ab_hmap_node *currNode = stpWordMap->bucket + i;
        while(currNode != NULL && currNode->key != NULL)
        {
            int keyLen = strlen(currNode->key);
            int valueLen = strlen(currNode->value);
         
            szWordArray[wordCount] = (char*)ab_calloc(keyLen + valueLen + 1 + 1,sizeof(char));
            strncat(szWordArray[wordCount], currNode->key, keyLen);
            strncat(szWordArray[wordCount], " ", 1);
            strncat(szWordArray[wordCount], currNode->value, valueLen);                
            wordCount++;
    
            currNode = currNode->next;
        }
    }       
    
    ABU_WORDS_DICT_S *stpDict = ABU_WORDS_DICT_loadDictFromArray((const char **)szWordArray,wordCount);
    
    ab_hmap_destroy(stpWordMap);
    for(int i  = 0;i < wordCount;i++)
    {
        ABU_FREE(szWordArray[ i ]);
    }
    ABU_FREE(szWordArray);
    
    return stpDict;
}/*}}}*/
    
    
ab_hmap *NAME_createNameStemMap(const char **szDictArray, const uint32_t uiDictCount)
{/*{{{*/
    ABU_ASSERT(szDictArray != NULL)
    
    ab_hmap *stpNameStemMap = ab_hmap_init(1024);
    
    for(uint32_t i = 0;i < uiDictCount;i++)
    {
        if(strstr(szDictArray[i], NAME_STEM_DICT) == NULL)
            continue;
        
        FILE *fp = ab_fopen(szDictArray[i], "r");
        
        char szBuff[MAX_LINE_LENGTH];
        while(!feof(fp))
        {
            memset(szBuff, 0, sizeof(szBuff));
            fgets(szBuff, sizeof(szBuff), fp);
            abstru_strip(szBuff);
            if(szBuff[0] == 0)
                break;
            if(abstru_startswith(szBuff, "#"))
                continue;
            char *word = strtok(szBuff, SEPA);
            char *postag = strtok(NULL, SEPA);
            ab_hmap_insert(stpNameStemMap, word, postag);
        }
    
        ab_fclose(fp);            
    }
    
    return stpNameStemMap;
}/*}}}*/
    
    
ab_hmap *NAME_createNameTypeMap(const char **szDictArray, const uint32_t uiDictCount)
{/*{{{*/
    ABU_ASSERT(szDictArray != NULL);
    
    ab_hmap *stpNameTypeMap = ab_hmap_init(1024);
    for(uint32_t i = 0;i < uiDictCount;i++)
    {
        if(strstr(szDictArray[i], NAME_TYPE_DICT) == NULL)
            continue;
        
        FILE *fp = ab_fopen(szDictArray[i], "r");
        
        char szBuff[MAX_LINE_LENGTH];
        while(!feof(fp))
        {
            memset(szBuff, 0, sizeof(szBuff));
            fgets(szBuff, sizeof(szBuff), fp);
            abstru_strip(szBuff);
            if(szBuff[0] == 0)
                break;
            if(abstru_startswith(szBuff, "#"))
                continue;
            char *szWord = strtok(szBuff, SEPA);
            char *szPostag = strtok(NULL, SEPA);
            char szPostag2[strlen(szPostag) + 1];
            memset(szPostag2, 0, sizeof(szPostag));
            strcpy(szPostag2, szPostag);            
            abstru_strip(szPostag);
            abstru_rstrip_str(szPostag, "-1");
            abstru_rstrip_str(szPostag, "-0");
            if(strcmp(szPostag, NAME_TYPE) != 0)            
                continue;
            if(abstru_endswith(szPostag2, "1")) 
                ab_hmap_insert(stpNameTypeMap, szWord, "1");
            else
                ab_hmap_insert(stpNameTypeMap, szWord, "0");                    
        }
    
        ab_fclose(fp);            
    }
    
    return stpNameTypeMap;    
}/*}}}*/
    

ab_hset *NAME_createNameOrgSet(const char **szDictArray, const uint32_t uiDictCount)
{/*{{{*/

    ab_hset *stpNameOrgSet = ab_hset_init(16);
 
    for(uint32_t i = 0;i < uiDictCount;i++)
    {
        if(strstr(szDictArray[i], NAME_TYPE_DICT) == NULL)
            continue;
        
        FILE *fp = ab_fopen(szDictArray[i], "r");
        
        char szBuff[MAX_LINE_LENGTH];
        while(!feof(fp))
        {
            memset(szBuff, 0, sizeof(szBuff));
            fgets(szBuff, sizeof(szBuff), fp);
            abstru_strip(szBuff);
            if(szBuff[0] == 0)
                break;               
            if(abstru_startswith(szBuff, "#"))
                continue;
            char *szWord = strtok(szBuff, SEPA);
            char *szPostag = strtok(NULL, SEPA);
            if(strcmp(szPostag, NAME_ORG) != 0)
                continue; 
            ab_hset_insert(stpNameOrgSet, szWord);
        }
    
        ab_fclose(fp);            
    }
    
    return stpNameOrgSet;
}/*}}}*/ 
    
ab_hset *NAME_createNameStemOrgFeatSet(const char **szDictArray, const uint32_t uiDictCount)
{/*{{{*/

    ab_hset *stpNameStemOrgFeatSet = ab_hset_init(16);
 
    for(uint32_t i = 0;i < uiDictCount;i++)
    {
        if(strstr(szDictArray[i], NAME_TYPE_DICT) == NULL)
            continue;
        
        FILE *fp = ab_fopen(szDictArray[i], "r");
        
        char szBuff[MAX_LINE_LENGTH];
        while(!feof(fp))
        {
            memset(szBuff, 0, sizeof(szBuff));
            fgets(szBuff, sizeof(szBuff), fp);
            abstru_strip(szBuff);
            if(szBuff[0] == 0)
                break;               
            if(abstru_startswith(szBuff, "#"))
                continue;
            char *szWord = strtok(szBuff, SEPA);
            char *szPostag = strtok(NULL, SEPA);
            if(strcmp(szPostag, NAME_STEM_ORGFEAT) != 0)
                continue; 
            ab_hset_insert(stpNameStemOrgFeatSet, szWord);
        }
    
        ab_fclose(fp);            
    }
    
    return stpNameStemOrgFeatSet;
}/*}}}*/ 

ab_hset *NAME_createNameTypeOrgFeatSet(const char **szDictArray, const uint32_t uiDictCount)
{/*{{{*/

    ab_hset *stpNameTypeOrgFeatSet = ab_hset_init(16);
 
    for(uint32_t i = 0;i < uiDictCount;i++)
    {
        if(strstr(szDictArray[i], NAME_TYPE_DICT) == NULL)
            continue;
        
        FILE *fp = ab_fopen(szDictArray[i], "r");
        
        char szBuff[MAX_LINE_LENGTH];
        while(!feof(fp))
        {
            memset(szBuff, 0, sizeof(szBuff));
            fgets(szBuff, sizeof(szBuff), fp);
            abstru_strip(szBuff);
            if(szBuff[0] == 0)
                break;
            if(abstru_startswith(szBuff, "#"))
                continue;
            char *szWord = strtok(szBuff, SEPA);
            char *szPostag = strtok(NULL, SEPA);
            if(strcmp(szPostag, NAME_TYPE_ORGFEAT) != 0)
                continue; 
            ab_hset_insert(stpNameTypeOrgFeatSet, szWord);
        }
    
        ab_fclose(fp);            
    }
    
    return stpNameTypeOrgFeatSet;
}/*}}}*/

ab_hset *NAME_createNamePlaceSet(const char **szDictArray, const uint32_t uiDictCount)
{/*{{{*/
    ABU_ASSERT(szDictArray != NULL);
    
    ab_hset *stpNamePlaceSet = ab_hset_init(1024); 
    for(uint32_t i = 0;i < uiDictCount;i++)
    {
        if(strstr(szDictArray[i], NAME_PLACE_DICT) == NULL)
            continue;
        
        FILE *fp = ab_fopen(szDictArray[i], "r");
        
        char szBuff[MAX_LINE_LENGTH];
        while(!feof(fp))
        {
            memset(szBuff, 0, sizeof(szBuff));
            fgets(szBuff, sizeof(szBuff), fp);
            abstru_strip(szBuff);
            if(szBuff[0] == 0)
                break;
            if(abstru_startswith(szBuff, "#"))
                continue;
            char *word = strtok(szBuff, SEPA);
            ab_hset_insert(stpNamePlaceSet, word);
        }
    
        ab_fclose(fp);            
    }
    
    return stpNamePlaceSet;
}/*}}}*/
    
    
ab_hset *NAME_createAddrFeatWordSet()
{/*{{{*/
    
    ab_hset *stpAddrFeatWordSet = ab_hset_init(ADDR_FEAT_WORD_COUNT);
    
    for(int i = 0;i < ADDR_FEAT_WORD_COUNT;i++)
    {
        ab_hset_insert(stpAddrFeatWordSet,ADDR_FEAT_WORD[ i ]);
    }
    
    return stpAddrFeatWordSet;
}/*}}}*/
    
 
ABU_NAME_ATTACH_S * NAME_getAttach(ABU_NAME_S *stpName)
{/*{{{*/
    ABU_ASSERT(stpName != NULL);
    
    ABU_NAME_ATTACH_S *stpNameAttach = (ABU_NAME_ATTACH_S *)ab_calloc(1, sizeof(ABU_NAME_ATTACH_S));
    
    //origin name
    if(stpName->szOriginName != NULL)
    {
        stpNameAttach->originNameSign = abstru_hash(stpName->szOriginName);
        
        abgbstring * gbstring = abgbstring_new(stpName->szOriginName);
        stpNameAttach->gbOriginNameLen = gbstring->len;
        stpNameAttach->gbOriginName = (uint16_t*)ab_calloc(gbstring->len, sizeof(uint16_t));
        memcpy(stpNameAttach->gbOriginName, gbstring->gb_str, gbstring->len * sizeof(uint16_t));
        abgbstring_free(gbstring, TRUE);
    }
    
    if(stpName->stpNameRecord == NULL)
        return stpNameAttach;
    ABU_NAME_RECORD_S *stpNameRecord = stpName->stpNameRecord;
    
    //province city county
    if(stpNameRecord->szProvince != NULL)
        stpNameAttach->province4Char = PCC_get4Char(stpNameRecord->szProvince);
    
    if(stpNameRecord->szCity != NULL)
        stpNameAttach->city4Char = PCC_get4Char(stpNameRecord->szCity);
    
    if(stpNameRecord->szCounty != NULL)
        stpNameAttach->county4Char = PCC_get4Char(stpNameRecord->szCounty);

    //pre + stem + post
    if(stpNameRecord->szStemPre != NULL)
    {
        abgbstring * gbstring = abgbstring_new(stpNameRecord->szStemPre);
        stpNameAttach->gbStemPreLen = gbstring->len;
        stpNameAttach->gbStemPre = (uint16_t*)ab_calloc(gbstring->len, sizeof(uint16_t));
        memcpy(stpNameAttach->gbStemPre, gbstring->gb_str, gbstring->len * sizeof(uint16_t));
        abgbstring_free(gbstring, TRUE);
    }
    if(stpNameRecord->szStem != NULL)
    {
        abgbstring * gbstring = abgbstring_new(stpNameRecord->szStem);
        stpNameAttach->gbStemLen = gbstring->len;
        stpNameAttach->gbStem = (uint16_t*)ab_calloc(gbstring->len, sizeof(uint16_t));
        memcpy(stpNameAttach->gbStem, gbstring->gb_str, gbstring->len * sizeof(uint16_t));
        abgbstring_free(gbstring, TRUE);
    }
    if(stpNameRecord->szStemPost != NULL)
    {
        abgbstring * gbstring = abgbstring_new(stpNameRecord->szStem);
        stpNameAttach->gbStemPostLen = gbstring->len;
        stpNameAttach->gbStemPost = (uint16_t*)ab_calloc(gbstring->len, sizeof(uint16_t));
        memcpy(stpNameAttach->gbStemPost, gbstring->gb_str, gbstring->len * sizeof(uint16_t));
        abgbstring_free(gbstring, TRUE);
    }

    //type
    if(stpNameRecord->szType != NULL)
    {
        abgbstring * gbstring = abgbstring_new(stpNameRecord->szType);
        stpNameAttach->gbTypeLen = gbstring->len;
        stpNameAttach->gbType = (uint16_t*)ab_calloc(gbstring->len, sizeof(uint16_t));
        memcpy(stpNameAttach->gbType, gbstring->gb_str, gbstring->len * sizeof(uint16_t));
        abgbstring_free(gbstring, TRUE);
    }

     //assoprefix
    if(stpNameRecord->szAssoPrefix != NULL)
    {
        abgbstring * gbstring = abgbstring_new(stpNameRecord->szAssoPrefix);
        stpNameAttach->gbAssoPrefixLen = gbstring->len;
        stpNameAttach->gbAssoPrefix = (uint16_t*)ab_calloc(gbstring->len, sizeof(uint16_t));
        memcpy(stpNameAttach->gbAssoPrefix, gbstring->gb_str, gbstring->len * sizeof(uint16_t));
        abgbstring_free(gbstring, TRUE);
    }
 
    //assosuffix
    if(stpNameRecord->szAssoSuffix != NULL)
    {
        abgbstring * gbstring = abgbstring_new(stpNameRecord->szAssoSuffix);
        stpNameAttach->gbAssoSuffixLen = gbstring->len;
        stpNameAttach->gbAssoSuffix = (uint16_t*)ab_calloc(gbstring->len, sizeof(uint16_t));
        memcpy(stpNameAttach->gbAssoSuffix, gbstring->gb_str, gbstring->len * sizeof(uint16_t));
        abgbstring_free(gbstring, TRUE);
    }
 
    //appendix
    if(stpNameRecord->szAppendix != NULL)
    {
        abgbstring * gbstring = abgbstring_new(stpNameRecord->szAppendix);
        stpNameAttach->gbAppendixLen = gbstring->len;
        stpNameAttach->gbAppendix = (uint16_t*)ab_calloc(gbstring->len, sizeof(uint16_t));
        memcpy(stpNameAttach->gbAppendix, gbstring->gb_str, gbstring->len * sizeof(uint16_t));
        abgbstring_free(gbstring, TRUE);
    }
 
    return stpNameAttach;
}/*}}}*/
    
    
void NAME_destroyLibContent(ABU_NAME_LIB_S *stpNameLib)
{/*{{{*/
    if(stpNameLib == NULL) return;
    
    ABU_WORDS_DICT_destroyDict(stpNameLib->stpSegDict);
    ABU_WORDS_DICT_destroyDict(stpNameLib->stpSegDictNameType);
    ABU_WORDS_SEG_destroySegLib(stpNameLib->stpSegLib);
    
    ab_hmap_destroy(stpNameLib->stpNameStemMap);
    ab_hmap_destroy(stpNameLib->stpNameTypeMap);
    ab_hset_destroy(stpNameLib->stpNameOrgSet);
    ab_hset_destroy(stpNameLib->stpNameStemOrgFeatSet);
    ab_hset_destroy(stpNameLib->stpNameTypeOrgFeatSet);    
    ab_hset_destroy(stpNameLib->stpNamePlaceSet);
    ab_hset_destroy(stpNameLib->stpAddrFeatWordSet);
    
    ABU_MAPTREE_destroy(stpNameLib->stpMapTree);
}/*}}}*/
    
    
void NAME_destroyRecordContent(ABU_NAME_RECORD_S *stpNameRecord)
{/*{{{*/
    if(stpNameRecord == NULL) return;
    
    ABU_FREE(stpNameRecord->szProvince);
    ABU_FREE(stpNameRecord->szCity);
    ABU_FREE(stpNameRecord->szCounty);
   
    ABU_FREE(stpNameRecord->szStemPre);    
    ABU_FREE(stpNameRecord->szStem);
    ABU_FREE(stpNameRecord->szStemPost);
    ABU_FREE(stpNameRecord->szType);
    ABU_FREE(stpNameRecord->szOrg);
    
    ABU_FREE(stpNameRecord->szAssoPrefix);
    ABU_FREE(stpNameRecord->szAssoSuffix);
    ABU_FREE(stpNameRecord->szAppendix);
    
    //special info
    ABU_FREE(stpNameRecord->szMinStem);
    ABU_FREE(stpNameRecord->szMaxType);
    ABU_FREE(stpNameRecord->szBestType);    
    ABU_FREE(stpNameRecord->szBranchPlace);
    ABU_FREE(stpNameRecord->szMultipleShop);
}/*}}}*/
    
    
void NAME_destroyContent(ABU_NAME_S *stpName)
{/*{{{*/
    if(stpName == NULL) return;
    
    //step1:free the origin name
    ABU_FREE(stpName->szOriginName);
    
    //step2:free the name record
    ABU_NAME_destroyRecord(stpName->stpNameRecord);
    
    //step3:free the name attach
    NAME_destroyAttach(stpName->stpNameAttach);
}/*}}}*/


void NAME_destroyAttachContent(ABU_NAME_ATTACH_S *stpNameAttach)
{/*{{{*/
    if(stpNameAttach == NULL)
        return;
    
    ABU_FREE(stpNameAttach->gbOriginName);
    ABU_FREE(stpNameAttach->gbStemPre);    
    ABU_FREE(stpNameAttach->gbStem);
    ABU_FREE(stpNameAttach->gbStemPost);    
    ABU_FREE(stpNameAttach->gbType);
    ABU_FREE(stpNameAttach->gbAssoPrefix);
    ABU_FREE(stpNameAttach->gbAssoSuffix);
    ABU_FREE(stpNameAttach->gbAppendix);
}/*}}}*/
    
    
void NAME_destroyAttach(ABU_NAME_ATTACH_S *stpNameAttach)
{/*{{{*/
    if(stpNameAttach == NULL)
        return;
    
    NAME_destroyAttachContent(stpNameAttach);
    ABU_FREE(stpNameAttach);    
}/*}}}*/


uint32_t PCC_get4Char(const char * szField)
{/*{{{*/
    ABU_ASSERT(szField != NULL);
    
    int len = strlen(szField);
    uint32_t sign = 0;
    for(int i = 0;i < 4 && i < len;i++)
    {
        sign = sign << 8 | (unsigned char)szField[i];        
    }
    return sign;
}/*}}}*/
    

unsigned char NAME_endWithTypeOrgFeat(const ABU_NAME_LIB_S *stpNameLib, const char *str)
{/*{{{*/
    ABU_ASSERT(str != NULL);

    ab_hset *pset = stpNameLib->stpNameTypeOrgFeatSet;
    unsigned char endswith = FALSE;    
    for(int i = 0; i < pset->size;i++)
    {
        ab_hset_node *currnode = pset->bucket + i;
        while(currnode && currnode->key)
        {
            if(abstru_endswith(str, currnode->key))
            {                            
                endswith = TRUE;
            }
            currnode = currnode->next;            
        }        
    }        
    
    return endswith;
}/*}}}*/


unsigned char NAME_endWithStemOrgFeat(const ABU_NAME_LIB_S *stpNameLib, const char *str)
{/*{{{*/
    ABU_ASSERT(str != NULL);
    ab_hset *pset = stpNameLib->stpNameStemOrgFeatSet;
    unsigned char endswith = FALSE;    
    for(int i = 0; i < pset->size;i++)
    {
        ab_hset_node *currnode = pset->bucket + i;
        while(currnode && currnode->key)
        {
            if(abstru_endswith(str, currnode->key))
                endswith = TRUE;
            currnode = currnode->next;            
        } 
    }     
    
    return endswith;
}/*}}}*/


unsigned char NAME_endWithAddrFeat(const char *str)
{/*{{{*/
    ABU_ASSERT(str != NULL);
    if(str[0] == 0)
        return FALSE;
    
    for(int i = 0; i < ADDR_FEAT_WORD_COUNT;i++)
    {
        if(abstru_endswith(str, ADDR_FEAT_WORD[i]))
            return TRUE;            
    }    
    
    return FALSE;    
}/*}}}*/


unsigned char NAME_isEffectiveStem(const ABU_NAME_LIB_S *stpNameLib, const char *szStem)
{/*{{{*/
    ABU_ASSERT(szStem != NULL);
    unsigned char isEffective = TRUE;
    if(szStem[0] == 0 ||strcmp(szStem, "中国") == 0 || strcmp(szStem, "全国") == 0 ||
       (szStem[0] < 0 && strlen(szStem) == GB_LEN))
        isEffective = FALSE;      
    if(ABU_MAPTREE_isProvince(stpNameLib->stpMapTree, szStem) ||
       ABU_MAPTREE_isCity(stpNameLib->stpMapTree, szStem))
        isEffective = FALSE;        
    return isEffective;
}/*}}}*/


unsigned char NAME_isPartOfMiniStem(const ABU_NAME_LIB_S *stpNameLib, const char *szMiniStem, const char *szWord)
{/*{{{*/
    ABU_ASSERT(szMiniStem != NULL && szWord != NULL);
    if(NAME_isEffectiveStem(stpNameLib, szMiniStem) == FALSE ||
       isGBKCharacter(szWord) == TRUE)
        return TRUE;
     
    return FALSE;
}/*}}}*/


unsigned char NAME_isEffectivePlace(const ABU_NAME_LIB_S *stpNameLib, const char *szPlace, const char *szCity)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && szPlace != NULL);
    unsigned char iseffective = FALSE;
    if(ab_hset_haskey(stpNameLib->stpNamePlaceSet, szPlace))
        iseffective = TRUE;

    if(!iseffective)    
    {
        if(szCity == NULL)
        {
            if(ABU_MAPTREE_isProvince(stpNameLib->stpMapTree, szPlace) ||
               ABU_MAPTREE_isCity(stpNameLib->stpMapTree, szPlace) ||
               ABU_MAPTREE_isCounty(stpNameLib->stpMapTree, szPlace))
                iseffective = TRUE;
        }
        else
        {
            if(ABU_MAPTREE_isProvince(stpNameLib->stpMapTree, szPlace) ||
               abstru_startswith(szPlace, szCity) ||
               abstru_startswith(szCity, szPlace) ||
               ABU_MAPTREE_isParent(stpNameLib->stpMapTree, szCity, szPlace))
                iseffective = TRUE;
        }
    }   
    
    return iseffective;    
}/*}}}*/

ABU_NAME_RECORD_S * NAME_getRecord(const NAME_RECOG_LOG_S *stpSrc)
{/*{{{*/
    ABU_ASSERT(stpSrc != NULL);
    ABU_NAME_RECORD_S *stpDst = (ABU_NAME_RECORD_S *)ab_calloc(1, sizeof(ABU_NAME_RECORD_S));
    
    if(stpSrc->szProvince[0])
        stpDst->szProvince = abstru_strdup(stpSrc->szProvince);
    
    if(stpSrc->szCity[0])
        stpDst->szCity = abstru_strdup(stpSrc->szCity);
    
    if(stpSrc->szCounty[0])
        stpDst->szCounty = abstru_strdup(stpSrc->szCounty);
 
    if(stpSrc->szStemPre[0])
        stpDst->szStemPre = abstru_strdup(stpSrc->szStemPre);

    if(stpSrc->szStem[0])
        stpDst->szStem = abstru_strdup(stpSrc->szStem);

    if(stpSrc->szStemPost[0])
        stpDst->szStemPost = abstru_strdup(stpSrc->szStemPost);
    
    if(stpSrc->szType[0])
        stpDst->szType = abstru_strdup(stpSrc->szType);
    
    if(stpSrc->szOrg[0])
        stpDst->szOrg = abstru_strdup(stpSrc->szOrg);
    
    if(stpSrc->szAssoPrefix[0])
        stpDst->szAssoPrefix = abstru_strdup(stpSrc->szAssoPrefix);
    
    if(stpSrc->szAssoSuffix[0])
        stpDst->szAssoSuffix = abstru_strdup(stpSrc->szAssoSuffix);
    
    if(stpSrc->szAppendix[0])
        stpDst->szAppendix = abstru_strdup(stpSrc->szAppendix);

    //speical info
    if(stpSrc->szMinStem[0])
        stpDst->szMinStem = abstru_strdup(stpSrc->szMinStem);
   
    if(stpSrc->szMaxType[0])
        stpDst->szMaxType = abstru_strdup(stpSrc->szMaxType);    

    if(stpSrc->szBestType[0])
        stpDst->szBestType = abstru_strdup(stpSrc->szBestType);

    if(stpSrc->szMultipleShop[0])
        stpDst->szMultipleShop = abstru_strdup(stpSrc->szMultipleShop);

    if(stpSrc->szBranchPlace[0])
        stpDst->szBranchPlace = abstru_strdup(stpSrc->szBranchPlace);

    return stpDst;   
}/*}}}*/


void NAME_printLog(const NAME_RECOG_LOG_S *stpNameLog, FILE *fp)
{/*{{{*/
    ABU_ASSERT(stpNameLog != NULL && fp != NULL);
    
    fprintf(fp,"NAME_RECOG_LOG_S structrue:\n");
    
    if(stpNameLog->szProvince[0])
        fprintf(fp,"    szProvince : %s\n",stpNameLog->szProvince);
    if(stpNameLog->szCity[0])
        fprintf(fp,"    szCity : %s\n",stpNameLog->szCity);
    if(stpNameLog->szCounty[0])
        fprintf(fp,"    szCounty : %s\n",stpNameLog->szCounty);

    if(stpNameLog->szStemPre[0])
        fprintf(fp,"    szStemPre : %s\n", stpNameLog->szStemPre);               
    if(stpNameLog->szStem[0])
        fprintf(fp,"    szStem : %s\n",stpNameLog->szStem);
    if(stpNameLog->szStemPost[0])
        fprintf(fp,"    szStemPost:%s\n",stpNameLog->szStemPost);       
    if(stpNameLog->szType[0])
        fprintf(fp,"    szType : %s\n",stpNameLog->szType);
    if(stpNameLog->szOrg[0])
        fprintf(fp,"    szOrg : %s\n",stpNameLog->szOrg);
    
    if(stpNameLog->szAssoPrefix[0])
        fprintf(fp,"    szAssoPrefix : %s\n",stpNameLog->szAssoPrefix);
    if(stpNameLog->szAssoSuffix[0])
        fprintf(fp,"    szAssoSuffix : %s\n", stpNameLog->szAssoSuffix);

    if(stpNameLog->szAppendix[0])
        fprintf(fp,"    szAppendix : %s\n", stpNameLog->szAppendix);

    if(stpNameLog->szMinStem[0])
        fprintf(fp,"        szMinStem : %s\n", stpNameLog->szMinStem);
    if(stpNameLog->szMaxType[0])
        fprintf(fp,"        szMaxType : %s\n", stpNameLog->szMaxType);
    if(stpNameLog->szBestType[0])
        fprintf(fp,"        szBestType : %s\n", stpNameLog->szBestType);
    if(stpNameLog->szBranchPlace[0])
        fprintf(fp,"        szBranchPlace : %s\n", stpNameLog->szBranchPlace);
    if(stpNameLog->szMultipleShop[0])
        fprintf(fp,"        szMultipleShop : %s\n", stpNameLog->szMultipleShop);        
               
    fprintf(fp,"\n");
}/*}}}*/


void NAME_printRecord(const ABU_NAME_RECORD_S *stpNameRecord,FILE *fp)
{/*{{{*/
    ABU_ASSERT(stpNameRecord != NULL && fp != NULL);
    
    fprintf(fp,"ABU_NAME_RECORD_S structrue:\n");
    
    if(stpNameRecord->szProvince != NULL)
        fprintf(fp,"    szProvince : %s\n",stpNameRecord->szProvince);
    if(stpNameRecord->szCity != NULL)
        fprintf(fp,"    szCity : %s\n",stpNameRecord->szCity);
    if(stpNameRecord->szCounty != NULL)
        fprintf(fp,"    szCounty : %s\n",stpNameRecord->szCounty);

    if(stpNameRecord->szStemPre != NULL)
        fprintf(fp,"    szStemPre : %s\n", stpNameRecord->szStemPre);                
    if(stpNameRecord->szStem != NULL)
        fprintf(fp,"    szStem : %s\n",stpNameRecord->szStem);
    if(stpNameRecord->szStemPost != NULL)
        fprintf(fp,"    szStemPost:%s\n",stpNameRecord->szStemPost);       
    if(stpNameRecord->szType != NULL)
        fprintf(fp,"    szType : %s\n",stpNameRecord->szType);
    if(stpNameRecord->szOrg != NULL)
        fprintf(fp,"    szOrg : %s\n",stpNameRecord->szOrg);
    
    if(stpNameRecord->szAssoPrefix != NULL)
        fprintf(fp,"    szAssoPrefix : %s\n",stpNameRecord->szAssoPrefix);
    if(stpNameRecord->szAssoSuffix != NULL)
        fprintf(fp,"    szAssoSuffix : %s\n", stpNameRecord->szAssoSuffix);

    if(stpNameRecord->szAppendix != NULL)
        fprintf(fp,"    szAppendix : %s\n", stpNameRecord->szAppendix);

    if(stpNameRecord->szMinStem != NULL)
        fprintf(fp,"        szMinStem : %s\n", stpNameRecord->szMinStem);
    if(stpNameRecord->szMaxType != NULL)
        fprintf(fp,"        szMaxType : %s\n", stpNameRecord->szMaxType);
    if(stpNameRecord->szBestType != NULL)
        fprintf(fp,"        szBestType : %s\n", stpNameRecord->szBestType);    
    if(stpNameRecord->szBranchPlace != NULL)
        fprintf(fp,"        szBranchPlace : %s\n", stpNameRecord->szBranchPlace);
    if(stpNameRecord->szMultipleShop != NULL)
        fprintf(fp,"        szMultipleShop : %s\n", stpNameRecord->szMultipleShop);        
               
    fprintf(fp,"\n");
}/*}}}*/
    

void NAME_printAttach(const ABU_NAME_ATTACH_S *stpNameAttach, FILE *fp)
{/*{{{*/
    ABU_ASSERT(stpNameAttach != NULL);
    
    fprintf(fp,"ABU_NAME_ATTACH_S structure:\n");
    
    fprintf(fp, "gbOriginNameLen:%u gbOriginName:", stpNameAttach->gbOriginNameLen);
    for(uint16_t i = 0;i < stpNameAttach->gbOriginNameLen;i++)
    {
        fprintf(fp, "%u", stpNameAttach->gbOriginName[i]);
        if(i != (stpNameAttach->gbOriginNameLen - 1))
            fprintf(fp, "-");                                   
    }
    fprintf(fp, "\n");
    
    fprintf(fp, "province4Char:%u\n", stpNameAttach->province4Char);

    fprintf(fp, "city4Char:%u\n", stpNameAttach->city4Char);

    fprintf(fp, "county4Char:%u\n", stpNameAttach->county4Char);

    fprintf(fp, "gbStemPreLen:%u gbStemPre:", stpNameAttach->gbStemPreLen);
    for(uint16_t i = 0;i < stpNameAttach->gbStemPreLen;i++)
    {
        fprintf(fp, "%u", stpNameAttach->gbStemPre[i]);
        if(i != (stpNameAttach->gbStemPreLen - 1))
            fprintf(fp, "-");                                   
    }
    fprintf(fp, "\n");

    fprintf(fp, "gbStemLen:%u gbStem:", stpNameAttach->gbStemLen);
    for(uint16_t i = 0;i < stpNameAttach->gbStemLen;i++)
    {
        fprintf(fp, "%u", stpNameAttach->gbStem[i]);
        if(i != (stpNameAttach->gbStemLen - 1))
            fprintf(fp, "-");                                   
    }
    fprintf(fp, "\n");

    fprintf(fp, "gbStemPostLen:%u gbStemPost:", stpNameAttach->gbStemPostLen);
    for(uint16_t i = 0;i < stpNameAttach->gbStemPostLen;i++)
    {
        fprintf(fp, "%u", stpNameAttach->gbStemPost[i]);
        if(i != (stpNameAttach->gbStemPostLen - 1))
            fprintf(fp, "-");                                   
    }
    fprintf(fp, "\n");

    fprintf(fp, "gbTypeLen:%u gbType:", stpNameAttach->gbTypeLen);
    for(uint16_t i = 0;i < stpNameAttach->gbTypeLen;i++)
    {
        fprintf(fp, "%u", stpNameAttach->gbType[i]);
        if(i != (stpNameAttach->gbTypeLen - 1))
            fprintf(fp, "-");                                   
    }
    fprintf(fp, "\n");

    fprintf(fp, "gbAssoPrefixLen:%u gbAssoPrefix:", stpNameAttach->gbAssoPrefixLen);
    for(uint16_t i = 0;i < stpNameAttach->gbAssoPrefixLen;i++)
    {
        fprintf(fp, "%u", stpNameAttach->gbAssoPrefix[i]);
        if(i != (stpNameAttach->gbAssoPrefixLen - 1))
            fprintf(fp, "-");                                   
    }
    fprintf(fp, "\n");

    fprintf(fp, "gbAssoSuffixLen:%u gbAssoSuffix:", stpNameAttach->gbAssoSuffixLen);
    for(uint16_t i = 0;i < stpNameAttach->gbAssoSuffixLen;i++)
    {
        fprintf(fp, "%u", stpNameAttach->gbAssoSuffix[i]);
        if(i != (stpNameAttach->gbAssoSuffixLen - 1))
            fprintf(fp, "-");                                   
    }
    fprintf(fp, "\n");

    fprintf(fp, "gbAppendixLen:%u gbAppendix:", stpNameAttach->gbAppendixLen);
    for(uint16_t i = 0;i < stpNameAttach->gbAppendixLen;i++)
    {
        fprintf(fp, "%u", stpNameAttach->gbAppendix[i]);
        if(i != (stpNameAttach->gbAppendixLen - 1))
            fprintf(fp, "-");                                   
    }
    fprintf(fp, "\n");
}/*}}}*/


void NAME_print(const ABU_NAME_S *stpName,FILE *fp)
{/*{{{*/
    ABU_ASSERT(stpName != NULL);
    fprintf(fp,"ABU_NAME_S structure:\n");
    fprintf(fp,"szOriginName : %s\n",stpName->szOriginName);
    
    NAME_printRecord(stpName->stpNameRecord,fp);

    NAME_printAttach(stpName->stpNameAttach,fp);
}/*}}}*/


void NAME_printLib(const ABU_NAME_LIB_S *stpNameLib, FILE *fp)
{/*{{{*/
    fprintf(fp, "stpNameStemMap:count = %u\n", 
            stpNameLib->stpNameStemMap->len);
    fprintf(fp, "stpNameTypeSet:count = %u\n",
            stpNameLib->stpNameTypeMap->len);
    fprintf(fp, "stpNameOrgSet:count = %u\n", 
            stpNameLib->stpNameOrgSet->len);
    fprintf(fp, "stpNameStemOrgFeatSet:count = %u\n", 
            stpNameLib->stpNameStemOrgFeatSet->len);
    fprintf(fp, "stpNamePlaceSet:count = %u\n", 
            stpNameLib->stpNamePlaceSet->len);
    fprintf(fp, "stpAddrFeatWordSet:count = %u\n",
            stpNameLib->stpAddrFeatWordSet->len);
    ABU_MAPTREE_statMapTree(stpNameLib->stpMapTree, fp);
}/*}}}*/
