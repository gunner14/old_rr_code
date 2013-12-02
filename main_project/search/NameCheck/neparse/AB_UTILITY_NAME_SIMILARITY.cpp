/**
 * @file AB_UTILITY_NAME_SIMILARITY.c
 * @author bfzhang
 * @brief get name similarity
 *  create this file in 2007-11-09
 */
#include "abutility.h"
#include "abstring.h"
#include "name_util.h"
#include "AB_UTILITY_COMMON.h"
#include "AB_UTILITY_NAME.h"
#include "AB_UTILITY_NAME_RESOURCE.h"
#include "AB_UTILITY_NAME_SIMILARITY.h"
#include "shortname.h"

#define MAX_SIMI_VALUE 100
#define MIN_SIMI_VALUE 0
#define ADD_WEIGHT 1
#define DELETE_WEIGHT 1
#define SUBS_WEIGHT 2
#define NAME_THRESHHOLD 30

typedef struct _DynamicStem
{
	uint16_t gbStemLeft[NAME_LEN_MAX];
    uint16_t gbStemLenLeft;
	uint16_t gbStemRight[NAME_LEN_MAX];
    uint16_t gbStemLenRight;
}DynamicStem;
/**************************************************************/
/*****private interface declaration****************************/
/**************************************************************/
static int NAME_getSimi_single_single(const ABU_NAME_LIB_S *stpNameLib, 
                                      const ABU_NAME_S *pNameUnitLeft, 
                                      const ABU_NAME_S *pNameUnitRight);
static int NAME_getSimi_union_union(const ABU_NAME_LIB_S *stpNameLib,
                                    const ABU_NAME_S *pNameUnitLeft,
                                    const ABU_NAME_S *pNameUnitRight);
static int NAME_getSimi_single_union(const ABU_NAME_LIB_S *stpNameLib,
                                     const ABU_NAME_S *pNameUnitLeft,
                                     const ABU_NAME_S *pNameUnitRight);
static int NAME_getSimi_prepost(const ABU_NAME_S *pNameUnitLeft, 
                                const ABU_NAME_S *pNameUnitRight,
                                const uint16_t * gbDynamicStemLeft,                                
                                const uint16_t gbDynamicStemLenLeft);
static int NAME_getSimi_prepart(const ABU_NAME_S *pNameUnitLeft,
                                const ABU_NAME_S *pNameUnitRight,
                                const DynamicStem *pDynamicStem);
static int NAME_getSimi_postpart(const ABU_NAME_S *pNameUnitLeft,
                                 const ABU_NAME_S *pNameUnitRight);
static int NAME_getSimi_bylcs(const unsigned short *gbStemLeft,
                                  const unsigned int gbStemLeftLen,
                                  const unsigned short *gbStemRight,
                                  const unsigned int gbStemRightLen); 
static int NAME_getSimi_byeditdist(const unsigned short *gbFieldLeft,
                                   const unsigned int gbFieldLeftLen,
                                   const unsigned short *gbFieldRight,
                                   const unsigned int gbFieldRightLen);
//static unsigned char NAME_isLCSColl(const unsigned short *gbFieldLeft,
//                                    const unsigned int gbFieldLeftLen,
//                                    const unsigned short *gbFieldRight,
//                                    const unsigned int gbFieldRightLen);
static int normalize_simi(const int simi);
void get_dynamicStem(const ABU_NAME_LIB_S *stpNameLib,
                     const ABU_NAME_S *pNameUnitLeft,
                     const ABU_NAME_S *pNameUnitRight,
                     DynamicStem *pDynamicStem);   
/*******************************************************************/
/*****public interface implementation*******************************/
/*******************************************************************/
BOOL ABU_NAME_isColl(const ABU_NAME_LIB_S *stpNameLib,
                     const ABU_NAME_S *pNameUnitLeft,
                     const ABU_NAME_S *pNameUnitRight)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && pNameUnitLeft != NULL && pNameUnitRight != NULL);
  
#ifdef AB_DEBUG    
    fprintf(stdout,"#######################################################\n");
    fprintf(stdout,"following info for debug ABU_NAME_isColl\n");
#endif
    if(pNameUnitLeft == NULL || pNameUnitRight == NULL)
    {
#ifdef AB_DEBUG
        fprintf(stdout,"ABU_NAME_isColl:FALSE\n");
        if(pNameUnitLeft == NULL)
            fprintf(stdout,"    pNameUnitLeft = NULL\n");
        if(pNameUnitRight == NULL)
            fprintf(stdout,"    pNameUnitRight = NULL\n");
#endif
        return FALSE;
    }

    ABU_NAME_ATTACH_S *pAttachLeft = pNameUnitLeft->stpNameAttach;
    ABU_NAME_ATTACH_S *pAttachRight = pNameUnitRight->stpNameAttach;
    ABU_NAME_RECORD_S *pRecordLeft = pNameUnitLeft->stpNameRecord;
    ABU_NAME_RECORD_S *pRecordRight = pNameUnitRight->stpNameRecord;
    
#ifdef AB_DEBUG    
    fprintf(stdout,"pNameUnitLeft:\n");
    NAME_print(pNameUnitLeft, stdout);
    fprintf(stdout,"pNameUnitRight:\n");
    NAME_print(pNameUnitRight, stdout);
#endif


    if(pRecordLeft->szCounty != NULL && pRecordRight->szCounty != NULL &&
       pAttachLeft->county4Char != pAttachRight->county4Char )
    {
#ifdef AB_DEBUG 
        fprintf(stdout, "ABU_NAME_isColl:TRUE\n");
        fprintf(stdout, "    county collision\n");       
#endif
        return TRUE; 	 
    }

    if(pRecordLeft->szCity != NULL && pRecordRight->szCity != NULL &&
       pAttachLeft->city4Char != pAttachRight->city4Char)
    {
#ifdef AB_DEBUG
        fprintf(stdout, "ABU_NAME_isColl:TRUE\n");
        fprintf(stdout, "    city collistion\n");
#endif
        return TRUE;
    }

    if(pRecordLeft->szProvince != NULL && pRecordRight->szProvince != NULL &&
       pAttachLeft->province4Char != pAttachRight->province4Char)
    {
#ifdef AB_DEBUG
        fprintf(stdout, "ABU_NAME_isColl:TRUE\n");
        fprintf(stdout, "    province collision\n"); 
#endif
        return TRUE; 	 
    }

    if(abstru_isletters(pRecordLeft->szStem) && 
       abstru_isletters(pRecordRight->szStem) &&
       strcmp(pRecordLeft->szStem, pRecordRight->szStem) != 0)
    {
#ifdef AB_DEBUG
        fprintf(stdout, "ABU_NAME_isColl::TRUE\n");
        fprintf(stdout, "    both stems are letters, not exactly match\n");
#endif
        return TRUE;
    }

