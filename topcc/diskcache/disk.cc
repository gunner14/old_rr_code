/*
 * disk.cc
 *
 *  Created on: May 13, 2012
 *      Author: root
 *      Copyright: renren-inc
 */

#include "diskcache/disk.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include <vector>
#include "base3/logging.h"
#include "base3/ptime.h"

extern rel_time_t current_time;
extern bool gVerbose;
extern char gLogFile[];

/**
 * Disk Manager
 */
DiskManager::DiskManager()
    : enabled_(false) {
  pthread_mutex_init(&bufs_lock_, NULL);
  pthread_mutex_init(&append_lock_, NULL);
  sem_init(&bufs_sem_, 0, 0);
}
DiskManager::~DiskManager() {
  if (enabled_) {
    for (size_t i = 0; i < disk_levels_.size(); ++i) {
      delete disk_levels_[i];
    }
    for (size_t i = 0; i < bufs_.size(); ++i) {
      delete[] bufs_[i];
    }
  }
  pthread_mutex_destroy(&bufs_lock_);
  pthread_mutex_destroy(&append_lock_);
  sem_destroy(&bufs_sem_);
}
bool DiskManager::CheckLevelConf(const DiskLevelConf* disk_level_confs,
                                 const unsigned num_confs) {
  const size_t file_size = DiskFile::kFileBlockSize * DiskFile::kBlockPerFile;
  if (num_confs == 0) {
    LOG(INFO) << "No disk configured";
    return false;
  }
  for (unsigned i = 0; i < num_confs; ++i) {
    bool ok = false;
    const DiskLevelConf& conf = disk_level_confs[i];
    for (size_t j = 0; j < conf.size; ++j) {
      const DiskLevelDir* dir = conf.dirs[j];
      if (dir->size >= file_size) {
        ok = true;
        break;
      }
    }
    if (!ok) {
      LOG(ERROR) << "CheckLevelConf error: level " << i << " is too small";
      return false;
    }
  }
  return true;
}
bool DiskManager::Init(const DiskLevelConf* disk_level_confs,
                       const unsigned num_confs, const unsigned num_threads) {
  // init log
  InitLogging(gLogFile, logging::LOG_ONLY_TO_FILE, logging::DONT_LOCK_LOG_FILE,
              logging::APPEND_TO_OLD_LOG_FILE);
  // pid, thread_id, timestamp, tickcount
  logging::SetLogItems(true, true, true, false);

  if (!CheckLevelConf(disk_level_confs, num_confs)) {
    return false;
  }

  enabled_ = true;
  // disk levels
  for (unsigned i = 0; i < num_confs; ++i) {
    disk_levels_.push_back(new DiskLevel(disk_level_confs + i, i));
  }
  const unsigned kMaxReadBufNum = 32;
  unsigned num_bufs =
      num_threads > kMaxReadBufNum ? kMaxReadBufNum : num_threads;
  for (unsigned i = 0; i < num_bufs; ++i) {
    ReleaseBuf(new char[1024 * 1024]);
  }
  return true;
}

bool DiskManager::Enabled() const {
  return enabled_;
}
char* DiskManager::AquireBuf() {
  while (sem_wait(&bufs_sem_) == -1) {
    LOG(ERROR) << "sem_wait error";
  }
  pthread_mutex_lock(&bufs_lock_);
  char* buf = bufs_.back();
  bufs_.pop_back();
  pthread_mutex_unlock(&bufs_lock_);
  return buf;
}
void DiskManager::ReleaseBuf(char* buf) {
  pthread_mutex_lock(&bufs_lock_);
  bufs_.push_back(buf);
  pthread_mutex_unlock(&bufs_lock_);
  sem_post(&bufs_sem_);
}
void DiskManager::RDLock(size_t level, size_t file) {
  DiskLevel* disk_level = disk_levels_[level];
  disk_level->RDLock(file);
}
void DiskManager::WRLock(size_t level, size_t file) {
  DiskLevel* disk_level = disk_levels_[level];
  disk_level->WRLock(file);
}
void DiskManager::UnLock(size_t level, size_t file) {
  DiskLevel* disk_level = disk_levels_[level];
  disk_level->UnLock(file);
}

