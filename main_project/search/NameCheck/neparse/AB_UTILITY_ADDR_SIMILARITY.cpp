/**
 * @file AB_UTILITY_ADDR_SIMILARITY.c
 * @author bfzhang kliu
 * @brief get addr similarity
 *  create this file in 2007-11-09
 */
#include "abutility.h"
#include "abstring.h"
//#include "Distance.h"
#include "STRING_UTILITY.h"
#include "AB_UTILITY_ADDR.h"
#include "AB_UTILITY_ADDR_RESOURCE.h"
#include "AB_UTILITY_ADDR_SIMILARITY.h"

#define MAX_SIMI_VALUE 100
#define MIN_SIMI_VALUE 0
#define TOWN_VALUE 5
#define STREET_VALUE 50
#define NUMBER_VALUE 40
#define ADDR_THRESHHOLD 40

#define ADD_WEIGHT 1
#define DELETE_WEIGHT 1
#define SUBS_WEIGHT 2
#define ACCU_TEN 10

/**************************************************************/
/*****private interface declaration****************************/
/**************************************************************/
/*
static int ADDR_getSimi_street(const unsigned short *gbStreetLeft,
                               const unsigned int gbStreetLeftLen,
                               const unsigned short *gbStreetRight,
                               const unsigned int gbStreetRightLen);
*/
static int ADDR_getSimi_otherfield(const unsigned short *gbFieldLeft,
                              const unsigned int gbFieldLeftLen,
                              const unsigned short *gbFieldRight,
                              const unsigned int gbFieldRightLen);
static unsigned char startendwith_eachother(const char *szLeft,
                                            const char *szRight);
/*
static unsigned char ADDR_isStreetColl(const unsigned short *gbStreetLeft,
                                       const unsigned int gbStreetLeftLen,
                                       const unsigned short *gbStreetRight,
                                       const unsigned int gbStreetRightLen);
*/
/*******************************************************************/
/*****public interface implementation*******************************/
/*******************************************************************/
BOOL ABU_ADDR_isColl(const ABU_ADDR_S *pAddrUnitLeft,
                     const ABU_ADDR_S *pAddrUnitRight)
{/*{{{*/
    if(pAddrUnitLeft == NULL || pAddrUnitRight == NULL)
        return FALSE;
    
    ABU_ADDR_RECORD_S *pRecordLeft = pAddrUnitLeft->stpAddrRecord;
    ABU_ADDR_RECORD_S *pRecordRight  = pAddrUnitRight->stpAddrRecord;
    ABU_ADDR_ATTACH_S *pAttachLeft = pAddrUnitLeft->stpAddrAttach;
    ABU_ADDR_ATTACH_S *pAttachRight = pAddrUnitRight->stpAddrAttach;

    if(pAttachLeft->originAddrSign == pAttachRight->originAddrSign) 
        return FALSE;

    if((pRecordLeft->szCity != NULL && pRecordRight->szCity != NULL &&
        pAttachLeft->city4Char != pAttachRight->city4Char ) ||

       (pRecordLeft->szProvince != NULL && pRecordRight->szProvince != NULL &&
        pAttachLeft->province4Char != pAttachRight->province4Char ))
    {     
        return TRUE;
    }

    unsigned char sameStreetNO = FALSE;
    unsigned char streetNOColl = FALSE;
    if(pRecordLeft->szStreetNO != NULL && pRecordRight->szStreetNO != NULL)
    {
        if(pRecordLeft->streetNO == pRecordRight->streetNO)
            sameStreetNO = TRUE;
        else
            ;
            /*streetNOColl = TRUE;*/
    }
    if(streetNOColl)
        return TRUE;
    
    unsigned char sameStreet = FALSE;
    if(pRecordLeft->szStreet != NULL && pRecordRight->szStreet != NULL &&
       strcmp(pRecordLeft->szStreet, pRecordRight->szStreet) == 0)
        sameStreet = TRUE;

    if(pRecordLeft->szCounty != NULL && pRecordRight->szCounty != NULL &&
        pAttachLeft->county4Char != pAttachRight->county4Char)
    {
        if(!sameStreet || !sameStreetNO)
            return TRUE;
    }

    return FALSE;
}/*}}}*/