#ifdef AB_DEBUG
    fprintf(stdout, "ABU_NAME_isColl:FALSE\n");
#endif

    return FALSE;
}/*}}}*/

int ABU_NAME_getSimi(const ABU_NAME_LIB_S *stpNameLib,
                     const ABU_NAME_S *pNameUnitLeft,
                     const ABU_NAME_S *pNameUnitRight)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && pNameUnitLeft != NULL && pNameUnitRight != NULL);

#ifdef AB_DEBUG
    fprintf(stdout,"#####################################################\n");
    fprintf(stdout,"following info for debug ABU_NAME_getSimi\n");
#endif
    if(pNameUnitLeft == NULL || pNameUnitRight == NULL)
    {
#ifdef AB_DEBUG
        fprintf(stdout, "ABU_NAME_getSimi:%d\n", MIN_SIMI_VALUE);
        if(pNameUnitLeft == NULL)
            fprintf(stdout,"    pNameUnitLeft = NULL\n");
        if(pNameUnitRight == NULL)
            fprintf(stdout,"    pNameUnitRight = NULL\n");
#endif
        return MIN_SIMI_VALUE;
    }

    ABU_NAME_ATTACH_S *pAttachLeft = pNameUnitLeft->stpNameAttach;
    ABU_NAME_ATTACH_S *pAttachRight = pNameUnitRight->stpNameAttach;
    ABU_NAME_RECORD_S *pRecordLeft = pNameUnitLeft->stpNameRecord;
    ABU_NAME_RECORD_S *pRecordRight = pNameUnitRight->stpNameRecord;

    if(pAttachLeft->originNameSign == pAttachRight->originNameSign &&
	   strcmp(pNameUnitLeft->szOriginName, pNameUnitRight->szOriginName) == 0)
    {
#ifdef AB_DEBUG
        fprintf(stdout, "ABU_NAME_getSimi:%d\n", MAX_SIMI_VALUE);
        fprintf(stdout, "    originNames are same\n");
#endif
		return MAX_SIMI_VALUE;
    }

    int simi = short_name_simi((const char *)stpNameLib->szDictDir, pNameUnitLeft, pNameUnitRight);
    /*int simi = 0;*/
    if(simi > 0) /* if one is a short_alias of another, return a similarity, else return 0 */
        return simi;

    if(pRecordLeft->szAssoSuffix == NULL && pRecordRight->szAssoSuffix == NULL)
        return NAME_getSimi_single_single(stpNameLib, pNameUnitLeft, pNameUnitRight);
    else if(pRecordLeft->szAssoSuffix != NULL && pRecordRight->szAssoSuffix != NULL)
        return NAME_getSimi_union_union(stpNameLib, pNameUnitLeft, pNameUnitRight);
    else
        return NAME_getSimi_single_union(stpNameLib, pNameUnitLeft, pNameUnitRight);
}/*}}}*/

int ABU_NAME_getStemSimi(const ABU_NAME_LIB_S *stpNameLib,
                         const ABU_NAME_S *pNameUnitLeft, 
                         const ABU_NAME_S *pNameUnitRight)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && pNameUnitLeft != NULL && pNameUnitRight != NULL);

    if(stpNameLib == NULL || pNameUnitLeft == NULL || pNameUnitRight == NULL)
    {
        return 0;
    }
    const double stem_init_weight = 0.60;
    const double type_init_weight = 0.30;
    const double appendix_init_weight = 0.10;
    const double damping_weight = 0.70;
    double stem_weight = stem_init_weight;
    double type_weight = type_init_weight;
    double appendix_weight = appendix_init_weight; 


    DynamicStem dynamicStem;
    memset(&dynamicStem, 0, sizeof(dynamicStem));
    get_dynamicStem(stpNameLib, pNameUnitLeft, pNameUnitRight, &dynamicStem);        

    double stem_simi = (double)NAME_getSimi_bylcs(dynamicStem.gbStemLeft, dynamicStem.gbStemLenLeft,
                                                  dynamicStem.gbStemRight, dynamicStem.gbStemLenRight);
#ifdef AB_DEBUG       
    fprintf(stdout, "stem_simi:%.2f \n",stem_simi);
#endif    
    return (int)stem_simi;
}
/******************************************************************************/
/*****private interface implementation*****************************************/
/******************************************************************************/
static int NAME_getSimi_single_single(const ABU_NAME_LIB_S *stpNameLib,
                                      const ABU_NAME_S *pNameUnitLeft, 
                                      const ABU_NAME_S *pNameUnitRight)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && pNameUnitLeft != NULL && pNameUnitRight != NULL);

    ABU_NAME_ATTACH_S *pAttachLeft = pNameUnitLeft->stpNameAttach;
    ABU_NAME_ATTACH_S *pAttachRight = pNameUnitRight->stpNameAttach;
    ABU_NAME_RECORD_S *pRecordLeft = pNameUnitLeft->stpNameRecord;
    ABU_NAME_RECORD_S *pRecordRight = pNameUnitRight->stpNameRecord;
 
    const double stem_init_weight = 0.60;
    const double type_init_weight = 0.30;
    const double appendix_init_weight = 0.10;
    const double damping_weight = 0.70;
    double stem_weight = stem_init_weight;
    double type_weight = type_init_weight;
    double appendix_weight = appendix_init_weight; 

#ifdef AB_DEBUG       
    fprintf(stdout, "both of name-units are single-structure\n");
    fprintf(stdout, "stem-simi is calculated by longest common string\n");
    fprintf(stdout, "other-simi is calculated by edit distance\n");
    fprintf(stdout, "init_weight:\n");
    fprintf(stdout, "    stem_weight:%.2f type_weight:%.2f appendix_weight:%.2f\n",
                    stem_init_weight, type_init_weight, appendix_init_weight);
