#include "cache_handler.h"

#include "boost/lexical_cast.hpp"

#include "util/cpp/TimeCost.h"
#include "config_handler.h"
#include "UtilCxx/src/LogWrapper.h"
#include "base/consistenthash.h"
#include "tripod2/cc/core/static_cache_proxy.h"

using namespace MyUtil;
using namespace std;
using namespace xce::tripod;
using namespace xce::util;

namespace xce {
namespace tripod {
namespace checker {

CacheHandler::CacheHandler(Queue<int> *queue, const int batch, const string &ns, const string &biz,
    const string &xml_path_old, const string &xml_path_new, const int method) :
    ConsumerThread<int>(queue, batch), _ns(ns), _biz(biz), _method(method) {
  MCE_INFO("[CacheHandler]Init, batch=" << batch << ", ns=" << _ns << ", biz=" << _biz
      << ", xml_old=" << xml_path_old << ", xml_new=" << xml_path_new);
  //1. 从xml获取新旧配置，计算差别集
  ConfigHandler handler;
  Config config_old, config_new;
  handler.Parse(xml_path_old, &config_old);
  handler.Parse(xml_path_new, &config_new);
  NodeMap node_add, node_del, node_diff;
  Cluster::Diff(node_add, config_old.Caches(), config_new.Caches());
  Cluster::Diff(node_del, config_new.Caches(), config_old.Caches());
  NodeMap::iterator it_add = node_add.begin();
  NodeMap::iterator it_del = node_del.begin();
  for(; it_add != node_add.end(); ++it_add) {
    for(NodeSet::iterator itv = it_add->second.begin(); itv != it_add->second.end(); ++itv) {
      MCE_WARN(it_add->first << " add: " << itv->ToString());
    }
  }
  for(; it_del != node_del.end(); ++it_del) {
    for(NodeSet::iterator itv = it_del->second.begin(); itv != it_del->second.end(); ++itv) {
      MCE_WARN(it_del->first << " del: " << itv->ToString());
    }
  }
  Cluster::Merge(node_add, node_del);

  //2. 判断是否指定的ns和biz是否在差别集中
  NodeMap::iterator it_ns = node_add.find(ns);
  if(it_ns != node_add.end()) {
    list<string> items_old, items_new;
    NodeSet::iterator it = config_old.Caches()[ns].begin();
    for(; it != config_old.Caches()[ns].end(); ++it) {
      items_old.push_back(it->ToString());
    }
    it = config_new.Caches()[ns].begin();
    for(; it != config_new.Caches()[ns].end(); ++it) {
      items_new.push_back(it->ToString());
    }
    _locator_old = new Locator(config_old, Config::CACHE, ns);
    _locator_new = new Locator(config_new, Config::CACHE, ns);
    _proxy_old = new StaticCacheProxy(ns, biz, items_old);
    _proxy_new = new StaticCacheProxy(ns, biz, items_new);
  } else {
    MCE_WARN(ns << " no changes !!!");
  }
}

//执行三次，防止超时，打不同的log，对于三次都失败的需要查原因处理
bool CacheHandler::consum(vector<int> ids) {
  MyUtil::Clock clock;
  int uid = ids.front();
  for(vector<int>::iterator it = ids.begin(); it != ids.end(); ++it) {
    string key = boost::lexical_cast<string>(*it);
    if(_locator_old->Locate(key) != _locator_new->Locate(key)) {
      MCE_INFO(_ns << ":" << _biz << ":" << key << " diff, old: "
          << _locator_old->Locate(key) << " new: " << _locator_new->Locate(key));
      bool success = false;
      for(int i = 0; i < 3; i++) {
        MCE_DEBUG("[CacheHandler]Handle key=" << key);
        std::string value;
        switch(_method) {
          case REMOVE:
            success = _proxy_old->Remove(key); break;
          case LOAD:
            success = true; break;
            //TODO 之前想错了，无法直接load，需要改
            //_proxy_new->Load(key, 30*24*3600, true); break;
          case SET:
            success = _proxy_old->Get(key, &value);
            if (success) {
              success = _proxy_new->Set(key, value, 7*24*3600);
            }
            break;
          default:
            MCE_WARN("[CacheHandler]Handle unknown method:" << _method); break;
        }
        if(success) {
          break;
        } else {
          MCE_WARN("[CacheHandler]Handle_Fail, key=" << key << " try_count=" << i);
        }
      }
      if(!success) {
        MCE_WARN("[CacheHandler]Handle_Final_Fail, key=" << key);
      }
    }
  }
  MCE_INFO("[CacheHandler]:" << uid << ", count=" << ids.size()
      << ", time_cost=" << clock.step()/1000 << " ms");
}


}}}// namespace xce::tripod::checker