int ABU_ADDR_getSimi(const ABU_ADDR_S *pAddrUnitLeft,
                     const ABU_ADDR_S *pAddrUnitRight)
{/*{{{*/
    if(pAddrUnitLeft == NULL || pAddrUnitRight == NULL)
        return MIN_SIMI_VALUE;
	
    ABU_ADDR_RECORD_S *pRecordLeft = pAddrUnitLeft->stpAddrRecord;
    ABU_ADDR_RECORD_S *pRecordRight  = pAddrUnitRight->stpAddrRecord;
    ABU_ADDR_ATTACH_S *pAttachLeft = pAddrUnitLeft->stpAddrAttach;
    ABU_ADDR_ATTACH_S *pAttachRight = pAddrUnitRight->stpAddrAttach;

    //if(pAttachLeft->gbBriefAddrLen == 0 || pAttachRight->gbBriefAddrLen == 0)
    //    return MIN_SIMI_VALUE;

    double town_weight = 0.05;
    double street_weight = 0.50;
    double streetno_weight = 0.40;
    double addrleft_weight = 0.05;
    double street2town_dump_weight = 0.8;
    double street2left_dump_weight = 0.9;
    

    double town_simi = ADDR_getSimi_otherfield(pAttachLeft->gbTown, pAttachLeft->gbTownLen,
                                       pAttachRight->gbTown, pAttachRight->gbTownLen);
   
    double street_simi = 0;
    if(pRecordLeft->szStreet != NULL && pRecordRight->szStreet != NULL)
    { 
        int streetFeatGBLen = 0;
        for(int i = 0; i < STREET_FEAT_WORD_COUNT; i++ )
	    {
	        if(abstru_endswith(pRecordLeft->szStreet,STREET_FEAT_WORD[i]) && 
		       abstru_endswith(pRecordRight->szStreet,STREET_FEAT_WORD[i]))
	        {
		        streetFeatGBLen = STREET_FEAT_WORD_GBLEN[i];
			    break;
            }
        }

        street_simi = ADDR_getSimi_otherfield(pAttachLeft->gbStreet,
                                          pAttachLeft->gbStreetLen - streetFeatGBLen,
                                          pAttachRight->gbStreet,
                                          pAttachRight->gbStreetLen- streetFeatGBLen);
        
        const double street_dumping_weight = 0.80;
        if(street_simi < MAX_SIMI_VALUE * street_dumping_weight &&
           startendwith_eachother(pRecordLeft->szStreet, pRecordRight->szStreet))
            street_simi = MAX_SIMI_VALUE * street_dumping_weight;
    }
   
    double streetno_simi = MIN_SIMI_VALUE;
    if(street_simi > 0 &&
       pRecordLeft->szStreetNO != NULL && pRecordRight->szStreetNO != NULL &&
       pRecordLeft->streetNO == pRecordRight->streetNO)
    {
        //streetno_simi = MAX_SIMI_VALUE * street_simi / MAX_SIMI_VALUE;
        if(street_simi == MAX_SIMI_VALUE ||
           startendwith_eachother(pRecordLeft->szStreet, pRecordRight->szStreet))
            streetno_simi = MAX_SIMI_VALUE;
        else
            streetno_simi = street_simi;
    }

    double addrleft_simi = ADDR_getSimi_otherfield(pAttachLeft->gbAddrLeft,
                                                pAttachLeft->gbAddrLeftLen,
                                                pAttachRight->gbAddrLeft,
                                                pAttachRight->gbAddrLeftLen);

    //tuning weight        
    if(pRecordLeft->szStreet == NULL || pRecordRight->szStreet == NULL)
    {
        if(pRecordLeft->szTown != NULL && pRecordRight->szTown != NULL)
            town_weight += street_weight * street2town_dump_weight;
        else if(pRecordLeft->szAddrLeft != NULL && pRecordRight->szAddrLeft != NULL)
        {
            int sumaddrleft_len = pAttachLeft->gbAddrLeftLen + pAttachRight->gbAddrLeftLen;
            int normaddrleft_len = 12;//average 6 gb is enough
            if(sumaddrleft_len > normaddrleft_len)
                sumaddrleft_len = normaddrleft_len;
            addrleft_weight += (street_weight + streetno_weight) * street2left_dump_weight * 
                               sumaddrleft_len / normaddrleft_len;
            if(addrleft_weight < 0.40 &&
               strcmp(pRecordLeft->szAddrLeft, pRecordRight->szAddrLeft) == 0)
                addrleft_weight = 0.40;                
        }
    }
 
    int simi = town_weight * town_simi + street_weight * street_simi + 
               streetno_weight * streetno_simi + addrleft_weight * addrleft_simi + 0.5;
    return simi;
}/*}}}*/


