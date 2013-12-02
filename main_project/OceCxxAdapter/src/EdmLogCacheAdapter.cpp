#include "EdmLogCacheAdapter.h"

using namespace std;
using namespace MyUtil;
using namespace xce::ad;

const char SPLIT = '_';

EdmLogCachePrx EdmLogCacheAdapter::getManagerOneway(int server_index){
  return getProxyOneway(server_index);
}

EdmLogCachePrx EdmLogCacheAdapter::getManager(int server_index){
  return getProxy(server_index);
}

int EdmLogCacheAdapter::GetClusterIndex(const string& key){
  int count = 0; 
  for (size_t i = 0; (i < key.size() && (key[i] != SPLIT)); ++i){
    count += key[i];
  }
  
  return count % getCluster();
}

void EdmLogCacheAdapter::Click(const string& key) {
  int cluster_index = GetClusterIndex(key);
  return getManagerOneway(cluster_index)->Click(key);
}

void EdmLogCacheAdapter::Add(const string& key, const string& value) {
  int cluster_index = GetClusterIndex(key);
  getManagerOneway(cluster_index)->Add(key, value);
}

void EdmLogCacheAdapter::Expose(const string& key, Ice::Int index){
  int cluster_index = GetClusterIndex(key);
  getManagerOneway(cluster_index)->Expose(key, index);
}