#endif    

    DynamicStem dynamicStem;
    memset(&dynamicStem, 0, sizeof(dynamicStem));
    get_dynamicStem(stpNameLib, pNameUnitLeft, pNameUnitRight, &dynamicStem);        

    double stem_simi = (double)NAME_getSimi_bylcs(dynamicStem.gbStemLeft, dynamicStem.gbStemLenLeft,
                                                  dynamicStem.gbStemRight, dynamicStem.gbStemLenRight);
    double type_simi = (double)NAME_getSimi_byeditdist(pAttachLeft->gbType, pAttachLeft->gbTypeLen,
                                            pAttachRight->gbType, pAttachRight->gbTypeLen);
    
    double appendix_simi = (double)NAME_getSimi_byeditdist(pAttachLeft->gbAppendix, pAttachLeft->gbAppendixLen,
                                                pAttachRight->gbAppendix,pAttachRight->gbAppendixLen);
    
    double stem_smooth_coefficient = (stem_simi / MAX_SIMI_VALUE) * (stem_simi / MAX_SIMI_VALUE);

    const double STEM_WEIGHT=0.5;
    const double TYPE_WEIGHT=0.3;
    const double APPENDIX_WEIGHT=0.2;
    //
    bool either_type = (pRecordLeft->szType != NULL) ^ (pRecordRight->szType != NULL);
    bool neither_type = (pRecordLeft->szType == NULL) & (pRecordRight->szType == NULL); 
    bool either_appendix = (pRecordLeft->szAppendix != NULL) ^ (pRecordRight->szAppendix != NULL);
    bool neither_appendix = (pRecordLeft->szAppendix == NULL) & (pRecordRight->szAppendix == NULL);
    //reset weight
    float shared_type_weight = 0.0;
    if (either_type) {
        shared_type_weight = damping_weight * type_weight;
        type_weight = 0.0;
    }
    else if (neither_type) {
        shared_type_weight = type_weight;
        type_weight = 0.0;
    }

    float shared_appendix_weight = 0.0;
    if (either_appendix) {
        shared_appendix_weight = damping_weight * appendix_weight;
        appendix_weight = 0.0;
    }
    else if (neither_appendix) {
        shared_appendix_weight = appendix_weight;
        appendix_weight = 0.0;
    }

    if (type_weight == 0.0 && appendix_weight == 0.0)
        stem_weight += (shared_type_weight + shared_appendix_weight) * stem_smooth_coefficient;
    else if (type_weight == 0.3 && appendix_weight == 0.0) {
        stem_weight += shared_appendix_weight * STEM_WEIGHT/(STEM_WEIGHT+TYPE_WEIGHT) * stem_smooth_coefficient;
        type_weight += shared_appendix_weight * TYPE_WEIGHT/(STEM_WEIGHT+TYPE_WEIGHT);
    }
    else if (type_weight == 0.0 && appendix_weight == 0.1) {
        stem_weight += shared_type_weight * STEM_WEIGHT/(STEM_WEIGHT+APPENDIX_WEIGHT) * stem_smooth_coefficient;
        appendix_weight += shared_type_weight * APPENDIX_WEIGHT/(STEM_WEIGHT+APPENDIX_WEIGHT);
    }
    
    if (stem_simi > 80 && (type_weight > 0 || appendix_weight > 0)) {
        //stem_simi is high, we need to enhance type/appendix weight.
        stem_weight -= 0.2;
        if (type_weight > 0 && appendix_weight > 0) {
            type_weight += 0.2 * TYPE_WEIGHT/(TYPE_WEIGHT+APPENDIX_WEIGHT);
            appendix_weight += 0.2 * APPENDIX_WEIGHT/(TYPE_WEIGHT+APPENDIX_WEIGHT);
        }
        else if (type_weight > 0)
            type_weight += 0.2;
        else if (appendix_weight > 0) 
            appendix_weight += 0.2;
    }
    if (type_simi > 80)
        //if type_simi is high, we need to reduce its weight.
        type_weight = type_weight * stem_smooth_coefficient;
    if (appendix_simi > 80)
        appendix_weight = appendix_weight * stem_smooth_coefficient;

    //calculate total simi    
    double simi= stem_simi * stem_weight + type_simi * type_weight + appendix_simi * appendix_weight;
  
    double prepart_simi = (double)NAME_getSimi_prepart(pNameUnitLeft, pNameUnitRight, &dynamicStem);
    double postpart_simi = (double)NAME_getSimi_postpart(pNameUnitLeft, pNameUnitRight);
    double prepart_weight = stem_init_weight + type_init_weight;
    double postpart_weight = appendix_init_weight;
    if(pRecordLeft->szAppendix == NULL && pRecordRight->szAppendix == NULL)
    {
        prepart_weight += postpart_weight;
        postpart_weight = 0;
    }
    double crude_simi = prepart_weight * prepart_simi + postpart_weight * postpart_simi;
    int norm_simi = normalize_simi((simi + crude_simi) / 2 + 0.5);    

#ifdef AB_DEBUG    
    fprintf(stdout, "reset_weight:\n");
    fprintf(stdout, "    stem_weight:%.2f type_weight:%.2f appendix_weight:%.2f\n",
                    stem_weight, type_weight, appendix_weight);
    fprintf(stdout, "field_simi:\n");
    fprintf(stdout, "    stem_simi:%.1f type_simi:%.1f appendix_simi:%.1f\n",
                    stem_simi, type_simi, appendix_simi);
    fprintf(stdout, "formula:simi=stem_weight * stem_simi + type_weight * type_simi + ");
    fprintf(stdout, "appendix_weight * appendix_simi\n");
    fprintf(stdout, "simi:%.1f=%.2f * %.1f + %.2f * %.1f + %.2f * %.1f\n", simi, stem_weight, stem_simi,
                      type_weight, type_simi, appendix_weight, appendix_simi);
    fprintf(stdout, "crude_simi:%.1f=%.2f * %.1f + %.2f * %.1f\n",
                      crude_simi, prepart_weight, prepart_simi, postpart_weight, postpart_simi);
    //smooth
    fprintf(stdout, "smooth simi:%d = int((%.1f + %.1f) / 2 + 0.5)\n", norm_simi, simi, crude_simi);
