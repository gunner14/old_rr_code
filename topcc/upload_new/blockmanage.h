#ifndef _BLOCK_MANAGE_H_
#define _BLOCK_MANAGE_H_
#include <string>
#include <vector>
#include <libmemcached/memcached.h>

namespace upload {

class BlockManager {
 public:
  BlockManager();
  ~BlockManager();
  bool Init(const std::string& strConfFile);
  // 将某块写入cache,返回0成功,1,所有块满,不用再写,-1,失败
  int SetBlock(const std::string &hostid, const std::string &uploadid, int index, int count, const std::string &data, std::string *alldata, std::vector<int> *losts);
  void ClearBlock(const std::string &hostid, const std::string &uploadid, int count);
 private:
  bool GetCacheIndexs(const std::string &key, std::vector<int> *blocks, uint64_t *cas); // 返回cache的每一个块的大小,0表示未cache
  bool GetCacheBlocks(const std::string &key, const std::vector<int> &blocks, std::vector<std::string> *block);
 private:
  memcached_st *m_client;
};

extern BlockManager g_blockmanager;
} // namespace
#endif

