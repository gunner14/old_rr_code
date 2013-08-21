#ifndef _COL_HEAD_
#define _COL_HEAD_

#include <iostream>
#include <string>
#include "idx.h"

namespace arch {

#define DB_MAX=65535;

class DbInfo;
class DbManager {
 public:
  void Add(int i);
  void Remove(int i);

 private:
  std::map<int, DbInfo > dbs_;
};


class DbInfo {

 public:
  DbInfo();
  bool Init(int fileid); 
  void Close();
  
  bool AddOriginal(uint32 uid, int64 gid, const std::string &title);
  bool Add(uint32 uid, int64 gid, const std::string &value);

  bool Get(uint32 uid, int64 gid, std::string *value);
  bool Gets(uint32 uid, int offset, int len, std::vector<std::string> *values);
  bool GetOriginals(uint32 uid, int64 gid, td::vector<std::string> *values);

  bool Remove(int64 gid);
  bool Remove_uid(uint32 uid);
  bool Remove(original);
 
 private:
  IndexFile idx_commentator_; // 评论员
  IndexFile idx_comment_; // 评论

  int db_file_;

};


};

#endif // _COL_HEAD_
