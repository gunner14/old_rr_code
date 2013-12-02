/**
 * @file tcc2scc.h
 * @author dairui.cui
 * @brief transfer traditional chinese character to simple chinese character.
 */
#ifndef TCC2SCC_H_20120423
#define TCC2SCC_H_20120423

#include "HASHMAP.h"

HashMapPtr initTCC2SCCMap();
void destoryTCC2SCCMap(HashMapPtr ptrWordMap);
void tcc2scc_gbk(char* szScc,const char* szTcc,HashMapPtr ptrWordMap);

#endif