int ABU_ADDR_getSimiByMapGrid(const ABU_ADDR_S *pAddrUnitLeft,
                              const ABU_ADDR_S *pAddrUnitRight,
                              const char *szMapGridLeft,
                              const char *szMapGridRight,
                              const unsigned int accuLeft,
                              const unsigned int accuRight)
{/*{{{*/
	if(szMapGridLeft == NULL || szMapGridRight == NULL) 
        return MIN_SIMI_VALUE;
	if(pAddrUnitLeft == NULL || pAddrUnitRight == NULL) 
        return MIN_SIMI_VALUE;
	
	int addrSimi = 0;
	int distance = 0;// GetDistance((char *)szMapGridLeft,(char *)szMapGridRight);
	
	if(distance == 0)
	{
        int highAccu = MAX(accuLeft, accuRight);
        int lowAccu = MIN(accuLeft, accuRight);    
		
		addrSimi = MAX_SIMI_VALUE;
        
		addrSimi -= (highAccu >= ACCU_TEN)?0:(ACCU_TEN - highAccu) * 10;
		
        addrSimi -= (highAccu - lowAccu) * 10;
	}
	
	return addrSimi;
}/*}}}*/


BOOL ABU_ADDR_isColl_debug(const ABU_ADDR_S *pAddrUnitLeft,
                           const ABU_ADDR_S *pAddrUnitRight,
                           FILE *fpDebug)
{/*{{{*/
    ABU_ASSERT(fpDebug != NULL);

    fprintf(fpDebug,"\n####################################################\n");
    fprintf(fpDebug,"following info for debug ABU_ADDR_isColl\n");
    if(pAddrUnitLeft == NULL || pAddrUnitRight == NULL)
    {
        fprintf(fpDebug,"ABU_ADDR_isColl:FALSE\n");
        if(pAddrUnitLeft == NULL)
            fprintf(fpDebug,"    pAddrUnitLeft = NULL\n");
        if(pAddrUnitRight == NULL)
            fprintf(fpDebug,"    pAddrUnitRight = NULL\n");
        return FALSE;
    }

    ABU_ADDR_RECORD_S *pRecordLeft = pAddrUnitLeft->stpAddrRecord;
    ABU_ADDR_RECORD_S *pRecordRight  = pAddrUnitRight->stpAddrRecord;
    ABU_ADDR_ATTACH_S *pAttachLeft = pAddrUnitLeft->stpAddrAttach;
    ABU_ADDR_ATTACH_S *pAttachRight = pAddrUnitRight->stpAddrAttach;

    fprintf(fpDebug,"pAddrUnitLeft:\n");
    ABU_ADDR_print(pAddrUnitLeft, fpDebug);
    fprintf(fpDebug,"pAddrUnitRight:\n");
    ABU_ADDR_print(pAddrUnitRight, fpDebug);

    if(pAttachLeft->originAddrSign == pAttachRight->originAddrSign)
    {
        fprintf(fpDebug, "ABU_ADDR_isColl:FALSE\n");
        fprintf(fpDebug, "    originAddrSign same\n"); 
        return FALSE;
    }

    if(pRecordLeft->szCity != NULL && pRecordRight->szCity != NULL &&
       pAttachLeft->city4Char != pAttachRight->city4Char)
    {
        fprintf(fpDebug, "ABU_ADDR_isColl:TRUE\n");
        fprintf(fpDebug, "    city collision\n"); 
        return TRUE;
    }

    if(pRecordLeft->szProvince != NULL && pRecordRight->szProvince != NULL &&
       pAttachLeft->province4Char != pAttachRight->province4Char)
    {
        fprintf(fpDebug, "ABU_ADDR_isColl:TRUE\n");
        fprintf(fpDebug, "    province collision\n"); 
        return TRUE;
    }
    
    unsigned char sameStreetNO = FALSE;
    unsigned char streetNOColl = FALSE;
    if(pRecordLeft->szStreetNO != NULL && pRecordRight->szStreetNO != NULL)
    {
        if(pRecordLeft->streetNO == pRecordRight->streetNO)
            sameStreetNO = TRUE;
        else
            streetNOColl = TRUE;
    }
    if(streetNOColl)
    {
        fprintf(fpDebug, "ABU_ADDR_isColl:TRUE\n");
        fprintf(fpDebug, "    streetNO collision\n");
        return TRUE;
    }
    
    unsigned char sameStreet = FALSE;
    if(pRecordLeft->szStreet != NULL && pRecordRight->szStreet != NULL &&
       strcmp(pRecordLeft->szStreet, pRecordRight->szStreet) == 0)
        sameStreet = TRUE;

    if(pRecordLeft->szCounty != NULL && pRecordRight->szCounty != NULL &&
        pAttachLeft->county4Char != pAttachRight->county4Char)
    {
        if(!sameStreet || !sameStreetNO)
        {
            fprintf(fpDebug, "ABU_ADDR_isColl:TRUE\n");
            fprintf(fpDebug, "    county collision\n");
            return TRUE;
        }
    }

    /*
    if(pRecordLeft->szStreet != NULL && pRecordRight->szStreet != NULL && !sameStreet) 
    {
        if(ADDR_isStreetColl(pAttachLeft->gbStreet, pAttachLeft->gbStreetLen,
                         pAttachRight->gbStreet, pAttachRight->gbStreetLen))
        {
            fprintf(fpDebug, "ABU_ADDR_isColl:\n");
            fprintf(fpDebug, "    street collision\n");
            return TRUE;
        }

    }
    */

    fprintf(fpDebug, "ABU_ADDR_isColl:FALSE\n");
    fprintf(fpDebug, "###################################################\n\n");
    return FALSE;
}/*}}}*/


