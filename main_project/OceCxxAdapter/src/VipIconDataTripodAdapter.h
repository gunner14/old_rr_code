#ifndef XCE_ADAPTER_VIPICONDATA_VIPICONDATATRIPODADAPTER_H_
#define XCE_ADAPTER_VIPICONDATA_VIPICONDATATRIPODADAPTER_H_

#include "VipShareInfo.h"
#include "VipInfo.h"
#include "OceProto/proto/vip_info.pb.h"
#include "tripod2/cc/core/single_table_tripod_client.h"
#include "Singleton.h"
#include <boost/shared_ptr.hpp>

namespace xce {
namespace adapter {
namespace vipicondata {

using namespace xce::vipinfo; //slice
using xce::tripod::SingleTableTripodClient;
using xce::tripoddata::vipinfo::PbVipIconUrl;
using xce::tripoddata::vipinfo::PbVipHatUrl;
using xce::tripoddata::vipinfo::PbVipMemberInfo;
typedef xce::tripoddata::vipinfo::PbVipMemberInfo_PbMemberInfo PbMemberInfo;

class VipIconDataTripodAdapter : public MyUtil::Singleton<VipIconDataTripodAdapter> {

public:
	/*
	 * ids : 用户id
	 * types : 需要取的用户vip类型，见enum VIP_TYPE
	 * 
	 * return : 返回用户的信息，map的key为用户id，value为要取的类型对应的数据的sequence
	 */
	std::map<int, xce::vipinfo::VipIconDataSeq> GetVipIconDataSeqMap(const MyUtil::IntSeq& ids, const MyUtil::IntSeq& types);

  /*
   * ids : 用户id
   * types : 需要取的用户vip类型，见enum VIP_TYPE
   * 
   * return : 返回用户的信息，map的key为用户id，value为要取的类型对应的数据的sequence
   */
	std::map<int, xce::vipinfo::VipFeedDataSeq> GetVipFeedDataSeqMap(const MyUtil::IntSeq& ids, const MyUtil::IntSeq& types);

  virtual ~VipIconDataTripodAdapter();

private:
  friend class MyUtil::Singleton<VipIconDataTripodAdapter>;
  VipIconDataTripodAdapter();
  boost::shared_ptr<SingleTableTripodClient<PbVipIconUrl> > icon_client_;
  boost::shared_ptr<SingleTableTripodClient<PbVipHatUrl> > hat_client_;
  boost::shared_ptr<SingleTableTripodClient<PbVipMemberInfo> > member_client_;
};


class VipInfoUtil {
public:
  static void parse(const PbMemberInfo &info, const int user_id,
      const std::string &icon_url, xce::vipinfo::VipIconDataPtr& data);
  static void parse(const PbMemberInfo &member_info, const int user_id, 
      const PbVipIconUrl &icon_info, const PbVipHatUrl &hat_info, VipFeedDataPtr& data);

};

}}} //namespace xce::adapter::vipicondata

#endif

