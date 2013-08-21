/*
 * diskmanage.h
 *
 *  Created on: May 14, 2012
 *      Author: root
 *      Copyright: renren-inc
 */

#ifndef TOPCC_DISKCACHE_DISKMANAGE_H_
#define TOPCC_DISKCACHE_DISKMANAGE_H_
#include <pthread.h>
#include "diskcache/common_struct.h"

extern "C" {
void BlockAddrRefPtrFree(DiskFileBlockAddrRefPtr* ptr);

// Synchronize
void DiskManagerRDLock(size_t level, size_t file);
void DiskManagerWRLock(size_t level, size_t file);
void DiskManagerUnLock(size_t level, size_t file);
char* DiskManagerAquireBuf();
void DiskManagerReleaseBuf(char* buf);

bool DiskManagerInit(const DiskLevelConf* confs, unsigned num_confs,
                     unsigned num_threads);
bool DiskManagerEnabled();

// Get/Append
enum DataGetStatus DiskManagerGet(const DiskDataAddr addr, char* buf,
                                  const unsigned buf_size, unsigned* vallen);
bool DiskManagerAppend(const char *key, KeyLengthType keylen, const char *val,
                       ValueLengthType vallen, rel_time_t exp_time,
                       DiskDataAddr* addr);
}

#endif  // TOPCC_DISKCACHE_DISKMANAGE_H_
