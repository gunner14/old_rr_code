#ifndef _VLDB_IDX_HEAD_
#define _VLDB_IDX_HEAD_

#include <vector>
#include <map>
#include <string>
#include <memory>

#include <db.h>

#include "base/basictypes.h"

namespace arch {

const int kDbPageSize = 30;
const int kMaxFreeDataSize = 128;
const int kPageAlign = 256;
const int kItemAlign = 16;

// 当PageId是uid时,32bit,当是blogid时,64Bit
typedef unsigned int PageId;
// 一般为评论id,64Bit
typedef uint64 ItemId;


class Index;
class DocumentFile {
 public:
  DocumentFile();
  // 返回 0创建db,1打开已有的,-1错误
  int Open(int docid, const char *path, bool is2index);
  bool Close();

  int64 size();
  Index* index();

 private:
  bool Read(size_t pos, size_t size, char *buf);
  int64 ReadSome(size_t pos, size_t size, char *buf);
  bool Write(size_t pos, size_t size, const char *buf);
  bool GetFreeSize(uint32 minsize, uint32 *size, int64 *offset);
  bool PushFreeSize(uint32 size, int64 offset);
  void RemoveFreeSize(uint32 size, int64 offset);

  friend class Index;
  Index *pi_;
  std::multimap<uint32, int64> free_;

  int df_;  // file description
  int version_;
};

//struct __db_env;
//typedef struct __db_env DB_ENV;
//struct __db;
//typedef struct __db DB;
struct PageTable;
struct DBPage;

class Index {
 public:
  Index();

  int Add(PageId gid, ItemId pid, const std::string &value); // return page number, -1 error

  // 返回总页数,-1标示出错
  int GetPage(PageId pid, int idx, std::vector<std::pair<ItemId,std::string> > *values);

  // 返回flag标记,如删除等,-1标示出错,256无此项
  int Get(PageId pid, ItemId iid, std::string *value); // if have second index, gid = 0, else assign value

  // 需要自己释放values,返回页数
  int GetPageList(PageId pid, PageTable **values);
  int GetPageSize(PageId pid);

  void RemovePage(PageId pid);
  void RemoveItem(ItemId iid);
 private:
  friend class DocumentFile;
  bool Init(DocumentFile *df, const char *path, const char *file, const char *file2);
  bool Close();
  bool UpdateIndex2(DBPage *olddbp, int64 pos);
  bool SelectSpace(uint32 advice_page, PageTable *pt);
  bool NewPage(PageId pid, ItemId iid,
                           PageTable *pt, int size,
                           char *oldbuf,
                           int oldsize, // old db page user page
                           const std::string &value);
  bool SetIndex2(ItemId iid, int64 offset, int32 len);
  bool GetPageData(PageTable *pt, std::vector<std::pair<ItemId,std::string> > *values);

  DocumentFile * df_;
  DB_ENV *bdb_env_; 
  DB *bdb_db_;
  DB *bdb_db2_;
  int64 size_; // file size cache, db file must engross
};

};
#endif // define
