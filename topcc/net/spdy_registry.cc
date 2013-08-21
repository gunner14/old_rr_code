#include "net/spdy_registry.h"

namespace xoa {

XoaRegistry::XoaRegistry()
: timeout_(4000) {
  last_ = time(NULL);
  srand(last_);
}

void XoaRegistry::Clear() {
  bad_addr_set_.clear();
  good_addr_set_.clear();
}

void  XoaRegistry::Trash(const tcp::endpoint &ep, int mod) {
  good_addr_set_.erase(ep);
  int test = good_addr_set_.size(); // TODO:test
  bad_addr_set_.insert(ep);
  //current_endpoint_.port(0);
}

bool XoaRegistry::SelectIp(tcp::endpoint *ep, int mod) {
  if (good_addr_set_.size() < bad_addr_set_.size()) {
    time_t t = time(NULL);
    if (t - last_ > timeout_) { // may reset
      last_ = t;
      if (!Reset())
        return false;
    } else if(good_addr_set_.empty()) {
      return false;
    }
  }
  size_t sz = good_addr_set_.size();
  int idx = rand() % sz; // 随机
  std::set<tcp::endpoint>::iterator it  = good_addr_set_.begin();
  //std::string test = it->address().to_string();
  std::advance(it, idx);
  if (ep)
    *ep = *it;
  return true;
}

DnsXoaRegistry::DnsXoaRegistry(const Address &addr)
: remote_addr_(addr)
, resolver_(ios_) {
  QueryDns(addr);
}

bool DnsXoaRegistry::Reset() {
  return QueryDns(remote_addr_);
}

bool DnsXoaRegistry::QueryDns(const Address &addr) {

  bool seccess  = false;
  tcp::resolver::query query(addr.first, addr.second);
  tcp::resolver::iterator it = resolver_.resolve(query);
  tcp::resolver::iterator it_end;
  for (; it != it_end; ++it) {
    good_addr_set_.insert(*it);
    seccess = true;
  }
  return seccess;
}

bool SimpleXoaRegistry::Register(const std::string &serviceid, const std::string &addr, int port) {
  serviceid_ = serviceid;
  //std::vector<std::pair<std::string, int> >::const_iterator it = good_addr_set_.find(serviceid);
  //for (; it != ips.end(); ++it) {
   // std::string test = it->first;
    boost::asio::ip::address add;
    boost::system::error_code ec;
    add = boost::asio::ip::address::from_string(addr, ec);
    if (ec)
      return false;
    //std::string test1 = add.to_string();
    good_addr_set_.insert(tcp::endpoint(add, port));
  //}
  return true;
}

bool SimpleXoaRegistry::Reset() {
  good_addr_set_.swap(bad_addr_set_);
}

ZookeeperBasedRegistry::ZookeeperBasedRegistry(const std::string &addr, const std::string &path)
: zookeeper_addr_(addr)
, service_path_(path) {
  Resolve(zookeeper_addr_, service_path_);
}

bool ZookeeperBasedRegistry::Resolve(const std::string &addr, const std::string &path) {
  if (addr.empty())
   return false;
  Clear();
  zookeeper_addr_ = addr;

  bool seccess = false;
  boost::asio::ip::address add;
  net::ConfigClient cc(addr, path);
  std::vector<std::pair<std::string, int> > v;
  bool b = cc.GetIpGroup(&v);
  std::vector<std::pair<std::string, int> >::iterator it = v.begin();
  for(; it != v.end(); ++it) {
    add.from_string(it->first);
    good_addr_set_.insert(tcp::endpoint(add, it->second));
  }
  return seccess;
}

bool ZookeeperBasedRegistry::Reset() {
  return Resolve(zookeeper_addr_, service_path_);
}

} // namespace
