#include "memcache.h"

#include <sys/stat.h>
#include <iostream>
#include <fstream>
using namespace std;

CMemcache g_memcache;
NginxCache g_nginxcache;

CMemcache::CMemcache() {
  m_client = NULL;
}
CMemcache::~CMemcache() {
  if (m_client)
    memcached_free (m_client);
}
bool CMemcache::Init(const string& strConfFile) {
  static time_t tLastModifyTime = 0;
  struct stat st;
  if (stat(strConfFile.c_str(), &st) != 0) {
    cerr << "file not found:" << strConfFile << endl;
    return false;
  }
  if (st.st_mtime <= tLastModifyTime) {
    return true;
  }
  tLastModifyTime = st.st_mtime;

  ifstream fin(strConfFile.c_str());
  if (!fin) {
    cerr << "open file fail:" << strConfFile << endl;
    return false;
  }

  memcached_st* client = memcached_create(NULL);
  memcached_return& rc = m_rc;

  string strLine;
  string strHost;
  uint32_t nPort;
  size_t nPos = string::npos;
  while (getline(fin, strLine)) {
    if (strLine.size() < 2 || strLine[0] == '#')
      continue;

    nPos = strLine.find("\t");
    if (nPos != string::npos) {
      strHost = strLine.substr(0, nPos);
      nPort = atoi(strLine.substr(nPos + 1).c_str());
    } else {
      strHost = strLine;
      nPort = 11211;
    }
    rc = memcached_server_add(client, strHost.c_str(), nPort);
  }

  if (m_client)
    memcached_free (m_client);
  m_client = client;
  memcached_behavior_set(m_client, MEMCACHED_BEHAVIOR_DISTRIBUTION,
                         MEMCACHED_DISTRIBUTION_CONSISTENT);  //重要,必须调用此语句使"一致性hash生效"
  return true;
}
bool CMemcache::Set(const string& strKey, const string& strValue,
                    time_t expiration_secs, uint32_t flags) {
  memcached_return& rc = m_rc;
  rc = memcached_set(m_client, strKey.c_str(), strKey.size(), strValue.c_str(),
                     strValue.size(), expiration_secs, flags);
  return rc == MEMCACHED_SUCCESS;
}
bool CMemcache::Delete(const string& strKey) {
  memcached_return& rc = m_rc;
  rc = memcached_delete(m_client, strKey.c_str(), strKey.size(), 0);	//最后一个参数表示多少秒后再删除,0表示立即删除
  return rc == MEMCACHED_SUCCESS;
}
string CMemcache::Get(const string& strKey) {
  string strValue;
  size_t len = 0;
  memcached_return& rc = m_rc;
  uint32_t flags;
  char *value = memcached_get(m_client, strKey.c_str(), strKey.size(), &len,
                              &flags, &rc);
  if (value) {
    strValue = string(value, len);
    free(value);
  }
  return strValue;
}

uint32_t CMemcache::Gets(map<string, string>& mapKey2Value) {
  uint32_t nExsitCount = 0;
  uint32_t num_keys = mapKey2Value.size();
  char **keys = new char*[num_keys];
  size_t* keys_length = new size_t[num_keys];
  size_t i = 0;
  for (map<string, string>::iterator itMap = mapKey2Value.begin();
      itMap != mapKey2Value.end(); itMap++) {
    const string& strKey = itMap->first;
    keys[i] = (char *) strKey.c_str();
    keys_length[i] = strKey.size();
    i++;
  }

  memcached_return& rc = m_rc;
  rc = memcached_mget(m_client, keys, keys_length, num_keys);
  char return_key[MEMCACHED_MAX_KEY];
  size_t return_key_length;
  char *return_value;
  size_t return_value_length;
  uint32_t flags;
  do {
    return_value = memcached_fetch(m_client, return_key, &return_key_length,
                                   &return_value_length, &flags, &rc);
    if (rc == MEMCACHED_END)
      break;
    if (return_value) {
      string strKey = string(return_key, return_key_length);
      mapKey2Value[strKey] = string(return_value, return_value_length);
      free(return_value);
      nExsitCount++;
    }
  } while (true);

  return nExsitCount;
}

struct CacheItem {
  std::string key;
  std::string ip;
  int port;
};

CacheItem _cache[10] = { { "/fmn046", "10.3.22.50", 11211 }, { "/fmn047",
    "10.3.22.50", 11311 }, { "/fmn048", "10.3.22.50", 11311 }, { "/fmn049",
    "10.3.22.50", 11411 }, { "/fmn050", "10.3.22.50", 11411 },

{ "/fmn051", "10.3.22.51", 11211 }, { "/fmn052", "10.3.22.51", 11311 }, {
    "/fmn053", "10.3.22.51", 11311 }, { "/fmn054", "10.3.22.51", 11411 }, {
    "/fmn055", "10.3.22.51", 11411 }, };

std::map<std::string, int> _servermap;

NginxCache::NginxCache() {
  client_ = NULL;
}

NginxCache::~NginxCache() {
  if (client_)
    memcached_free (client_);
}

static uint32_t my_hash_function(const char *str, size_t length,
                                 void *context) {
  if (length <= 7)  // sizeof("/fmn055")
    return 0;
  std::map<std::string, int>::iterator it = _servermap.find(
      std::string(str, 7));
  if (it != _servermap.end())
    return it->second;
  return 0;
}

bool NginxCache::Init(const string& strConfFile) {

  if (client_)
    memcached_free (client_);
  client_ = memcached_create(NULL);

  for (int i = 0; i < sizeof(_cache) / sizeof(CacheItem); ++i) {
    CacheItem &ci = _cache[i];
    _servermap.insert(std::make_pair(ci.key, i));
    rc_ = memcached_server_add(client_, ci.ip.c_str(), ci.port);
  }
  memcached_behavior_set(client_, MEMCACHED_BEHAVIOR_DISTRIBUTION,
                         MEMCACHED_DISTRIBUTION_MODULA);  //重要,必须调用此语句使"一致性hash生效"

  hashkit_st new_kit;
  const hashkit_st *kit = memcached_get_hashkit(client_);
  hashkit_clone(&new_kit, kit);
  hashkit_return_t hash_rc = hashkit_set_custom_function(&new_kit,
                                                         my_hash_function,
                                                         NULL);
  memcached_set_hashkit(client_, &new_kit);

  return true;
}

bool NginxCache::Set(const string& strKey, const string& strValue,
                     time_t expiration_secs, uint32_t flags) {
  rc_ = memcached_set(client_, strKey.c_str(), strKey.size(), strValue.c_str(),
                      strValue.size(), expiration_secs, flags);
  return rc_ == MEMCACHED_SUCCESS;
}
