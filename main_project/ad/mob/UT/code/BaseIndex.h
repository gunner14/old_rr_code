/*
 *  Baseindex.h
 *
 *   Created on: 2012-4-10
 *       Author: yongjun.yin
 *  Description: 定义定向索引的抽象基类 AbstractBaseIndex
 *               定义定向索引的模板类   BaseIndex< KEY_TYPE, AdGroupPtr=AdGroupPtr >
 */

#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include <vector>
#include <map>
#include <string>
#include <ostream>
#include "Util.h"
#include "Struct.h"

namespace xce{
namespace ad{

class AbstractBaseIndex : public IceUtil::Shared {
public:
  virtual size_t Size()=0;
  virtual string Print()=0;
};
typedef IceUtil::Handle<AbstractBaseIndex> IndexPtr;

template<class KEY_TYPE>
class BaseIndex : public AbstractBaseIndex {
public:
  typedef typename vector<KEY_TYPE>::const_iterator KEY_ITERATOR;
  typedef typename map< KEY_TYPE, set<AdGroupPtr> >::const_iterator KEY_VALUE_ITERATOR;

  bool Add(KEY_TYPE& key, const AdGroupPtr& value) {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    index_[key].insert(value);
    return true;
  }
  bool Add(vector<KEY_TYPE>& keys, const AdGroupPtr& value) {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    KEY_ITERATOR it = keys.begin();
    for(; it != keys.end(); ++it) {
      index_[*it].insert(value);
    }
    return true;
  }
  bool Del(vector<KEY_TYPE>& keys, const AdGroupPtr& value) {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    KEY_ITERATOR it = keys.begin();
    for(; it != keys.end(); ++it) {
      index_[*it].erase(value);
    }
    return true;
  }
  void Get(const KEY_TYPE key, set<AdGroupPtr>& result) {
    result.clear();
		IceUtil::RWRecMutex::RLock lock(mutex_);
    KEY_VALUE_ITERATOR it = index_.find(key);
    if(it != index_.end()) {
      result = it->second;
    }
  }
  void GetWithAll(const KEY_TYPE key, set<AdGroupPtr>& result) {
    result.clear();
		IceUtil::RWRecMutex::RLock lock(mutex_);
    KEY_VALUE_ITERATOR it = index_.find(key);
    if(it != index_.end()) {
      const set<AdGroupPtr>& key_result = it->second;
      result.insert(key_result.begin(), key_result.end());
    }
    it = index_.find( ALL );
    if(it != index_.end()) {
      const set<AdGroupPtr>& key_result = it->second;
      result.insert(key_result.begin(), key_result.end());
    }
  }
  size_t Size() {
    return index_.size();
  }
  virtual string Print() {
	ostringstream result;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	KEY_VALUE_ITERATOR it = index_.begin();
	for(;it != index_.end(); ++it) {
	  result << "[" << (it->first) << "]: ";
	  set<AdGroupPtr>::const_iterator it_val = (it->second).begin();
	  for(; it_val != (it->second).end(); ++it_val) {
		result << (*it_val)->group_id() << " ";
	  }
	  result << "\n";
	}
	return result.str();
  }
protected:
  map< KEY_TYPE, set<AdGroupPtr> > index_;
  IceUtil::RWRecMutex mutex_;

  friend class AdMobMonitorI; 
};

}
}
