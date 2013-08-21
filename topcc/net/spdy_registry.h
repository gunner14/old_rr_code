#ifndef _NET_SPDY_REGISTRY_H_
#define _NET_SPDY_REGISTRY_H_
#include "net/config_client.h"
#include <set>
#include <boost/asio.hpp>

namespace xoa {
using boost::asio::ip::tcp;
typedef std::pair<std::string, std::string> Address;

class XoaRegistry {
 public:
  XoaRegistry();

  // 返回一个有效的ip, 暂时mod还没生效
  void Trash(const tcp::endpoint &ep, int mod);
  bool SelectIp(tcp::endpoint *ep, int mod);
  virtual bool Reset() = 0;

 protected:
  void Clear();
  // 当前的连接已经断开,放入回收队列中
  void TrashCurrent();
  std::set<tcp::endpoint> good_addr_set_;
  std::set<tcp::endpoint> bad_addr_set_;
  std::string serviceid_;
  time_t last_;
  int timeout_;
};

class DnsXoaRegistry :  public XoaRegistry {
 public:
  DnsXoaRegistry(const Address &addr);
  bool Reset();
 private:
  bool QueryDns(const Address &addr);
  tcp::resolver resolver_;
  boost::asio::io_service ios_;
  Address remote_addr_;
};

class SimpleXoaRegistry : public XoaRegistry {
 public:
  bool Register(const std::string &serviceid, const std::string &addr, int port);
  virtual bool Reset();
};

class ZookeeperBasedRegistry : public XoaRegistry {
 public:
  ZookeeperBasedRegistry(const std::string &addr, const std::string &path);
  virtual bool Reset();
 private:
  bool Resolve(const std::string &addr, const std::string &path);
  std::string zookeeper_addr_;
  std::string service_path_;
};

} // namespace
#endif
