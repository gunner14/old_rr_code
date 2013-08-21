/*
 * diskmanage.cc
 *
 *  Created on: May 14, 2012
 *      Author: root
 *      Copyright: renren-inc
 */
#include "diskcache/diskmanage.h"

#include <stdio.h>
#include <stdlib.h>
#include "diskcache/common_struct.h"
#include "diskcache/disk.h"
#include "base3/logging.h"
#include "base3/ptime.h"

const DiskFileBlockAddr kInvalidDiskFileBlockAddr = { 0 };

DiskManager gDiskManager;

void DiskManagerRDLock(size_t level, size_t file) {
  gDiskManager.RDLock(level, file);
}
void DiskManagerWRLock(size_t level, size_t file) {
  gDiskManager.WRLock(level, file);
}
void DiskManagerUnLock(size_t level, size_t file) {
  gDiskManager.UnLock(level, file);
}
char* DiskManagerAquireBuf() {
  return gDiskManager.AquireBuf();
}
void DiskManagerReleaseBuf(char* buf) {
  gDiskManager.ReleaseBuf(buf);
}
bool DiskManagerEnabled() {
  return gDiskManager.Enabled();
}
bool DiskManagerInit(const DiskLevelConf* disk_level_confs, unsigned num_confs,
                     unsigned num_threads) {
  return gDiskManager.Init(disk_level_confs, num_confs, num_threads);
}

bool DiskManagerAppend(const char *key, KeyLengthType keylen, const char *val,
                       ValueLengthType vallen, rel_time_t exp_time,
                       DiskDataAddr* addr) {
  PTIME(pt, "DiskAppend", true, 0);
  return gDiskManager.AppendData(key, keylen, val, vallen, exp_time, addr);
}
DataGetStatus DiskManagerGet(const DiskDataAddr addr, char* buf,
                             const unsigned buf_size, unsigned int *vallen) {
  char ptbuf[32];
  snprintf(ptbuf, sizeof(ptbuf), "DiskGet: level(%d)",
           addr.block_addr_->data_->level_);
  PTIME(pt, ptbuf, true, 0);
  return gDiskManager.GetData(addr, buf, buf_size, vallen);
}

void DiskFileBlockAddrRefPtrDecRef(DiskFileBlockAddrRefPtr* ptr) {
#ifdef DEBUG_CACHE
  LOG(INFO) << "DecRefPtr: " << ptr << ", refcount " << ptr->refcount;
#endif
  if (refcount_decr32(&ptr->refcount) == 0) {
    LOG(INFO) << "DelRefPtr: " << ptr;
    free(ptr);
  }
}
void DiskFileBlockAddrRefPtrIncRef(DiskFileBlockAddrRefPtr* ptr) {
#ifdef DEBUG_CACHE
  LOG(INFO) << "IncRefPtr: " << ptr << ", refcount " << ptr->refcount;
#endif
  if (ptr->refcount == 0) {
    LOG(INFO) << "NewRefPtr: " << ptr;
  }
  refcount_incr32(&ptr->refcount);
}