DataGetStatus DiskManager::GetData(DiskDataAddr addr, char* buf,
                                   const unsigned buf_size, unsigned* vallen) {
  DiskLevel* level = disk_levels_[addr.block_addr_->data_->level_];
  return level->GetData(addr, buf, buf_size, vallen);
}

bool DiskManager::AppendData(const char *key, KeyLengthType keylen,
                             const char *val, ValueLengthType vallen,
                             rel_time_t exp_time, DiskDataAddr* addr) {
  ScopedLock append_lock(&append_lock_);
  // only one thread is allowed to MODIFY disks
  if (disk_levels_.empty())
    return false;

  // calculate formated data length
  size_t space = DiskFile::DiskDataItem::TotalSize(keylen, vallen);
  // find space in first level
  if (!disk_levels_[0]->HasSpace(space)) {
    for (int i = 0; i < static_cast<int>(disk_levels_.size()); ++i) {
      bool force = i == static_cast<int>(disk_levels_.size()) - 1;
      if (disk_levels_[i]->KickBlock(force)) {
        for (int j = i - 1; j >= 0; --j) {
          char ptbuf[32];
          snprintf(ptbuf, sizeof(ptbuf), "MoveBlock: level(%d->%d)", j, j + 1);
          PTIME(pt, ptbuf, true, 0);
          DiskLevel::MoveBlock(disk_levels_[j], disk_levels_[j + 1]);
        }
        break;
      }
    }
  }

  disk_levels_[0]->AppendData(key, keylen, val, vallen, exp_time, addr);
  return true;
}

/**
 * Disk level
 */

DiskLevel::DiskLevel() {
}
DiskLevel::DiskLevel(const DiskLevelConf* conf, size_t level)
    : level_((uint8_t) level) {
  const size_t file_size = DiskFile::kFileBlockSize * DiskFile::kBlockPerFile;
  const unsigned kMaxPathLen = 512;
  char path[kMaxPathLen];
  size_t dir_count = 0, dir_space_allocated = 0;
  DiskLevelDir* dir = conf->dirs[dir_count];
  for (size_t file_count = 0;;) {
    dir_space_allocated += file_size;
    if (dir_space_allocated > dir->size) {
      if (dir_count + 1 < conf->size) {
        ++dir_count;
        dir = conf->dirs[dir_count];
        dir_space_allocated = 0;
        continue;
      } else {
        break;
      }
    }
    snprintf(path, sizeof(path), "%s/%lu", dir->dir, file_count);
    AddFile(new DiskFile(path, level, file_count));
    ++file_count;
  }
  Init();
}
DiskLevel::~DiskLevel() {
  for (size_t i = 0; i < files_.size(); ++i) {
    delete files_[i];
  }
}

void DiskLevel::RDLock(size_t file) {
  files_[file]->RDLock();
}
void DiskLevel::WRLock(size_t file) {
  files_[file]->WRLock();
}
void DiskLevel::UnLock(size_t file) {
  files_[file]->UnLock();
}
bool DiskLevel::HasSpace(size_t space) const {
  return files_[current_file_]->HasSpace(space);
}

DataGetStatus DiskLevel::GetData(const DiskDataAddr& addr, char* buf,
                                 const unsigned buf_size,
                                 unsigned int *vallen) {
  DiskFile* file = files_[addr.block_addr_->data_->file_];
  return file->GetData(addr, buf, buf_size, vallen);
}

