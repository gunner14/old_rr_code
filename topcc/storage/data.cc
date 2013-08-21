#include "storage/vldb.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace arch {

const char kHeadMagicCode[] = "\xe4\xe4VLDB\xe4\xe4";
const char kVersion[] = "\x00\x01";

DocumentFile::DocumentFile()
: pi_(new Index)
, df_(-1)
, version_(0) {
}

bool DocumentFile::Close() {
  if (df_ != -1)
    close(df_);
  return pi_->Close();
}

bool DocumentFile::Read(size_t pos, size_t size, char *buf) {
  if (df_ == -1)
    return false;
  return  pread(df_, buf, size, pos) == (ssize_t) size;
}

int64 DocumentFile::ReadSome(size_t pos, size_t size, char *buf) {
  if (df_ == -1)
    return -1;
  int64 sz = pread(df_, buf, size, pos);
  return sz;
}

bool DocumentFile::Write(size_t pos, size_t size, const char *buf) {
  if (df_ == -1)
    return false;
  return::pwrite(df_, buf, size, pos) == (ssize_t) size;
}

bool DocumentFile::GetFreeSize(uint32 minsize, uint32 *size, int64 *offset) {
  if (!size || !offset)
    return false;
  for (std::multimap<uint32, int64>::iterator it = free_.begin(); it != free_.end(); it++) {
    if (it->first >= minsize) {
      *size = it->first;
      *offset = it->second;
    }
  }
  return false;
}

bool DocumentFile::PushFreeSize(uint32 size, int64 offset) {
  free_.insert(std::make_pair(size, offset));
  return true;
}

void DocumentFile::RemoveFreeSize(uint32 size, int64 offset) {
  
  for (std::multimap<uint32, int64>::iterator it = free_.begin(); it != free_.end(); it++) {
    if (it->first == size && it->second == offset) {
      free_.erase(it);
      return;
    }
  }
}

int64 DocumentFile::size() {
  int64 size = -1;
  struct stat s;
  if(fstat(df_, &s) == 0) {
    if (s.st_size < 64 * 1024)
      return 64 * 1024;
    return s.st_size;
  }
  return -1;
}

Index* DocumentFile::index() {
  return pi_;
}

//  
//  +----------------------------------+
//  |\xE4 \xE4 V L D B \xE4 \xE4       | 8 byte
//  +----------------------------------+
//  |  0     1|
//  +----------------------------------+
//  |               Data               |
//  +----------------------------------+

int DocumentFile::Open(int docid, const char *path, bool is2index) {
  int flags = O_RDWR | O_CREAT | O_LARGEFILE;
  char name[256];
  char id[10] = "", id2[10] = "" ;
  int res = -1;

  // main db
  sprintf(name, "%s/%d.dat", path, docid);
  df_ = open(name, flags, S_IRUSR|S_IWUSR|S_IRGRP);
  if (df_ == -1)
    return -1;

  char buf[64*1024];
  bool b = Read(0, 64*1024, buf);
  if (b) {
    if (memcmp(buf, kHeadMagicCode, sizeof(kHeadMagicCode) - 1) != 0)
      return -1;
    if (memcmp(buf+8, kVersion, 2) != 0)
      return -1;
    res = 1;
  } else {
   b = memcpy(buf, kHeadMagicCode, sizeof(kHeadMagicCode) - 1);
   b = memcpy(buf + sizeof(kHeadMagicCode) - 1, kVersion, sizeof(kVersion) - 1);
   if (Write(0, 64*1024, buf) == false)
     return -1;
   res = 0;
  }

  // index
  sprintf(id, "%d.idx", docid);
  if (is2index)
    sprintf(id2, "%d.id2", docid);

  b = pi_->Init(this, path, id, id2);
  if (!b) {
    close(df_);
  }

  return res;
}

}