#endif    

    return norm_simi;
}/*}}}*/


static int NAME_getSimi_union_union(const ABU_NAME_LIB_S *stpNameLib,
                                    const ABU_NAME_S *pNameUnitLeft,
                                    const ABU_NAME_S *pNameUnitRight)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && pNameUnitLeft != NULL && pNameUnitRight != NULL);

    ABU_NAME_ATTACH_S *pAttachLeft = pNameUnitLeft->stpNameAttach;
    ABU_NAME_ATTACH_S *pAttachRight = pNameUnitRight->stpNameAttach;
    ABU_NAME_RECORD_S *pRecordLeft = pNameUnitLeft->stpNameRecord;
    ABU_NAME_RECORD_S *pRecordRight = pNameUnitRight->stpNameRecord;

    const double stem_init_weight = 0.50;
    const double type_init_weight = 0.20;
    const double assoprefix_init_weight = 0.10;
    const double assosuffix_init_weight = 0.10;
    const double appendix_init_weight = 0.10;
    const double damping_weight = 0.80;

    double stem_weight = stem_init_weight;
    double type_weight = type_init_weight;
    double assoprefix_weight = assoprefix_init_weight;
    double assosuffix_weight = assosuffix_init_weight;
    double appendix_weight = appendix_init_weight;

#ifdef AB_DEBUG
    fprintf(stdout, "both of name-units are union-structure\n");
    fprintf(stdout, "stem-simi is calculated by longest common string\n");
    fprintf(stdout, "other-simi is calculated by edit distance\n");
    fprintf(stdout, "init_weight:\n");
    fprintf(stdout, "    stem_weight:%.2f type_weight:%.2f assoprefix_weight=%.2f ",
                     stem_init_weight, type_init_weight, assoprefix_init_weight);
    fprintf(stdout, "assosuffix_weight:%.2f appendix_weight:%.2f\n",
                     assosuffix_init_weight, appendix_init_weight);
#endif

    DynamicStem dynamicStem;
    memset(&dynamicStem, 0, sizeof(dynamicStem));
    get_dynamicStem(stpNameLib, pNameUnitLeft, pNameUnitRight, &dynamicStem);        

    double stem_simi = (double)NAME_getSimi_bylcs(dynamicStem.gbStemLeft, dynamicStem.gbStemLenLeft,
                                                  dynamicStem.gbStemRight, dynamicStem.gbStemLenRight);
    
    double type_simi = NAME_getSimi_byeditdist(pAttachLeft->gbType, pAttachLeft->gbTypeLen,
                                               pAttachRight->gbType, pAttachRight->gbTypeLen);

    double assoprefix_simi = NAME_getSimi_byeditdist(pAttachLeft->gbAssoPrefix,
                                                     pAttachLeft->gbAssoPrefixLen,
                                                     pAttachRight->gbAssoPrefix,
                                                     pAttachRight->gbAssoPrefixLen);
    
    double assosuffix_simi = NAME_getSimi_byeditdist(pAttachLeft->gbAssoSuffix,
                                                     pAttachLeft->gbAssoSuffixLen,
                                                     pAttachRight->gbAssoSuffix,
                                                     pAttachRight->gbAssoSuffixLen);
    double appendix_simi = appendix_simi = NAME_getSimi_byeditdist(pAttachLeft->gbAppendix, 
                                                                   pAttachLeft->gbAppendixLen,
                                                                   pAttachRight->gbAppendix,
                                                                   pAttachRight->gbAppendixLen);

    double stem_smooth_coefficient = (stem_simi / MAX_SIMI_VALUE) * (stem_simi / MAX_SIMI_VALUE);
    double assosuffix_smooth_coefficient = (assosuffix_simi / MAX_SIMI_VALUE) * 
                                           (assosuffix_simi / MAX_SIMI_VALUE);
                                                
    //reset weight
    unsigned char stem_type_same = FALSE;
     if(pRecordLeft->szAppendix == NULL && pRecordRight->szAppendix == NULL)
    {
        assoprefix_weight +=  appendix_weight * assoprefix_weight / 
                              (assoprefix_weight + assosuffix_weight);
        assosuffix_weight +=  appendix_weight * assosuffix_weight /
                              (assoprefix_weight + assosuffix_weight);
        appendix_weight = 0;
    }

    if(pRecordLeft->szType == NULL || pRecordRight->szType == NULL)
    {
        if(pRecordLeft->szType == NULL && pRecordRight->szType == NULL)
            stem_weight += type_weight * stem_smooth_coefficient;
        else
            stem_weight += type_weight * damping_weight * stem_smooth_coefficient;
        type_weight = 0;
    }
    
    if(pRecordLeft->szAssoPrefix == NULL || pRecordRight->szAssoPrefix == NULL)
    {
        if(pRecordLeft->szAssoPrefix == NULL && pRecordRight->szAssoPrefix == NULL)
            assosuffix_weight += assoprefix_weight * assosuffix_smooth_coefficient;
        else
            assosuffix_weight += assoprefix_weight * damping_weight * assosuffix_smooth_coefficient;
    }

    if(stem_simi == MAX_SIMI_VALUE)
    {
        if(type_simi == MAX_SIMI_VALUE ||
           (pRecordLeft->szType == NULL && pRecordRight->szType == NULL))
        {
            assoprefix_weight +=  (stem_weight + type_weight) * assoprefix_weight / 
                                  (assoprefix_weight + assosuffix_weight);
            assosuffix_weight +=  (stem_weight + type_weight) * assosuffix_weight /
                                  (assoprefix_weight + assosuffix_weight);
            stem_weight = 0;
            type_weight = 0;
            stem_type_same = TRUE;
        }
    }
    
    type_weight = type_weight * stem_smooth_coefficient;
    
    //calculate total simi
    double simi = stem_simi * stem_weight + type_simi * type_weight + 
               assoprefix_simi * assoprefix_weight + 
               assosuffix_simi * assosuffix_weight + 
               appendix_simi * appendix_weight;

    double prepart_simi = (double)NAME_getSimi_prepart(pNameUnitLeft, pNameUnitRight, &dynamicStem);
    double postpart_simi = (double)NAME_getSimi_postpart(pNameUnitLeft, pNameUnitRight);
    double prepart_weight = stem_init_weight + type_init_weight;
    double postpart_weight = assoprefix_init_weight + assosuffix_init_weight + appendix_init_weight;
    if(pRecordLeft->szAssoPrefix == NULL && pRecordRight->szAssoPrefix == NULL &&
       pRecordLeft->szAssoSuffix == NULL && pRecordRight->szAssoSuffix == NULL &&
       pRecordLeft->szAppendix == NULL && pRecordRight->szAppendix == NULL)
    {
        prepart_weight += postpart_weight;
        postpart_weight = 0;
    }
    double crude_simi = prepart_weight * prepart_simi + postpart_weight * postpart_simi;

