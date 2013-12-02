/*
 *  InvertedIndex.h
 *
 *   Created on: 2012-4-10
 *       Author: yongjun.yin
 *  Description: 定义各种定向索引及品牌和中小的倒排索引
 */

#ifndef __AD_MOB_INVERTED_INDEX_H__
#define __AD_MOB_INVERTED_INDEX_H__

#include "Struct.h"
#include "BaseIndex.h"
#include "FeedMemcProxy/client/user_profile_client.h"
#include "Handler.h"

#include "Singleton.h"
#include <vector>
#include <map>

using namespace std;

namespace xce{
namespace ad{

class RectangleIndex : public BaseIndex<Rectangle> {
public:
  typedef vector<Rectangle>::const_iterator KEY_ITERATOR;
  typedef map< Rectangle, set<AdGroupPtr> >::const_iterator KEY_VALUE_ITERATOR;
  
  void Get(const Rectangle key, set<AdGroupPtr>& groups);
  void GetWithAll(const Rectangle key, set<AdGroupPtr>& groups);

  friend class InvertedIndex;
};

class AreaUIntIndex : public BaseIndex<uint64_t> {
public:
  void GetWithAll(const uint64_t key, set<AdGroupPtr>& groups);
};

typedef BaseIndex<short> GenderIndex;
typedef BaseIndex<short> AgeIndex;
typedef BaseIndex<short> StageIndex;
typedef BaseIndex<short> GradeIndex;
typedef BaseIndex<short> NetworkIndex;
typedef BaseIndex<short> ResolutionIndex;
typedef IceUtil::Handle< BaseIndex<short> > ShortIndexPtr;

typedef BaseIndex<uint64_t> SchoolIndex;
typedef BaseIndex<uint64_t> PlatformIndex;
typedef BaseIndex<uint64_t> Brand3GIndex;
typedef IceUtil::Handle< BaseIndex<uint64_t> > UIntIndexPtr;

typedef AreaUIntIndex AreaIndex;
typedef AreaUIntIndex SchoolAreaIndex;
typedef IceUtil::Handle< AreaUIntIndex > AreaIndexPtr;

typedef RectangleIndex LBSIndex;
typedef IceUtil::Handle< LBSIndex > LBSIndexPtr;

class DimIndex : public IceUtil::Shared {
public:
  DimIndex();
 
  ShortIndexPtr gender_index_;
  ShortIndexPtr stage_index_;
  ShortIndexPtr grade_index_;
  ShortIndexPtr age_index_;
  ShortIndexPtr network_index_;
  ShortIndexPtr resolution_index_;

  UIntIndexPtr school_index_;
  UIntIndexPtr platform_index_;
  UIntIndexPtr brand3G_index_;
  
  AreaIndexPtr area_index_;
  AreaIndexPtr school_area_index_;

  LBSIndexPtr lbs_index_;

  map<int, IndexPtr> index_dict_;
};
typedef IceUtil::Handle<DimIndex> DimIndexPtr;

//反向索引
class InvertedIndex : public MyUtil::Singleton<InvertedIndex> {
public:
  void Get(const HandlerPara& para, const Ice::Long zone, InvertedIndexType type, set<AdGroupPtr>& groups);
  void GetForTest(const HandlerPara& para, const Ice::Long zone, set<AdGroupPtr>& groups, const int index_flag, const int type_flag);

  void TotalLoad();
  void IncLoad();
  void SetDB(string DBInstance) {
    DBInstance_ = DBInstance;
  }
  string GetDB() {
    return DBInstance_;
  }
private:
  inline void BuildIndex(const AdBidUnit& bid_unit, AdGroupPtr group, map<Ice::Long, DimIndexPtr>& zone_index);
  void ShowIndexSize();
private:
  map<Ice::Long, DimIndexPtr> zone_brand_index_;
  map<Ice::Long, DimIndexPtr> zone_self_index_;
  string DBInstance_;

  IceUtil::RWRecMutex mutex_;

  friend class AdMobMonitorI;
};

}
}
#endif
