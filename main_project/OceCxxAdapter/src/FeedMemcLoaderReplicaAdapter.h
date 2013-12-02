#ifndef _MC_PROXY_LOADER_REPLICA_ADAPTER_H_
#define _MC_PROXY_LOADER_REPLICA_ADAPTER_H_

#include <boost/lexical_cast.hpp>

#include "AdapterI.h"
#include "FeedMemcProxy.h"
#include "Singleton.h"


namespace xce {
namespace feed {

using namespace MyUtil;

class FeedMemcLoaderReplicaAdapter : public MyUtil::ReplicatedClusterAdapterI<FeedMemcLoaderPrx>, public Singleton<FeedMemcLoaderReplicaAdapter>
{
public:
  FeedMemcLoaderReplicaAdapter(): MyUtil::ReplicatedClusterAdapterI<FeedMemcLoaderPrx>("ControllerFeedMemcLoader", 120, 300, new XceFeedControllerChannel()) {
  }

  void LoadFeedSeq(MyUtil::LongSeq ids){
    if(ids.empty()){
      return;
    }
   // getManagerOneWay(ids.at(0))->LoadFeedSeq(ids);
    getManagerOneway(ids.at(0))->LoadFeedSeq(ids);
  }
  void LoadFeedByKey(const string & key) {
    getManagerOneway(GetIdByKey(key))->LoadFeedByKey(key);
  }

  void LoadFeedSeqByKey(const MyUtil::StrSeq & keys) {
    if(!keys.empty()) {
      getManagerOneway(GetIdByKey(keys[0]))->LoadFeedSeqByKey(keys);
    }
  }

  Str2StrMap GetFeedSeqByKey(const MyUtil::StrSeq & keys) {
    if(!keys.empty()) {
      return getManager(GetIdByKey(keys[0]))->GetFeedSeqByKey(keys);
    }
    return Str2StrMap();
  }

  FeedContentDict GetFeedDict(const vector<Ice::Long> & ids, int timeout) {
    if(!ids.empty()) {
      return getManager(ids.at(0))->GetFeedDict(ids);
    }
    return FeedContentDict();
  }

  FeedContentDict GetFeedDictTimeout(int uid,const vector<Ice::Long> & ids, int icetimeout,int dbtimeout) {
    if(!ids.empty()) {
      return getManager(uid)->GetFeedDictTimeout(uid,ids,dbtimeout);
    }
    return FeedContentDict();
  }

private:
  Ice::Long GetIdByKey(const string & key) {
    Ice::Long id = 0;
    size_t pos = key.find_first_of('#');
    if (pos != string::npos) {
      try {
        id = boost::lexical_cast<Ice::Long>(key.c_str() + pos + 1);
      } catch (boost::bad_lexical_cast & e) {}
    }

    return id;
  }

  FeedMemcLoaderPrx getManagerOneway(int id){
    return getProxyOneway(id);
  }

  FeedMemcLoaderPrx getManager(int id){
    return getProxy(id);
  }
};

}
}

#endif // _MC_PROXY_LOADER_REPLICA_ADAPTER_H_