#ifdef AB_DEBUG    
    fprintf(stdout, "reset_weight:\n");
    fprintf(stdout, "    stem_weight:%.2f type_weight:%.2f assoprefix_weight=%.2f ",
                     stem_weight, type_weight, assoprefix_weight);
    fprintf(stdout, "assosuffix_weight:%.2f appendix_weight:%.2f\n",
                     assosuffix_weight, appendix_weight);
    fprintf(stdout, "field-simi:\n");
    fprintf(stdout, "    stem_simi:%.1f type_simi=%.1f assoprefix_simi:%.1f ",
                     stem_simi, type_simi, assoprefix_simi);
    fprintf(stdout, "assosuffix_simi:%.1f appendix_simi:%.1f\n",
                     assosuffix_simi, appendix_simi);
    fprintf(stdout, "formula:simi=stem_weight * stem_simi + type_weight * type_simi + ");
    fprintf(stdout, "assoprefix_weight * assoprefix_simi + assosuffix_weight * ");
    fprintf(stdout, "assosuffix_simi + appendix_weight * appendix_simi\n");
    fprintf(stdout, "result:%.1f=%.2f * %.1f + %.2f * %.1f + %.2f * %.1f + %.2f * %.1f + %.2f * %.1f\n",
                      simi, stem_weight, stem_simi,type_weight, type_simi, 
                      assoprefix_weight, assoprefix_simi, assosuffix_weight,
                      assosuffix_simi, appendix_weight, appendix_simi);
    fprintf(stdout, "crude_simi:%.1f=%.2f * %.1f + %.2f * %.1f\n",
                      crude_simi, prepart_weight, prepart_simi, postpart_weight, postpart_simi);
#endif

    //smooth
    if(!stem_type_same)
    {
#ifdef AB_DEBUG
        fprintf(stdout, "smooth simi:%.1f = (%.1f + %.1f) / 2\n", (simi + crude_simi)/2, simi, crude_simi);
#endif
        simi = (simi + crude_simi) / 2;
    }

    return normalize_simi(simi + 0.5);
}/*}}}*/


static int NAME_getSimi_single_union(const ABU_NAME_LIB_S *stpNameLib,
                                     const ABU_NAME_S *pNameUnitLeft,
                                     const ABU_NAME_S *pNameUnitRight)
{/*{{{*/
    ABU_ASSERT(pNameUnitLeft != NULL && pNameUnitRight != NULL);

    ABU_NAME_ATTACH_S *pAttachLeft = pNameUnitLeft->stpNameAttach;
    ABU_NAME_ATTACH_S *pAttachRight = pNameUnitRight->stpNameAttach;
    ABU_NAME_RECORD_S *pRecordLeft = pNameUnitLeft->stpNameRecord;
    ABU_NAME_RECORD_S *pRecordRight = pNameUnitRight->stpNameRecord;

    const double stem_init_weight = 0.50;
    const double type_init_weight = 0.20;
    const double post_init_weight = 0.30;
    const double damping_weight = 0.80;
    double stem_weight = stem_init_weight;
    double type_weight = type_init_weight;
    double post_weight = post_init_weight;

#ifdef AB_DEBUG
    fprintf(stdout, "name-units are single-union-structure\n");
    fprintf(stdout, "stem-simi is calculated by longest common string\n");
    fprintf(stdout, "other-simi is calculated by edit distance\n");
    fprintf(stdout, "init_weight:\n");
    fprintf(stdout, "    stem_weight:%.2f type_weight:%.2f post_weight=%.2f\n",
                     stem_init_weight, type_init_weight, post_init_weight);
#endif

    DynamicStem dynamicStem;
    memset(&dynamicStem, 0, sizeof(dynamicStem));
    get_dynamicStem(stpNameLib, pNameUnitLeft, pNameUnitRight, &dynamicStem);        

    double stem_simi = (double)NAME_getSimi_bylcs(dynamicStem.gbStemLeft, dynamicStem.gbStemLenLeft,
                                                  dynamicStem.gbStemRight, dynamicStem.gbStemLenRight);
    
    double type_simi = NAME_getSimi_bylcs(pAttachLeft->gbType, pAttachLeft->gbTypeLen,
                                          pAttachRight->gbType, pAttachRight->gbTypeLen);

    double prepart_simi = (double)NAME_getSimi_prepart(pNameUnitLeft, pNameUnitRight, &dynamicStem);
    
    double postpart_simi = (double)NAME_getSimi_postpart(pNameUnitLeft, pNameUnitRight);
 
    double stem_smooth_coefficient = (stem_simi / MAX_SIMI_VALUE) * (stem_simi / MAX_SIMI_VALUE);

    //reset weight
    unsigned char stem_type_same = FALSE;
    if(pRecordLeft->szType == NULL || pRecordRight->szType == NULL)
    {
        if(pRecordLeft->szType == NULL && pRecordRight->szType == NULL)
            stem_weight += type_weight * stem_smooth_coefficient;
        else
            stem_weight += type_weight * damping_weight * stem_smooth_coefficient;
        type_weight = 0;
    }
    
    if(stem_simi == MAX_SIMI_VALUE)
    {
        if(type_simi == MAX_SIMI_VALUE ||
           (pRecordLeft->szType == NULL && pRecordRight->szType == NULL))
        {
            post_weight += stem_weight + type_weight;
            stem_weight = 0;
            type_weight = 0;
            stem_type_same = TRUE;
        }
    }
    
    type_weight = type_weight * stem_smooth_coefficient;
    
    //calculat total simi
    double simi = stem_weight * stem_simi + type_weight * type_weight + post_weight * postpart_simi; 

    double prepart_weight = stem_init_weight + type_init_weight;
    double postpart_weight = post_init_weight;
    if(pRecordLeft->szAssoPrefix == NULL && pRecordRight->szAssoPrefix == NULL &&
       pRecordLeft->szAssoSuffix == NULL && pRecordRight->szAssoSuffix == NULL &&
       pRecordLeft->szAppendix == NULL && pRecordRight->szAppendix == NULL)
    {
        prepart_weight += postpart_weight;
        postpart_weight = 0;
    }
    double crude_simi = prepart_weight * prepart_simi + postpart_weight * postpart_simi;
#ifdef AB_DEBUG
    fprintf(stdout, "reset_weight:\n");
    fprintf(stdout, "    stem_weight:%.2f type_weight:%.2f post_weight=%.2f\n",
                     stem_weight, type_weight, post_weight);
    fprintf(stdout, "field-simi:\n");
    fprintf(stdout, "    stem_simi:%.1f type_simi=%.1f postpart_simi:%.1f\n",
                     stem_simi, type_simi, postpart_simi);
    fprintf(stdout, "formula:simi=stem_weight * stem_simi + type_weight * type_simi + ");
    fprintf(stdout, "postpart_weight * postpart_simi\n");
    fprintf(stdout, "result:%.1f=%.2f * %.1f + %.2f * %.1f + %.2f * %.1f\n",
                      simi, stem_weight, stem_simi,type_weight, type_simi, post_weight, postpart_simi);
    fprintf(stdout, "crude_simi:%.1f=%.2f * %.1f + %.2f * %.1f\n",
                      crude_simi, prepart_weight, prepart_simi, postpart_weight, postpart_simi);
#endif
    //smooth
    if(!stem_type_same)
    {
#ifdef AB_DEBUG
        fprintf(stdout, "smooth simi:%.1f = (%.1f + %.1f) / 2\n", (simi + crude_simi) / 2, simi, crude_simi);
#endif
        simi = (simi + crude_simi) / 2;
    }

    if(simi < NAME_THRESHHOLD)
    {
        double prepost_simi = 0;
        const double prepost_dumping_weight = 0.50;
        if(pRecordLeft->szAssoSuffix == NULL)
            prepost_simi = NAME_getSimi_prepost(pNameUnitLeft, pNameUnitRight, dynamicStem.gbStemLeft, dynamicStem.gbStemLenLeft);
        else
            prepost_simi = NAME_getSimi_prepost(pNameUnitRight, pNameUnitLeft, dynamicStem.gbStemRight, dynamicStem.gbStemLenRight);
        simi = prepost_simi * prepost_dumping_weight;
#ifdef AB_DEBUG        
        fprintf(stdout, "simi smoothed by pre-post compare\n");
        fprintf(stdout, "simi=%.1f\n", simi);
#endif
    }

    return normalize_simi(simi + 0.5);
}/*}}}*/


