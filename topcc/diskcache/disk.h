/*
 * disk.h
 *
 *  Created on: May 13, 2012
 *      Author: root
 *      Copyright: renren-inc
 */

#ifndef TOPCC_DISKCACHE_DISK_H_
#define TOPCC_DISKCACHE_DISK_H_

#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include "diskcache/common_struct.h"

class DiskManager;
class DiskLevel;
class DiskFile;

extern "C" uint32_t refcount_incr32(uint32_t *refcount);
extern "C" uint32_t refcount_decr32(uint32_t *refcount);
extern "C" void DiskFileBlockAddrRefPtrIncRef(DiskFileBlockAddrRefPtr* ptr);
extern "C" void DiskFileBlockAddrRefPtrDecRef(DiskFileBlockAddrRefPtr* ptr);

template<typename IntegerType>
IntegerType RoundUpTo(IntegerType value, IntegerType align);

class ScopedLock {
 public:
  explicit ScopedLock(pthread_mutex_t* lock)
      : lock_(lock) {
    pthread_mutex_lock(lock_);
  }
  ~ScopedLock() {
    pthread_mutex_unlock(lock_);
  }
 private:
  pthread_mutex_t* lock_;
};

class DiskManager {
 public:
  DiskManager();
  ~DiskManager();
  bool Init(const DiskLevelConf* disk_level_confs, const unsigned num_confs,
            const unsigned num_threads);
  bool CheckLevelConf(const DiskLevelConf* disk_level_confs,
                      const unsigned num_confs);
  bool Enabled() const;
  DataGetStatus GetData(DiskDataAddr addr, char* buf, const unsigned buf_size,
                        unsigned* vallen);
  bool AppendData(const char *key, KeyLengthType keylen, const char *val,
                  ValueLengthType vallen, rel_time_t exp_time,
                  DiskDataAddr* addr);
  char* AquireBuf();
  void ReleaseBuf(char* buf);
  void RDLock(size_t level, size_t file);
  void WRLock(size_t level, size_t file);
  void UnLock(size_t level, size_t file);

 private:
  std::vector<DiskLevel*> disk_levels_;
  // buffer used in disk get, buffer size is 1M
  std::vector<char*> bufs_;
  sem_t bufs_sem_;
  pthread_mutex_t bufs_lock_;
  // append disk should be sequential
  pthread_mutex_t append_lock_;
  bool enabled_;
};

class DiskLevel {
 public:
  DiskLevel();
  DiskLevel(const DiskLevelConf* conf, size_t level);
  ~DiskLevel();

 public:
  bool HasSpace(size_t space) const;
  DataGetStatus GetData(const DiskDataAddr& addr, char* buf,
                        const unsigned buf_size, unsigned int *vallen);
  bool AppendData(const char *key, KeyLengthType keylen, const char *val,
                  ValueLengthType vallen, rel_time_t exp_time,
                  DiskDataAddr* addr);
  bool KickBlock(bool force);
  static void MoveBlock(DiskLevel* src_level, DiskLevel* dest_level);
  void RDLock(size_t file);
  void WRLock(size_t file);
  void UnLock(size_t file);

 private:
  void AddFile(DiskFile* file);
  void Init();
  void NextBlock(size_t* file_index, size_t* block_index) const;

 private:
  uint8_t level_;
  std::vector<DiskFile*> files_;
  size_t current_file_;
  size_t current_file_block_;
};

class DiskFile {
 public:
  class BlockInfo {
    friend class DiskFile;

   public:
    BlockInfo() {
      expire_time_ = 0;
      block_addr_ref_ptr_ = static_cast<DiskFileBlockAddrRefPtr*>(malloc(
          sizeof(DiskFileBlockAddrRefPtr)));
      memset(block_addr_ref_ptr_, 0, sizeof(DiskFileBlockAddrRefPtr));
      DiskFileBlockAddrRefPtrIncRef(block_addr_ref_ptr_);
    }
    ~BlockInfo() {
      DiskFileBlockAddrRefPtrDecRef(block_addr_ref_ptr_);
    }
    void AddItem(rel_time_t expire_time) {
      expire_time_ = std::max(expire_time_, expire_time);
      DiskFileBlockAddrRefPtrIncRef(block_addr_ref_ptr_);
    }
    void Reset() {
      expire_time_ = 0;
    }

   private:
    rel_time_t expire_time_;
    DiskFileBlockAddrRefPtr* block_addr_ref_ptr_;
  };
  // pack length MUST matches the \kAlign
#pragma pack(2)
  struct DiskDataItem {
    KeyLengthType key_len_;
    CheckCodeType check_code_;
    ValueLengthType value_len_;
    // key and value
    char data[0];
    static const size_t kAlign = 2;

   public:
    static char* KeyBuf(DiskDataItem* item) {
      return item->data;
    }
    static char* ValueBuf(DiskDataItem* item, KeyLengthType key_len) {
      return item->data + RoundUpTo<ValueLengthType>(key_len, kAlign);
    }
    static CheckCodeType CalcCheckCode(KeyLengthType key_len,
                                       ValueLengthType value_len) {
      return CheckCodeType(key_len + value_len);
    }
    static unsigned TotalSize(KeyLengthType key_len,
                              ValueLengthType value_len) {
      return sizeof(DiskDataItem) + RoundUpTo<ValueLengthType>(key_len, kAlign)
          + RoundUpTo<ValueLengthType>(value_len, kAlign);
    }
  };
#pragma pack()

 public:
  DiskFile(const char* path, size_t level, size_t file);
  ~DiskFile();
  void MapWriteBlock(size_t block_index);
  void UnmapWriteBlock();
  char* MapReadBlock(size_t block_index);
  void UnmapReadBlock(char* block);
  void FreeBlockAddrRefPtr(size_t block_index);

  bool HasSpace(size_t space);
  bool BlockExpired(size_t block_index);
  void SetBlockInfo(size_t block_index, const BlockInfo& block_info);
  const BlockInfo& GetBlockInfo(size_t block_index) const;
  char* CurrentBlock();
  void RDLock();
  void WRLock();
  void UnLock();

 public:
  bool AppendData(const char *key, KeyLengthType keylen, const char *val,
                  ValueLengthType vallen, rel_time_t exp_time,
                  DiskDataAddr* addr);
  DataGetStatus GetData(const DiskDataAddr& addr, char* buf,
                        const unsigned buf_size, unsigned int *vallen);

 private:
  // address of each block, it should be read only
  DiskFileBlockAddr* block_addrs_;
  // dynamic information for each block
  BlockInfo* block_infos_;
  char* cur_block_;
  uint64_t level_ :LEVEL_BITS;
  uint64_t file_ :FILE_BITS;
  uint64_t cur_block_index_ :BLOCK_BITS;
  uint64_t offset_in_block_ :OFFSET_BITS;
  int filedes_;
  pthread_rwlock_t rwlock_;

 public:
  static const size_t kFileBlockSize = 16 * 1024 * 1024;
  static const size_t kBlockPerFile = 128;
};

template<typename IntegerType>
inline IntegerType RoundUpTo(IntegerType value, IntegerType align) {
  return (value + align - 1) & ~(align - 1);
}

#endif  // TOPCC_DISKCACHE_DISK_H_
