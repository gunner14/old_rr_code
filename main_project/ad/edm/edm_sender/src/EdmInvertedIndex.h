/*
 *  EdmInvertedIndex.h
 *  @Author: jieyu.zeng
 *  @Date: 2012-02-24
 *  @Description: Edm 索引
 *
 */

#ifndef EDMINVERTEDINDEX_H_
#define EDMINVERTEDINDEX_H_
#include <set>
#include <IceUtil/RWRecMutex.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include "EdmStruct.h"
#include "FeedMemcProxy/client/user_profile_client.h"
#include "LRUCache.h"

namespace xce {
namespace ad {
using namespace std;
using namespace MyUtil;
using namespace IceUtil;
using namespace __gnu_cxx;

class IntIndex : public IceUtil::Shared {
public:
  bool Add(const int key, AdGroupPtr group);
  bool Add(vector<int>& keys, AdGroupPtr group);
  bool Del(vector<int>& keys, AdGroupPtr group);
  set<AdGroupPtr> Get(const int key);
  set<AdGroupPtr> GetWithAll(const int key);
  void intersection(const int key, set<AdGroupPtr>& other);  //求交

private:
  hash_map<int, set<AdGroupPtr> > index_;
  IceUtil::RWRecMutex mutex_;
};
typedef IceUtil::Handle<IntIndex> IntIndexPtr;
typedef IntIndex GenderIndex;
typedef IntIndex AgeIndex;
typedef IntIndex StageIndex;
typedef IntIndex GradeIndex;

class UInt64Index : public IceUtil::Shared {
public:
  virtual bool Add(const uint64_t key, AdGroupPtr group);
  virtual bool Add(vector<uint64_t>& keys, AdGroupPtr group);
  virtual bool Del(vector<uint64_t>& keys, AdGroupPtr group);
  set<AdGroupPtr> Get(const uint64_t key);
  virtual set<AdGroupPtr> GetWithAll(const uint64_t key);
  set<AdGroupPtr> GetWithMultiKeys(const vector<uint64_t>& keys);
  void intersection(string& this_key, UInt64Index& other, set<AdGroupPtr>& output);  //求交

protected:
  hash_map<uint64_t, set<AdGroupPtr> > index_;
  IceUtil::RWRecMutex mutex_;
};
typedef IceUtil::Handle<UInt64Index> UInt64IndexPtr;

class AreaUInt64Index : public UInt64Index {
public:
  virtual set<AdGroupPtr> GetWithAll(const uint64_t key);
};

struct DimIndex : public IceUtil::Shared {
  DimIndex() {
    gender_index_ = new IntIndex();
    stage_index_ = new IntIndex();
    grade_index_ = new IntIndex();
    age_index_ = new IntIndex();
    area_index_ = new AreaUInt64Index();
    school_index_ = new UInt64Index();
    school_area_index_ = new AreaUInt64Index();

    company_index_ = new UInt64Index();
    interest_index_ = new UInt64Index();
  };
  IntIndexPtr gender_index_;
  IntIndexPtr stage_index_;
  IntIndexPtr grade_index_;
  IntIndexPtr age_index_;
  UInt64IndexPtr area_index_;
  UInt64IndexPtr school_index_;
  UInt64IndexPtr school_area_index_;

  UInt64IndexPtr company_index_;
  UInt64IndexPtr interest_index_;
};

typedef IceUtil::Handle<DimIndex> DimIndexPtr;

/***************************************************
* Edm索引，按member_type
*
*/
class EdmInvertedIndex : public MyUtil::Singleton<EdmInvertedIndex> {
public:
  EdmInvertedIndex() : load_all_(true) {
    TaskManager::instance().scheduleRepeated(new ChangeFlagTimer());
  };
  bool Get(const UserProfile& profile, int edm_member_type, set<AdGroupPtr>& groups);
  bool GetBidGroups(const UserProfile& profile, PubType edm_member_type, set<AdGroupPtr>& groups);
  bool Load(hash_map<Ice::Long, AdGroupPtr>& groups);
  bool TotalLoad(hash_map<Ice::Long, AdGroupPtr>& groups); //全量更新
  bool IncLoad(hash_map<Ice::Long, AdGroupPtr>& groups); //增量更新
  void set_load_all(bool load_all) {
    load_all_ = load_all;
  }

private: 
  class ChangeFlagTimer : virtual public MyUtil::Timer{
    public:
      ChangeFlagTimer() : Timer(24 * 60 * 60 * 1000) {
      }
      virtual void handle() {
        EdmInvertedIndex::instance().set_load_all(true);
      };
  };

  long last_update_time_;
  bool load_all_;
  hash_map<Ice::Long, AdGroupPtr> cached_group_;
  LRUCache lru_cache_;
  hash_map<int, DimIndexPtr> edm_index_; //按edm member类型索引
  IceUtil::RWRecMutex mutex_;
};

}
}

#endif /* EDMINVERTEDINDEX_H_ */
