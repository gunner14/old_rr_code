#include "VipIconDataTripodAdapter.h"

namespace xce {
namespace adapter {
namespace vipicondata {

VipIconDataTripodAdapter::VipIconDataTripodAdapter() {
  icon_client_.reset(new SingleTableTripodClient<PbVipIconUrl>("xce_misc", "vip_icon_url"));
  hat_client_.reset(new SingleTableTripodClient<PbVipHatUrl>("xce_misc", "vip_hat_url"));
  member_client_.reset(new SingleTableTripodClient<PbVipMemberInfo>("xce_misc", "vip_member_info"));
}

VipIconDataTripodAdapter::~VipIconDataTripodAdapter() {
  icon_client_.reset();
  hat_client_.reset();
  member_client_.reset();
}

std::map<int, xce::vipinfo::VipIconDataSeq> VipIconDataTripodAdapter::GetVipIconDataSeqMap(
    const MyUtil::IntSeq& ids, const MyUtil::IntSeq& types) {
  std::map<int, xce::vipinfo::VipIconDataSeq> result;
  if(ids.empty()) {
    return result;
  }

  //1. 从Cache中取VipMember信息
  std::map<long, PbVipMemberInfo> vip_members;
  std::list<long> ids_list(ids.begin(), ids.end());
  member_client_->MultiGet(ids_list, &vip_members); 

  //2. 从会员信息取用到的icon_id的集合
  std::set<int> icon_ids_set;
  std::set<int> type_set(types.begin(), types.end());
  for(std::map<long, PbVipMemberInfo>::const_iterator its = vip_members.begin(); its != vip_members.end(); ++its) {
    const ::google::protobuf::RepeatedPtrField<PbMemberInfo> & infoList = its->second.member_infos();
    ::google::protobuf::RepeatedPtrField<PbMemberInfo>::const_iterator it = infoList.begin();
    for(; it != infoList.end(); ++it) {
      if(type_set.find(it->type()) != type_set.end()) {
        icon_ids_set.insert(it->icon_id());
      }
    }
  }

  //3. 从Cache中取icon_url
  std::list<long> icon_ids_list(icon_ids_set.begin(), icon_ids_set.end());
  std::map<long, PbVipIconUrl> vip_icons;
  icon_client_->MultiGet(icon_ids_list, &vip_icons);

  //4. 组合
  std::map<long, PbVipMemberInfo>::const_iterator its = vip_members.begin();
  for(; its != vip_members.end(); ++its) {
    xce::vipinfo::VipIconDataSeq icon_datas;
    const ::google::protobuf::RepeatedPtrField<PbMemberInfo> & infoList = its->second.member_infos();
    ::google::protobuf::RepeatedPtrField<PbMemberInfo>::const_iterator it = infoList.begin();
    for(; it != infoList.end(); ++it) {
      if(type_set.find(it->type()) != type_set.end()) {
        xce::vipinfo::VipIconDataPtr data = new xce::vipinfo::VipIconData;
        std::map<long, PbVipIconUrl>::const_iterator icon_info = vip_icons.find(it->icon_id());
        if(icon_info != vip_icons.end()) {
          VipInfoUtil::parse(*it, its->first, icon_info->second.url(), data);
          icon_datas.push_back(data);
        }
      }
    }
    if(!icon_datas.empty()) {
      result[(int)its->first] = icon_datas;
    }
  }
  return result;
}

std::map<int, xce::vipinfo::VipFeedDataSeq> VipIconDataTripodAdapter::GetVipFeedDataSeqMap(
    const MyUtil::IntSeq& ids, const MyUtil::IntSeq& types) {
  std::map<int, xce::vipinfo::VipFeedDataSeq> result;
  if(ids.empty()) {
    return result;
  }

  //1. 从Cache中取VipMember信息
  std::map<long, PbVipMemberInfo> vip_members;
  std::list<long> ids_list(ids.begin(), ids.end());
  member_client_->MultiGet(ids_list, &vip_members); 

  //2. 从会员信息取用到的icon_id的集合
  std::set<int> icon_ids_set, hat_ids_set;
  std::set<int> type_set(types.begin(), types.end());
  for(std::map<long, PbVipMemberInfo>::const_iterator its = vip_members.begin(); its != vip_members.end(); ++its) {
    const ::google::protobuf::RepeatedPtrField<xce::tripoddata::vipinfo::PbVipMemberInfo_PbMemberInfo > & infoList = its->second.member_infos();
    ::google::protobuf::RepeatedPtrField<xce::tripoddata::vipinfo::PbVipMemberInfo_PbMemberInfo >::const_iterator it = infoList.begin();
    for(; it != infoList.end(); ++it) {
      if(type_set.find(it->type()) != type_set.end()) {
        icon_ids_set.insert(it->icon_id());
        hat_ids_set.insert(it->hat_id());
      }
    }
  }

  //3. 从Cache中取icon_url和hat_url
  std::list<long> icon_ids_list(icon_ids_set.begin(), icon_ids_set.end());
  std::map<long, PbVipIconUrl> vip_icons;
  icon_client_->MultiGet(icon_ids_list, &vip_icons);
  std::list<long> hat_ids_list(hat_ids_set.begin(), hat_ids_set.end());
  std::map<long, PbVipHatUrl> vip_hats;
  hat_client_->MultiGet(hat_ids_list, &vip_hats);

  //4. 组合
  std::map<long, PbVipMemberInfo>::const_iterator its = vip_members.begin();
  for(; its != vip_members.end(); ++its) {
    xce::vipinfo::VipFeedDataSeq icon_datas;
    const ::google::protobuf::RepeatedPtrField<PbMemberInfo > & infoList = its->second.member_infos();
    ::google::protobuf::RepeatedPtrField<PbMemberInfo >::const_iterator it = infoList.begin();
    for(; it != infoList.end(); ++it) {
      if(type_set.find(it->type()) != type_set.end()) {
        xce::vipinfo::VipFeedDataPtr data = new xce::vipinfo::VipFeedData;
        std::map<long, PbVipIconUrl>::const_iterator icon_info = vip_icons.find(it->icon_id());
        std::map<long, PbVipHatUrl>::const_iterator hat_info = vip_hats.find(it->hat_id());
        if(icon_info != vip_icons.end() && hat_info != vip_hats.end()) {
          VipInfoUtil::parse(*it, its->first, icon_info->second, hat_info->second, data);
          icon_datas.push_back(data);
        }
      }
    }
    if(!icon_datas.empty()) {
      result[(int)its->first] = icon_datas;
    }
  }
  return result;
}

void VipInfoUtil::parse(const PbMemberInfo &member, const int user_id, const std::string &icon_url,
    VipIconDataPtr& data) {
  data->id = user_id;
  data->hideUrl = icon_url;
  data->type = member.type();
  data->iconId = member.icon_id();
  data->level = member.level();
  data->status = member.status();
}

void VipInfoUtil::parse(const PbMemberInfo &member_info, const int user_id, const PbVipIconUrl &icon_info,
    const PbVipHatUrl &hat_info, VipFeedDataPtr& data) {
  VipMemberMetaDataPtr member = new VipMemberMetaData;
  member->id = user_id;
  member->type = member_info.type();
  member->level = member_info.level();
  member->status = member_info.status();
  Icon2UrlDataPtr icon = new Icon2UrlData;
  icon->iconId = icon_info.id();
  icon->hideUrl = icon_info.url();
  HatDataPtr hat = new HatData;
  hat->hatId = hat_info.id();
  hat->hideUrl = hat_info.url();
  hat->status = hat_info.status();
  data->member = member;
  data->icon = icon;
  data->hat = hat;
}

}}} //namespace xce::adapter::vipicondata

