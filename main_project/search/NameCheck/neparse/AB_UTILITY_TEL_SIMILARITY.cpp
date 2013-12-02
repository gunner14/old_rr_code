/**
 * @file AB_UTILITY_TEL_SIMILARITY.c
 * @author kliu
 * @brief get tel similarity
 *  create this file in 2007-11-09
 */
#include "abstring.h"
#include "abutility.h"
#include "AB_UTILITY_TEL.h"
#include "AB_UTILITY_TEL_RESOURCE.h"
#include "AB_UTILITY_TEL_SIMILARITY.h"

static const int MAX_SIMI_VALUE = 100;
static const int MIN_SIMI_VALUE = 0;


BOOL ABU_TEL_isColl(const ABU_TEL_S *pTelUnitLeft,
                    const ABU_TEL_S *pTelUnitRight)
{/*{{{*/
    if(pTelUnitLeft == NULL || pTelUnitRight == NULL)
        return FALSE;

    ABU_TEL_RECORD_S *pRecordLeft = pTelUnitLeft->stpTelRecord;
    ABU_TEL_RECORD_S *pRecordRight = pTelUnitRight->stpTelRecord;

    if(pRecordLeft->szCountryCode != NULL && 
       pRecordRight->szCountryCode != NULL &&
       strcmp(pRecordLeft->szCountryCode, pRecordRight->szCountryCode) != 0)
        return TRUE;

    if(pRecordLeft->szCityCode != NULL &&
       pRecordRight->szCityCode != NULL &&
       strcmp(pRecordLeft->szCityCode, pRecordRight->szCityCode) != 0)
        return TRUE;

    return FALSE;

}/*}}}*/


int ABU_TEL_getSimi(const ABU_TEL_S *pTelUnitLeft,
                    const ABU_TEL_S *pTelUnitRight)
{/*{{{*/
    if(pTelUnitLeft == NULL || pTelUnitRight == NULL)
        return MIN_SIMI_VALUE;
	
    ABU_TEL_RECORD_S *pRecordLeft = pTelUnitLeft->stpTelRecord;
    ABU_TEL_ATTACH_S *pAttachLeft = pTelUnitLeft->stpTelAttach;
    ABU_TEL_RECORD_S *pRecordRight = pTelUnitRight->stpTelRecord;
    ABU_TEL_ATTACH_S *pAttachRight = pTelUnitRight->stpTelAttach;

	int leftLen = pAttachLeft->telLen;
	int rightLen = pAttachRight->telLen;

    int commonHeadLen = abstru_getcommonhead(pRecordLeft->szTel, 
                                             pRecordRight->szTel); 

	return ( 2 * commonHeadLen * MAX_SIMI_VALUE)/(leftLen + rightLen);
}/*}}}*/


BOOL ABU_TEL_RECOG_isColl(const ABU_TEL_RECOG_S *pTelUnitLeft,
                          const ABU_TEL_RECOG_S *pTelUnitRight)
{/*{{{*/
    if(pTelUnitLeft == NULL || pTelUnitRight == NULL)
        return FALSE;

    for(int i = 0;i < pTelUnitLeft->uiTelCount;i++)
    {
        for(int j = 0;j < pTelUnitRight->uiTelCount;j++)
        {
            if(ABU_TEL_isColl(pTelUnitLeft->stpTelArray[i], 
                              pTelUnitRight->stpTelArray[j]) == TRUE)
                return TRUE;
        }
    }
	
	return FALSE;
}/*}}}*/


int ABU_TEL_RECOG_getSimi(const ABU_TEL_RECOG_S *pTelUnitLeft,
                          const ABU_TEL_RECOG_S *pTelUnitRight)
{/*{{{*/
    if(pTelUnitLeft == NULL || pTelUnitRight == NULL)
        return MIN_SIMI_VALUE;       
	
    int bestTelSimi = 0;
	
    for(int i = 0;i < pTelUnitLeft->uiTelCount;i++)
    {
        for(int j = 0;j < pTelUnitRight->uiTelCount;j++)
        {
            int telSimi = ABU_TEL_getSimi(pTelUnitLeft->stpTelArray[i],
                                          pTelUnitRight->stpTelArray[j]);
            bestTelSimi = MAX(bestTelSimi, telSimi);                
        }        
    }
    return bestTelSimi;
}/*}}}*/
