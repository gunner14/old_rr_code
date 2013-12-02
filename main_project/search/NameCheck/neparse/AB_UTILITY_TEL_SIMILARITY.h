/**
 * @file AB_UTILITY_TEL_SIMILARITY.h
 * @author kliu
 * @brief get tel similarity
 *  create this file in 2007-11-09
 */

#ifndef AB_UTILITY_TEL_SIMILARITY_H
#define AB_UTILITY_TEL_SIMILARITY_H

#include "AB_UTILITY_COMMON.h"
#include "AB_UTILITY_TEL.h"
#include "AB_UTILITY_TEL_RECOGNISE.h"

BOOL ABU_TEL_isColl(const ABU_TEL_S *pTelUnitLeft,
                    const ABU_TEL_S *pTelUnitRight);

int ABU_TEL_getSimi(const ABU_TEL_S *pTelUnitLeft,
                    const ABU_TEL_S *pTelUnitRight);

BOOL ABU_TEL_RECOG_isColl(const ABU_TEL_RECOG_S *pTelUnitLeft,
                          const ABU_TEL_RECOG_S *pTelUnitRight);

int ABU_TEL_RECOG_getSimi(const ABU_TEL_RECOG_S *pTelUnitLeft,
                          const ABU_TEL_RECOG_S *pTelUnitRight);
#endif