int ABU_ADDR_getSimi_debug(const ABU_ADDR_S *pAddrUnitLeft,
                           const ABU_ADDR_S *pAddrUnitRight,
                           FILE *fpDebug)
{/*{{{*/
    ABU_ASSERT(fpDebug != NULL);

    fprintf(fpDebug,"#######################################################\n");
    fprintf(fpDebug,"following info for debug ABU_ADDR_getSimi\n");
	
    if(pAddrUnitLeft == NULL || pAddrUnitRight == NULL)
    {
        fprintf(fpDebug, "ABU_ADDR_getSimi:%d\n", MIN_SIMI_VALUE);
        if(pAddrUnitLeft == NULL)
            fprintf(fpDebug,"    pAddrUnitLeft = NULL\n");
        if(pAddrUnitRight == NULL)
            fprintf(fpDebug,"    pAddrUnitRight = NULL\n");
        return MIN_SIMI_VALUE;
    }
    ABU_ADDR_RECORD_S *pRecordLeft = pAddrUnitLeft->stpAddrRecord;
    ABU_ADDR_RECORD_S *pRecordRight  = pAddrUnitRight->stpAddrRecord;
    ABU_ADDR_ATTACH_S *pAttachLeft = pAddrUnitLeft->stpAddrAttach;
    ABU_ADDR_ATTACH_S *pAttachRight = pAddrUnitRight->stpAddrAttach;

    /*
    if(pAttachLeft->gbBriefAddrLen == 0 || pAttachRight->gbBriefAddrLen == 0)
    { 
        fprintf(fpDebug, "ABU_ADDR_getSimi:%d\n", MIN_SIMI_VALUE);
        if(pAttachLeft->gbBriefAddrLen == 0)
            fprintf(fpDebug, "pAttachLeft->gbBriefAddrLen = 0\n");
        if(pAttachRight->gbBriefAddrLen == 0)
            fprintf(fpDebug, "pAttachRight->gbBriefAddrLen = 0\n");
        return MIN_SIMI_VALUE;
    }
    */

    fprintf(fpDebug,"pAddrUnitLeft:\n");
    ABU_ADDR_print(pAddrUnitLeft, fpDebug);
    fprintf(fpDebug,"pAddrUnitRight:\n");
    ABU_ADDR_print(pAddrUnitRight, fpDebug);

    double town_weight = 0.05;
    double street_weight = 0.50;
    double streetno_weight = 0.40;
    double addrleft_weight = 0.05;
    double street2town_dump_weight = 0.8;
    double street2left_dump_weight = 0.9;
    
    fprintf(fpDebug, "init_weight:\n");
    fprintf(fpDebug, "    town_weight:%.2f street_weight:%.2f", 
                     town_weight, street_weight);
    fprintf(fpDebug, "streetno_weight:%.2f addrleft_weight:%.2f\n",
                     streetno_weight, addrleft_weight);
    fprintf(fpDebug, "street2town_dump_weight:%.2f street2left_dump_weight:%.2f\n",
                    street2town_dump_weight, street2left_dump_weight);

    double town_simi = ADDR_getSimi_otherfield(pAttachLeft->gbTown, pAttachLeft->gbTownLen,
                                       pAttachRight->gbTown, pAttachRight->gbTownLen);
   
    double street_simi = 0;
    if(pRecordLeft->szStreet != NULL && pRecordRight->szStreet != NULL)
    { 
        int streetFeatGBLen = 0;
        for(int i = 0; i < STREET_FEAT_WORD_COUNT; i++ )
	    {
	        if(abstru_endswith(pRecordLeft->szStreet,STREET_FEAT_WORD[i]) && 
		       abstru_endswith(pRecordRight->szStreet,STREET_FEAT_WORD[i]))
	        {
		        streetFeatGBLen = STREET_FEAT_WORD_GBLEN[i];
			    break;
            }
        }

        street_simi = ADDR_getSimi_otherfield(pAttachLeft->gbStreet,
                                          pAttachLeft->gbStreetLen - streetFeatGBLen,
                                          pAttachRight->gbStreet,
                                          pAttachRight->gbStreetLen- streetFeatGBLen);
        const double street_dumping_weight = 0.80;
        if(street_simi < MAX_SIMI_VALUE * street_dumping_weight &&
           startendwith_eachother(pRecordLeft->szStreet, pRecordRight->szStreet))
            street_simi = MAX_SIMI_VALUE * street_dumping_weight;
    }
   
    double streetno_simi = MIN_SIMI_VALUE;
    if(street_simi > 0 &&
       pRecordLeft->szStreetNO != NULL && pRecordRight->szStreetNO != NULL &&
       pRecordLeft->streetNO == pRecordRight->streetNO)
    {
        //streetno_simi = MAX_SIMI_VALUE * street_simi / MAX_SIMI_VALUE;
        if(street_simi == MAX_SIMI_VALUE ||
           startendwith_eachother(pRecordLeft->szStreet, pRecordRight->szStreet))
            streetno_simi = MAX_SIMI_VALUE;
        else
            streetno_simi = street_simi;
    }

    double addrleft_simi = ADDR_getSimi_otherfield(pAttachLeft->gbAddrLeft,
                                                pAttachLeft->gbAddrLeftLen,
                                                pAttachRight->gbAddrLeft,
                                                pAttachRight->gbAddrLeftLen);

    //tuning weight        
    if(pRecordLeft->szStreet == NULL || pRecordRight->szStreet == NULL)
    {
        if(pRecordLeft->szTown != NULL && pRecordRight->szTown != NULL)
            town_weight += street_weight * street2town_dump_weight;
        else if(pRecordLeft->szAddrLeft != NULL && pRecordRight->szAddrLeft != NULL)
        {
            int sumaddrleft_len = pAttachLeft->gbAddrLeftLen + pAttachRight->gbAddrLeftLen;
            int normaddrleft_len = 12;//average 6 gb is enough
            if(sumaddrleft_len > normaddrleft_len)
                sumaddrleft_len = normaddrleft_len;
            addrleft_weight += (street_weight + streetno_weight) * street2left_dump_weight * 
                               sumaddrleft_len / normaddrleft_len;
            if(addrleft_weight < 0.40 &&
               strcmp(pRecordLeft->szAddrLeft, pRecordRight->szAddrLeft) == 0)
                addrleft_weight = 0.40;                
        }
    }

    int simi = town_weight * town_simi + street_weight * street_simi + 
               streetno_weight * streetno_simi + addrleft_weight * addrleft_simi + 0.5;
    
    fprintf(fpDebug, "post_weight:\n");
    fprintf(fpDebug, "    town_weight:%.2f street_weight:%.2f ", 
                     town_weight, street_weight);
    fprintf(fpDebug, "streetno_weight:%.2f addrleft_weight:%.2f\n",
                     streetno_weight, addrleft_weight);
    fprintf(fpDebug, "field_simi:\n");
    fprintf(fpDebug, "    town_simi:%.1f street_simi:%.1f ", town_simi, street_simi);
    fprintf(fpDebug, "streetno_simi:%.1f addrleft_simi:%.1f\n", streetno_simi, addrleft_simi);
    fprintf(fpDebug, "formula:simi = town_weight * town_simi + street_weight * street_simi");
    fprintf(fpDebug, "streetno_weight * streetno_simi + addrleft_weight * addrleft_simi\n");
    fprintf(fpDebug, "result:%d = %.2f * %.1f + %.2f * %.1f + %.2f * %.1f + %.2f * %.1f\n",
                     simi, town_weight, town_simi, street_weight, street_simi, 
                     streetno_weight, streetno_simi, addrleft_weight, addrleft_simi); 
    fprintf(fpDebug,"#######################################################\n");

    return simi;
}/*}}}*/
/*******************************************************************/
/*****private interface implementation******************************/
/*******************************************************************/
/*
static int ADDR_getSimi_street(const unsigned short *gbStreetLeft,
                               const unsigned int gbStreetLeftLen,
                               const unsigned short *gbStreetRight,
                               const unsigned int gbStreetRightLen)
{
    if(gbStreetLeft == NULL || gbStreetRight == NULL)
        return MIN_SIMI_VALUE;
    if(gbStreetLeftLen == 0 || gbStreetRightLen == 0)
        return MIN_SIMI_VALUE;

    
    int leftLCSPos = 0, rightLCSPos = 0;
    int streetLCSLen = abstru_getlcs_gb(gbStreetLeft, gbStreetLeftLen,
            gbStreetRight, gbStreetRightLen, &leftLCSPos, &rightLCSPos);
    if(streetLCSLen < 2 )
    {
        if(streetLCSLen == 1 && gbStreetLeftLen == 1 && gbStreetRightLen == 1)
            return MAX_SIMI_VALUE;
        else
            return MIN_SIMI_VALUE;
    }
    return MAX_SIMI_VALUE * 2 * streetLCSLen / (gbStreetLeftLen + gbStreetRightLen);    
}
*/


