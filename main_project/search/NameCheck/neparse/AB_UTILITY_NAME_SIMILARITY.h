/**
 * @file AB_UTILITY_NAME_SIMILARITY.h
 * @author bfzhang
 * @brief get name similarity
 *  create this file in 2007-11-10
 */

#ifndef AB_UTILITY_NAME_SIMILARITY_H
#define AB_UTILITY_NAME_SIMILARITY_H

#include "AB_UTILITY_NAME.h"

/**
 * test whether two Name Units is collision
 */
BOOL ABU_NAME_isColl(const ABU_NAME_LIB_S *stpNameLib,
                     const ABU_NAME_S *pNameUnitLeft,
                     const ABU_NAME_S *pNameUnitRight);

/** 
 * get similarity for two Name Units 
 */
int ABU_NAME_getSimi(const ABU_NAME_LIB_S *stpNameLib,
                     const ABU_NAME_S *pNameUnitLeft,
                     const ABU_NAME_S *pNameUnitRight);


int ABU_NAME_getStemSimi(const ABU_NAME_LIB_S *stpNameLib,
                     const ABU_NAME_S *pNameUnitLeft,
                     const ABU_NAME_S *pNameUnitRight);
#endif
