#ifndef __MEMC_PROXY_ICE__
#define __MEMC_PROXY_ICE__

#include <Util.ice>

module xce
{
module ad
{

class ProxyInfo{
  string  ip;
  int     port;
  int     connections;
};

class AdMemcProxy {
  // 指定 upstream 服务器列表
  bool SetUpstreamList(string cfg);

  // 获取proxy的 memcache方式 监听端口
  string GetEndpoint();
  ProxyInfo GetProxyInfo();
};


};
};

#endif
