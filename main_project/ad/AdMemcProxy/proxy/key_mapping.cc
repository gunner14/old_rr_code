#include "key_mapping.h"

#include "consist_hash.h"

#include "ServiceI.h"
#include "LogWrapper.h"

using namespace std;
using namespace boost::asio;
using namespace MyUtil;

namespace xce {
namespace ad {

static map<string, Continuum *> g_clusters;

#define USE_ONLINE

#ifndef   USE_ONLINE
static string memcached_nodes = "10.7.18.58:11211 4096;"\
                                "10.3.23.75:11211 1024;" \
                                "10.3.22.26:11211 2048;" \
																"10.3.19.214:11211 8192;";
#else
static string memcached_nodes = "10.3.24.199:11211 5120;"\
                                "10.3.24.200:11211 5120;" \
                                "10.3.24.223:11211 5120;" \
																"10.3.24.224:11211 5120;";
#endif
bool InitCacheClusters() {
 {
    string ns = "DEFAULT";
    Continuum * c = new Continuum;
    if (c->SetCacheNodes(memcached_nodes)) {
      MCE_INFO("加载 Continuum " << ns << ":" << memcached_nodes);
      g_clusters.insert(make_pair(ns, c));
    } else {
      MCE_WARN("加载 Continuum 失败 : " << ns << "-" << memcached_nodes);
      delete c;
    }
  }
 
 return true;
}

ip::tcp::endpoint GetEndpointByKey(const char * key, size_t len) {
  size_t delim_pos = 0;
  for (; delim_pos < len; ++ delim_pos) {
    if (key[delim_pos] == '#') {
      break;
    }
  }

  Continuum * c = 0;

  if (delim_pos != len) {
    string tmp(key, delim_pos);
    map<string, Continuum *>::const_iterator it = g_clusters.find(tmp);
    if (it != g_clusters.end()) {
      c = it->second;
    }
  } else {
    delim_pos = 0;
  }

  if (!c) {
    c = g_clusters["DEFAULT"];
  }
  
  ip::tcp::endpoint ep = c->LocateCacheNode(key, len);

  MCE_DEBUG("key=" << string(key, len) << " cache_node=" << ep);

  return ep;
}
}
}
