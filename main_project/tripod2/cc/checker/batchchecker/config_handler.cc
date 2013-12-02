#include "config_handler.h"

#include "boost/algorithm/string.hpp"

#include "UtilCxx/src/Parser.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xce {
namespace tripod {

Node::Node(const std::string& addr, const std::string& port) : addr_(addr), port_(port) {
}

Node::Node(const std::string& nick, const std::string& addr,
    const std::string& port) : nick_(nick), addr_(addr), port_(port) {
}

Node::Node(const std::string& node) {
  vector<string> items;
  boost::split(items, node, boost::is_any_of(":"));
  if(items.size() == 2 || items.size() == 3) {
    int i;
    if(items.size() == 3) {
      nick_ = items.at(i++);
    }
    addr_ = items.at(i++);
    port_ = items.at(i++);
  } else {
    MCE_WARN("[Node]node parse error node: " << node);
  }
}

// 转为为字符串格式为: [nickname:]addr:port
std::string Node::ToString() const {
  if (nick_.empty()) {
    return "" + addr_ + ":" + port_;
  } else {
    return "" + nick_ + ":" + addr_ + ":" + port_;
  }
}

bool Node::operator<(const Node &m) const {
  return (this->ToString() < m.ToString());
}

std::string Node::Nick() const {
  return this->nick_;
}

std::string Node::Addr() const {
  return this->addr_;
}

std::string Node::Port() const {
  return this->port_;
}

void Cluster::Diff(NodeMap& result, const NodeMap& lhs, const NodeMap& rhs) {
  for(NodeMap::const_iterator itl = lhs.begin(); itl != lhs.end(); ++itl) {
    NodeSet set_a = itl->second;
    NodeSet set_b;
    NodeMap::const_iterator itr = rhs.find(itl->first);
    if(itr != rhs.end()) {
      set_b = itr->second;
    }
    vector<Node> vec;
    set_difference(set_a.begin(), set_a.end(), set_b.begin(), set_b.end(), back_inserter(vec));
    if(vec.size()) {
      result[itl->first].insert(vec.begin(), vec.end());
    }
  }
}

void Cluster::Merge(NodeMap& result, const NodeMap& delta) {
  for(NodeMap::const_iterator it = delta.begin(); it != delta.end(); ++it) {
    result[it->first].insert(it->second.begin(), it->second.end());
  }
}

NodeMap& Config::Caches() {
  return caches_;
}

NodeMap& Config::CacheManagers() {
  return cache_managers_;
}

NodeMap& Config::ProducerManagers() {
  return producer_managers_;
}

bool ConfigHandler::Parse(const string& file, Config* config) {
  MyUtil::DocumentPtr doc = MyUtil::Parser::parse(file);
  MyUtil::NodeList root = doc->getChildren();
  if(!root.size() || root[0]->getName() != "tripod") {
    MCE_WARN("root tripod not found in xml file!");
    return false;
  }
  MyUtil::NodeList cluster = root[0]->getChildren();
  for(MyUtil::NodeList::const_iterator it = cluster.begin(); it != cluster.end(); ++it) {
    if((*it)->getName() == "cachemanager") {
      MyUtil::NodeList cachemanager = (*it)->getChildren();
    } else if ((*it)->getName() == "producermanager") {
      MyUtil::NodeList cachemanager = (*it)->getChildren();
    } else if ((*it)->getName() == "cache") {
      MyUtil::NodeList cache = (*it)->getChildren();
      for(MyUtil::NodeList::const_iterator it_ca = cache.begin(); it_ca != cache.end(); ++it_ca) {
        if((*it_ca)->getName() == "namespace") {
          string ns = (*it_ca)->getAttribute("id");
          MyUtil::NodeList nodes = (*it_ca)->getChildren();
          for(MyUtil::NodeList::const_iterator it_n = nodes.begin(); it_n != nodes.end(); ++it_n) {
            if((*it_n)->getName() == "node") {
              string nick = (*it_n)->getAttribute("nickname");
              string addr = (*it_n)->getAttribute("address");
              string port = (*it_n)->getAttribute("port");
              Node node(nick, addr, port);
              if(config->Caches().find(ns) != config->Caches().end()) {
                config->Caches()[ns].insert(node);
              } else {
                NodeSet node_set;
                node_set.insert(node);
                config->Caches()[ns] = node_set;
              }
            }
          }
        }
      }
    }
  }
  return true;
}

Locator::Locator(const xce::Continuum &continuum, const int vcount) {
  //TODO

}

Locator::Locator(const vector<string> &item, const int vcount) {
  for(vector<string>::const_iterator it = item.begin(); it != item.end(); ++it) {
    continuum_.Add(*it, vcount);
  }
  continuum_.Rebuild();
}

Locator::Locator(Config &config, const int cluster, const string &ns, const int vcount) {
  NodeMap *cluster_map = NULL;
  switch (cluster) {
    case Config::CACHE:
      cluster_map = &config.Caches(); break;
    case Config::CACHEMANAGER:
      cluster_map = &config.CacheManagers(); break;
    case Config::PRODUCERMANAGER:
      cluster_map = &config.ProducerManagers(); break;
    default:
      break; 
  }
  if(cluster_map) {
    NodeMap::iterator it_ns = cluster_map->find(ns);
    if(it_ns != cluster_map->end()) {
      NodeSet::iterator its_a = it_ns->second.begin();
      for(;its_a != it_ns->second.end(); ++its_a) {
        continuum_.Add(its_a->Nick(), vcount);
      }
      continuum_.Rebuild();
    }
  }
}

string Locator::Locate(const string &key) {
  return continuum_.Locate(xce::Continuum::Hash(key));
}


}} //namespace xce::tripod
