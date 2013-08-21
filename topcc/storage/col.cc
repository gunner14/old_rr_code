#include "col/col.h"
#include "base/hash.h"

namespace arch {
  struct RecordHead {
    uint16 magic;
    unsigned char keylen; // max key 256
    unsigned char flag; // del=1,zip=2
    uint32 vallen:32; 
    uint32 uid;
  };


  static uint32 hash(uint64 gid) {
    return MurmurHash2(char*(&gid), sizeof(gid), 0x8964);
  }

  bool InfoCol::init(int fileid) {
    char str[100];
    sprintf(str, "/mnt/mfs/c/%d.db",fileid);
    db_file_ = open(str, O_RDWR|O_CREAT, S_IRWXU);
    if (db_file_ ==  -1)
      return false;
    sprintf(str, "/mnt/mfs/c/%d.idx",fileid);
    idx_file_ = open(str, O_RDWR|O_CREAT, S_IRWXU);
    if (idx_file_ == -1) {
      close(db_file_);
      return false;
    }
  }

  bool InfoCol::put(uint32_t uid, int64_t gid, const std::string &value) {
    uint32 hash = hash(gid);

  }
  bool InfoCol::get(uint32_t uid, int64_t gid, std::string *value) {

  }
  bool InfoCol::gets(uint32_t uid, int offset, int len, std::vector<std::string> *values) {

  }
  bool InfoCol::remove(int64_t gid) {

  }
  bool InfoCol::removes(uint32_t uid) {

  }
 
 private:
 



};
