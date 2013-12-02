/**
 * @file AB_UTILITY_ADDR_SIMILARITY.h
 * @author kliu
 * @brief get addr similarity
 *  create this file in 2007-11-09
 */

#ifndef AB_UTILITY_ADDR_SIMILARITY_H
#define AB_UTILITY_ADDR_SIMILARITY_H

#include "AB_UTILITY_ADDR.h"

BOOL ABU_ADDR_isColl(const ABU_ADDR_S *pAddrUnitLeft,
                     const ABU_ADDR_S *pAddrUnitRight);

int ABU_ADDR_getSimi(const ABU_ADDR_S *pAddrUnitLeft,
                     const ABU_ADDR_S *pAddrUnitRight);

int ABU_ADDR_getSimiByMapGrid(const ABU_ADDR_S *pAddrUnitLeft,
                              const ABU_ADDR_S *pAddrUnitRight,
                              const char *szMapGridLeft,
                              const char *szMapGridRight,
                              const unsigned int accuLeft,
                              const unsigned int accuRight);

BOOL ABU_ADDR_isColl_debug(const ABU_ADDR_S *pAddrUnitLeft,
                           const ABU_ADDR_S *pAddrUnitRight,
                           FILE *fpDebug);

int ABU_ADDR_getSimi_debug(const ABU_ADDR_S *pAddrUnitLeft,
                           const ABU_ADDR_S *pAddrUnitRight,
                           FILE *fpDebug);
#endif
