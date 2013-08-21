#include "blockmanage.h"
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "base3/logging.h"

namespace upload {
BlockManager g_blockmanager;
const int kMaxBlockNumber = 256;

BlockManager::BlockManager() {
}
BlockManager::~BlockManager()
{
  if(m_client)
    memcached_free(m_client);
}

std::string debugstr(const char *str, int sz) {
  return std::string(str, sz);
}

// after use need delete point
inline char* MakeBlockKey(const std::string &key, int index, size_t *len) {
  if (!len)
    return NULL;
  char tmp[20];
  sprintf(tmp, "_%d", index);
  int tmplen = strlen(tmp);
  *len = key.size() + tmplen;
  char *blockkey = new char[*len];
  memcpy(blockkey, key.c_str(), key.size());
  memcpy(blockkey + key.size(), tmp, tmplen);
  return blockkey;
}

bool BlockManager::Init(const std::string& strConfFile) {
  static time_t tLastModifyTime=0;
  struct stat st;
  if (stat(strConfFile.c_str(),&st) != 0) {
    std::cerr << "file not found:" << strConfFile << std::endl;
    return false;
  }
  if (st.st_mtime<=tLastModifyTime) {
    return true;
  }
  tLastModifyTime=st.st_mtime;

  std::ifstream fin(strConfFile.c_str());
  if(!fin){
    std::cerr << "open file fail:" << strConfFile << std::endl;
    return false;
  }
  
  memcached_st* client = memcached_create(NULL);
  memcached_return rc;

  std::string strLine;
  std::string strHost;
  uint32_t nPort;
  const std::string kSep(": ");
  const uint32_t kDefaultPort = 11211;
  std::vector<std::string> host_port_vec;
  while (getline(fin,strLine)) {
    boost::trim(strLine);
    if (strLine.size() < 2 || strLine[0] == '#')
      continue;
    boost::split(host_port_vec, strLine, boost::is_any_of(kSep), boost::token_compress_on);
    if (host_port_vec.size() == 1) {
      strHost = host_port_vec[0];
      nPort = kDefaultPort;
    }
    else if (host_port_vec.size() == 2) {
      strHost = host_port_vec[0];
      nPort = atoi(host_port_vec[1].c_str());
    }
    else {
      // wrong host port configure
      continue;
    }
    
    rc = memcached_server_add(client, strHost.c_str(), nPort);
  }
  
  if (m_client)
    memcached_free(m_client);
  m_client=client;
  memcached_behavior_set(m_client, MEMCACHED_BEHAVIOR_DISTRIBUTION, MEMCACHED_DISTRIBUTION_CONSISTENT);  //重要,必须调用此语句使"一致性hash生效"
  memcached_behavior_set(m_client, MEMCACHED_BEHAVIOR_SUPPORT_CAS, 1);  //重要,支持cas写入"
  return true;
}
std::string debuglog(const std::vector<int> indexs) {
  std::string log;
  for (std::vector<int>::const_iterator it = indexs.begin(); it != indexs.end(); ++it) {
    char tmp[20];
    sprintf(tmp, "%d,", *it);
    log += tmp;
  }
  return log;
}
int BlockManager::SetBlock(const std::string &hostid, const std::string &uploadid, int index, int count, const std::string &data, std::string *alldata, std::vector<int> *losts) {
  uint64_t cas;
  int i = 0;
  char* strindexs;
  uint32_t flags;
  memcached_return rc;
  std::vector<int> indexs;
  std::vector<std::string> blocks;
  std::string key = "UPID_" + uploadid + "_" + hostid;
  std::vector<int>::iterator it;
  LOG(INFO) << "-- add new block, key: " << key << " index: " << index << " count: " << count << " data size: " << data.size() << " data: " << debugstr(data.c_str(), 16);
  do {
    bool b = GetCacheIndexs(key, &indexs, &cas);
    if (!b) { // 空的情况
      cas = 0;
      indexs.assign(count, 0);
      LOG(INFO) << "---- first set:" << key;
    }
    //blocks[index] = data.size();
    for (it = indexs.begin(); it != indexs.end(); ++it, ++i) {
      if (*it == 0) {
        if(i != index)
          break;
      }
    }
    if (it == indexs.end()) { // 完整
      blocks.clear();
      if (!GetCacheBlocks(key, indexs, &blocks)) { // 某些块可能过期丢失,重新置索引
        i = 0;
        for (std::vector<std::string>::iterator it1 = blocks.begin(); it1 != blocks.end(); ++it1, ++i) {
          if (it1->empty())
            indexs[i] = 0;
        }
        LOG(INFO) << "---- index full,buf block lost: " << key << " indexs: " << debuglog(indexs);
      } else { // 成功取出,清除cache
        LOG(INFO) << "---- index full,block full,all block connect: " << key << " indexs: " << debuglog(indexs);
        if (alldata)
          blocks[index] = data; // 注意以下2行的次序
          alldata->clear();
          i = 0;
          for (std::vector<std::string>::iterator it2 = blocks.begin(); it2 != blocks.end(); ++it2, ++i) {
            alldata->append(*it2);
            LOG(INFO) << "---- connect block, N.O.: " << i << " datasize: " << it2->size() << " data: " << debugstr(it2->c_str(), 16);
          }
          ClearBlock(hostid, uploadid, indexs.size());
          LOG(INFO) << "---- connect block, finish: datasize: " << alldata->size() << " data: " << debugstr(alldata->c_str(), 32);
          return 1;
      }
    } else {
      LOG(INFO) << "---- index no full: " << key << " indexs: " << debuglog(indexs);
    }

    indexs[index] = data.size();
    
    strindexs = (char*)(int*)&indexs.front();
    if (cas == 0) {
      rc = memcached_add(m_client, key.c_str(), key.size(), strindexs, indexs.size() * 4,  60 * 5, flags);
      LOG(INFO) << "---- put index add";
      } else {
       rc = memcached_cas(m_client, key.c_str(), key.size(), strindexs, indexs.size() * 4,  60 * 5, flags, cas);
       LOG(INFO) << "---- put index cas: " << cas;
      }
    if (rc == MEMCACHED_SUCCESS) {
      size_t keylen;
      char* key1 = MakeBlockKey(key, index, &keylen);
      rc = memcached_set(m_client, key1, keylen, data.c_str(), data.size(),  60 * 5, flags);
      if (rc == MEMCACHED_SUCCESS) {
        LOG(INFO) << "---- add block success, key : " << debugstr(key1,keylen) << " data: " << debugstr(data.c_str(), 16);
        if (losts) {
          i = 0; // 生成不存在块队列
          for (it = indexs.begin(); it != indexs.end(); ++it, ++i) {
            if (*it == 0) {
              losts->push_back(i);
            }
          }
        }
        delete key1;
        return 0;
      } else {
        LOG(ERROR) << "---- add block fail, key : " << debugstr(key1, keylen);
        delete key1;
        return -1;
      }
    } else if (rc != MEMCACHED_DATA_EXISTS) { // && rc != MEMCACHED_NOTSTORED) {
      LOG(ERROR) << "---- put/add index fail, memcache failt: " << rc;
      return -1;
    }
    LOG(WARNING) << "---- add index fail,retry, cas change : " << cas;
  } while(1);// cas错误,说明被修改,重来
}

bool BlockManager::GetCacheIndexs(const std::string &key, std::vector<int> *blocks, uint64_t *cas) {
  std::string strValue;
  size_t len = 0;
  memcached_return rc;
  uint32_t flags;
  char *value = memcached_get(m_client, key.c_str(), key.size(), &len, &flags, &rc);
  if (value) {
    if (blocks)
      blocks->assign((int*)value, (int*)value + len/4);
    if (cas)
      *cas = memcached_result_cas(&m_client->result);//.item_cas;
    free(value);
    return true;
  }
  return false;
}

bool BlockManager::GetCacheBlocks(const std::string &key, const std::vector<int> &indexs, std::vector<std::string> *blocks) {
  uint32_t existcount = 0;
  uint32_t num_keys = indexs.size();
  char **keys = new char*[num_keys];
  size_t* keys_length = new size_t[num_keys];
  memcached_return rc;

  int getcount = 0;
  int i = 0;
  for (std::vector<int>::const_iterator it = indexs.begin(); it != indexs.end(); it++, i++) {
    if (*it != 0) {
      keys[getcount] = MakeBlockKey(key, i, &keys_length[getcount]);
      LOG(INFO) << "---- get a block, key: " << debugstr(keys[getcount], keys_length[getcount]) << " keys_length: " << keys_length[getcount] << " count: " << getcount;
      getcount++;
    }
  }
  rc = memcached_mget(m_client, keys, keys_length, getcount); 
  for (int i = 0; i < getcount; ++i) {
    delete keys[i];
  }

  char return_key[MEMCACHED_MAX_KEY];
  size_t return_key_length;
  char *return_value;
  size_t return_value_length;
  uint32_t flags;
  blocks->resize(indexs.size()); // resize & set 0
  do {
    return_value = memcached_fetch(m_client, return_key, &return_key_length, &return_value_length, &flags, &rc);
    if (rc == MEMCACHED_END)
      break;
    if (rc != MEMCACHED_SUCCESS) {
      LOG(ERROR) << "memcache fetch error, uploadid: " << key;
      break;
    }
    if(return_value){
      //std::string strKey = std::string(return_key, return_key_length);
      char* pos = strrchr(return_key, '_'); //.find_last_of('_');
      if (pos == NULL) {
        continue;
      }
      return_key[return_key_length] = '\0'; // return_key 足够大
      int idx = atoi(pos+1);
      LOG(INFO) << "---- fetch a block: " << return_key << " data: " << debugstr(return_value, 16);
      blocks->at(idx) = std::string(return_value, return_value_length);
      free(return_value);
      existcount++;
    }
  } while (true);

  delete keys;
  delete keys_length;
  return  (existcount == getcount);
}

void BlockManager::ClearBlock(const std::string &hostid, const std::string &uploadid, int count) {
  memcached_return rc;
  LOG(INFO) << "---- clear block: " << uploadid << " count: " << count;
  std::string key = "UPID_" + uploadid + "_" + hostid;
  rc=memcached_delete(m_client, key.c_str(), key.size(), 0);
  char *blockkey;
  size_t blockkeylen;
  for (int i = 0; i < count; ++i) {
    blockkey = MakeBlockKey(key, i, &blockkeylen);
    rc = memcached_delete(m_client, blockkey, blockkeylen, 0);
    delete blockkey;
  }
  return ;
}

} // namespace