static int NAME_getSimi_prepost(const ABU_NAME_S *pNameUnitLeft, 
                                const ABU_NAME_S *pNameUnitRight,
                                const uint16_t * gbDynamicStemLeft,                                
                                const uint16_t gbDynamicStemLenLeft)
{/*{{{*/
    ABU_ASSERT(pNameUnitLeft != NULL && pNameUnitRight != NULL && gbDynamicStemLeft != NULL);

    ABU_NAME_ATTACH_S *pAttachLeft = pNameUnitLeft->stpNameAttach;
    ABU_NAME_ATTACH_S *pAttachRight = pNameUnitRight->stpNameAttach;
    
    uint16_t leftPart[pAttachLeft->gbStemPreLen + gbDynamicStemLenLeft+ 
                      pAttachLeft->gbStemPostLen + pAttachLeft->gbTypeLen];
    uint16_t rightPart[pAttachRight->gbAssoPrefixLen + pAttachRight->gbAssoSuffixLen + pAttachRight->gbAppendixLen];
 
    uint16_t leftGBLen = 0;
    uint16_t rightGBLen = 0;
 
    memcpy(leftPart, pAttachLeft->gbStemPre, pAttachLeft->gbStemPreLen * sizeof(uint16_t));
    leftGBLen += pAttachLeft->gbStemPreLen;            
    memcpy(leftPart + leftGBLen, gbDynamicStemLeft, gbDynamicStemLenLeft * sizeof(uint16_t));
    leftGBLen += gbDynamicStemLenLeft;
    memcpy(leftPart + leftGBLen, pAttachLeft->gbStemPost, pAttachLeft->gbStemPostLen * sizeof(uint16_t));
    leftGBLen += pAttachLeft->gbStemPostLen;    
    memcpy(leftPart + leftGBLen, pAttachLeft->gbType, pAttachLeft->gbTypeLen * sizeof(uint16_t));
    leftGBLen += pAttachLeft->gbTypeLen;

    memcpy(rightPart, pAttachRight->gbAssoPrefix, pAttachRight->gbAssoPrefixLen * sizeof(uint16_t));
    rightGBLen += pAttachRight->gbAssoPrefixLen;
    memcpy(rightPart + rightGBLen, pAttachRight->gbAssoSuffix, pAttachRight->gbAssoSuffixLen * sizeof(uint16_t));
    rightGBLen += pAttachRight->gbAssoSuffixLen;
    memcpy(rightPart + rightGBLen, pAttachRight->gbAppendix, pAttachRight->gbAppendixLen * sizeof(uint16_t));
    rightGBLen += pAttachRight->gbAppendixLen;

    return NAME_getSimi_byeditdist(leftPart, leftGBLen, rightPart, rightGBLen); 
}/*}}}*/