static int ADDR_getSimi_otherfield(const unsigned short *gbFieldLeft,
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


static unsigned char startendwith_eachother(const char *szLeft,
                                            const char *szRight)
{/*{{{{*/
    ABU_ASSERT(szLeft != NULL && szRight != NULL);
    if(abstru_startswith(szLeft, szRight) ||
       abstru_startswith(szRight, szLeft) ||
       abstru_endswith(szLeft, szRight) ||
       abstru_endswith(szRight, szLeft))
        return TRUE;
    return FALSE;    
}/*}}}}*/

/*
static unsigned char ADDR_isStreetColl(const unsigned short *gbStreetLeft,
                                       const unsigned int gbStreetLeftLen,
                                       const unsigned short *gbStreetRight,
                                       const unsigned int gbStreetRightLen)
{
    if(gbStreetLeft == NULL || gbStreetRight == NULL)
        return FALSE;
    if(gbStreetLeftLen == 0 || gbStreetRightLen == 0)
        return FALSE;
    
    int leftLCSPos = 0, rightLCSPos = 0;
    int lcsLen = abstru_getlcs_gb(gbStreetLeft, gbStreetLeftLen,
            gbStreetRight, gbStreetRightLen, &leftLCSPos, &rightLCSPos);
    if(lcsLen < 2)
        return TRUE;
    return FALSE;
}*/
