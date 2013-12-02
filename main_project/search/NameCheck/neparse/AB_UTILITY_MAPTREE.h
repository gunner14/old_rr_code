/************************************************************
 * Filename: AB_UTILITY_MAPTREE.h
 * 
 * Date:2007--6-09
 * Description: 
 *         define the pulbic interface go Chinese MapTree
*         implement the mapTree in hash linked list
* in currVersion:
*         countryCount = 1
*         provinceCount = 34
*         cityCount = 334
*         countyCount = 2818
* Velocity:
* ABU_MAPTREE_isParent: call 1kw->3s
 * change log
 * (1) create the .c file in 2007-06-09
 * (2) alter the .c file in 2007-08-31
 * **********************************************************/
#ifndef AB_UTILITY_MAPTREE_H_
#define AB_UTILITY_MAPTREE_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "AB_UTILITY_COMMON.h"

typedef struct _ABU_MAPTREE_S ABU_MAPTREE_S;

/*
 * @desc: create the mapTree
 * @return: fail return NULL;
 */
ABU_MAPTREE_S *ABU_MAPTREE_create();


/*
 * @desc: if the addr exists in mapTree
 * @return: succeed return 1;fail reurn 0
 */
unsigned char ABU_MAPTREE_haskey(const ABU_MAPTREE_S *stpMapTree,
                                 const char *szAddr);

/*
 * if county has more than one Cityparent or ProvinceParent,get the count of its parents.
 * if nothing, return 0, else , return the count of its ProvinceParent.
 * fox example,the ProvinceParent of "朝阳区" -> "北京" ,"吉林"
 * */
unsigned int ABU_MAPTREE_getProvinceParentCount(const ABU_MAPTREE_S* stpMapTree,
                                                const char* szAddr);

unsigned int ABU_MAPTREE_getCityParentCount(const ABU_MAPTREE_S* stpMapTree,
                                            const char* szAddr);
/*
 * if the count of the parent is more than 1,use this function to get the list.
 */
void ABU_MAPTREE_getProvinceParentList(const ABU_MAPTREE_S* stpMapTree,
                                       const char* szAddr,
                                       char** szParentList);

void ABU_MAPTREE_getCityParentList(const ABU_MAPTREE_S* stpMapTree,
                                       const char* szAddr,
                                       char** szParentList);

const char *ABU_MAPTREE_getProvinceParent(const ABU_MAPTREE_S *stpMapTree,
                                          const char *szAddr);


const char *ABU_MAPTREE_getCityParent(const ABU_MAPTREE_S *stpMapTree,
                                      const char *szAddr);

/**
 * @retval is province return 1;else return 0
 */
unsigned char ABU_MAPTREE_isProvince(const ABU_MAPTREE_S *stpMapTree,
                                     const char *szAddr);

/**
 * @retval is full province return 1;else return 0
 */
unsigned char ABU_MAPTREE_isFullProvince(const ABU_MAPTREE_S *stpMapTree,
                                         const char *szAddr);

/**
 * @retval is alias province return 1;else return 0
 */
unsigned char ABU_MAPTREE_isAliasProvince(const ABU_MAPTREE_S *stpMapTree,
                                          const char *szAddr);

/**
 * @retval is city return 1;else return 0
 */ 
unsigned char ABU_MAPTREE_isCity(const ABU_MAPTREE_S *stpMapTree,
                                 const char *szAddr);

/**
 * @retval is full city return 1;else return 0
 */ 
unsigned char ABU_MAPTREE_isFullCity(const ABU_MAPTREE_S *stpMapTree,
                                     const char *szAddr);

/**
 * @retval is alis city return 1;else return 0
 */ 
unsigned char ABU_MAPTREE_isAliasCity(const ABU_MAPTREE_S *stpMapTree,
                                      const char *szAddr);

/**
 * @retval is county return 1;else return 0
 */
unsigned char ABU_MAPTREE_isCounty(const ABU_MAPTREE_S *stpMapTree,
                                   const char *szAddr);

/**
 * @retval is full county return 1;else return 0
 */
unsigned char ABU_MAPTREE_isFullCounty(const ABU_MAPTREE_S *stpMapTree,
                                       const char *szAddr);

/**
 * @retval is alias county return 1;else return 0
 */
unsigned char ABU_MAPTREE_isAliasCounty(const ABU_MAPTREE_S *stpMapTree,
                                        const char *szAddr);

/**
 * @retval success return 1;else return 0
 */
unsigned char ABU_MAPTREE_isParent(const ABU_MAPTREE_S *stpMapTree,
                                   const char *szParentAddr,
                                   const char *szChildAddr);

/**
 * @retval success return 1;else return 0
 * @param province city county can be NULL
 */
unsigned char ABU_MAPTREE_isRightPCC(const ABU_MAPTREE_S *stpMapTree,
                                     const char *szProvince, 
                                     const char *szCity,
                                     const char *szCounty);

void ABU_MAPTREE_destroy(ABU_MAPTREE_S *stpMapTree);


/**
 * @param stpMapTree the pointer of mapTree
 * @brief define the addr is ambiguous addr or not
 * @retval is ambiguous addr return 1;else return 0;
 */
unsigned char ABU_MAPTREE_isAmbiAddr(const ABU_MAPTREE_S *stpMapTree,
                                     const char *szAddr);


/**
 * @param stpMapTree the pointer of mapTree
 * @param szAddr can be NULL
 * @brief  get the full name of province
 *         if(szAddr is province)
 *            return full-province(新疆->新疆维吾尔自治区)
 *         else
 *            return NULL
 */
const char *ABU_MAPTREE_getFullProvince(const ABU_MAPTREE_S *stpMapTree,
                                        const char *szAddr);

/**
 * @param stpMapTree the pointer of mapTree
 * @param szAddr can be NULL
 * @param parent can be NULL
 * @brief  get the full name of city
 *         if(szAddr is city)
 *            return full-city(南京->南京市)
 *         else
 *            return NULL
 *  use parent to remove ambiguous
 */
const char *ABU_MAPTREE_getFullCity(const ABU_MAPTREE_S *stpMapTree,
                                    const char *szAddr,
                                    const char *szParent);

/**
 * @param stpMapTree the pointer of mapTree
 * @param szAddr can be NULL
 * @param szParent can be NULL
 * @brief  get the full name of county
 *         if(szAddr is city)
 *            return full-city(南京->南京市)
 *         else
 *            return NULL
 *  use parent to remove ambiguous
 */
const char *ABU_MAPTREE_getFullCounty(const ABU_MAPTREE_S *stpMapTree,
                                      const char *szAddr,
                                      const char *szParent);

/*
 * for debugging
 * @param stpMapTree the pointer of maptTree
 * @param fp the output file pointer
 * @brief  if szAddr exists in mapTree,
 *         then display all the child of this addr,and print the mapTree
 * @retval szAddr exists in mapTree,return 0;else return -1
 */
int ABU_MAPTREE_displayMapTree(const ABU_MAPTREE_S *stpMapTree,
                               const char *szAddr,
                               FILE *fp);


/*
 * for debugging
 * @brief get statistical info of mapTree
 */
void ABU_MAPTREE_statMapTree(const ABU_MAPTREE_S * stpMapTree,
                             FILE *fp);

/**
 * for debugging
 * @brief get the ambiguous info for addr
 */
void ABU_MAPTREE_statAmbiInfo(const ABU_MAPTREE_S *stpMapTree,
                              const char *szAddr,
                              FILE  *fp);

#endif /*AB_UTILITY_MAPTREE_H_*/