static int NAME_getSimi_prepart(const ABU_NAME_S *pNameUnitLeft,
                                const ABU_NAME_S *pNameUnitRight,
                                const DynamicStem *pDynamicStem)
{/*{{{*/
    ABU_ASSERT(pNameUnitLeft != NULL && pNameUnitRight != NULL);

    ABU_NAME_ATTACH_S *pAttachLeft = pNameUnitLeft->stpNameAttach;
    ABU_NAME_ATTACH_S *pAttachRight = pNameUnitRight->stpNameAttach;
 
    uint16_t leftPart[pAttachLeft->gbStemPreLen + pDynamicStem->gbStemLenLeft + 
                      pAttachLeft->gbStemPostLen + pAttachLeft->gbTypeLen];
    uint16_t rightPart[pAttachRight->gbStemPreLen + pDynamicStem->gbStemLenRight + 
                       pAttachRight->gbStemPostLen + pAttachRight->gbTypeLen];
    uint16_t leftGBLen = 0;
    uint16_t rightGBLen = 0;

    memcpy(leftPart, pAttachLeft->gbStemPre, pAttachLeft->gbStemPreLen * sizeof(uint16_t));
    leftGBLen += pAttachLeft->gbStemPreLen;
    memcpy(leftPart + leftGBLen, pDynamicStem->gbStemLeft, pDynamicStem->gbStemLenLeft * sizeof(uint16_t));
    leftGBLen += pDynamicStem->gbStemLenLeft;
    memcpy(leftPart + leftGBLen, pAttachLeft->gbStemPost, pAttachLeft->gbStemPostLen * sizeof(uint16_t));
    leftGBLen += pAttachLeft->gbStemPostLen;
    memcpy(leftPart + leftGBLen, pAttachLeft->gbType, pAttachLeft->gbTypeLen * sizeof(uint16_t));
    leftGBLen += pAttachLeft->gbTypeLen;

    memcpy(rightPart, pAttachRight->gbStemPre, pAttachRight->gbStemPreLen * sizeof(uint16_t));
    rightGBLen += pAttachRight->gbStemPreLen;    
    memcpy(rightPart + rightGBLen, pDynamicStem->gbStemRight, pDynamicStem->gbStemLenRight * sizeof(uint16_t));
    rightGBLen += pDynamicStem->gbStemLenRight;
    memcpy(rightPart + rightGBLen, pAttachRight->gbStemPost, pAttachRight->gbStemPostLen * sizeof(uint16_t));
    rightGBLen += pAttachRight->gbStemPostLen;        
    memcpy(rightPart + rightGBLen, pAttachRight->gbType, pAttachRight->gbTypeLen * sizeof(uint16_t));
    rightGBLen += pAttachRight->gbTypeLen;

    return NAME_getSimi_byeditdist(leftPart, leftGBLen, rightPart, rightGBLen); 
}/*}}}*/


static int NAME_getSimi_postpart(const ABU_NAME_S *pNameUnitLeft,
                                 const ABU_NAME_S *pNameUnitRight)
{/*{{{*/
    ABU_ASSERT(pNameUnitLeft != NULL && pNameUnitRight != NULL);

    ABU_NAME_ATTACH_S *pAttachLeft = pNameUnitLeft->stpNameAttach;
    ABU_NAME_ATTACH_S *pAttachRight = pNameUnitRight->stpNameAttach;
 
    uint16_t leftPart[pAttachLeft->gbAssoPrefixLen + pAttachLeft->gbAssoSuffixLen + pAttachLeft->gbAppendixLen];
    uint16_t rightPart[pAttachRight->gbAssoPrefixLen + pAttachRight->gbAssoSuffixLen + pAttachRight->gbAppendixLen];
    uint16_t leftGBLen = 0;
    uint16_t rightGBLen = 0;

    memcpy(leftPart, pAttachLeft->gbAssoPrefix, pAttachLeft->gbAssoPrefixLen * sizeof(uint16_t));
    leftGBLen += pAttachLeft->gbAssoPrefixLen;
    memcpy(leftPart + leftGBLen, pAttachLeft->gbAssoSuffix, pAttachLeft->gbAssoSuffixLen * sizeof(uint16_t));
    leftGBLen += pAttachLeft->gbAssoSuffixLen;
    memcpy(leftPart + leftGBLen, pAttachLeft->gbAppendix, pAttachLeft->gbAppendixLen * sizeof(uint16_t));
    leftGBLen += pAttachLeft->gbAppendixLen;

    memcpy(rightPart, pAttachRight->gbAssoPrefix, pAttachRight->gbAssoPrefixLen * sizeof(uint16_t));
    rightGBLen += pAttachRight->gbAssoPrefixLen;
    memcpy(rightPart + rightGBLen, pAttachRight->gbAssoSuffix, pAttachRight->gbAssoSuffixLen * sizeof(uint16_t));
    rightGBLen += pAttachRight->gbAssoSuffixLen;
    memcpy(rightPart + rightGBLen, pAttachRight->gbAppendix, pAttachRight->gbAppendixLen * sizeof(uint16_t));
    rightGBLen += pAttachRight->gbAppendixLen;

    return NAME_getSimi_byeditdist(leftPart, leftGBLen, rightPart, rightGBLen); 
}/*}}}*/


static int NAME_getSimi_bylcs(const unsigned short *gbStemLeft,
                              const unsigned int gbStemLeftLen,
                              const unsigned short *gbStemRight,
                              const unsigned int gbStemRightLen)
{/*{{{*/
    if(gbStemLeft == NULL || gbStemRight == NULL)
        return MIN_SIMI_VALUE;
    if(gbStemLeftLen == 0 || gbStemRightLen == 0)
        return MIN_SIMI_VALUE;

    int leftLCSPos = 0, rightLCSPos = 0;
    int stemLCSLen = abstru_getlcs_gb(gbStemLeft,gbStemLeftLen,
            gbStemRight,gbStemRightLen,&leftLCSPos,&rightLCSPos);
    if(stemLCSLen < 2)
        return MIN_SIMI_VALUE;
    return MAX_SIMI_VALUE * 2 * stemLCSLen / (gbStemLeftLen + gbStemRightLen);
}/*}}}*/


