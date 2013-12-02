#ifndef XCE_TRIPOD_CONFIG_HANDLER_
#define XCE_TRIPOD_CONFIG_HANDLER_

#include <set>
#include <vector>
#include <map>
#include <string>

#include "base/consistenthash.h"

namespace xce {
namespace tripod {

using namespace std;

class Node {
public:
  Node(const string& addr, const string& port);
  Node(const string& nick, const string& addr, const string& port);
  explicit Node(const string& node);
  string ToString() const;  // 转为为字符串格式为: [nickname:]addr:port
  bool operator<(const Node &m) const;
  string Nick() const;
  string Addr() const;
  string Port() const;
private:
  string nick_;  // 只有cache才有
  string addr_;
  string port_;
};

typedef vector<Node> NodeVec;
typedef set<Node> NodeSet;
typedef map<string, NodeSet> NodeMap;

class Cluster {
public:
  static void Diff(NodeMap& result, const NodeMap& lhs, const NodeMap& rhs);
  static void Merge(NodeMap& result, const NodeMap& delta);
};

class Config {
public:
  NodeMap& Caches();
  NodeMap& CacheManagers();
  NodeMap& ProducerManagers();
  const static int CACHE = 0;
  const static int CACHEMANAGER = 1;
  const static int PRODUCERMANAGER = 2;
private:
  NodeMap caches_;
  NodeMap cache_managers_;
  NodeMap producer_managers_;
};

//配置处理
class ConfigHandler {
public:
  ConfigHandler(){}
  //从配置文件获取配置
  bool Parse(const string& file, Config *config);
};

//一致性哈希定位器
class Locator {
public:
  Locator(const xce::Continuum &continuum, const int vcount = 100);
  Locator(const vector<string> &item, const int vcount = 100);
  Locator(Config &config, const int cluster, const string & ns, const int vcount = 100);

  string Locate(const string &key);
private:
  xce::Continuum continuum_;
};


}} //namespace xce::tripod
#endif
