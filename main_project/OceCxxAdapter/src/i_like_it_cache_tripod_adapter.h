#ifndef __ILIKEIT_CACHE_TRIPOD_ADAPTER_H__
#define __ILIKEIT_CACHE_TRIPOD_ADAPTER_H__

#include "boost/shared_ptr.hpp"
#include "OceSlice/slice/ILikeIt.h"
#include "app/ILikeIt/share/ILikeItUtil.h"
#include "tripod2/cc/core/tripod_client.h"


namespace xce {
namespace ilikeit {

//TODO: 只改了首页打开用的接口，其他都没改

class ILikeItCacheTripodAdapter : public MyUtil::Singleton<ILikeItCacheTripodAdapter> {
 public:
  int getLikeCount(int type, long id);
  MyUtil::IntSeq getLikeCache(int type, long id);

//  LikeInfoPtr getLikeDetail(const std::string& gidStr, int uid, int limit, bool showStrangers=true);
//  LikeInfo2Ptr getLikeDetail2(const std::string& gidStr, int uid, int limit, bool showStrangers=true);
	
//  LikeInfoPtr getLikeDetail(const GlobalIdPtr& gid, int uid, int limit, bool showStrangers=true);
//  LikeInfo2Ptr getLikeDetail2(const GlobalIdPtr& gid, int uid, int limit, bool showStrangers=true);

//  LikeInfoSeq getLikeDetailBatchG(const std::vector<GlobalIdPtr>& gids, int uid, int limit, int detailNum);
//  LikeInfo2Seq getLikeDetailBatchG2(const std::vector<GlobalIdPtr>& gids, int uid, int limit, int detailNum);
	
//  LikeInfoSeq getLikeDetailBatch(const std::vector<std::string>& gidStrs, int uid, int limit, int detailNum);
//  LikeInfo2Seq getLikeDetailBatch2(const std::vector<std::string>& gidStrs, int uid, int limit, int detailNum);

 protected:
  friend class MyUtil::Singleton<ILikeItCacheTripodAdapter>;
  ILikeItCacheTripodAdapter();
  LikeDataPtr getWithKey(const GlobalIdPtr& gid);

 private:
//  template<class Type, class TypePtr>
//  TypePtr getLikeInfo(const GlobalIdPtr& gid, ::Ice::Int uid, ::Ice::Int limit, bool showStrangers);
//  template<class Type, class TypePtr>
//  vector<TypePtr> getLikeInfoBatch(const GlobalIdSeq& gids, ::Ice::Int uid, ::Ice::Int limit, ::Ice::Int detailNum);

  boost::shared_ptr<xce::tripod::TripodClient> tripod_client_;
  static const std::string kNameSpace;
  static const std::string kBizId;

}; 
 

}}

#endif