static int NAME_getSimi_byeditdist(const unsigned short *gbFieldLeft,
                               const unsigned int gbFieldLeftLen,
                               const unsigned short *gbFieldRight,
                               const unsigned int gbFieldRightLen)
{/*{{{*/
    if(gbFieldLeft == NULL || gbFieldRight == NULL)
        return MIN_SIMI_VALUE;
    if(gbFieldLeftLen == 0 || gbFieldRightLen == 0)
        return MIN_SIMI_VALUE;
    
    int editDist = abstru_geteditdist_gb(gbFieldLeft,gbFieldLeftLen,
            gbFieldRight,gbFieldRightLen,ADD_WEIGHT,DELETE_WEIGHT,SUBS_WEIGHT);
    int totalLen = gbFieldLeftLen + gbFieldRightLen;        
    editDist = MIN(editDist, totalLen);
    return MAX_SIMI_VALUE - MAX_SIMI_VALUE * editDist / totalLen;
}/*}}}*/


static int normalize_simi(const int simi)
{/*{{{*/
    if(simi > MAX_SIMI_VALUE)
    {
        fprintf(stderr, "normalize simi error:simi > 100\n");
        return MAX_SIMI_VALUE;
    }
    return simi;    
}/*}}}*/


void get_dynamicStem(const ABU_NAME_LIB_S *stpNameLib,
                     const ABU_NAME_S *pNameUnitLeft,
                     const ABU_NAME_S *pNameUnitRight,
                     DynamicStem *pDynamicStem)
{/*{{{*/
    ABU_ASSERT(stpNameLib != NULL && pNameUnitLeft != NULL && pNameUnitRight != NULL);
    ABU_NAME_ATTACH_S *pAttachLeft = pNameUnitLeft->stpNameAttach;
    ABU_NAME_ATTACH_S *pAttachRight = pNameUnitRight->stpNameAttach;
    ABU_NAME_RECORD_S *pRecordLeft = pNameUnitLeft->stpNameRecord;
    ABU_NAME_RECORD_S *pRecordRight = pNameUnitRight->stpNameRecord;

    char szStemLeft[NAME_LEN_MAX + 1];
    char szStemRight[NAME_LEN_MAX + 1];    
    memset(szStemLeft, 0, sizeof(szStemLeft));
    memset(szStemRight, 0, sizeof(szStemRight));
    strcpy(szStemLeft, pRecordLeft->szStem);
    strcpy(szStemRight, pRecordRight->szStem);

    ABU_WORDS_SEGRESULT_SL_S * stpSegLeft = ab_segmenter_seg(szStemLeft,
            strlen(szStemLeft),stpNameLib->stpSegDict,stpNameLib->stpSegLib);
    ABU_WORDS_SEGRESULT_SL_S * stpSegRight = ab_segmenter_seg(szStemRight,
            strlen(szStemRight),stpNameLib->stpSegDict,stpNameLib->stpSegLib);
    if(stpSegLeft && stpSegRight)
    {
        char *szFirstLeft = (stpSegLeft->pWordArray + 0)->szWord;
        char *szFirstRight = (stpSegRight->pWordArray + 0)->szWord;
        unsigned char isCityLeft = FALSE;
        unsigned char isCityRight = FALSE;        
        if(ABU_MAPTREE_isProvince(stpNameLib->stpMapTree, szFirstLeft) ||
           ABU_MAPTREE_isCity(stpNameLib->stpMapTree, szFirstLeft))
        { 
            isCityLeft = TRUE; 
        }
        if(ABU_MAPTREE_isProvince(stpNameLib->stpMapTree, szFirstRight) ||
           ABU_MAPTREE_isCity(stpNameLib->stpMapTree, szFirstRight))
        {
            isCityRight = TRUE;
        }
        if(isCityLeft || isCityRight)
        {
            if(isCityLeft && isCityRight)
            {
                if(abstru_endswith(szFirstLeft, szFirstRight) || abstru_endswith(szFirstRight, szFirstLeft))
                {
                    abstru_lstrip_str(szStemLeft, szFirstLeft);
                    abstru_lstrip_str(szStemRight, szFirstRight);
                }                
            }
            else
            {
                if(isCityLeft)
                {
                    abstru_lstrip_str(szStemLeft, szFirstLeft);
                }
                if(isCityRight)
                {
                    abstru_lstrip_str(szStemRight, szFirstRight);
                }
            }            
        }        
    }

    if(strcmp(szStemLeft, pRecordLeft->szStem) != 0)
    { 
        abgbstring * gbstring = abgbstring_new(szStemLeft);
        pDynamicStem->gbStemLenLeft = gbstring->len;
        memcpy(pDynamicStem->gbStemLeft, gbstring->gb_str, gbstring->len * sizeof(uint16_t));
        abgbstring_free(gbstring, TRUE);
    }
    else
    {
        pDynamicStem->gbStemLenLeft = pAttachLeft->gbStemLen;
        memcpy(pDynamicStem->gbStemLeft, pAttachLeft->gbStem, pAttachLeft->gbStemLen * sizeof(uint16_t));
    }

    if(strcmp(szStemRight, pRecordRight->szStem) != 0)
    { 
        abgbstring * gbstring = abgbstring_new(szStemRight);
        pDynamicStem->gbStemLenRight = gbstring->len;
        memcpy(pDynamicStem->gbStemRight, gbstring->gb_str, gbstring->len * sizeof(uint16_t));
        abgbstring_free(gbstring, TRUE);
    }
    else
    {
        pDynamicStem->gbStemLenRight = pAttachRight->gbStemLen;
        memcpy(pDynamicStem->gbStemRight, pAttachRight->gbStem, pAttachRight->gbStemLen * sizeof(uint16_t));
    }

    ab_segmenter_free(stpSegLeft);
    stpSegLeft = NULL;
    ab_segmenter_free(stpSegRight);
    stpSegRight = NULL;
}

#if 0
static unsigned char NAME_isLCSColl(const unsigned short *gbFieldLeft,
                                    const unsigned int gbFieldLeftLen,
                                    const unsigned short *gbFieldRight,
                                    const unsigned int gbFieldRightLen)
{
    if(gbFieldLeft == NULL || gbFieldRight == NULL)
        return FALSE;
    if(gbFieldLeftLen == 0 || gbFieldRightLen == 0)
        return FALSE;

    int leftLCSPos = 0, rightLCSPos = 0;
    int lcsLen = abstru_getlcs_gb(gbFieldLeft, gbFieldLeftLen,
                                  gbFieldRight, gbFieldRightLen,
                                  &leftLCSPos, &rightLCSPos);
    if(lcsLen < 2)
        return TRUE;

    return FALSE;
}
#endif
