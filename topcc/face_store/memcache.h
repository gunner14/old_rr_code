#ifndef _MEMCACHE_H
#define _MEMCACHE_H
#include <libmemcached/memcached.h>
#include <string>
#include <map>
using std::string;
using std::map;

class CMemcache {
 public:
  CMemcache();
  ~CMemcache();
  ///
  bool Init(const string& strConfFile = "./conf/memcache.conf");

  ///
  bool Set(const string& strKey, const string& strValue,
           time_t expiration_secs = 1800, uint32_t flags = 0);

  ///
  bool Delete(const string& strKey);

  ///
  string Get(const string& strKey);

  memcached_return Error() {
    return m_rc;
  }

  ///获取多个key的值,结果保存在mapKey2Value中(value为空则没有值),返回值存在的个数
  uint32_t Gets(map<string, string>& mapKey2Value);
 private:
  memcached_st *m_client;  //长链接,多少个server,就最多有多少个socket链接,当前CMemcache对象析构时,才会释放

  memcached_return m_rc;		//用来保存每次操作的状态
};
extern CMemcache g_memcache;

class NginxCache {
 public:
  NginxCache();
  ~NginxCache();
  bool Init(const string& strConfFile = "./conf/nginxcache.conf");

  bool Set(const string& strKey, const string& strValue,
           time_t expiration_secs = 7200, uint32_t flags = 0);

  memcached_return Error() {
    return rc_;
  }

 private:
  memcached_st *client_;   //长链接,多少个server,就最多有多少个socket链接,当前CMemcache对象析构时,才会释放

  memcached_return rc_;    //用来保存每次操作的状态
};
extern NginxCache g_nginxcache;
#endif
