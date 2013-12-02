
#ifndef _XCE_USER_PROFILE_CLIENT_H_
#define _XCE_USER_PROFILE_CLIENT_H_

#include <Ice/Ice.h>

#include "MemcachedClientPool.h"
#include "Singleton.h"

namespace xce {
namespace feed {

using namespace MyUtil;


class FeedUserPropertyData{
public:
	typedef Ice::Long StoreType;
  FeedUserPropertyData(const string& d) : data_(d){ }
  FeedUserPropertyData(){ }
  const string& GetOriginalData() const { return data_; }

  StoreType  GetMaxFeedId(){ return GetData(0); }
  StoreType  GetRefreshTime(){ return GetData(1); }
  StoreType  GetMaxFavFeedId(){ return GetData(2); }
  StoreType  GetActiveFriendCount(){ return GetData(3); }
  StoreType  GetMinFeedId() { return GetData(4); }

  bool  SetMaxFeedId(StoreType v){ return SetData(0, v); }
  bool  SetRefreshTime(StoreType v){ return SetData(1, v); }
  bool  SetMaxFavFeedId(StoreType v){ return SetData(2, v); }
  bool  SetActiveFriendCount(StoreType v){ return SetData(3, v); }
  bool  SetMinFeedId(StoreType v) { return SetData(4, v); }
  
  StoreType  GetData(int idx){
    const char* p = data_.c_str();
    int len = data_.size();
    if((size_t)idx < (len/(sizeof(StoreType))))
      return *(((StoreType*)p) + idx);
    return 0;
  }

  bool SetData(int idx, StoreType v){
    const char* p = data_.c_str();
    int len = data_.size();
    if((size_t)idx < (len/(sizeof(StoreType)))){
      *(((StoreType*)p) + idx) = v;
      return true;
    }else{
      int d = ((idx+1)*sizeof(StoreType)) - len;
      char buf[1024] = {0};
      if(d>1024)
        return false;

      data_.insert(data_.end(), buf, buf+d);
      return SetData(idx, v);
    }
  }

private:
  string data_; 
};


class FeedUserPropertyClient : public Singleton<FeedUserPropertyClient> {
public:
  FeedUserPropertyClient();
  ~FeedUserPropertyClient();

  FeedUserPropertyData Get(Ice::Int uid);
  bool Set(int uid, FeedUserPropertyData& data);
private:
  PoolManager pool_;
};

}
}
#endif // _XCE_USER_PROFILE_CLIENT_H_