bool DiskLevel::AppendData(const char *key, KeyLengthType keylen,
                           const char *val, ValueLengthType vallen,
                           rel_time_t exp_time, DiskDataAddr* addr) {
  return files_[current_file_]->AppendData(key, keylen, val, vallen, exp_time,
                                           addr);
}
void DiskLevel::NextBlock(size_t* file_index, size_t* block_index) const {
  assert(file_index);
  assert(block_index);

  *file_index = current_file_;
  *block_index = current_file_block_;
  if (++*block_index == DiskFile::kBlockPerFile) {
    *block_index = 0;
    if (++*file_index == files_.size()) {
      *file_index = 0;
    }
  }
}
bool DiskLevel::KickBlock(bool force) {
  char ptbuf[32];
  snprintf(ptbuf, sizeof(ptbuf), "KickBlock: level(%u)", level_);
  PTIME(pt, ptbuf, true, 0);
  size_t file_index, block_index;
  NextBlock(&file_index, &block_index);
  DiskFile* file = files_[file_index];
  if (force || file->BlockExpired(block_index)) {
    DiskFile* cur_file = files_[current_file_];
    cur_file->UnmapWriteBlock();

    file->WRLock();
    file->FreeBlockAddrRefPtr(block_index);
    file->UnLock();

    file->MapWriteBlock(block_index);
    current_file_ = file_index;
    current_file_block_ = block_index;
    return true;
  }
  return false;
}
void DiskLevel::MoveBlock(DiskLevel* src_level, DiskLevel* dest_level) {
  // get source level's oldest blocks
  size_t src_file_index, src_block_index;
  src_level->NextBlock(&src_file_index, &src_block_index);

  DiskFile* src_file = src_level->files_[src_file_index];
  DiskFile* dest_file = dest_level->files_[dest_level->current_file_];
  char* src = src_file->MapReadBlock(src_block_index);
  char* dest = dest_file->CurrentBlock();
  // copy block and block info
  memcpy(dest, src, DiskFile::kFileBlockSize);
  src_file->UnmapReadBlock(src);
  // modify src_file's item address
  src_file->WRLock();
  dest_file->SetBlockInfo(dest_level->current_file_block_,
                          src_file->GetBlockInfo(src_block_index));
  src_file->UnLock();

  // unmap source level's current write block
  DiskFile* src_cur_file = src_level->files_[src_level->current_file_];
  src_cur_file->UnmapWriteBlock();
  // update source level's current block
  src_level->current_file_ = src_file_index;
  src_level->current_file_block_ = src_block_index;
  src_file->MapWriteBlock(src_block_index);
}
void DiskLevel::Init() {
  current_file_ = files_.size() - 1;
  current_file_block_ = DiskFile::kBlockPerFile - 1;
}

void DiskLevel::AddFile(DiskFile* file) {
  files_.push_back(file);
}

/**
 * Disk File
 */
DiskFile::DiskFile(const char* path, size_t level, size_t file)
    : level_(level),
      file_(file) {
  filedes_ = open(path, O_RDWR | O_CREAT | O_LARGEFILE,
                  S_IRUSR | S_IWUSR | S_IRGRP);
  ftruncate(filedes_, (off_t) kBlockPerFile * kFileBlockSize);
  block_addrs_ = new DiskFileBlockAddr[kBlockPerFile];
  for (size_t i = 0; i < kBlockPerFile; ++i) {
    block_addrs_[i].level_ = level_;
    block_addrs_[i].file_ = file_;
    block_addrs_[i].block_ = i;
    block_addrs_[i].valid_ = 1;
  }
  block_infos_ = new BlockInfo[kBlockPerFile];
  cur_block_index_ = kBlockPerFile - 1;
  offset_in_block_ = kFileBlockSize - 1;
  cur_block_ = NULL;
  pthread_rwlock_init(&rwlock_, NULL);
}
DiskFile::~DiskFile() {
  delete[] block_addrs_;
  delete[] block_infos_;
  if (cur_block_) {
    munmap(cur_block_, kFileBlockSize);
  }
  close(filedes_);
  pthread_rwlock_destroy(&rwlock_);
}
void DiskFile::RDLock() {
  pthread_rwlock_rdlock(&rwlock_);
}
void DiskFile::WRLock() {
  pthread_rwlock_wrlock(&rwlock_);
}
void DiskFile::UnLock() {
  pthread_rwlock_unlock(&rwlock_);
}
bool DiskFile::BlockExpired(size_t block_index) {
  return block_infos_[block_index].expire_time_ < current_time;
}
void DiskFile::FreeBlockAddrRefPtr(size_t block_index) {
  // point block_addr to the invalid_block_addr
  // and schedule the block_ref_ptr to be deleted
  block_infos_[block_index].block_addr_ref_ptr_->data_ =
      &kInvalidDiskFileBlockAddr;
  DiskFileBlockAddrRefPtrDecRef(block_infos_[block_index].block_addr_ref_ptr_);
}
void DiskFile::SetBlockInfo(size_t block_index, const BlockInfo& block_info) {
  block_infos_[block_index] = block_info;
  block_infos_[block_index].block_addr_ref_ptr_->data_ =
      &block_addrs_[block_index];
}
const DiskFile::BlockInfo& DiskFile::GetBlockInfo(size_t block_index) const {
  return block_infos_[block_index];
}
char* DiskFile::CurrentBlock() {
  return cur_block_;
}
bool DiskFile::AppendData(const char *key, KeyLengthType keylen,
                          const char *val, ValueLengthType vallen,
                          rel_time_t exp_time, DiskDataAddr* addr) {
  BlockInfo& cur_block_info = block_infos_[cur_block_index_];
  cur_block_info.AddItem(exp_time);
  if (addr) {
    addr->block_addr_ = cur_block_info.block_addr_ref_ptr_;
    addr->offset_in_block_ = offset_in_block_;
  }
  DiskDataItem* item = reinterpret_cast<DiskDataItem*>(cur_block_
      + offset_in_block_);
  item->key_len_ = keylen;
  item->value_len_ = vallen;
  item->check_code_ = DiskDataItem::CalcCheckCode(keylen, vallen);
  memcpy(DiskDataItem::KeyBuf(item), key, keylen);
  memcpy(DiskDataItem::ValueBuf(item, keylen), val, vallen);
  offset_in_block_ += DiskDataItem::TotalSize(keylen, vallen);
  return true;
}
DataGetStatus DiskFile::GetData(const DiskDataAddr& addr, char* buf,
                                const unsigned buf_size, unsigned int *vallen) {
  char* block = MapReadBlock(addr.block_addr_->data_->block_);
  DiskDataItem* item = reinterpret_cast<DiskDataItem*>(block
      + addr.offset_in_block_);
  if (item->key_len_ == 0
      || item->check_code_
          != DiskDataItem::CalcCheckCode(item->key_len_, item->value_len_)) {
    UnmapReadBlock(block);
    LOG(ERROR) << "CheckCodeError: level " << addr.block_addr_->data_->level_
        << ", file " << addr.block_addr_->data_->file_ << ", block "
        << addr.block_addr_->data_->block_ << ", offset "
        << addr.offset_in_block_;
    return kCheckCodeError;
  }
  if (item->value_len_ > buf_size) {
    UnmapReadBlock(block);
    LOG(ERROR) << "DataGetBufOverflow: level "
        << addr.block_addr_->data_->level_ << ", file "
        << addr.block_addr_->data_->file_ << ", block "
        << addr.block_addr_->data_->block_ << ", offset "
        << addr.offset_in_block_;
    return kDataGetBufOverflow;
  }
  memcpy(buf, DiskDataItem::ValueBuf(item, item->key_len_), item->value_len_);
  if (vallen) {
    *vallen = item->value_len_;
  }
  UnmapReadBlock(block);
  return kDataGetOK;
}

void DiskFile::MapWriteBlock(size_t block_index) {
  cur_block_ = static_cast<char*>(mmap(NULL, kFileBlockSize, PROT_WRITE,
                                       MAP_SHARED, filedes_,
                                       off_t(block_index) * kFileBlockSize));
  offset_in_block_ = 0;
  cur_block_index_ = block_index;
  block_infos_[cur_block_index_].Reset();
  if (level_ == 0) {
    DiskFileBlockAddrRefPtr* ptr = static_cast<DiskFileBlockAddrRefPtr*>(malloc(
        sizeof(DiskFileBlockAddrRefPtr)));
    memset(ptr, 0, sizeof(DiskFileBlockAddrRefPtr));
    ptr->data_ = &block_addrs_[block_index];
    DiskFileBlockAddrRefPtrIncRef(ptr);
    block_infos_[cur_block_index_].block_addr_ref_ptr_ = ptr;
  }
}
void DiskFile::UnmapWriteBlock() {
  if (cur_block_) {
    munmap(cur_block_, kFileBlockSize);
    cur_block_ = NULL;
  }
}

char* DiskFile::MapReadBlock(size_t block_index) {
  return static_cast<char*>(mmap(NULL, kFileBlockSize, PROT_READ, MAP_PRIVATE,
                                 filedes_, off_t(block_index) * kFileBlockSize));
}
void DiskFile::UnmapReadBlock(char* block) {
  munmap(block, kFileBlockSize);
}

bool DiskFile::HasSpace(size_t space) {
  return offset_in_block_ + space <= kFileBlockSize;
}
