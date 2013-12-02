#include "TalkFunTask.h"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "TalkFunUtil.h"
#include "XmppTools.h"
#include "MessageType.h"
#include "TalkProtoAdapter.h"
#include "IMIdGeneratorAdapter.h"
#include "TalkGroupLoaderAdapter.h"
#include "TalkGroupMemberLoaderAdapter.h"
#include "PhotoUrlTools.h"
#include "TalkFunStateAdapter.h"
#include "UserCacheAdapter.h"
#include "UserNetworkAdapter.h"
#include "ReceiversHandler.h"
#include "TalkGroupViewLoaderAdapter.h"
#include "TalkGroupAdminMsgLoaderAdapter.h"
#include "GroupChatMessageLoaderAdapter.h"
#include "BuddyByIdReplicaAdapter.h"
#include "FeedMemcProxy/client/talk_cache_client.h"
#include "FeedMemcProxy/client/talk_group_client.h"
#include "FeedMemcProxy/client/gc_message_client.h"
#include "FeedMemcProxy/client/talk_group_member_list.h"
#include "FeedMemcProxy/client/talk_group_view_client.h"

using namespace com::xiaonei::talk::function::muc;
using namespace com::xiaonei::talk::fun::state;
using namespace com::xiaonei::talk::group::memberloader;
using namespace com::xiaonei::talk::group::admmsgloader;
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::proto;
using namespace com::xiaonei::talk::group::loader;
using namespace xce::buddy::adapter;
using namespace com::xiaonei::talk;
using namespace xce::talk;
using namespace xce::talk::recent;
using namespace xce::xntalk;
using namespace xce::xntalk::groupchat;
using namespace xce::usercache;
using namespace std;
using namespace MyUtil;
using namespace mop::hi::svc::adapter;

/*说明：打印数据信息
 *参数1: 需要输出的数据
 */
void TalkFunTask::PrintResults(const Str2StrMap &print_map) {
  for (Str2StrMap::const_iterator test_iter=print_map.begin(); test_iter!=print_map.end(); test_iter++) {
    MCE_DEBUG(source_task_ << "   first:" << test_iter->first<<"   second:" << test_iter->second);
  }
} 

/*说明：处理需要发送的数据
 */
void TalkFunTask::handle() {
  TimeStat ts; 
  CreateResults();
  PrintResults(common_data_);
//  PrintResults(presence_data_);
//  SendData(common_data_);
//  SendData(presence_data_);
  SendData();
  FunStatManager::instance().Stat(source_task_, ts.getTime(), (ts.getTime() > 250.0)); 
}

/*说明：向talkproto服务发送结果数据
 */
void TalkFunTask::SendData() {
  if (common_data_.size() > 2) {
    try {
      TalkProtoAdapter::instance().Receive(common_data_);
    }catch(Ice::Exception& e) {    
      MCE_WARN("TalkFunMuc::SendCommonData:" << source_task_ << ": ------>TalkProtoAdapter::.Receive:  jid:"<< from_jid_ <<" error:" << e); 
    } 
  }
/*
  if (!presence_data_seq_.empty()) {
    try {
      TalkProtoAdapter::instance().BatchReceive(presence_data_seq_);
    }catch(Ice::Exception& e) {    
      MCE_WARN("TalkFunMuc::SendPresenceData:" << source_task_ << ": ------>TalkProtoAdapter::.BatchReceive:  jid:"<< from_jid_ <<" error:" << e); 
    } 
  }
 */
  for (vector<Str2StrMap>::const_iterator presence_iter=presence_data_seq_.begin(); presence_iter!=presence_data_seq_.end(); presence_iter++) {
    if ((*presence_iter).size() > 2) {
      try {
        TalkProtoAdapter::instance().Receive(*presence_iter);
      }catch(Ice::Exception& e) {    
        MCE_WARN("TalkFunMuc::SendPresenceData:" << source_task_ << ": ------>TalkProtoAdapter::.Receive:  jid:"<< from_jid_ <<" error:" << e); 
      } 
    }
  }
}

/*说明：如果指定的用户不在线，生成离线数据
 *参数1：离线消息
 *参数2：发送用户
 */
void TalkFunTask::SendOfflineAdminMsgs(MyUtil::Str2StrMap message,const IntSeq &receivers) {
  if (receivers.empty())
    return;
  JidSeqMap jids;
  try {
    jids = TalkFunStateAdapter::instance().getUsersJids(receivers, 4);    //如果人人桌面不在线，就算离线
  } catch (Ice::Exception &e) {
    MCE_WARN(source_task_ << "-->TalkFunTask::SendOfflineAdminMsgs ----> ids.size:"
        << receivers.size() <<"  TalkFunStateAdapter::instance().getUsersJids  error:" << e);
    is_correct_ = false;
    code_ = "internal-server-error";
    description_ = "服务暂时不可用";
  }
  if (is_correct_) {
    IntSeq offline_receivers;
    JidSeqMap::iterator jid_iter;
    for (IntSeq::const_iterator user_iter=receivers.begin(); user_iter!=receivers.end(); user_iter++) {
      jid_iter = jids.find(*user_iter);
      if (jid_iter == jids.end()) {
        MCE_DEBUG(source_task_ << "-->TalkFunTask::SendOfflineAdminMsgs ---->uid:" <<(*user_iter) << "   not found");
        offline_receivers.push_back(*user_iter);
      } else
        MCE_DEBUG(source_task_ << "-->TalkFunTask::SendOfflineAdminMsgs ---->uid:" <<(*user_iter)<<"   found");
    }
    MCE_DEBUG(source_task_ << "-->TalkFunTask::SendOfflineAdminMsgs ----> ids.size:" << offline_receivers.size());
    for (IntSeq::const_iterator offline_iter=offline_receivers.begin(); offline_iter!=offline_receivers.end(); offline_iter++) {
      message["to_id"] = boost::lexical_cast<string>(*offline_iter);
      string msg_str;
      TalkFunUtil::instance().Map2String(message, msg_str);
      MCE_DEBUG(source_task_ << "-->TalkFunTask::SendOfflineAdminMsgs ----> msg:" << msg_str);
      if (!msg_str.empty()) {
        try {
          TalkGroupAdminMsgLoaderAdapter::instance().saveMsgAsyn(*offline_iter, msg_str);
        }catch(Ice::Exception& e) {    
          MCE_WARN("TalkFunMuc::SendOfflineAdminMsgs:" << source_task_ <<" error:" << e); 
        } 
      }
    }
  }              
}

/*说明：创建群并返回结果
 */
void CreateGroupTask::CreateResults() {
  MCE_DEBUG("CreateGroupTask::CreateResults  IQ_RESULT:" << IQ_RESULT);

  int actor_uid = 0;
  string actor_uid_str;
  common_data_["message_type"] = boost::lexical_cast<string>(IQ_RESULT);
  Str2StrMap::const_iterator iter;
  if ((iter=paras_.find("actor_uid")) != paras_.end()) { 
    actor_uid_str = iter->second;
    actor_uid = boost::lexical_cast<int>(actor_uid_str);
    vector<string> receivers;
    receivers.push_back(iter->second);
    common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
  }
  
  int UserMaxSizeOfGroups = kUserMaxSizeOfSolidGroups;
  int UserMaxSizeOfGroupMembers = kUserMaxSizeOfSolidGroupMembers;
  if ((iter=paras_.find("iq:id")) != paras_.end())
    common_data_["iq.id"] = iter->second;
  Str2StrMap::const_iterator name_iter = paras_.find("iq.query.item:name");
  if (name_iter == paras_.end()) {
    code_ = "bad-request";
    description_ = "群名称缺失";
    is_correct_ = false;
  }
  MCE_DEBUG("CreateGroupTask::CreateResults1");
  if (is_correct_) {
    string group_name;
    string group_introduction;
    string group_auth = "confirm";
    string group_head_url;
    string group_type;
    string fixed;
    string item_type;
    string solid_group_type = boost::lexical_cast<string>(kSolidGroup);
    string discuss_group_type = boost::lexical_cast<string>(kDiscussGroup);
    group_name = name_iter->second;
    if (!TalkFunUtil::instance().SpamString(group_name, actor_uid)) {
      MCE_INFO("CreateGroupTask::CreateResults  SpamString error.    uid:" << actor_uid_str <<"   group_name:" << group_name);
      return;
    }
    if ((iter=paras_.find("iq.query.item:introduction")) != paras_.end())
      group_introduction = iter->second;
    if (!group_introduction.empty() && !TalkFunUtil::instance().SpamString(group_introduction, actor_uid)) {
      MCE_INFO("CreateGroupTask::CreateResults  SpamString error.    uid:" << actor_uid_str <<"   group_introduction:" <<group_introduction);
      return;
    }
    if ((iter=paras_.find("iq.query.item:auth")) != paras_.end())
      group_auth = iter->second;
    if ((iter=paras_.find("iq.query.item:portrait")) != paras_.end())
      group_head_url = iter->second;
    if ((iter=paras_.find("iq.query.item:fixed")) != paras_.end()) {
      fixed = iter->second;
      if (fixed == "1")
        group_type = solid_group_type; 
      else if (fixed == "0")
        group_type = discuss_group_type; 
    }
    MCE_DEBUG("CreateGroupTask::CreateResults2 actor_uid:" << actor_uid);
    int vip = TalkFunUtil::instance().CheckVip(actor_uid);
    MCE_DEBUG("CreateGroupTask::CreateResults2.5   vip" << vip);
    if (group_type == solid_group_type) {
      if (vip == 2) {
        UserMaxSizeOfGroups = kVipMaxSizeOfSolidGroups;
        UserMaxSizeOfGroupMembers = kVipMaxSizeOfSolidGroupMembers;
      }
      //判断已创建的固定群的数量
      LongSeq room_ids;
      try {
        TalkGroupViewLoaderAdapter::instance().GetCreatSessions(actor_uid, room_ids);
      } catch (Ice::Exception &e) {
        MCE_WARN("CreateGroupTask::CreateGroup----> uid:"<< actor_uid <<"   TalkGroupViewLoaderAdapter::instance().GetCreatSessions  error:" << e);
        code_ = "internal-server-error";
        description_ = str(boost::format("服务暂时不可用,%d 获取产生的群列表失败") % actor_uid);
        is_correct_ = false;
      }
      MCE_DEBUG("CreateGroupTask::CreateGroup----> uid:"<< actor_uid <<"    room_ids.size:"<< room_ids.size() << "   UserMaxSizeOfGroups:" <<UserMaxSizeOfGroups);
      if (room_ids.size() >= UserMaxSizeOfGroups) {
        code_ = "resource-constraint";
        description_ = str(boost::format("%d 创建的群超出上线") % actor_uid);;
        is_correct_ = false;
      }
    } else {
      UserMaxSizeOfGroups = kVipMaxSizeOfDiscussGroups;
      UserMaxSizeOfGroupMembers = kVipMaxSizeOfDiscussGroupMembers;
    } 
    MCE_DEBUG("CreateGroupTask::CreateResults3  UserMaxSizeOfGroups:" << UserMaxSizeOfGroups << "    UserMaxSizeOfGroupMembers:" << UserMaxSizeOfGroupMembers);
    if (is_correct_) {
      MCE_DEBUG("CreateGroupTask::CreateResults22");
      Ice::Long group_id = 0;
      try {
        group_id = IMIdGeneratorAdapter::instance().getId(21);
      } catch (Ice::Exception &e) {
        MCE_WARN("CreateGroupTask::CreateGroup----> uid:"<< actor_uid <<"   IMIdGeneratorAdapter::instance().getId(21)  error:" << e);
        code_ = "internal-server-error";
        description_ = "服务暂时不可用,群id产生失败";
        is_correct_ = false;
      }
      if (is_correct_) {
        MCE_DEBUG("CreateGroupTask::CreateResults33");
        GroupInfoPtr group_info = new GroupInfo;
        group_info->id = group_id;
        group_info->name = group_name;
//        group_info->name = TalkFunUtil::instance().DBEscapeString(group_name);
        group_info->head = group_head_url;

        MCE_DEBUG("CreateGroupTask::CreateResults33.5   to_id:" << common_data_["to_id"] << "    group_type:" << group_type);
        group_info->creator = actor_uid;
        group_info->introduction = group_introduction;
        //group_info->introduction = TalkFunUtil::instance().DBEscapeString(group_introduction);
        group_info->type = boost::lexical_cast<int>(group_type);
        group_info->authentication = group_auth;
        group_info->level = 1;
        group_info->toplimit = UserMaxSizeOfGroupMembers;
        group_info->saveonline = 0;
        group_info->notify = "all";

        MCE_DEBUG("CreateGroupTask::CreateResults44");
        try {
          TalkGroupLoaderAdapter::instance().AddGroupInfo(group_info);
        } catch (Ice::Exception &e) {
          MCE_WARN("CreateGroupTask::CreateGroup----> uid:"<< actor_uid <<"   TalkGroupLoaderAdapter::instance().SetGroupInfo  error:" << e);
          code_ = "internal-server-error";
          description_ = str(boost::format("房间%d 创建失败") % group_id );
          is_correct_ = false;
        }
        if (is_correct_) {
          GroupMemberDict members_dict;
          GroupMemberSeq members;
          GroupMemberPtr member = new GroupMember;
          member->uid = actor_uid;
          member->level = kOwner;
          members.push_back(member); 
          members_dict.insert(pair<Ice::Long, GroupMemberSeq>(group_id, members));
          bool add_ret = true;
          try {
            add_ret = TalkGroupMemberLoaderAdapter::instance().AddNewGroup(members_dict);
          } catch (Ice::Exception &e) {
            MCE_WARN("CreateGroupTask::CreateGroup----> uid:"<< actor_uid <<"    TalkGroupMemberLoaderAdapter::instance().AddNewGroup  error:" << e);
            add_ret = false;
          }
          if (!add_ret) {
            code_ = "internal-server-error";
            description_ = str(boost::format("用户%d 加入房间%d 失败") %actor_uid % group_id );
            is_correct_ = false;
          }
          if (is_correct_) {
            TalkGroupViewPtr group_view = new TalkGroupView;
            group_view->userId = actor_uid;
            group_view->sessionId = group_id;
            group_view->isVisible = 1;
            group_view->hasRead = 0;
            group_view->sessionType = boost::lexical_cast<int>(group_type);
            group_view->affiliation = kOwner;
            MCE_DEBUG("CreateGroupTask::CreateResults ---->  current_time:" << TalkFunUtil::instance().GetCurrentTime());
            group_view->updateTime = TalkFunUtil::instance().GetCurrentTime();
            group_view->saveonline = 0;
            group_view->notify = "all";
            group_view->msgid = 0;
            try {
              is_correct_ = TalkGroupViewLoaderAdapter::instance().Update(actor_uid, group_view); //单调一次16ms，1000次3703ms
            } catch (Ice::Exception &e) {
              MCE_WARN("CreateGroupTask::CreateResults ----> uid:"<< actor_uid <<"    TalkGroupViewLoaderAdapte::instance().Update error:" << e);
              is_correct_ = false;
            }
            if (!is_correct_) {
              code_ = "internal-server-error";
              description_ = str(boost::format("用户%d 写入房间%d 视图失败") %actor_uid% group_id );
            }
          }
          MCE_DEBUG("CreateGroupTask::CreateResults55");
          if (is_correct_) {
            common_data_["iq.roomid"] = boost::lexical_cast<string>(group_id);
            common_data_["iq.roomname"] = TalkFunUtil::instance().XMLEncode(group_name);
            common_data_["item_type"] = "1";

            string ownername;
            TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(actor_uid);
            if (u) {
              ownername = u->name;
            } 
            Str2StrMap presence_data;
            presence_data["message_type"] = boost::lexical_cast<string>(PRESENCE_COMMON);
            presence_data["config_type"] = config_type_;
            presence_data["item_type"] = "3";
            presence_data["from_jid"] = from_jid_;
            presence_data["presence.roomname"] = TalkFunUtil::instance().XMLEncode(group_name);
            presence_data["presence.authentication"] = group_auth;
            presence_data["presence.introduction"] = TalkFunUtil::instance().XMLEncode(group_introduction);
            presence_data["presence.portrait"] = group_head_url;
            presence_data["presence.roomid"] = boost::lexical_cast<string>(group_id);
            presence_data["presence.fixed"] = fixed;
            presence_data["presence.owner"] = actor_uid_str;
            presence_data["presence.ownername"] = ownername;
            presence_data["presence.saveonline"] = "0";
            presence_data["presence.notify"] = "all";
            
            MCE_DEBUG("CreateGroupTask::CreateResults66   fixed:" << fixed << "   actor_uid:" << actor_uid_str);
            vector<int> receivers;
            receivers.push_back(actor_uid);
            presence_data["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
            presence_data_seq_.push_back(presence_data);
          }
        }
      }
    }
  }
  MCE_DEBUG("CreateGroupTask::CreateResults5");
  if (!is_correct_) {
    common_data_["item_type"] = "2";
    common_data_["iq.code"] = code_;
    common_data_["iq.reason"] = description_;
    common_data_["iq.roomid"] = "error";
  }
}

/*说明：搜索群并返回结果
 */
void SerachGroupTask::CreateResults() {
  MCE_DEBUG("SerachGroupTask::CreateResults  IQ_RESULT:" << IQ_RESULT);
  string actor_uid;
  common_data_["message_type"] = boost::lexical_cast<string>(IQ_RESULT);
  Str2StrMap::const_iterator iter;
  if ((iter=paras_.find("actor_uid")) != paras_.end()) { 
    actor_uid = iter->second;
    vector<string> receivers;
    receivers.push_back(iter->second);
    common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
  }
  if ((iter=paras_.find("iq:id")) != paras_.end())
    common_data_["iq.id"] = iter->second;
  string serach_type, serach_content;
  if ((iter=paras_.find("iq.query.type")) != paras_.end())
    serach_type = iter->second;
  if ((iter=paras_.find("iq.query.content")) != paras_.end())
    serach_content = iter->second;
  if (serach_type=="" || serach_content=="") {
    is_correct_ = false;
    code_ = "bad-request";
    description_ = "搜索类型或搜索内容缺失";
  }
  MCE_DEBUG("SerachGroupTask::CreateResults  serach_type:" << serach_type << "   serach_conetnt:" <<serach_content);
  if (is_correct_) {
    if (serach_type == "roomid") {
      bool get_group_info = true;
      Ice::Long roomid = 0;
      try {
        roomid = boost::lexical_cast<Ice::Long>(serach_content);
      } catch(boost::bad_lexical_cast& e)  {  
        MCE_WARN(" SerachGroupTask::CreateResults ----> uid:"<< actor_uid <<"  serach_content:" << serach_content << " error:" << e.what());
        get_group_info = false;
      }
      GroupInfoPtr group_info;
      if (get_group_info) {
        if (!TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad(roomid,group_info)) {
          MCE_WARN("SerachGroupTask::CreateResults--->TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad  uid:" 
              << actor_uid << "  roomid:" << roomid);
        //  code_ = "internal-server-error";
        //  description_ = str(boost::format("获取房间%d 信息失败") % roomid );
        //  is_correct_ = false;
        } 
      }
      MCE_DEBUG(" SerachGroupTask::CreateResults  found. roomid:" << roomid);
      common_data_["item_type"] = "1";
      if (group_info) {
        common_data_["iq.query.0.roomjid"] = boost::lexical_cast<string>(group_info->id) +"@conference.talk.renren.com";
        common_data_["iq.query.0.roomname"] =  TalkFunUtil::instance().XMLEncode(group_info->name);
        common_data_["iq.query.0.owner"] = boost::lexical_cast<string>(group_info->creator);
        common_data_["iq.query.0.introduction"] =  TalkFunUtil::instance().XMLEncode(group_info->introduction);
        common_data_["iq.query.0.portrait"] = group_info->head;
        common_data_["iq.query.0.authentication"] = group_info->authentication;
        string is_fixed = "0";
        if (group_info->type == kSolidGroup)
          is_fixed = "1"; 
        else if (group_info->type == kDiscussGroup)
          is_fixed = "0"; 
         common_data_["iq.query.0.fixed"] = is_fixed;
        string username;
        TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(boost::lexical_cast<int>(group_info->creator));
        if (u) {
          username = u->name;
        } 
        common_data_["iq.query.0.ownername"] =  TalkFunUtil::instance().XMLEncode(username);
        GroupMemberSeq gm_seq;
        if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad(roomid, gm_seq)) {
          MCE_WARN("SerachGroupTask::CreateResults---> TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad uid:" << actor_uid << "  roomid:" << roomid);
        }
        common_data_["iq.query.0.count"] = boost::lexical_cast<string>(gm_seq.size());
      }
    } else if (serach_type == "keywords") {
    }
  }
  if (!is_correct_) {
    common_data_["item_type"] = "2";
    common_data_["iq.roomid"] = "error";
    common_data_["iq.code"] = code_;
    common_data_["iq.reason"] = description_;
  }
}

/*说明：获取群列表并返回结果
 */
void GetMembersTask::CreateResults() {
  common_data_["message_type"] = boost::lexical_cast<string>(IQ_RESULT);
  string serach_type, serach_content;
  Str2StrMap::const_iterator iter;
  int actor_uid = 0;
  if ((iter=paras_.find("actor_uid")) != paras_.end()) { 
    actor_uid = boost::lexical_cast<int>(iter->second);
    vector<string> receivers;
    receivers.push_back(iter->second);
    common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
  }
  if ((iter=paras_.find("iq:id")) != paras_.end())
    common_data_["iq.id"] = iter->second;
  Ice::Long room_id = 0;
  if ((iter=paras_.find("iq:to")) != paras_.end()) {
    string room_id_str = TalkFunUtil::instance().GetIDFromJID(iter->second);
    common_data_["iq.roomid"] = room_id_str; 
    room_id = boost::lexical_cast<Ice::Long>(room_id_str);
  }
  if (room_id == 0) {
    is_correct_ = false;
    code_ = "bad-request";
    description_ = "请求房间的id错误";
  }
  if (is_correct_) {
    GroupMemberSeq gm_seq;
    if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad(room_id, gm_seq)) {
      MCE_WARN("GetMembersTask::CreateResults---> TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad uid:" << actor_uid << "  roomid:" << room_id);
      is_correct_ = false;
      code_ = "internal-server-error";
      description_ = str(boost::format("获取房间%d 成员信息失败") % room_id );
    }
    IntSeq memberids;
    GroupMemberInfoDict members_info;
    if (is_correct_) {
      if (gm_seq.size() > 0) {
        for (size_t i=0; i<gm_seq.size(); ++i) {
          GroupMemberInfoPtr member_info = new GroupMemberInfo;
          member_info->uid = gm_seq.at(i)->uid;
          memberids.push_back(member_info->uid);
          member_info->affiliation = TalkFunUtil::instance().PrivilegeInt2String(gm_seq.at(i)->level);
          member_info->state = "offline";
          members_info.insert(pair<int, GroupMemberInfoPtr>(member_info->uid, member_info));
        }
        AvatarMap online_info;
        try {
          online_info = TalkFunStateAdapter::instance().getUsersAvatars(memberids); 
        }
        catch (Ice::Exception &e) {
          MCE_WARN("GetMembersTask::CreateResults----> TalkFunStateAdapter::instance().GetUsersOnlineInfo  ids.size:"<< memberids.size() <<"  error:" << e);
          /*
          is_correct_ = false;
          code_ = "internal-server-error";
          description_ = "服务暂时不可用";
          */
        }
        for (AvatarMap::const_iterator avatar_iter=online_info.begin(); avatar_iter!=online_info.end(); avatar_iter++) {
          members_info[avatar_iter->first]->state = TalkFunUtil::instance().GetOnlineStatFromAvatars(avatar_iter->second);
          members_info[avatar_iter->first]->equipment = TalkFunUtil::instance().GetOnlineTypeFromAvatars(avatar_iter->second);
        }
        map<int, UserCachePtr> user_cache;
        try {
          user_cache = UserCacheAdapter::instance().GetUserCache(memberids);
        } catch (Ice::Exception &e) {
          MCE_WARN("GetMembersTask::CreateResults----> UserCacheAdapter::instance().GetUserCache    ids.size:"<< memberids.size() <<"    error:" << e);
        }
        for (map<int, UserCachePtr>::const_iterator user_iter=user_cache.begin(); user_iter!=user_cache.end(); user_iter++) {        
          members_info[user_iter->first]->gender = TalkFunUtil::instance().CutUtf8(user_iter->second->gender(), 1);
          members_info[user_iter->first]->name = user_iter->second->name();
          members_info[user_iter->first]->tinyurl = user_iter->second->tinyurl();
          /*
          TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(user_iter->first);
          if (!u) {
            MCE_WARN("GetMembersTask::CreateResults---> TalkCacheClient::instance().GetUserByIdWithLoad  uid:" <<user_iter->first);
          } else
            members_info[user_iter->first]->tinyurl = u->tinyurl;
            */
          MCE_DEBUG("GetMembersTask::CreateResults----> UserCacheAdapter::instance().GetUserCache  uid: " <<user_iter->first<<"   tinyurl:"<< user_iter->second->tinyurl());
        }
        int pos = 0;
        for (GroupMemberInfoDict::const_iterator members_iter=members_info.begin(); members_iter!=members_info.end(); members_iter++,pos++) {
          common_data_[str(boost::format("iq.query.%d.uid") % pos)] = boost::lexical_cast<string>(members_iter->first);
          common_data_[str(boost::format("iq.query.%d.name") % pos)] =  TalkFunUtil::instance().XMLEncode(members_iter->second->name);
          common_data_[str(boost::format("iq.query.%d.affiliation") % pos)] = members_iter->second->affiliation;
          common_data_[str(boost::format("iq.query.%d.gender") % pos)] = members_iter->second->gender;
          common_data_[str(boost::format("iq.query.%d.state") % pos)] = members_iter->second->state;
          common_data_[str(boost::format("iq.query.%d.equipment") % pos)] = members_iter->second->equipment;
          common_data_[str(boost::format("iq.query.%d.portrait") % pos)] =  PhotoUrlHelper::instance().GetFullUrl(members_iter->second->tinyurl);
          string network;
          NetworkSeq networks;
          try {
            networks = UserNetworkAdapter::instance().getNetworkSeq(members_iter->first);
          } catch (Ice::Exception &e) {
            MCE_WARN("GetMembersTask::CreateResults----> UserNetworkAdapter::instance().getNetworkSeq   networks.size:"<< networks.size() <<"    error:" << e);
          }
          if (networks.size() > 0) {
            network = networks.at(networks.size()-1)->networkName; 
          } 
          common_data_[str(boost::format("iq.query.%d.network") % pos)] = network;
        }
      }
      common_data_["item_type"] = "1";
    }
  }
  if (!is_correct_) {
    common_data_["item_type"] = "2";
    common_data_["iq.code"] = code_;
    common_data_["iq.reason"] = description_;
  }
}

/*说明：设置群信息并返回结果
 */
void SetGroupConfigTask::CreateResults() {
  common_data_["message_type"] = boost::lexical_cast<string>(IQ_RESULT);
  Str2StrMap new_group_infos;
  Str2StrMap::const_iterator iter;
  Ice::Long roomid=0;
  string roomid_str;
  if ((iter=paras_.find("iq:id")) != paras_.end())
    common_data_["iq.id"] = iter->second;
  if ((iter=paras_.find("iq:to")) != paras_.end()) {
    roomid_str = common_data_["iq.roomid"] = TalkFunUtil::instance().GetIDFromJID(iter->second);   
    roomid = boost::lexical_cast<Ice::Long>(roomid_str);
  }
  int actor_uid=0;
  string actor_uid_str;
  if ((iter=paras_.find("actor_uid")) != paras_.end()) {
    actor_uid_str = iter->second;
    actor_uid = boost::lexical_cast<int>(actor_uid_str);
  }
  IntSeq receivers;
  receivers.push_back(actor_uid);
  common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
  string type;
  if ((iter=paras_.find("iq.query.type")) != paras_.end())
    type = iter->second;
  GroupMemberSeq gm_seq;
  if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad(roomid, gm_seq)) {
    MCE_WARN("SetGroupConfigTask::CreateResults---> TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad uid:"
        << actor_uid << "  roomid:" << roomid);
    is_correct_ = false;
    code_ = "internal-server-error";
    description_ = str(boost::format("获取房间%d 成员信息失败") % roomid );
  }
  int ownerid = 0;
  Ice::Int actor_level = kMember;
  for (GroupMemberSeq::const_iterator iter=gm_seq.begin(); iter!=gm_seq.end(); iter++) {
    MCE_DEBUG("SetGroupConfigTask::CreateResults--->    uid:" << (*iter)->uid << "    level:" << (*iter)->level);
    if ((*iter)->level==kOwner) 
      ownerid = (*iter)->uid;
    if ((*iter)->uid == actor_uid)
      actor_level = (*iter)->level;
  }
  if (type == "all") {
    if (actor_level == kMember) {
      is_correct_ = false;
      code_ = "forbidden";
      description_ = "群成员没有权限修改群信息";
    }
  }
  MCE_DEBUG("SetGroupConfigTask::CreateResults   type:" << type << "   actor_uid:"<<actor_uid);
  if (is_correct_) {
    Str2StrMap presence_data;
    presence_data["presence.roomid"] = roomid_str;
    presence_data["presence.show"] = "1";
    if (type == "all") {
      if ((iter=paras_.find("iq.query.name")) != paras_.end()) {
        string name = iter->second;
        if (!TalkFunUtil::instance().SpamString(name, actor_uid)) {
          MCE_INFO("SetGroupConfigTask::CreateResults  SpamString error.    uid:" << actor_uid_str <<"   name:" <<name);
          return;
        }
        presence_data["presence.roomname"] =  TalkFunUtil::instance().XMLEncode(name); 
        new_group_infos["session_name"] = str(boost::format("'%s'") % name);
      }
      if ((iter=paras_.find("iq.query.icon")) != paras_.end()) {
        presence_data["presence.portrait"] =iter->second; 
        new_group_infos["session_head"] = str(boost::format("'%s'") % iter->second);
      }
      if ((iter=paras_.find("iq.query.announcement")) != paras_.end()) {
        string announcement = iter->second;
        if (!TalkFunUtil::instance().SpamString(announcement, actor_uid)) {
          MCE_INFO("SetGroupConfigTask::CreateResults  SpamString error.    uid:" << actor_uid_str <<"   announcement:" <<announcement);
          return;
        }
        presence_data["presence.announcement"] = TalkFunUtil::instance().XMLEncode(announcement); 
        new_group_infos["announcement"] = str(boost::format("'%s'") % announcement);
      }
      if ((iter=paras_.find("iq.query.introduction")) != paras_.end()) {
        string introduction = iter->second;
        if (!TalkFunUtil::instance().SpamString(introduction, actor_uid)) {
          MCE_INFO("SetGroupConfigTask::CreateResults  SpamString error.    uid:" << actor_uid_str <<"   introduction:" <<introduction);
          return;
        }
        presence_data["presence.introduction"] = TalkFunUtil::instance().XMLEncode(introduction); 
        new_group_infos["session_intro"] =  str(boost::format("'%s'") % introduction); 
      }
      if ((iter=paras_.find("iq.query.authentication")) != paras_.end()) {
        presence_data["presence.authentication"] = iter->second; 
        new_group_infos["authentication"] =  str(boost::format("'%s'") % iter->second);   
      }
      presence_data["presence.fixed"] = "1";
      presence_data["presence.owner"] = boost::lexical_cast<string>(ownerid);
      string ownername;
      TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(ownerid);
      if (!u) {
        MCE_WARN("SetGroupConfigTask::CreateResults---> TalkCacheClient::instance().GetUserByIdWithLoad  uid:" << ownerid);
      }else
        ownername = u->name; 
      presence_data["presence.ownername"] = ownername;
      try {
        MCE_DEBUG("SetGroupConfigTask::CreateResults---->update groupinfo  uid:"<< actor_uid <<"   roomid:"<< roomid );
        is_correct_ = TalkGroupLoaderAdapter::instance().UpdateGroupInfo(roomid, new_group_infos); 
      } catch (Ice::Exception &e) {
        MCE_WARN("SetGroupConfigTask::CreateResults---->TalkGroupLoaderAdapter::instance().SetGroupInfo  uid:"<< actor_uid <<"   roomid:"<< roomid <<"   error:" << e);
        is_correct_ = false;
      }
    }
    
//    for (Str2StrMap::const_iterator temp_iter=new_group_infos.begin(); temp_iter!=new_group_infos.end(); temp_iter++)
//      MCE_DEBUG("SetGroupConfigTask::CreateResults---->   new_group_infos   key:" << temp_iter->first << "   seoncd:" <<temp_iter->second );  
    new_group_infos.clear();
    if ((iter=paras_.find("iq.query.notify")) != paras_.end()) {
      presence_data["presence.notify"] = iter->second; 
      new_group_infos["notify"] = iter->second;
    }
    if ((iter=paras_.find("iq.query.saveonline")) != paras_.end()) 
      new_group_infos["saveonline"] = presence_data["presence.saveonline"] = iter->second; 
    if (!new_group_infos.empty()) {
      try {
        is_correct_ = TalkGroupViewLoaderAdapter::instance().UpdateItem(actor_uid, roomid, new_group_infos); 
      } catch (Ice::Exception &e) {
        MCE_WARN("SetGroupConfigTask::CreateResults ----> uid:"<< actor_uid <<"  TalkGroupViewLoaderAdapter::instance().UpdateItem  error:" << e);
        is_correct_ = false;
      }
    }
    if (is_correct_) {
      common_data_["item_type"] = "1";
      presence_data["item_type"] = "3";
      presence_data["message_type"] = boost::lexical_cast<string>(PRESENCE_COMMON);
      presence_data["config_type"] = config_type_;
      presence_data["from_jid"] = from_jid_;
      presence_data["presence.roomid"] = roomid_str;
      presence_data["presence.type"] = type;

      if (type == "all") {
        GroupMemberSeq gm_seq;
        if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad(roomid, gm_seq)) {
          MCE_WARN("SetGroupConfigTask::CreateResults---> TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad  uid:" 
              << actor_uid << "  roomid:" << roomid);
          is_correct_ = false;
          code_ = "internal-server-error";
          description_ = str(boost::format("获取房间%d 成员信息失败") % roomid );
        }
        for (GroupMemberSeq::const_iterator member_iter=gm_seq.begin(); member_iter!=gm_seq.end(); member_iter++) { 
          Str2StrMap temp_presence;
          temp_presence.insert(presence_data.begin(), presence_data.end());
          if (actor_uid==(*member_iter)->uid && type=="all")
            temp_presence["presence.show"] = "0";

          IntSeq presence_receivers;      
          presence_receivers.push_back((*member_iter)->uid);
          temp_presence["to_id"] = ReceiversHandler::ReceiversAssembly(presence_receivers);
          temp_presence["presence.affiliation"] = TalkFunUtil::instance().PrivilegeInt2String((*member_iter)->level);
          presence_data_seq_.push_back(temp_presence);
        }
      } else if (type == "notify") {
          presence_data["presence.show"] = "0";
          IntSeq presence_receivers;      
          presence_receivers.push_back(actor_uid);
          presence_data["to_id"] = ReceiversHandler::ReceiversAssembly(presence_receivers);
          presence_data_seq_.push_back(presence_data);
      }
    }
    if (!is_correct_) {
      code_ = "internal-server-error";
      description_ = str(boost::format("更新房间%d 信息失败") % roomid );
    }
  }
  if (!is_correct_) {
    common_data_["item_type"] = "2";
    common_data_["iq.code"] = code_;
    common_data_["iq.reason"] = description_;
  }
}

/*说明：获取群信息并返回结果
 */
void GetGroupConfigTask::CreateResults() {
  common_data_["message_type"] = boost::lexical_cast<string>(IQ_RESULT);
  string roomid_str;
  Str2StrMap new_group_infos;
  Str2StrMap::const_iterator iter;
  if ((iter=paras_.find("iq:id")) != paras_.end())
    common_data_["iq.id"] = iter->second;
  if ((iter=paras_.find("iq:to")) != paras_.end()) 
    roomid_str = common_data_["iq.roomid"] = TalkFunUtil::instance().GetIDFromJID(iter->second);
  if (roomid_str == "") {
    code_ = "bad-request";
    description_ = "群JID错误";
    is_correct_ = false;
  }
  MCE_DEBUG("GetGroupConfigTask::CreateResults  step1   roomid_str:" << roomid_str );
  int actor_uid = 0;
  if ((iter=paras_.find("actor_uid")) != paras_.end()) { 
    actor_uid = boost::lexical_cast<int>(iter->second);
    vector<string> receivers;
    receivers.push_back(iter->second);
    common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
  }
  MCE_DEBUG("GetGroupConfigTask::CreateResults  step2.  actor_uid:" << actor_uid);
  if (is_correct_) {
    Ice::Long roomid = boost::lexical_cast<Ice::Long>(roomid_str);
    GroupInfoPtr group_info;
    if (!TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad(roomid,group_info)) {
      MCE_WARN("GetGroupConfigTask::CreateResults--->TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad uid:" << actor_uid << "  roomid:" << roomid_str);
      code_ = "internal-server-error";
      description_ = str(boost::format("获取房间%s 信息失败") % roomid_str);
      is_correct_ = false;
    } 
    if (!group_info) {
      code_ = "resource_unavailable";
      description_ = str(boost::format("获取房间%d 不存在") % roomid);
      is_correct_ = false;
    }
    MCE_DEBUG("GetGroupConfigTask::CreateResults  step3. ");
    if (is_correct_) {
      string ownername;
      TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(group_info->creator);
      if (!u) {
        MCE_WARN("GetGroupConfigTask::CreateResults---> TalkCacheClient::instance().GetUserByIdWithLoad  uid:" << group_info->creator);
      }else
        ownername = u->name; 
      MCE_DEBUG("GetGroupConfigTask::CreateResults  stemp 4found. roomid:" << roomid);
      common_data_["item_type"] = "1";
      common_data_["iq.roomid"] = boost::lexical_cast<string>(group_info->id);
      common_data_["iq.name"] = TalkFunUtil::instance().XMLEncode(group_info->name);
      common_data_["iq.roomowner"] = boost::lexical_cast<string>(group_info->creator);
      common_data_["iq.ownername"] = ownername;
      common_data_["iq.introduction"] = TalkFunUtil::instance().XMLEncode(group_info->introduction);
      common_data_["iq.icon"] = group_info->head;
      common_data_["iq.announcement"] = TalkFunUtil::instance().XMLEncode(group_info->announcement);
      common_data_["iq.authentication"] = group_info->authentication;
      common_data_["iq.notify"] = group_info->notify;
      common_data_["iq.saveonline"] = boost::lexical_cast<string>(group_info->saveonline);
//      common_data_["iq.level"] = boost::lexical_cast<string>(group_info->level);
 //     common_data_["iq.toplimit"] = boost::lexical_cast<string>(group_info->toplimit);
    }
  }
  if (!is_correct_) {
    common_data_["item_type"] = "2";
    common_data_["iq.code"] = code_;
    common_data_["iq.reason"] = description_;
  }
}

/*说明：加入群并返回结果
 */
void JoinGroupTask::JoinGroup(const Ice::Long roomid, const int receiverid) {
  MCE_DEBUG(source_task_ << "::JoinGroup ----> roomid:"<< roomid <<"    receiverid:" << receiverid);
  GroupMemberSeq members;
  GroupMemberPtr member = new GroupMember;
  member->uid = receiverid;
  member->level = kMember;
  members.push_back(member); 
  bool add_ret = true;
  try {
    add_ret = TalkGroupMemberLoaderAdapter::instance().AddNewMember(roomid, members);
  } catch (Ice::Exception &e) {
    MCE_WARN(source_task_ << "::CreateResults ----> uid:"<< receiverid <<"    TalkGroupMemberLoaderAdapter::instance().AddNewMember  error:" << e);
  }
  if (!add_ret) {
    code_ = "internal-server-error";
    description_ = str(boost::format("用户%d 加入房间%d 失败") %receiverid % roomid );
    is_correct_ = false;
  }
  if (is_correct_) {
    TalkGroupViewPtr group_view = new TalkGroupView;
    group_view->userId = receiverid;
    group_view->sessionId = roomid;
    group_view->isVisible = 1;
    group_view->hasRead = 0;
    group_view->sessionType = 0;
    group_view->affiliation = kMember;
    group_view->updateTime = TalkFunUtil::instance().GetCurrentTime();
    group_view->saveonline = 0;
    group_view->notify = "all";
    group_view->msgid = 0;
    bool ret = true;
    try {
      ret = TalkGroupViewLoaderAdapter::instance().Update(receiverid, group_view);
    } catch (Ice::Exception &e) {
      MCE_WARN(source_task_ << "::CreateResults ----> uid:"<< receiverid <<"    TalkGroupMemberLoaderAdapter::instance().AddNewMember  error:" << e);
      ret = false;
    }
    if (!ret) {
      code_ = "internal-server-error";
      description_ = str(boost::format("用户%d 写入房间%d 视图失败") %receiverid % roomid );
      is_correct_ = false;
    }
  }
}

/*说明：加入群时检测是否群已满并返回结果
 *参数1：群主id
 *参数2：群大小
 *返回值：是否群已满
 */
bool JoinGroupTask::CheckGroupFull(const int owner_id, const int group_size) {
  bool ret = true;
  size_t  UserMaxSizeOfGroupMembers = kUserMaxSizeOfSolidGroupMembers;
  int vip = TalkFunUtil::instance().CheckVip(owner_id);
  if (vip == 2) 
    UserMaxSizeOfGroupMembers = kVipMaxSizeOfSolidGroupMembers ;
  if (group_size >= UserMaxSizeOfGroupMembers) 
    ret = false;
  return ret;
}

/*说明：加入群后向群中其它成员发送presence
 *参数1：房间id
 *参数2：接受人id
 *参数3：接受人id
 */
void JoinGroupTask::SendPresence(const Ice::Long room_id, const int receiverid, const string &to_id) {
  IntSeq userids;
  userids.push_back(receiverid);
  map<int, UserCachePtr> user_cache;
  try {
    user_cache = UserCacheAdapter::instance().GetUserCache(userids);
  } catch (Ice::Exception &e) {
    MCE_WARN(source_task_ << "::CreateResults----> ids.size:"<< userids.size() <<"  UserCacheAdapter::instance().GetUserCache  error:" << e);
  }
  if (is_correct_) {
    string user_name, user_gender;
    map<int, UserCachePtr>::const_iterator user_iter = user_cache.find(receiverid);
    if (user_iter != user_cache.end()) {
      user_gender = TalkFunUtil::instance().CutUtf8(user_iter->second->gender(),1);
      user_name = user_iter->second->name();
    }
    Str2StrMap presence_data;
    GroupInfoPtr group_info;
    if (!TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad(room_id,group_info)) {
      MCE_WARN(source_task_ << "::CreateResults--->TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad  uid:" 
          << receiverid << "  roomid:" << room_id);
//      code_ = "internal-server-error";
 //     description_ = str(boost::format("获取房间%d 信息失败") % roomid );
//      is_correct_ = false;
    } 
    MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step2   ");
    if (group_info) {
      string ownername;
      TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(group_info->creator);
      if (u) {
        ownername = u->name;
      } 
      vector<int> receivers;
      receivers.push_back(receiverid);
      presence_data["to_id"] = ReceiversHandler::ReceiversAssembly(receivers); ;
      presence_data["config_type"] = config_type_;
      presence_data["actor_jid"] = from_jid_;
      presence_data["item_type"] = "5"; 
      presence_data["presence.roomid"]  = boost::lexical_cast<string>(room_id);
      presence_data["presence.roomname"] = TalkFunUtil::instance().XMLEncode(group_info->name);
      presence_data["presence.portrait"] = "www.weibo.com";
      presence_data["presence.announcement"] = TalkFunUtil::instance().XMLEncode(group_info->announcement);
      presence_data["presence.affiliation"] = "member";
      presence_data["presence.fixed"] = "1";
      presence_data["presence.introduction"] = TalkFunUtil::instance().XMLEncode(group_info->introduction);
      presence_data["presence.authentication"] = group_info->authentication;
      presence_data["presence.notify"] = group_info->notify;
      presence_data["presence.saveonline"] = boost::lexical_cast<string>(group_info->saveonline);
      presence_data["presence.owner"] = boost::lexical_cast<string>(group_info->creator);
      presence_data["presence.ownername"] = ownername;
      presence_data_seq_.push_back(presence_data);
    }
    presence_data.clear();

    presence_data["config_type"] = config_type_;
    presence_data["actor_jid"] = from_jid_;
    presence_data["to_id"] = to_id;
    presence_data["item_type"] = "3"; 
    presence_data["message_type"] = boost::lexical_cast<string>(PRESENCE_COMMON);        
    presence_data["presence.roomid"]  = boost::lexical_cast<string>(room_id);
    presence_data["presence.from_name"]  = TalkFunUtil::instance().XMLEncode(user_name);
    presence_data["presence.applicant_sex"]  = user_gender;
    presence_data["presence.applicantid"]  = boost::lexical_cast<string>(receiverid);
    AvatarNSeqMap online_info;
    try {
      online_info = TalkFunStateAdapter::instance().GetUsersOnlineInfo(userids); 
    } catch (Ice::Exception &e) {
      MCE_WARN(source_task_ << "::CreateResults----> ids.size:"<< userids.size() <<"  TalkFunStateAdapter::instance().GetUsersOnlineInfo  error:" << e);
      /*
      is_correct_ = false;
      code_ = "internal-server-error";
      description_ = "服务暂时不可用";
      */
    }
    string state="offline", equipment;
    AvatarNSeqMap::const_iterator avatar_iter = online_info.find(receiverid);
    if (avatar_iter != online_info.end()) {
      JidSeq jids;
      for (AvatarNSeq::const_iterator seq_iter=(avatar_iter->second).begin(); seq_iter!=(avatar_iter->second).end(); seq_iter++)
        jids.push_back((*seq_iter)->jid);
      equipment =  TalkFunUtil::instance().GetOnlineTypeFromJids(jids);
      state = "online";
    }
    string network;
    NetworkSeq networks;
    try {
      networks = UserNetworkAdapter::instance().getNetworkSeq(receiverid);
    } catch (Ice::Exception &e) {
      MCE_WARN(source_task_ << "::CreateResults----> UserNetworkAdapter::instance().getNetworkSeq  uid:"<< receiverid 
          <<" networks.size:"<< networks.size() <<"    error:" << e);
    }
    if (networks.size() > 0) {
      network = networks.at(networks.size()-1)->networkName; 
    } 
    presence_data["presence.applicant_network"]  = network;
    presence_data["presence.applicant_state"]  = state;
    presence_data["presence.applicant_equipment"]  = equipment;
    presence_data_seq_.push_back(presence_data);
  }
}

/*说明：申请加入群并返回结果
 */
void ApplyToJoinGroupTask::CreateResults() {
  common_data_["message_type"] = boost::lexical_cast<string>(IQ_RESULT);
  string actor_uid_str, roomid_str;
  int actor_uid;
  Str2StrMap::const_iterator iter;
  if ((iter=paras_.find("message:to")) != paras_.end()) 
    roomid_str = common_data_["message.roomid"] = TalkFunUtil::instance().GetIDFromJID(iter->second);
  if ((iter=paras_.find("actor_uid")) != paras_.end()) { 
    actor_uid_str = common_data_["from_uid"] = iter->second;
    actor_uid = boost::lexical_cast<int>(actor_uid_str);
  }
  if (!TalkFunUtil::instance().CheckApplyLimit(actor_uid))
    return;
  MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step1   actor_uid:" << actor_uid<<"   roomid_str:" <<roomid_str);
  Ice::Long roomid = boost::lexical_cast<Ice::Long>(roomid_str);
  GroupInfoPtr group_info;
  if (!TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad(roomid,group_info)) {
    MCE_WARN("ApplyToJoinGroupTask::CreateResults--->TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad  uid:" 
        << actor_uid << "  roomid:" << roomid_str);
    code_ = "internal-server-error";
    description_ = str(boost::format("获取房间%d 信息失败") % roomid );
    is_correct_ = false;
  } 
  MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step2   ");
  if (!group_info) {
    code_ = "resource_unavailable";
    description_ = str(boost::format("加入的房间%d 不存在") % roomid);
    is_correct_ = false;
  } else
    common_data_["message.roomname"] = TalkFunUtil::instance().XMLEncode(group_info->name);
  MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step3   ");
  IntSeq general_receivers;
  general_receivers.push_back(actor_uid);
  common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(general_receivers); //to_id默认返回发送人，如果需要重新修改

  if (group_info->authentication == "none") {
    code_ = "not-allowed";
    description_ = str(boost::format("房间%d 不许加入") % roomid);
    is_correct_ = false;
  }
  MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step4   ");
  if (is_correct_) {
    GroupMemberSeq gm_seq;
    if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad(roomid, gm_seq)) {
      MCE_WARN("ApplyToJoinGroupTask::CreateResults---> TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad uid:"
          << actor_uid << "  roomid:" << roomid_str);
      is_correct_ = false;
      code_ = "internal-server-error";
      description_ = str(boost::format("获取房间%d 成员信息失败") % roomid );
    }
    MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step5   ");
    if ((iter=paras_.find("message.x.apply.reason")) != paras_.end()) {
      string reason = iter->second;
      if (!reason.empty() && !TalkFunUtil::instance().SpamString(reason, actor_uid)) {
        MCE_INFO("ApplyToJoinGroupTask::CreateResults  SpamString error.    uid:" << actor_uid_str <<"   reason:" <<reason);
        return;
      }
      MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step6   ");
      if (gm_seq.size() >= group_info->toplimit) {
        is_correct_ = false;
        code_ = "resource-constraint";
        description_ = str(boost::format("房间%d 成员已到限额 %d") % roomid % group_info->toplimit);
      }
      bool return_flag = false;
      int owner_id = 0;
      const size_t gm_seq_size = gm_seq.size();
      for (size_t pos=0; pos<gm_seq_size; ++pos) {
        if (gm_seq.at(pos)->uid == actor_uid)
          return_flag = true;
        else if (gm_seq.at(pos)->level==kOwner)
          owner_id = gm_seq.at(pos)->uid;
      }
      if (return_flag)
        return;
      MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step7  size: "<< gm_seq.size() <<"   toplimit:"<<group_info->toplimit <<"   auth:" <<group_info->authentication);
      if (is_correct_) {
        if (!CheckGroupFull(owner_id, gm_seq.size())) {
          is_correct_ = false;
          code_ = "resource-constraint";
          description_ = str(boost::format("房间%d 成员已到限额 %d") % roomid % gm_seq.size());
        }
        if (is_correct_) {
          common_data_["message.msg"] = TalkFunUtil::instance().XMLEncode(reason);          
          common_data_["message.roomid"] = roomid_str;          
          MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step7.5   ");
          if (group_info->authentication == "confirm") {
            vector<int> admin_receivers;
            for (size_t i=0; i<gm_seq.size(); ++i) {
              if (gm_seq.at(i)->level == kOwner || gm_seq.at(i)->level == kAdmin)
                admin_receivers.push_back(gm_seq.at(i)->uid);      
            }
            MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step8   ");
            TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(actor_uid);
            if (!u) {
              MCE_WARN("TalkFunTask::GetUserNameById---> TalkCacheClient::instance().GetUserByIdWithLoad  uid:" << actor_uid);
              code_ = "internal-server-error";
              description_ = str(boost::format("获取用户 %d的信息失败") % actor_uid);
              is_correct_ = false;
            } 
            if (is_correct_) {
              MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step9   ");
              common_data_["message.applicantname"] = u->name;
              if (is_correct_) {
                MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step10   ");
                common_data_["message.roomname"] = TalkFunUtil::instance().XMLEncode(group_info->name);          
                common_data_["item_type"] = "1"; 
                common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(admin_receivers);
                /////////////////
                SendOfflineAdminMsgs(common_data_, admin_receivers);
                ///////////////////////
              }
            }
          } else if (group_info->authentication == "any") {
            MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step11   ");
            JoinGroup(roomid, actor_uid); 
            if (is_correct_) {
              common_data_["message.response_type"] = "accept";
              common_data_["message.roomname"] =TalkFunUtil::instance().XMLEncode(group_info->name);          
              common_data_["message.from_uid"] = boost::lexical_cast<string>(owner_id);          
              TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(owner_id);
              if (!u) {
                MCE_WARN("ApplyToJoinGroupTask::CreateResults---> TalkCacheClient::instance().GetUserByIdWithLoad  uid:" << actor_uid);
              } else
                common_data_["message.fromname"] = TalkFunUtil::instance().XMLEncode(u->name);
              common_data_["item_type"] = "2";           
              /////////////////
              SendOfflineAdminMsgs(common_data_, general_receivers);
              ///////////////////////

              vector<int> receivers;
              for (size_t i=0; i<gm_seq.size(); ++i)
                receivers.push_back(gm_seq.at(i)->uid);      
              string to_id = ReceiversHandler::ReceiversAssembly(receivers);
              MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step12   actor_uid:"<<actor_uid <<"   roomid:"<<roomid<<"  to_id:" << to_id);
              SendPresence( roomid,actor_uid, to_id);
            }
          }
        }
      }
    } else if ((iter=paras_.find("message.x.response.reason")) != paras_.end()) {
      MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step11   ");
      common_data_["message.msg"] = TalkFunUtil::instance().XMLEncode(iter->second);
      int receiverid = 0;
      string response_type;
      if ((iter=paras_.find("message.x.response:type")) != paras_.end()) { 
        response_type = common_data_["message.response_type"] = iter->second;
      }
      MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step12   response_type:" << response_type);
      IntSeq receivers;
      if ((iter=paras_.find("message.x.response:to")) != paras_.end()) { 
        MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step13   to:" << iter->second);
        const string receiverid_str =  TalkFunUtil::instance().GetIDFromJID(iter->second);
        receiverid = boost::lexical_cast<int>(receiverid_str);
        int pos = 0;
        /// 如果用户已经在群中，不发送
        const int gm_seq_size = gm_seq.size();
        for (; pos<gm_seq_size; ++pos) {
          if (gm_seq.at(pos)->uid == receiverid)
            break;
        }
        MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step14   to_id:" << receiverid_str);
        if (pos != gm_seq_size)
          return;
        MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step15   to_id:" << receiverid_str);
        receivers.push_back(receiverid);
      }
      MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step16");
      common_data_["item_type"] = "2"; 
      TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(actor_uid);
      if (!u) {
        MCE_WARN("ApplyToJoinGroupTask::CreateResults---> TalkCacheClient::instance().GetUserByIdWithLoad  uid:" << actor_uid);
      } else
        common_data_["message.fromname"] = TalkFunUtil::instance().XMLEncode(u->name);
      common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
      /////////////////
      SendOfflineAdminMsgs(common_data_, receivers);
      ///////////////////////

      if (response_type == "accept") {
        MCE_DEBUG("ApplyToJoinGroupTask::CreateResults  step17");
        JoinGroup(roomid, receiverid);
        if (is_correct_) {
          vector<string> receivers;
          for (size_t i=0; i<gm_seq.size(); ++i)
            receivers.push_back(boost::lexical_cast<string>(gm_seq.at(i)->uid));      
          string to_id = ReceiversHandler::ReceiversAssembly(receivers);
          SendPresence(roomid, receiverid, to_id);
        }
      }
    }
  }
  if (!is_correct_) {
    common_data_["item_type"] = "4";
    common_data_["message.code"] = code_;
    common_data_["message.reason"] = description_;
  }
}

/*说明：转移群并返回结果
 */
void TransferGroupTask::CreateResults() {
  common_data_["message_type"] = boost::lexical_cast<string>(GROUPCHAT_MESSAGE);
  MCE_DEBUG("TransferGroupTask::CreateResults--->  step0"); 
  int type = 0;
  int actor_uid=0;
  string actor_uid_str;
  int transferer_id=0, receiver_id=0;
  string transferer_id_str, receiver_id_str;
  string roomid_str;
  Str2StrMap::const_iterator iter;
  if ((iter=paras_.find("message.x.transfer:to")) != paras_.end()) {
    receiver_id_str = TalkFunUtil::instance().GetIDFromJID(iter->second);
    receiver_id = boost::lexical_cast<int>(receiver_id_str);
    type = 0;
  } else if ((iter=paras_.find("message.x.response:to")) != paras_.end()) {
    transferer_id_str = TalkFunUtil::instance().GetIDFromJID(iter->second);
    transferer_id = boost::lexical_cast<int>(transferer_id_str);
    type = 1;
  }

  MCE_DEBUG("TransferGroupTask::CreateResults--->  step1    iter->second:" << iter->second); 
  if ((iter=paras_.find("message:to")) != paras_.end()) {
    roomid_str = TalkFunUtil::instance().GetIDFromJID(iter->second);
  }
  common_data_["message.roomid"] = roomid_str;
  Ice::Long roomid = boost::lexical_cast<Ice::Long>(roomid_str);

  if ((iter=paras_.find("actor_uid")) != paras_.end()) {
    actor_uid_str = iter->second;
    actor_uid = boost::lexical_cast<int>(actor_uid_str);
    common_data_["message.userid"] = actor_uid_str;
  }
  MCE_DEBUG("TransferGroupTask::CreateResults--->  step2   actor_uid:" << actor_uid); 

  IntSeq common_receivers;
  common_receivers.push_back(actor_uid);
  common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(common_receivers);

  GroupMemberSeq gm_seq;
  if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad(roomid, gm_seq)) {
    MCE_WARN("TransferGroupTask::CreateResults---> TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad uid:" 
        << actor_uid << "  roomid:" << roomid_str);
    is_correct_ = false;
    code_ = "internal-server-error";
    description_ = str(boost::format("用户%d 从群%d 查询用户列表发生异常") % actor_uid % roomid);
  }
  if (is_correct_) {
    MCE_DEBUG("TransferGroupTask::CreateResults--->  step3   gm_seq.size:" << gm_seq.size()); 
    int check_uid = 0;
    if (type == 0)
      check_uid = actor_uid;
    else if (type == 1)
      check_uid = transferer_id;
    GroupMemberSeq::const_iterator member_iter=gm_seq.begin();
    for (; member_iter!=gm_seq.end(); member_iter++) {
      if (((*member_iter)->uid==check_uid) && ((*member_iter)->level==kOwner))
        break;
    }
    if (member_iter == gm_seq.end()) {
      is_correct_ = false;
      code_ = "forbidden";
      description_ = str(boost::format("用户%d 在群%d 的权限不足") % check_uid % roomid);
    }
  }
  MCE_DEBUG("TransferGroupTask::CreateResults--->  step4"); 

  if (is_correct_) {
    if (type == 0) {
      string passwd;
      if ((iter=paras_.find("message.x.transfer.passwd")) != paras_.end()) 
        passwd = iter->second;
      if (!TalkFunUtil::instance().CheckPassport(actor_uid_str, passwd)) {
        is_correct_ = false;
        code_ = "recipient-unavailable";
        description_ = str(boost::format("用户%d 在群%d 中输入密码错误") % actor_uid_str % roomid);
      }
      MCE_DEBUG("TransferGroupTask::CreateResults--->  step5"); 
    } else if (type == 1) {
       ///获取被装让着当前的创建的群数
      size_t  UserMaxSizeOfGroups = kUserMaxSizeOfSolidGroups;
      int vip = TalkFunUtil::instance().CheckVip(receiver_id);
      if (vip == 2) 
        UserMaxSizeOfGroups = kVipMaxSizeOfSolidGroups;
      //判断已创建的固定群的数量
      LongSeq room_ids;
      try {
        TalkGroupViewLoaderAdapter::instance().GetCreatSessions(receiver_id, room_ids);
      } catch (Ice::Exception &e) {
        MCE_WARN("TransferGroupTask::CreateGroup----> uid:"<< receiver_id <<"   TalkGroupViewLoaderAdapter::instance().GetCreatSessions  error:" << e);
        code_ = "internal-server-error";
        description_ = str(boost::format("服务暂时不可用,%d 获取产生的群列表失败") % receiver_id);
        is_correct_ = false;
      }
      if (is_correct_ && UserMaxSizeOfGroups<=room_ids.size()) {
        code_ = "resource-constraint";
        description_ = str(boost::format("%d 创建的群超出上线") % receiver_id);;
        is_correct_ = false;
      }
    }
    if (is_correct_) {
      MCE_DEBUG("TransferGroupTask::CreateResults--->  step10"); 
      GroupInfoPtr group_info;
      if (!TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad(roomid,group_info)) {
        MCE_WARN("TransferGroupTask::CreateResults--->TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad  uid:"
            << actor_uid << "  roomid:" << roomid_str);
        code_ = "internal-server-error";
        description_ = str(boost::format("获取房间%d 信息失败") % roomid );
        is_correct_ = false;
      } 
      if (!group_info) {
        code_ = "resource_unavailable";
        description_ = str(boost::format("转让的房间%d 不存在") % roomid);
        is_correct_ = false;
      } else
        common_data_["message.roomname"] = TalkFunUtil::instance().XMLEncode(group_info->name);

      MCE_DEBUG("TransferGroupTask::CreateResults--->  step11i  roomname:" << group_info->name); 
      TalkUserPtr u;
      if (is_correct_ && actor_uid>0) {
        u = TalkCacheClient::instance().GetUserByIdWithLoad(actor_uid);
        if (!u) {
          MCE_WARN("TransferGroupTask::CreateResults---> TalkCacheClient::instance().GetUserByIdWithLoad  uid:" << iter->second);
          code_ = "internal-server-error";
          description_ = str(boost::format("获取用户%d 的信息失败") % actor_uid);
          is_correct_ = false;
        } 
      }
      MCE_DEBUG("TransferGroupTask::CreateResults--->  step12"); 
      if (is_correct_) {
        string response_type;      
        common_data_["message.username"] = TalkFunUtil::instance().XMLEncode(u->name);
        MCE_DEBUG("TransferGroupTask::CreateResults--->  step13"); 
        if (type == 0) {       
          string reason, passwd;
          if ((iter=paras_.find("message.x.transfer.reason")) != paras_.end()) 
            reason = iter->second;
          if (!TalkFunUtil::instance().SpamString(reason, actor_uid)) {
            MCE_INFO("TransferGroupTask::CreateResults  SpamString error.    uid:" << actor_uid_str <<"   reason:" << reason);
            return;
          }
          common_data_["message.reason"] = TalkFunUtil::instance().XMLEncode(reason);
          common_data_["item_type"] = "1";
        } else if (type == 1) {
          MCE_DEBUG("TransferGroupTask::CreateResults--->  step13.1"); 
          if ((iter=paras_.find("message.x.response:type")) != paras_.end()) 
            response_type = iter->second;
          common_data_["message.response_type"] = response_type;
          common_data_["item_type"] = "2";
          MCE_DEBUG("TransferGroupTask::CreateResults--->  step13.2"); 

          if (response_type == "accept") {
            MCE_DEBUG("TransferGroupTask::CreateResults--->  step13.3"); 
            Str2StrMap props;
            props["session_creater"] =  actor_uid_str;
            try {
              is_correct_ = TalkGroupLoaderAdapter::instance().UpdateGroupInfo(roomid, props); 
            } catch (Ice::Exception &e) {
              MCE_WARN("TransferGroupTask::CreateResults---->TalkGroupLoaderAdapter::instance().UpdateGroupInfo  uid:"
                  << actor_uid <<"   roomid:"<< roomid <<"   error:" << e);
              is_correct_ = false;
            }
            MCE_DEBUG("TransferGroupTask::CreateResults--->  step13.4"); 
            if (is_correct_) {
              MCE_DEBUG("TransferGroupTask::CreateResults--->  step13.5"); 
              GroupMemberSeq member_seq;
              GroupMemberPtr receiver_info = new GroupMember;
              receiver_info->uid = actor_uid;
              receiver_info->level = kOwner;
              GroupMemberPtr transferer_info = new GroupMember;
              transferer_info->uid = transferer_id;
              transferer_info->level = kMember;            
              member_seq.push_back(receiver_info);
              member_seq.push_back(transferer_info);
              GroupMemberDict member_dict;
              member_dict.insert(pair<Ice::Long, GroupMemberSeq>(roomid, member_seq));
              MCE_DEBUG("TransferGroupTask::CreateResults--->  step13.6    receiver:" <<receiver_info->uid<<"   level:"<<receiver_info->level
                  <<"   transferer:"<<transferer_info->uid << "   level:"<<transferer_info->level); 

              try {
                is_correct_ = TalkGroupMemberLoaderAdapter::instance().SetMember(member_dict);
              } catch (Ice::Exception &e) {
                MCE_WARN("TransferGroupTask::CreateResults----> uid:"<< actor_uid <<"  TalkGroupMemberLoaderAdapter::instance().SetMember  error:" << e);
                is_correct_ = false;
              }
              if (!is_correct_) {
                is_correct_ = false;
                code_ = "internal-server-error";
                description_ = str(boost::format("用户%d 更新群%d 成员权限失败") % actor_uid % roomid);
              }
              MCE_DEBUG("TransferGroupTask::CreateResults--->  step13.6 set_ret:" ); 
              if (is_correct_) {
                Str2StrMap view_info;
                view_info["affiliation"] = boost::lexical_cast<string>(kOwner);
                try {
                  is_correct_ = TalkGroupViewLoaderAdapter::instance().UpdateItem(actor_uid, roomid, view_info);
                } catch (Ice::Exception &e) {
                  MCE_WARN("TransferGroupTask::CreateResults----> uid:"<< actor_uid <<"   rooomid:" << roomid
                      <<"  TalkGroupViewLoaderAdapter::instance().UpdateItem  error:" << e);
                  code_ = "internal-server-error";
                  description_ = str(boost::format("用户%d 更新群%d 视图失败") % actor_uid % roomid);
                  is_correct_ = false;
                }
                if (is_correct_) {
                  view_info["affiliation"] = boost::lexical_cast<string>(kMember);
                  try {
                    is_correct_ = TalkGroupViewLoaderAdapter::instance().UpdateItem(transferer_id, roomid, view_info);
                  } catch (Ice::Exception &e) {
                    MCE_WARN("TransferGroupTask::CreateResults----> uid:"<< transferer_id <<"   rooomid:" << roomid
                        <<"  TalkGroupViewLoaderAdapter::instance().UpdateItem  error:" << e);
                    code_ = "internal-server-error";
                    description_ = str(boost::format("用户%d 更新群%d 视图失败") % actor_uid % roomid);
                    is_correct_ = false;
                  }
                }
              }
            }
          }
        } else {
          is_correct_ = false;
        }
        MCE_DEBUG("TransferGroupTask::CreateResults--->  step14"); 
        if (is_correct_) {
          common_receivers.clear();
          if (type == 0)          
            common_receivers.push_back(receiver_id);
          else if (type == 1)
            common_receivers.push_back(transferer_id);
          common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(common_receivers);
          /////////////////
          SendOfflineAdminMsgs(common_data_, common_receivers);
          ///////////////////////

          MCE_DEBUG("TransferGroupTask::CreateResults--->  step15"); 
          if (type==1 && response_type=="accept") {
            IntSeq presence_receivers; 
            for (GroupMemberSeq::const_iterator member_iter=gm_seq.begin(); member_iter!=gm_seq.end(); member_iter++) 
              presence_receivers.push_back((*member_iter)->uid);
                     
            Str2StrMap receiver_presence_data;
            receiver_presence_data["config_type"] = config_type_;
            receiver_presence_data["item_type"] = "4";
            receiver_presence_data["from_jid"] = from_jid_;
            receiver_presence_data["message_type"] = boost::lexical_cast<string>(PRESENCE_COMMON);        
            receiver_presence_data["to_id"] = ReceiversHandler::ReceiversAssembly(presence_receivers);
            receiver_presence_data["presence.roomid"] = roomid_str;
            receiver_presence_data["presence.actor_id"] = transferer_id_str;
            string member_name;
            TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(transferer_id);
            if (u) 
              member_name = u->name;
            string owner_name;
            u = TalkCacheClient::instance().GetUserByIdWithLoad(actor_uid);
            if (u) 
              owner_name = u->name;
            receiver_presence_data["presence.owner_name"] = TalkFunUtil::instance().XMLEncode(owner_name);
            receiver_presence_data["presence.actor_name"] = TalkFunUtil::instance().XMLEncode(member_name);
            receiver_presence_data["presence.affiliation"] = "owner";
            receiver_presence_data["presence.user_id"] = actor_uid_str;
            presence_data_seq_.push_back(receiver_presence_data);
            Str2StrMap transferer_presence_data;
            transferer_presence_data.insert(receiver_presence_data.begin(), receiver_presence_data.end());
            transferer_presence_data["presence.user_id"] = transferer_id_str;
            transferer_presence_data["presence.affiliation"] = "member";
            presence_data_seq_.push_back(transferer_presence_data);

          }
        }
      }
    }
  }
  if (!is_correct_) {
    common_data_["item_type"] = "3";
    common_data_["message.code"] = code_;
    common_data_["message.description"] = description_;
  }
}

/*说明：销毁群并返回结果
 */
void DestroyGroupTask::CreateResults() {
  common_data_["message_type"] = boost::lexical_cast<string>(IQ_RESULT);
  string roomid_str, passwd;
  Ice::Long roomid;
  Str2StrMap new_group_infos;
  Str2StrMap::const_iterator iter;
  if ((iter=paras_.find("iq:id")) != paras_.end()) {
    common_data_["iq.id"] = iter->second;
  }    
  if ((iter=paras_.find("iq:to")) != paras_.end()) {
    roomid_str = common_data_["iq.roomid"] = TalkFunUtil::instance().GetIDFromJID(iter->second);
  
    MCE_DEBUG("DestroyGroupTask::CreateResults  step0   roomid_str:" << roomid_str << "  second:" << iter->second);
  }    
  if (roomid_str == "") {
    code_ = "bad-request";
    description_ = "群JID错误";
    is_correct_ = false;
  }
  roomid = boost::lexical_cast<Ice::Long>(roomid_str);
  string actor_uid_str;
  int actor_uid = 0;
  if ((iter=paras_.find("actor_uid")) != paras_.end()) {
    actor_uid_str = iter->second;
    actor_uid = boost::lexical_cast<int>(actor_uid_str);
  }    
  vector<string> receivers;
  receivers.push_back(actor_uid_str);
  common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
  MCE_DEBUG("DestroyGroupTask::CreateResults  step1   roomid:" << roomid_str);
  if (is_correct_) {
    if ((iter=paras_.find("iq.query.passwd")) != paras_.end()) {
      passwd = iter->second;
    }    
    if (!TalkFunUtil::instance().CheckPassport(actor_uid_str, passwd)) {
      MCE_INFO("DestroyGroupTask::CreateResults  checkpassport error.  uid:" << actor_uid_str <<"   pwd:" <<passwd);
      is_correct_ = false;
      code_ = "recipient-unavailable";
      description_ = str(boost::format("用户%d 在群%d 中输入密码错误") % actor_uid_str % roomid);
    }
    if (is_correct_) {
      MCE_DEBUG("DestroyGroupTask::CreateResults  step2  actor_uid:" << actor_uid);
      GroupInfoPtr group_info;
      if (!TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad(roomid,group_info)) {
        MCE_WARN("DestroyGroupTask::CreateResults--->TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad  uid:" 
            << actor_uid_str << "  roomid:" << roomid_str);
        code_ = "internal-server-error";
        description_ = str(boost::format("获取房间%d 信息失败") % roomid );
        is_correct_ = false;
      } 
      if (!group_info) {
        code_ = "resource_unavailable";
        description_ = str(boost::format("销毁的房间%d 不存在") % roomid);
        is_correct_ = false;
      }
      MCE_DEBUG("DestroyGroupTask::CreateResults  step3  actor_uid:" << actor_uid);
      if (is_correct_) {
        MCE_DEBUG("DestroyGroupTask::CreateResults  step4:");
        GroupMemberSeq gm_seq;
        if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad(roomid, gm_seq)) {
          MCE_WARN("DestroyGroupTask::CreateResults----> uid:"<< actor_uid <<"  roomid:"<<roomid
              <<"  TalkGroupLoaderAdapter::instance().SetGroupInfo" );
          is_correct_ = false;
          code_ = "internal-server-error";
          description_ = str(boost::format("用户%s 获取群%d 成员列表失败") %actor_uid_str % roomid );
        }    
        MCE_DEBUG("DestroyGroupTask::CreateResults  step5");
        if (is_correct_) {
          MCE_DEBUG("DestroyGroupTask::CreateResults  step6");
          IntSeq presence_receivers;
          bool is_owner = false;
          for (size_t i=0; i<gm_seq.size(); ++i) {
            if ((gm_seq.at(i))->uid==actor_uid && (gm_seq.at(i))->level==kOwner)
              is_owner = true;
            presence_receivers.push_back(gm_seq.at(i)->uid);
          }
          if (!is_owner) {
            MCE_WARN("DestroyGroupTask::CreateResults----> uid:"<< actor_uid <<"  roomid:"<<roomid <<"  不是管理员");
            is_correct_ = false;
            code_ = "not-allowed";
            description_ = str(boost::format("用户%s 没有权限销毁群%d ") %actor_uid_str % roomid);;
          }
          MCE_DEBUG("DestroyGroupTask::CreateResults  step7");
          if (is_correct_) {
            MCE_DEBUG("DestroyGroupTask::CreateResults  step8");
            LongSeq roomids;
            roomids.push_back(roomid);
            bool op_ret = true;
            try {
              op_ret = TalkGroupLoaderAdapter::instance().DeleteGroupInfo(roomids);
              if (op_ret)
                op_ret = TalkGroupMemberLoaderAdapter::instance().DeleteGroup(roomids);
            } catch (Ice::Exception &e) {
              MCE_WARN("DestroyGroupTask::CreateResults----> TalkGroupLoader::instance().DeleteGroupInfo   userid:" << actor_uid_str << "   roomid:"<<roomid<<
                 "   error:" << e);
              is_correct_ = false;
              code_ = "internal-server-error";
              description_ = str(boost::format("用户%s 销毁群%d 失败") %actor_uid_str % roomid );
            }
            if (is_correct_) {
              for (GroupMemberSeq::const_iterator gm_iter=gm_seq.begin(); gm_iter!=gm_seq.end(); gm_iter++) {
                try {
                  TalkGroupViewLoaderAdapter::instance().DeleteAsyn((*gm_iter)->uid, roomid, kSolidGroup);
                } catch (Ice::Exception &e) {
                  MCE_WARN("DestroyGroupTask::CreateResults----> TalkGroupViewLoaderAdapter::instance().DeleteAsyn  userid:" <<(*gm_iter)->uid << "   roomid:"<<roomid<<
                     "   error:" << e);
                  is_correct_ = false;
                  code_ = "internal-server-error";
                  description_ = str(boost::format("用户%d 销毁群%d 视图失败") %(*gm_iter)->uid % roomid );
                }
              }
              if (is_correct_) {
                MCE_DEBUG("DestroyGroupTask::CreateResults  step9");
                common_data_["item_type"] = "1";

                Str2StrMap presence_data;
                presence_data["config_type"] = config_type_;
                presence_data["item_type"] = "2";
                presence_data["from_jid"] = from_jid_;
                presence_data["message_type"] = boost::lexical_cast<string>(PRESENCE_COMMON);        
                presence_data["to_id"] = ReceiversHandler::ReceiversAssembly(presence_receivers);
                presence_data["presence.roomid"] = roomid_str;
                presence_data["presence.roomname"] = TalkFunUtil::instance().XMLEncode(group_info->name);
                presence_data["presence.userid"] = actor_uid_str;
                TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(actor_uid);
                if (!u) {
                  MCE_WARN("DestroyGroupTask::CreateResults---> TalkCacheClient::instance().GetUserByIdWithLoad  uid:" << actor_uid);
                } else 
                  presence_data["presence.username"] = TalkFunUtil::instance().XMLEncode(u->name);
                presence_data_seq_.push_back(presence_data);
                /////////////////
                SendOfflineAdminMsgs(presence_data, presence_receivers);
                ///////////////////////
              }
            }
          }
        }
      }
    }
  }
  if (!is_correct_) {
    common_data_["item_type"] = "3";
    common_data_["iq.code"] = code_;
    common_data_["iq.reason"] = description_;
  }
}

/*说明：获取群限制并返回结果
 */
void GetGroupLimitInfoTask::CreateResults() {
  Str2StrMap::const_iterator iter;
  common_data_["message_type"] = boost::lexical_cast<string>(IQ_RESULT);
  if ((iter=paras_.find("iq:id")) != paras_.end())
    common_data_["iq.id"] = iter->second;
  if ((iter=paras_.find("actor_uid")) != paras_.end()) { 
    vector<string> receivers;
    receivers.push_back(iter->second);
    common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
  }
  common_data_["item_type"] = "1";
}

/*说明：获取群列表并返回结果
 */
void GetGroupListTask::CreateResults() {
  Str2StrMap::const_iterator iter;
  common_data_["message_type"] = boost::lexical_cast<string>(IQ_RESULT);
  if ((iter=paras_.find("iq:id")) != paras_.end())
    common_data_["iq.id"] = iter->second;
  int actor_uid = 0;
  if ((iter=paras_.find("actor_uid")) != paras_.end()) { 
    actor_uid = boost::lexical_cast<int>(iter->second);
    vector<string> receivers;
    receivers.push_back(iter->second);
    common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
  }
  vector<Ice::Long> roomids;
  GroupItemInfoSeq group_items;
  TalkGroupViewSeq talk_group_views;
  if (!TalkGroupViewClient::instance().GetAllGroups(actor_uid, talk_group_views)) {
    MCE_WARN("GetGroupListTask::CreateResults----> uid:"<< actor_uid <<"  TalkGroupViewClient::instance().GetRecentGroup");
    is_correct_ = false;
    code_ = "internal-server-error";
    description_ = str(boost::format("用户%s 取群列错误") %actor_uid);;
  }
  MCE_INFO("GetGroupListTask::CreateResults----> uid:"<< actor_uid <<"     talk_group_views.size:" <<  talk_group_views.size() << "   is_correct_:" << is_correct_);
  common_data_["item_type"] = "1";
  if (is_correct_) {
    for (TalkGroupViewSeq::const_iterator iter=talk_group_views.begin(); iter!=talk_group_views.end(); iter++) {
      GroupItemInfo item;
      item.id = (*iter)->sessionId;
      roomids.push_back(item.id);
      MCE_DEBUG("GetGroupListTask::CreateResults----> roomid:"<< item.id <<"  sessionType:" <<(*iter)->sessionType << "  affiliation:" <<(*iter)->affiliation);
      if ((*iter)->sessionType == kSolidGroup)
        item.is_fixed = "1";
      else 
        item.is_fixed = "0";
      item.affiliation = TalkFunUtil::instance().PrivilegeInt2String((*iter)->affiliation);
      item.saveonline = (*iter)->saveonline;
      item.notify = (*iter)->notify;
      group_items.push_back(item);
//      group_items.insert(pair<Ice::Long, GroupItemInfo>(item.id, item));
    }
    if (!roomids.empty()) {
      GroupInfoDict group_infos;
      if (!TalkGroupInfoClient::instance().GetGroupInfosBySeqWithLoad(roomids, group_infos)) {
        MCE_WARN("GetGroupListTask::CreateResults--->TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad   uid:"
            << actor_uid << "  group_infos.size:" << group_infos.size());
        code_ = "internal-server-error";
        description_ = str(boost::format("用户 %d获取房间列表失败") % actor_uid );
        is_correct_ = false;
      } 
      if (is_correct_) {
        if (group_infos.size() > 0) {
          int pos = 0;
          GroupInfoDict::const_iterator group_info_iter ;
          for (GroupItemInfoSeq::iterator iter=group_items.begin(); iter!=group_items.end(); iter++, pos++) {
            group_info_iter = group_infos.find(iter->id);
            if (group_info_iter != group_infos.end()) {
                iter->name = group_info_iter->second->name;
                iter->photo = group_info_iter->second->head;
                iter->announcement = group_info_iter->second->announcement;
                iter->introduction = group_info_iter->second->introduction;
                iter->authentication = group_info_iter->second->authentication;
                iter->owner = group_info_iter->second->creator;
//                iter->notify = group_info_iter->second->notify;
//                iter->saveonline = group_info_iter->second->saveonline;
            }
            string user_name;
            TalkUserPtr talk_user = TalkCacheClient::instance().GetUserByIdWithLoad(iter->owner);
            if (talk_user)
              user_name = talk_user->name;
            common_data_[str(boost::format("iq.query.%d.roomid") % pos)] = boost::lexical_cast<string>(iter->id);
            common_data_[str(boost::format("iq.query.%d.name") % pos)] = TalkFunUtil::instance().XMLEncode(iter->name);
            common_data_[str(boost::format("iq.query.%d.portrait") % pos)] = iter->photo;
            common_data_[str(boost::format("iq.query.%d.fixed") % pos)] = iter->is_fixed;
            common_data_[str(boost::format("iq.query.%d.affiliation") % pos)] = iter->affiliation;
            common_data_[str(boost::format("iq.query.%d.introduction") % pos)] = TalkFunUtil::instance().XMLEncode(iter->introduction);
            common_data_[str(boost::format("iq.query.%d.announcement") % pos)] = TalkFunUtil::instance().XMLEncode(iter->announcement);
            common_data_[str(boost::format("iq.query.%d.authentication") % pos)] = iter->authentication;
            common_data_[str(boost::format("iq.query.%d.notify") % pos)] = iter->notify;
            common_data_[str(boost::format("iq.query.%d.owner") % pos)] = boost::lexical_cast<string>(iter->owner);
            common_data_[str(boost::format("iq.query.%d.ownername") % pos)] = user_name;
            common_data_[str(boost::format("iq.query.%d.saveonline") % pos)] = boost::lexical_cast<string>(iter->saveonline);
          }
        }
      }
    }

  }
  if (!is_correct_) {
    common_data_["item_type"] = "2";
    common_data_["iq.code"] = code_;
    common_data_["iq.reason"] = description_;
  }
}

/*说明：邀请加入群并返回结果
 */
void InviteToJoinGroupTask::CreateResults() {
  Str2StrMap::const_iterator iter;
  string actor_uid_str, roomid_str;
  Ice::Long roomid = 0;
  int actor_uid = 0;
  if ((iter=paras_.find("message:to")) != paras_.end()) {
    roomid_str = common_data_["message.roomid"] = TalkFunUtil::instance().GetIDFromJID(iter->second);
    roomid = boost::lexical_cast<Ice::Long>(roomid_str);
  }
  if ((iter=paras_.find("actor_uid")) != paras_.end()) { 
    actor_uid_str = common_data_["from_uid"] = iter->second;
    actor_uid = boost::lexical_cast<int>(actor_uid_str);
  }
  MCE_DEBUG("InviteToJoinGroupTask::CreateResults  step1----> actor_uid:" << actor_uid << "   roomid:" <<roomid_str);
  string user_name;
  TalkUserPtr talk_user = TalkCacheClient::instance().GetUserByIdWithLoad(actor_uid);
  if (talk_user)
    user_name = talk_user->name;
  string room_name;
  GroupInfoPtr group_info;
  if (!TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad(roomid, group_info)) {
    MCE_WARN("InviteToJoinGroupTask::CreateResults----> uid:"<< actor_uid <<"   roomid:" <<roomid_str
        <<"  TalkGroupLoaderAdapter::instance().LoadGroupInfo ");
    is_correct_ = false;
  } 
  if (group_info) 
    room_name = group_info->name;
  else
    is_correct_ = false;
  if (!is_correct_) {
    code_ = "internal-server-error";
    description_ = str(boost::format("用户 %d获取房间%s 信息失败") % actor_uid_str %roomid_str );
  }
  if (is_correct_) {
    GroupMemberSeq gm_seq;
    if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad(roomid, gm_seq)) {
      MCE_WARN("InviteToJoinGroupTask::CreateResults---> TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad uid:"
          << actor_uid << "  roomid:" << roomid_str);
      is_correct_ = false;
      code_ = "internal-server-error";
      description_ = str(boost::format("获取房间%s 成员信息失败") % roomid_str );
    }
    MCE_DEBUG("InviteToJoinGroupTask::CreateResults  step2   user_name:" << user_name);
    if (is_correct_) {
      if (!CheckGroupFull(group_info->creator, gm_seq.size())) {
        is_correct_ = false;
        code_ = "resource-constraint";
        description_ = str(boost::format("房间%d 成员已到限额 %d") % roomid % gm_seq.size());
      }
      if (is_correct_) {
        common_data_["message.roomname"] = TalkFunUtil::instance().XMLEncode(room_name);
        if ((iter=paras_.find("message.x.invite:to")) != paras_.end()) {
          MCE_DEBUG("InviteToJoinGroupTask::CreateResults  step3");
          string invitee_str = TalkFunUtil::instance().GetIDFromJID(iter->second);
          int invitee = boost::lexical_cast<int>(invitee_str);
          for (GroupMemberSeq::const_iterator member_iter=gm_seq.begin(); member_iter!=gm_seq.end(); member_iter++) 
            if ((*member_iter)->uid == invitee)
              return;
          string reason;
          if ((iter=paras_.find("message.x.invite.reason")) != paras_.end())
            reason = iter->second;
          if (!reason.empty() && !TalkFunUtil::instance().SpamString(reason, actor_uid)) {
            MCE_INFO("InviteToJoinGroupTask::CreateResults  SpamString error.    uid:" << actor_uid_str <<"   reason:" << reason);
            return;
          }
          common_data_["message.invitor"] = actor_uid_str;
          common_data_["message.invitorname"] = TalkFunUtil::instance().XMLEncode(user_name);
          common_data_["message.reason"] = TalkFunUtil::instance().XMLEncode(reason);
          common_data_["item_type"] = "1";
         
          MCE_DEBUG("InviteToJoinGroupTask::CreateResults  step4");
          IntSeq receivers;
          receivers.push_back(invitee);
          common_data_["to_id"]  = ReceiversHandler::ReceiversAssembly(receivers);
          /////////////////
          SendOfflineAdminMsgs(common_data_, receivers);
          ///////////////////////
        } else if ((iter=paras_.find("message.x.response:to")) != paras_.end()) {
          for (GroupMemberSeq::const_iterator member_iter=gm_seq.begin(); member_iter!=gm_seq.end(); member_iter++) 
            if ((*member_iter)->uid == actor_uid)
              return;
          string invitor_str = TalkFunUtil::instance().GetIDFromJID(iter->second);
          int invitor = boost::lexical_cast<int>(invitor_str);
          string response_type, reason;
          if ((iter=paras_.find("message.x.response:type")) != paras_.end())
            response_type = iter->second;
          if ((iter=paras_.find("message.x.invite.reason")) != paras_.end())
            reason = TalkFunUtil::instance().XMLEncode(iter->second);
          common_data_["message.response_type"] = response_type;
          common_data_["message.invitee"] = actor_uid_str;
          common_data_["message.inviteename"] = TalkFunUtil::instance().XMLEncode(user_name);
          common_data_["message.reason"] = TalkFunUtil::instance().XMLEncode(reason);

          IntSeq receivers;
          receivers.push_back(invitor);
          common_data_["to_id"]  = ReceiversHandler::ReceiversAssembly(receivers);
          common_data_["item_type"] = "2";
          /////////////////
          SendOfflineAdminMsgs(common_data_, receivers);
          ///////////////////////
            
          if (response_type == "accept") {
            MCE_DEBUG("InviteToJoinGroupTask::CreateResults---->   presence step0");
            JoinGroup(roomid, actor_uid);
            if (is_correct_) {
              MCE_DEBUG("InviteToJoinGroupTask::CreateResults---->   presence step1");
              vector<string> receivers;
              for (size_t i=0; i<gm_seq.size(); ++i)
                receivers.push_back(boost::lexical_cast<string>(gm_seq.at(i)->uid));      
              receivers.push_back(actor_uid_str);
              string to_id =  ReceiversHandler::ReceiversAssembly(receivers);
              SendPresence(roomid, actor_uid, to_id);
            }
          }
        }
      }
    }
  }
  if (!is_correct_) {
    common_data_["item_type"] = "4";
    common_data_["message.code"] = code_;
    common_data_["message.reason"] = description_;
  }
}

/*说明：踢出群并返回结果
 */
void KickUsersTask::CreateResults() {
  MCE_DEBUG("KickUsersTask::CreateResults   step1");
  common_data_["message_type"] = boost::lexical_cast<string>(IQ_RESULT);
  Str2StrMap::const_iterator iter;
  string roomid_str;
  int actor_uid = 0;
  string actor_uid_str;
  if ((iter=paras_.find("iq:id")) != paras_.end()) {
    common_data_["iq.id"] = iter->second;
  }
  if ((iter=paras_.find("iq:to")) != paras_.end()) {
    common_data_["iq.roomid"] = roomid_str = TalkFunUtil::instance().GetIDFromJID(iter->second);
  }
  if ((iter=paras_.find("actor_uid")) != paras_.end()) {
    actor_uid_str = iter->second;
    actor_uid = boost::lexical_cast<int>(actor_uid_str);
  }
  size_t item_count = 1;
  if ((iter=paras_.find("iq.query.item%")) != paras_.end()) {
    item_count = boost::lexical_cast<int>(iter->second);
  }
  vector<string> receivers;
  receivers.push_back(actor_uid_str);
  common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
  MCE_DEBUG("KickUsersTask::CreateResults   step2  item_count:" << item_count);
  IntSeq to_be_removed;
  for (size_t i=0; i<item_count; ++i) {
    string key;
    if (i == 0)
      key = "iq.query.item";
    else
      key = str(boost::format("iq.query.item@%d") % i);
    if ((iter=paras_.find(key+":jid")) != paras_.end()) {
      int uid = boost::lexical_cast<int>(TalkFunUtil::instance().GetIDFromJID(iter->second));
      to_be_removed.push_back(uid);
    }
  }
  Ice::Long roomid = boost::lexical_cast<Ice::Long>(roomid_str);
  string room_name;
  GroupInfoPtr group_info;
  if (!TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad(roomid ,group_info)) {
    MCE_WARN("QuitGroupTask::CreateResults----> uid:"<< actor_uid <<"   roomid:" <<roomid
        <<"  TalkGroupLoaderAdapter::instance().LoadGroupInfo ");
    code_ = "internal-server-error";
    description_ = str(boost::format("%d 获取房间%d 信息失败") %actor_uid % roomid);
    is_correct_ = false;
   } 
  if (group_info) 
    room_name = group_info->name;
  MCE_DEBUG("KickUsersTask::CreateResults   step3  to_be_removed.size:" << to_be_removed.size());
  GroupMemberSeq gm_seq;
  if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad(roomid, gm_seq)) {
    MCE_WARN(" KickUsersTask::CreateResults----> uid:"<< actor_uid <<"  roomid:" <<roomid
        <<"  TalkGroupMemberLoaderAdapter::instance().LoadGroupMember ");
    is_correct_ = false;
    code_ = "internal-server-error";
    description_ = str(boost::format("用户%s 从群%d 查询用户列表发生异常") % actor_uid_str % roomid);
  }
  MCE_DEBUG("KickUsersTask::CreateResults   step4   gm_seq.size:" << gm_seq.size());
  if (is_correct_) {
    MCE_DEBUG("KickUsersTask::CreateResults   step5");
    int actor_level = kMember;
    for (GroupMemberSeq::const_iterator member_iter=gm_seq.begin(); member_iter!=gm_seq.end(); member_iter++) {
      if ((*member_iter)->uid==actor_uid && ((*member_iter)->level==kOwner||(*member_iter)->level==kAdmin)) {
        actor_level = (*member_iter)->level;        
        break;      
      }
    }
    if (actor_level == kMember) {
      is_correct_ = false;
      code_ = "not-allow";
      description_ = str(boost::format("用户%d 从群%d 删除用户发生异常异常") % actor_uid % roomid);
    }
    for (IntSeq::iterator id_iter1=to_be_removed.begin(); id_iter1!=to_be_removed.end(); id_iter1++) {
      MCE_DEBUG("KickUsersTask::CreateResults   step5.5   id:" << *id_iter1 << "  actor_level:" << actor_level);
    }
    for (GroupMemberSeq::const_iterator member_iter1=gm_seq.begin(); member_iter1!=gm_seq.end(); member_iter1++) {
      MCE_DEBUG("KickUsersTask::CreateResults   step5.5   uid:" <<(*member_iter1)->uid <<"  level:" <<(*member_iter1)->level);
    }
    if (is_correct_) {
      IntSeq removed_ids;
      for (IntSeq::iterator id_iter=to_be_removed.begin(); id_iter!=to_be_removed.end(); id_iter++) {
        MCE_DEBUG("KickUsersTask::CreateResults   step6.   ");
        GroupMemberSeq::const_iterator member_iter;
        for (member_iter=gm_seq.begin(); member_iter!=gm_seq.end(); member_iter++) {
          if ((*member_iter)->uid==*id_iter && (*member_iter)->level>actor_level) 
            break;
        }
        if (member_iter != gm_seq.end())
          removed_ids.push_back(*id_iter);   
      }    
      MCE_DEBUG("KickUsersTask::CreateResults   step6.5  toberemoved.size:" << to_be_removed.size() << "   removed_ids.size:" << removed_ids.size());
      IntSeq rest_members;
      IntSeq all_members;
      for (GroupMemberSeq::const_iterator member_iter=gm_seq.begin(); member_iter!=gm_seq.end(); member_iter++) {
        IntSeq::const_iterator id_iter = find(removed_ids.begin(), removed_ids.end(), (*member_iter)->uid);
        if (id_iter == removed_ids.end())
          rest_members.push_back((*member_iter)->uid);      
        all_members.push_back((*member_iter)->uid);
      }
      string to_id = ReceiversHandler::ReceiversAssembly(all_members);
//      string to_id = ReceiversHandler::ReceiversAssembly(rest_members);
      MCE_DEBUG("KickUsersTask::CreateResults   step7  to_id:" << to_id);
      bool remove_ret = true;
      try {
        remove_ret = TalkGroupMemberLoaderAdapter::instance().RemoveMember(roomid, removed_ids);
      } catch (Ice::Exception &e) {
        MCE_WARN("KickUsersTask::CreateResults----> uid:"<< actor_uid <<"  TalkGroupMemberLoaderAdapter::instance().LoadGroupMember  error:" << e);
        remove_ret = false;
      }
      if (!remove_ret) {
        is_correct_ = false;
        code_ = "internal-server-error";
        description_ = str(boost::format("用户%d 删除群%s 成员失败") % actor_uid % roomid_str);
      }
      MCE_DEBUG("KickUsersTask::CreateResults   step8    removed_ids.size:" << removed_ids.size());
      if (is_correct_) {
        for (IntSeq::const_iterator uid_iter=removed_ids.begin(); uid_iter!=removed_ids.end(); uid_iter++) {
          try {
            TalkGroupViewLoaderAdapter::instance().DeleteAsyn(*uid_iter, roomid, kSolidGroup);
          } catch (Ice::Exception &e) {
            MCE_WARN("KickUsersTask::CreateResults----> TalkGroupViewLoaderAdapter::instance().DeleteAsyn  userid:" <<*uid_iter << "   roomid:"<<roomid<<
               "   error:" << e);
            is_correct_ = false;
            code_ = "internal-server-error";
            description_ = str(boost::format("用户%d 销毁群%d 视图失败") % *uid_iter % roomid );
          }
        }
        if (is_correct_) {
          MCE_DEBUG("KickUsersTask::CreateResults   step9");
          common_data_["item_type"] = "1"; 

          string user_name;
          TalkUserPtr talk_user = TalkCacheClient::instance().GetUserByIdWithLoad(actor_uid);
          if (talk_user)
            user_name = talk_user->name;
          IntSeq offline_receivers;
          for (IntSeq::const_iterator removed_iter=removed_ids.begin(); removed_iter!=removed_ids.end(); removed_iter++) {
            Str2StrMap presence_data;
            presence_data["config_type"] = config_type_;
            presence_data["item_type"] = "2";
            presence_data["from_jid"] = from_jid_;
            presence_data["message_type"] = boost::lexical_cast<string>(PRESENCE_COMMON);        
            presence_data["to_id"] = to_id;
            presence_data["presence.roomid"] = roomid_str;
            presence_data["presence.user_id"] = boost::lexical_cast<string>(*removed_iter);
            presence_data["presence.actor_id"] = boost::lexical_cast<string>(actor_uid);
            presence_data["presence.actor_name"] = TalkFunUtil::instance().XMLEncode(user_name);
            presence_data["presence.roomname"] = TalkFunUtil::instance().XMLEncode(room_name);
            presence_data_seq_.push_back(presence_data);
            /////////////////
            offline_receivers.clear();
            offline_receivers.push_back(*removed_iter);
            SendOfflineAdminMsgs(presence_data, offline_receivers);
            ///////////////////////
          }
        }
      }
    }
  }
  if (!is_correct_) {
    common_data_["item_type"] = "3";
    common_data_["iq.code"] = code_;
    common_data_["iq.reason"] = description_;
  }
}

/*说明：改变群成员权限并返回结果
 */
void ChangePrivilegeTask::CreateResults() {
  common_data_["message_type"] = boost::lexical_cast<string>(IQ_RESULT);
  Str2StrMap::const_iterator iter;
  if ((iter=paras_.find("iq:id")) != paras_.end())
    common_data_["iq.id"] = iter->second;
  string roomid_str;
  if ((iter=paras_.find("iq:to")) != paras_.end()) {
    common_data_["iq.room_jid"] = iter->second;
    roomid_str = TalkFunUtil::instance().GetIDFromJID(iter->second);
  }
  MCE_DEBUG("ChangePrivilegeTask::CreateResults--->  step1  roomid:" << roomid_str);
  int actor_uid = 0;
  string actor_uid_str;
  if ((iter=paras_.find("actor_uid")) != paras_.end()) { 
    actor_uid_str = iter->second;
    actor_uid = boost::lexical_cast<int>(actor_uid_str);
    vector<string> receivers;
    receivers.push_back(actor_uid_str);
    common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
  }
  size_t item_count = 1;
  if ((iter=paras_.find("iq.query.item%")) != paras_.end()) {
    item_count = boost::lexical_cast<int>(iter->second);
  }
  MCE_DEBUG("ChangePrivilegeTask::CreateResults--->  step2  roomid:" << roomid_str);
  map<int, GroupMemberPtr> members_privilege;
//  map<int, int> to_be_changed;
  for (size_t i=0; i<item_count; ++i) {
    string key;
    if (i == 0)
      key = "iq.query.item";
    else
      key = str(boost::format("iq.query.item@%d") % i);
    int uid = 0;
    string privilege;
    if ((iter=paras_.find(key+":jid")) != paras_.end()) 
      uid = boost::lexical_cast<int>(TalkFunUtil::instance().GetIDFromJID(iter->second));
    if ((iter=paras_.find(key+":affiliation")) != paras_.end()) 
      privilege = iter->second;
    if (uid>0 && privilege!="") {
      GroupMemberPtr member = new GroupMember;
      member->uid = uid;
      member->level = TalkFunUtil::instance().PrivilegeString2Int(privilege);
      members_privilege.insert(pair<int, GroupMemberPtr>(uid, member));
    }
 //   to_be_changed.insert(pair<int,string>(uid, privilege));
  }
  MCE_DEBUG("ChangePrivilegeTask::CreateResults--->  step3  item_count:" << item_count);
  Ice::Long roomid = boost::lexical_cast<Ice::Long>(roomid_str);
  GroupMemberSeq gm_seq;
  if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad(roomid, gm_seq)) {
    MCE_WARN("ChangePrivilegeTask::CreateResults---> TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad  uid:"
        << actor_uid << "  roomid:" << roomid_str);
    is_correct_ = false;
    code_ = "internal-server-error";
    description_ = str(boost::format("用户%d 从群%d 查询用户列表发生异常") % actor_uid % roomid);
  }
  MCE_DEBUG("ChangePrivilegeTask::CreateResults--->  step4  " );
  if (is_correct_) {
    IntSeq receivers;
    GroupMemberSeq to_be_changed;
    GroupMemberSeq::const_iterator member_iter = gm_seq.begin();
    for (; member_iter!=gm_seq.end(); member_iter++) {
      if ((*member_iter)->uid==actor_uid && (*member_iter)->level!=kOwner) {
        is_correct_ = false;
        code_ = "not_allow";
        description_ = str(boost::format("用户%d 从群%d 权限不够修改群成员权限,实际权限 %d") % actor_uid % roomid % (*member_iter)->level);
        break;
      }
      map<int, GroupMemberPtr>::iterator temp_iter = members_privilege.find((*member_iter)->uid);
      if (temp_iter != members_privilege.end()) {
        to_be_changed.push_back(temp_iter->second);
      }
      receivers.push_back((*member_iter)->uid);
    }
    string to_id = ReceiversHandler::ReceiversAssembly(receivers);
    MCE_DEBUG("ChangePrivilegeTask::CreateResults--->  step5  receivers.size:" << to_id);
    if (is_correct_) {
      for (GroupMemberSeq::const_iterator changed_iter = to_be_changed.begin(); changed_iter!=to_be_changed.end(); changed_iter++)
        MCE_DEBUG("ChangePrivilegeTask::CreateResults--->  step5.5   uid:"<< (*changed_iter)->uid << "    level:" << (*changed_iter)->level);
      GroupMemberDict member_dict;
      member_dict.insert(pair<Ice::Long, GroupMemberSeq>(roomid, to_be_changed));
      bool set_ret = true;
      try {
        set_ret = TalkGroupMemberLoaderAdapter::instance().SetMember(member_dict);
      } catch (Ice::Exception &e) {
        MCE_WARN("ChangePrivilegeTask::CreateResults----> uid:"<< actor_uid <<"  TalkGroupMemberLoaderAdapter::instance().LoadGroupMember  error:" << e);
        set_ret = false;
      }
      MCE_DEBUG("ChangePrivilegeTask::CreateResults--->  step7");
      if (!set_ret) {
        is_correct_ = false;
        code_ = "internal-server-error";
        description_ = str(boost::format("用户%d 更新群%s 成员权限失败") % actor_uid % roomid_str);
      }
      Str2StrMap view_info;
      if (is_correct_) {
        for (GroupMemberSeq::const_iterator view_iter=to_be_changed.begin(); view_iter!=to_be_changed.end(); view_iter++) {
          view_info["affiliation"] = boost::lexical_cast<string>((*view_iter)->level);
          try {
            TalkGroupViewLoaderAdapter::instance().UpdateItemAsyn((*view_iter)->uid, roomid, view_info);
          } catch (Ice::Exception &e) {
            MCE_WARN("ChangePrivilegeTask::CreateResults----> uid:"<< actor_uid <<"   rooomid:" << roomid <<"   update_uid:" << (*view_iter)->uid
                <<"  TalkGroupViewLoaderAdapter::instance().UpdateItem  error:" << e);
            continue;
          }
        }
        common_data_["item_type"] = "1";
      }
      MCE_DEBUG("ChangePrivilegeTask::CreateResults--->  step8");
      string user_name;
      TalkUserPtr talk_user = TalkCacheClient::instance().GetUserByIdWithLoad(actor_uid);
      if (talk_user)
        user_name = talk_user->name;
      // send presence
      IntSeq offline_receivers;
      MCE_DEBUG("ChangePrivilegeTask::CreateResults--->  step9  user_name:" << user_name);
      for (GroupMemberSeq::const_iterator member_iter=to_be_changed.begin(); member_iter!=to_be_changed.end(); member_iter++) {
        /*
        TalkUserMap::const_iterator user_iter = talk_users.find(member_iter->first);
        if (user_iter != talk_users.end())
          user_name = user_iter->second;
          */
        Str2StrMap presence_data;
        presence_data["config_type"] = config_type_;
        presence_data["item_type"] = "2";
        presence_data["from_jid"] = from_jid_;
        presence_data["message_type"] = boost::lexical_cast<string>(PRESENCE_COMMON);        
        presence_data["to_id"] = to_id;
        presence_data["presence.actor_name"] = TalkFunUtil::instance().XMLEncode(user_name);
        presence_data["presence.roomid"] = roomid_str;
        presence_data["presence.actor_id"] = actor_uid_str;
        presence_data["presence.user_id"] = boost::lexical_cast<string>((*member_iter)->uid);
        presence_data["presence.affiliation"] = TalkFunUtil::instance().PrivilegeInt2String((*member_iter)->level);
        /////////////////
        offline_receivers.clear();
        offline_receivers.push_back((*member_iter)->uid);
        SendOfflineAdminMsgs(presence_data, offline_receivers);
        ///////////////////////

        presence_data_seq_.push_back(presence_data);
      }
      MCE_DEBUG("ChangePrivilegeTask::CreateResults--->  step10");
    }
  }
}

/*说明：退出群并返回结果
 */
void QuitGroupTask::CreateResults() {
  Str2StrMap presence_data;
  common_data_["message_type"] = boost::lexical_cast<string>(IQ_RESULT);
  string actor_uid_str, roomid_str;
  Ice::Long roomid = 0;
  int actor_uid =0;
  Str2StrMap::const_iterator iter;
  if ((iter=paras_.find("iq:id")) != paras_.end()) {
    common_data_["iq.id"] = iter->second;
  }
  if ((iter=paras_.find("iq:to")) != paras_.end()) { 
    roomid_str = common_data_["iq.roomid"] = TalkFunUtil::instance().GetIDFromJID(iter->second);
    roomid = boost::lexical_cast<Ice::Long>(roomid_str);
  }
  MCE_DEBUG("QuitGroupTask::CreateResults  step1 roomid:" << roomid_str);
  if ((iter=paras_.find("actor_uid")) != paras_.end()) { 
    actor_uid_str = iter->second;
    actor_uid  = boost::lexical_cast<int>(actor_uid_str);
  }
  vector<string> receivers;
  receivers.push_back(actor_uid_str);
  common_data_["to_id"] =  ReceiversHandler::ReceiversAssembly(receivers);
  GroupMemberSeq gm_seq;
  if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad(roomid, gm_seq)) {
    MCE_WARN("QuitGroupTask::CreateResults---> TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad  uid:"
        << actor_uid << "  roomid:" << roomid_str);
    is_correct_ = false;
    code_ = "internal-server-error";
    description_ = str(boost::format("从群%d 删除用户%d发生异常,获取群列表失败") % roomid % actor_uid);
  }
  MCE_DEBUG("QuitGroupTask::CreateResults  step2  gm_seq.size:" << gm_seq.size());
  IntSeq presence_receivers;
  IntSeq offline_receivers;
  for (size_t i=0; i<gm_seq.size(); ++i) {
    if (gm_seq.at(i)->uid==actor_uid && (gm_seq.at(i)->level==kOwner)) {
      is_correct_ = false;
      code_ = "forbidden";
      description_ = str(boost::format("群主%d 无法从群%d 中退出") % actor_uid % roomid);
    }
    presence_receivers.push_back(gm_seq.at(i)->uid);
    if (gm_seq.at(i)->level==kOwner || gm_seq.at(i)->level==kAdmin)
      offline_receivers.push_back(gm_seq.at(i)->uid);
  }
  MCE_DEBUG("QuitGroupTask::CreateResults  step3:");
  if (is_correct_) {
    MCE_DEBUG("QuitGroupTask::CreateResults  step4");
    IntSeq ids;
    ids.push_back(actor_uid);
    try {
      is_correct_ = TalkGroupMemberLoaderAdapter::instance().RemoveMember(roomid, ids);
    } catch (Ice::Exception &e) {
      MCE_WARN("QuitGroupTask::CreateResults----> uid:"<< actor_uid <<"  TalkGroupMemberLoaderAdapter::instance().LoadGroupMember  error:" << e);
      is_correct_ = false;
    }
    if (!is_correct_) {
      is_correct_ = false;
      code_ = "internal-server-error";
      description_ = str(boost::format("从群%d 删除用户%d发生异常") % roomid %actor_uid);
    }
    MCE_DEBUG("QuitGroupTask::CreateResults  step5");
    if (is_correct_) {
      MCE_DEBUG("QuitGroupTask::CreateResults  step6");
      try {
        is_correct_ = TalkGroupViewLoaderAdapter::instance().Delete(actor_uid, roomid, kSolidGroup);
      } catch (Ice::Exception &e) {
        MCE_WARN("QuitGroupTask::CreateResults----> uid:"<< actor_uid <<"  roomid:"<< roomid<<"  TalkGroupViewLoaderAdapter::instance().Delete  error:" << e);
        is_correct_ = false;
      }
      if (!is_correct_) {
        is_correct_ = false;
        code_ = "internal-server-error";
        description_ = str(boost::format("从群%d 视图删除用户%d发生异常") % roomid %actor_uid);
      }
      if (is_correct_) {
        common_data_["item_type"] = "1";

        presence_data["config_type"] = config_type_;
        presence_data["item_type"] = "2";
        presence_data["from_jid"] = from_jid_;
        presence_data["message_type"] = boost::lexical_cast<string>(PRESENCE_COMMON);        
        presence_data["presence.roomid"] = roomid_str;
        presence_data["presence.userid"] = actor_uid_str;
        presence_data["to_id"] = ReceiversHandler::ReceiversAssembly(presence_receivers);
        TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(boost::lexical_cast<int>(actor_uid));
        if (!u) {
          MCE_WARN("QuitGroupTask::CreateResults---> TalkCacheClient::instance().GetUserByIdWithLoad  uid:" << actor_uid);
        } else 
          presence_data["presence.username"] = u->name;

        string room_name;
        GroupInfoPtr group_info;
        if (!TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad(roomid,group_info)) {
          MCE_WARN("QuitGroupTask::CreateResults----> uid:"<< actor_uid <<"   roomid:" <<roomid
              <<"  TalkGroupLoaderAdapter::instance().LoadGroupInfo ");
//          code_ = "internal-server-error";
//          description_ = str(boost::format("获取房间%d 信息失败") % group_id );
//          is_correct_ = false;
        } 
        if (group_info) 
          room_name = group_info->name;
        presence_data["presence.roomname"] = TalkFunUtil::instance().XMLEncode(room_name);
        presence_data_seq_.push_back(presence_data);
        /////////////////
        SendOfflineAdminMsgs(presence_data, offline_receivers);
        ///////////////////////
      }
    }
  }
  if (!is_correct_) {
    common_data_["item_type"] = "3";
    common_data_["iq.code"] = code_;
    common_data_["iq.reason"] = description_;
  }
}

/*说明：发送聊天消息
 */
void SendMessageTask::CreateResults() {
  common_data_["message_type"] = boost::lexical_cast<string>(GROUPCHAT_MESSAGE);
  string roomid_str, actor_uid_str;
  Ice::Long roomid = 0;
  string message_body;
  Str2StrMap serilized_data;
  int actor_uid = 0;
  for (Str2StrMap::const_iterator para_iter=paras_.begin(); para_iter!=paras_.end(); para_iter++) {
    string key = para_iter->first;
    if (key == "message:to") {
      roomid_str = common_data_["message.roomid"] = TalkFunUtil::instance().GetIDFromJID(para_iter->second);
      roomid = boost::lexical_cast<Ice::Long>(roomid_str);
    } else if (key == "actor_uid") {
      common_data_["message.memberid"] = actor_uid_str = para_iter->second;
      actor_uid = boost::lexical_cast<int>(actor_uid_str);
    } else if (key == "message.body") {
      message_body = common_data_["message.body"] = TalkFunUtil::instance().XMLEncode(para_iter->second);
      if (!TalkFunUtil::instance().SpamString(message_body, actor_uid)) {
        MCE_INFO("SendMessageTask::CreateResults  SpamString error.    uid:" << actor_uid <<"   message_body:" << message_body);
        return;
      }
    } else if (key == "message.format:bold") {
      serilized_data["message.bold"] = common_data_["message.bold"] = para_iter->second;
    }  else if (key == "message.format:face") {
      serilized_data["message.face"] = common_data_["message.face"] = para_iter->second;
    }  else if (key == "message.format:height") {
      serilized_data["message.height"] = common_data_["message.height"] = para_iter->second;
    }  else if (key == "message.format:ink") {
      serilized_data["message.ink"] = common_data_["message.ink"] = para_iter->second;
    }  else if (key == "message.format:italic") {
      serilized_data["message.italic"] = common_data_["message.italic"] = para_iter->second;
    }  else if (key == "message.usrdata:xmlns") {
      serilized_data["message.usrdata.xmlns"] = common_data_["message.usrdata.xmlns"] = para_iter->second;
    }  else if (key == "message.usrdata") {
      serilized_data["message.usrdata"] = common_data_["message.usrdata"] = para_iter->second;
    }
  }
  string user_name;
  TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(actor_uid);
  if (!u) {
    MCE_WARN("SendMessageTask::CreateResults---> TalkCacheClient::instance().GetUserByIdWithLoad  uid:" << actor_uid);
    user_name = actor_uid_str;
  } else
    user_name = u->name;
  common_data_["message.name"] = TalkFunUtil::instance().XMLEncode(user_name);
  GroupInfoPtr group_info;
  if (!TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad(roomid,group_info)) {
    MCE_WARN("SendMessageTask::CreateResults--->TalkGroupInfoClient::instance().GetGroupInfoByIdWithLoad  uid:" 
        << actor_uid << "  roomid:" << roomid);
    code_ = "internal-server-error";
    description_ = str(boost::format("获取房间%d 信息失败") % roomid );
    is_correct_ = false;
  } 
  int group_type = kSolidGroup;
  if (group_info)
    group_type = group_info->type;
  if (is_correct_) {
    GroupMemberSeq gm_seq;
    if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad(roomid, gm_seq)) {
      MCE_WARN("SendMessageTask::CreateResults---> TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad uid:" << actor_uid << "  roomid:" << roomid_str);
      is_correct_ = false;
      code_ = "internal-server-error";
      description_ = str(boost::format("获取房间%d 成员信息失败") % roomid);
    }
    if (is_correct_) {
      Ice::Long msg_id = 0;
      try {
        msg_id = IMIdGeneratorAdapter::instance().getId(22);
      } catch (Ice::Exception &e) {
        MCE_WARN("SendMessageTask::CreateResults----> uid:"<< actor_uid <<"  roomid:" << roomid
            <<"   IMIdGeneratorAdapter::instance().getId(22)  error:" << e);
        code_ = "internal-server-error";
        description_ = str(boost::format("用户%d 产生群%d 的消息id失败") %actor_uid % roomid);
        is_correct_ = false;
      }
      common_data_["message.msgid"] = boost::lexical_cast<string>(msg_id);
      if (is_correct_) {
        string content_info;
        TalkFunUtil::instance().Map2String(serilized_data, content_info);
        string current_time = TalkFunUtil::instance().GetCurrentTime();
        MessageBodyPtr msg_body = new MessageBody;
        msg_body->messageId = msg_id;
        msg_body->sessionId = roomid;
        msg_body->content = message_body;
        msg_body->contentInfo = content_info;
        msg_body->source = "xntalk";
        msg_body->sourceType = 0;
        msg_body->uid = actor_uid;
        msg_body->link = "";
        msg_body->attachFlag = 0;
        msg_body->createTime = current_time;
        MessageBodySeq msg_body_seq;
        msg_body_seq.push_back(msg_body);
        try {
          is_correct_ = GroupChatMessageLoaderAdapter::instance().SetMessages(msg_body_seq);
        } catch (Ice::Exception &e) {
          MCE_WARN("SendMessageTask::CreateResults----> uid:"<< actor_uid <<"  roomid:" << roomid
              <<"   IMIdGeneratorAdapter::instance().getId(22)  error:" << e);
          is_correct_ = false;
        }
        if (!is_correct_) {
          code_ = "internal-server-error";
          description_ = str(boost::format("用户%d 对群%d 的消息 %d写入失败") %actor_uid % roomid % msg_id);
        }
        if (is_correct_) {
          Str2StrMap view_map;
          view_map["update_time"] = current_time;
          view_map["msgid"] = boost::lexical_cast<Ice::Long>(msg_id);
          try {
            TalkGroupViewLoaderAdapter::instance().UpdateItemAsyn(actor_uid, roomid, view_map); 
          } catch (Ice::Exception &e) {
            MCE_WARN("SendMessageTask::CreateResults ----> uid:"<< actor_uid <<"  roomid:" << roomid
                <<"  TalkGroupViewLoaderAdapter::instance().UpdateItemAsyn  error:" << e);
            is_correct_ = false;
            code_ = "internal-server-error";
            description_ = str(boost::format("用户%d 对群%d 的视图 写入失败") %actor_uid % roomid);
          }
          Str2StrMap group_info;
          group_info["session_last_content"] = str(boost::format("'%s'") %message_body);
          group_info["session_last_time"] = str(boost::format("'%s'") %current_time);
          try {
            TalkGroupLoaderAdapter::instance().UpdateGroupInfoAsyn(roomid, group_info);
          } catch (Ice::Exception &e) {
            MCE_WARN("SendMessageTask::CreateResults ----> uid:"<< actor_uid <<"  roomid:" << roomid
                <<"  TalkGroupLoaderAdapter::instance().UpdateItemAsyn  error:" << e);
            is_correct_ = false;
            code_ = "internal-server-error";
            description_ = str(boost::format("用户%d 对群%d 的信息 写入失败") %actor_uid % roomid);
          }
          if (is_correct_) {
            vector<int> receivers;
            IntSeq member_ids;
            for (size_t i=0; i<gm_seq.size(); ++i) 
              member_ids.push_back(gm_seq.at(i)->uid);
            JidSeqMap members_jid;
            try {
              members_jid = TalkFunStateAdapter::instance().getUsersJids(member_ids);
            } catch (Ice::Exception &e) {
              MCE_WARN("SendMessageTask::CreateResults ----> uid:"<< actor_uid <<"  roomid:" << roomid
                  <<"  TalkFunStateAdapter::instance().getUserJids  error:" << e);
              is_correct_ = false;
              code_ = "internal-server-error";
              description_ = str(boost::format("用户%d 对群%d 的成员状态获取失败") %actor_uid % roomid);
            }
            MCE_DEBUG("SendMessageTask::CreateResults ---->  member_ids.size" << member_ids.size() << " online_size:" << members_jid.size());
            for (size_t i=0; i<gm_seq.size(); ++i) {
              JidSeqMap::const_iterator member_iter = members_jid.find(gm_seq.at(i)->uid);
              if (member_iter == members_jid.end()) {
                //写离线消息
              MCE_DEBUG("SendMessageTask::CreateResults ----> offline actor:"<< actor_uid << "   uid:" <<gm_seq.at(i)->uid
                  <<"  roomid:" << roomid << "   group_type:"<<group_type <<"  msgid:" <<msg_id);
                continue;
              }
              string online_type =  TalkFunUtil::instance().GetOnlineTypeFromJids(member_iter->second);
              MCE_DEBUG("SendMessageTask::CreateResults ----> online actor:"<< actor_uid << "   uid:" <<gm_seq.at(i)->uid
                  <<"  roomid:" << roomid << "   group_type:"<<group_type <<"  msgid:" <<msg_id <<"     online_type:" << online_type);
              if (online_type != "xntalk")     ///如果接入wp等，需要改
                continue;
              try {
                TalkGroupViewLoaderAdapter::instance().UpdateMsgid(gm_seq.at(i)->uid, roomid, group_type, msg_id);
              } catch (Ice::Exception &e) {
                MCE_WARN("SendMessageTask::CreateResults ----> uid:"<< actor_uid <<"  roomid:" << roomid << "   group_type:"<<group_type <<"  msgid:" <<msg_id
                    <<"  TalkGroupViewLoaderAdapter::instance().UpdateMsgid  error:" << e);
                is_correct_ = false;
                code_ = "internal-server-error";
                description_ = str(boost::format("用户%d 对群%d 的消息%d修改失败") %gm_seq.at(i)->uid % roomid %msg_id);
              }
              receivers.push_back(gm_seq.at(i)->uid);
            }
            common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
            common_data_["item_type"] = "1";
          }
        }
      }
    }
  }
  if (!is_correct_) {
    common_data_["item_type"] = "3";
    common_data_["message.code"] = code_;
    common_data_["message.reason"] = description_;
  }
}

/*说明：改变出席状态
 *参数1：用户id
 *参数2：在线类型
 *参数3：在线状态
 *参数4：是否在登录
 */
void ChangeStatusPresenceTask::SendPresence(const int userid, const string &online_type,const string &online_stat, const bool is_login) {
  MCE_DEBUG("ChangeStatusPresenceTask::SendPresence userid:"<<userid<<"   online_type:"<<online_type<<"   online_stat:" << online_stat << "  is_login:"<<is_login);
  string userid_str = boost::lexical_cast<string>(userid);
  TalkGroupViewSeq talk_group_views;
  if (!TalkGroupViewClient::instance().GetAllGroups(userid, talk_group_views)) {
    MCE_WARN("ChangeStatusPresenceTask::SendPresence----> uid:"<< userid <<"  TalkGroupViewClient::instance().GetRecentGroup");
    is_correct_ = false;
    code_ = "internal-server-error";
    description_ = str(boost::format("用户%s 取群列错误") % userid_str);
  }
  if (talk_group_views.empty())
    return;
  if (is_correct_) {
    string equipment;
    IntSeq buddy_ids;
    try {
      buddy_ids = BuddyByIdCacheAdapter::instance().getFriendListAsc(userid, 5000);
    } catch (Ice::Exception &e) {
      MCE_WARN("ChangeStatusPresenceTask::SendPresence----> BuddyByIdCacheAdapter::instance().getFriendListAsc  userid:" << userid  <<"  error:" << e);
      is_correct_ = false;
      code_ = "internal-server-error";
      description_ = str(boost::format("用户%s 取好友信息出现错误") % userid_str);
    }
    if (is_correct_) {
      AvatarSeq avatars;
      try {
        avatars = TalkFunStateAdapter::instance().getUserAvatars(userid); 
      } catch (Ice::Exception &e) {
        MCE_WARN("ChangeStatusPresenceTask::SendPresence----> TalkFunStateAdapter::instance().getUserJids  userid:" << userid 
            <<"  avatars.size:"<< avatars.size() <<"  error:" << e);
        is_correct_ = false;
        code_ = "internal-server-error";
        description_ = str(boost::format("用户%s 在线状态出现错误") % userid_str);
      }
      if (is_correct_) {
//  bool GetOnlineTypeAndStateFromAvatars(const com::xiaonei::talk::common::AvatarSeq &avatars, 
 //     std::string &client_type, std::string &online_type, const bool is_login);
        string temp_online_type = online_type;
        string temp_online_stat = online_stat;
        MCE_DEBUG("ChangeStatusPresenceTask::SendPresence---> avatars.size:" << avatars.size());
        if (!TalkFunUtil::instance().GetOnlineTypeAndStateFromAvatars(avatars,from_jid_, temp_online_type, temp_online_stat, is_login))  //如果没必要广播新的状态，直接返回
          return;
        Str2StrMap presence_data;
        presence_data["presence.equipment"] = temp_online_type;
        presence_data["presence.state"] = temp_online_stat;
        presence_data["item_type"] = "1";
        presence_data["message_type"] = boost::lexical_cast<string>(PRESENCE_COMMON);        
        presence_data["config_type"] = config_type_;
        presence_data["from_jid"] = from_jid_;
        presence_data["presence.userid"] = userid_str;
        for (TalkGroupViewSeq::const_iterator view_iter=talk_group_views.begin(); view_iter!=talk_group_views.end(); view_iter++) {
          presence_data["presence.roomid"] = boost::lexical_cast<string>((*view_iter)->sessionId);
          GroupMemberSeq gm_seq;
          if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad((*view_iter)->sessionId, gm_seq)) {
            MCE_WARN("ChangeStatusPresenceTask::SendPresence---> TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad uid:" << userid 
                << "  roomid:" <<(*view_iter)->sessionId );
            continue;
          }
          vector<int> receivers;
          for (GroupMemberSeq::const_iterator member_iter=gm_seq.begin(); member_iter!=gm_seq.end(); member_iter++) {
            if ((*member_iter)->uid!=userid && find(buddy_ids.begin(), buddy_ids.end(),(*member_iter)->uid)==buddy_ids.end())
              receivers.push_back((*member_iter)->uid);          
          }
          presence_data["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);  

          presence_data_seq_.push_back(presence_data);
        }
      }
    }
  }
}

/*说明：改变出席状态并返回结果
 */
void ChangeStatusPresenceTask::CreateResults() {
  MCE_DEBUG("ChangeStatusPresenceTask::CreateResults  step1");
  Str2StrMap::const_iterator iter;
  int userid = 0;
  string userid_str;
  if ((iter=paras_.find("actor_uid")) != paras_.end()) { 
    userid_str = iter->second;
    userid = boost::lexical_cast<int>(userid_str);
  }
  MCE_DEBUG("LoginPresenceTask::CreateResults  step2   uid:" << userid);
  if (userid > 0) {
    if ((iter=paras_.find("presence.priority")) != paras_.end()) {
      if ((iter=paras_.find("presence.show")) != paras_.end()) {
        SendPresence(userid, "xntalk", iter->second, true);
      } else
        SendPresence(userid, "xntalk", "online", true);
    } else
      SendPresence(userid, "xntalk", "offline", false);
  } 
  if (!is_correct_) {  
    common_data_["item_type"] = "2";
    common_data_["presence.code"] = code_;
    common_data_["presence.reason"] = description_;
    common_data_["message_type"] = boost::lexical_cast<string>(PRESENCE_COMMON);        
    common_data_["config_type"] = config_type_;
    common_data_["from_jid"] = from_jid_;
    common_data_["presence.userid"] = userid_str;
    MCE_WARN("LoginPresenceTask::CreateResults error.");
  }
}

/*说明：获取附件信息并返回结果
 */
void AttachmentMessageTask::CreateResults() {
  Str2StrMap::const_iterator iter;
  int actor_uid = 0;
  string actor_uid_str;
  if ((iter=paras_.find("actor_uid")) != paras_.end()) { 
    actor_uid_str = iter->second;
    actor_uid = boost::lexical_cast<int>(actor_uid_str);
  }
  if (actor_uid <= 0) 
    return;
  Ice::Long roomid = 0;
  if ((iter=paras_.find("message:to")) != paras_.end()) { 
    string roomid_str = TalkFunUtil::instance().GetIDFromJID(iter->second);
    roomid = boost::lexical_cast<Ice::Long>(roomid_str);
    common_data_["message.from"] = iter->second;
  }
  if (roomid <= 0)
    return;
  GroupMemberSeq gm_seq;
  if (!TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad(roomid, gm_seq)) {
    MCE_WARN("AttachmentMessageTask::CreateResults---> TalkGroupMemberClient::instance().GetGroupMembersByIdWithLoad uid:" << actor_uid << "  roomid:" << roomid);
    return;
  }
  IntSeq receivers;
  for (GroupMemberSeq::const_iterator member_iter=gm_seq.begin(); member_iter!=gm_seq.end(); member_iter++)
    receivers.push_back((*member_iter)->uid);
  common_data_["message_type"] = boost::lexical_cast<string>(GROUPCHAT_MESSAGE);
  common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
  common_data_["item_type"] = "1";
  if ((iter=paras_.find("message.usrdata")) != paras_.end()) { 
    common_data_["message.usrdata"] = iter->second;
  }
}

/*说明：获取离线信息并返回结果
 */
void OfflineMessageTask::CreateResults() {
  Str2StrMap::const_iterator iter;
  int actor_uid = 0;
  string actor_uid_str;
  if ((iter=paras_.find("actor_uid")) != paras_.end()) { 
    actor_uid_str = iter->second;
    actor_uid = boost::lexical_cast<int>(actor_uid_str);
  }
  string username;
  TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(actor_uid);
  if (u) {
    username = u->name;
  } 
  MCE_DEBUG("TalkFunMuc::SendOfflineAdminMsgs   step1  uid:"<<actor_uid); 
  if (actor_uid > 0) {
    //载入管理消息
    MCE_DEBUG("TalkFunMuc::SendOfflineAdminMsgs   step2"); 
    StrSeq admin_msgs;
    try {
      admin_msgs = TalkGroupAdminMsgLoaderAdapter::instance().getMsg(actor_uid);
    }catch(Ice::Exception& e) {    
      MCE_WARN("TalkFunMuc::SendOfflineAdminMsgs  uid:"<<actor_uid <<"  TalkGroupAdminMsgLoaderAdapter::instance().saveMsgAsyn error:" << e); 
    } 
    MCE_DEBUG("TalkFunMuc::SendOfflineAdminMsgs   step2  admin_msg.size:" << admin_msgs.size()); 
    try {
      TalkGroupAdminMsgLoaderAdapter::instance().delMsgAsyn(actor_uid);
    }catch(Ice::Exception& e) {    
      MCE_WARN("TalkFunMuc::SendOfflineAdminMsgs  uid:"<<actor_uid <<"  TalkGroupAdminMsgLoaderAdapter::instance().delMsgAsyn error:" << e); 
    } 
    for (StrSeq::const_iterator str_iter=admin_msgs.begin(); str_iter!=admin_msgs.end(); str_iter++) {
      MCE_DEBUG("TalkFunMuc::SendOfflineAdminMsgs   step3    str:" << (*str_iter)); 
      Str2StrMap message;
      TalkFunUtil::instance().String2Map(*str_iter, message);
      MCE_DEBUG("TalkFunMuc::SendOfflineAdminMsgs   step4    message.empty:" << message.empty()); 
      if (!message.empty())
        presence_data_seq_.push_back(message);      
    }

    //载入聊天消息
    TalkGroupViewSeq talk_group_views;
    if (!TalkGroupViewClient::instance().GetAllGroups(actor_uid, talk_group_views)) {
      MCE_WARN("OfflineMessageTask::CreateResults----> uid:"<< actor_uid <<"  TalkGroupViewClient::instance().GetRecentGroup");
      is_correct_ = false;
      code_ = "internal-server-error";
      description_ = str(boost::format("用户%s 取群列错误") %actor_uid);;
    }
    MCE_DEBUG("TalkFunMuc::SendOfflineAdminMsgs   step5   "); 
    if (is_correct_) {
      MCE_DEBUG("TalkFunMuc::SendOfflineAdminMsgs   step6   "); 
      MessageRequestMap requests;
      map<Ice::Long, int> group_types;
      for (TalkGroupViewSeq::const_iterator view_iter=talk_group_views.begin(); view_iter!=talk_group_views.end(); view_iter++) {
        MessageRequestPtr request = new MessageRequest;
        request->msgid = (*view_iter)->msgid;
        request->offset = 0;
        request->limit = 30;        
        requests[(*view_iter)->sessionId] = request;
        group_types[(*view_iter)->sessionId] = (*view_iter)->sessionType; 
        MCE_DEBUG("OfflineMessageTask::CreateResults----> uid:"<< actor_uid <<"   session_id:" <<(*view_iter)->sessionId << "  msgid:" <<request->msgid);
      }
      MCE_DEBUG("TalkFunMuc::SendOfflineAdminMsgs   step7   "); 
      IntSeq msg_receivers;
      msg_receivers.push_back(actor_uid);
      string to_id = ReceiversHandler::ReceiversAssembly(msg_receivers);
      MessageBodySeqMap messages = GroupChatMessageCacheClient::instance().GetOfflineMessages(requests);
      MCE_DEBUG("TalkFunMuc::SendOfflineAdminMsgs   step8   messages.size:" << messages.size()); 
      for (MessageBodySeqMap::const_iterator session_iter=messages.begin(); session_iter!=messages.end(); session_iter++) {
        Ice::Long max_msg_id = 0;
        MCE_DEBUG("TalkFunMuc::SendOfflineAdminMsgs   step9" <<"  session_id:" << session_iter->first<<"   message.size:" << (session_iter->second).size()); 
        for (MessageBodySeq::const_iterator msg_iter=(session_iter->second).begin();  msg_iter!=(session_iter->second).end(); msg_iter++) {
          Str2StrMap offline_message;
          offline_message["config_type"] = "10022";     //用聊天得模板展示
          offline_message["item_type"] = "1";
          offline_message["message.memberid"] = boost::lexical_cast<string>((*msg_iter)->uid);
          offline_message["message.roomid"] = boost::lexical_cast<string>((*msg_iter)->sessionId);
          offline_message["message.body"] = (*msg_iter)->content;
          offline_message["message.msgid"] = boost::lexical_cast<string>((*msg_iter)->messageId);
          Str2StrMap content_info;
          TalkFunUtil::instance().String2Map((*msg_iter)->contentInfo, content_info);
          MCE_DEBUG("TalkFunMuc::SendOfflineAdminMsgs   step10" <<"  content_info:" <<(*msg_iter)->contentInfo <<"   body:" << (*msg_iter)->content
             <<"   msgid:"<< (*msg_iter)->messageId<<"   message.size:" << content_info.size()); 
          offline_message.insert(content_info.begin(), content_info.end());
          string username;
          TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad((*msg_iter)->uid);
          if (u) {
            username = u->name;
          } 
          offline_message["message.name"] = username;
          offline_message["message.timestamp"] = FormatTimeStamp((*msg_iter)->createTime);
          offline_message["to_id"] = to_id;
          presence_data_seq_.push_back(offline_message);

          if ((*msg_iter)->messageId > max_msg_id)
            max_msg_id = (*msg_iter)->messageId;
        }
        if (!(session_iter->second).empty()) {
          try {
            TalkGroupViewLoaderAdapter::instance().UpdateMsgid(actor_uid, session_iter->first, group_types[session_iter->first], max_msg_id);
          } catch (Ice::Exception &e) {
            MCE_WARN("TalkFunMuc::SendOfflineAdminMsgs  ----> uid:"<< actor_uid <<"  roomid:" << session_iter->first
                << "   group_type:"<< group_types[session_iter->first] <<"  msgid:" <<max_msg_id
                <<"  TalkGroupViewLoaderAdapter::instance().UpdateMsgid  error:" << e);
            is_correct_ = false;
            code_ = "internal-server-error";
            description_ = str(boost::format("用户%d 对群%d 的消息%d修改失败") %actor_uid % session_iter->first %max_msg_id);
          }
        }
      }
    }
  }  
}

/*说明：格式化时间信息并返回结果
 *参数1: 时间信息
 *返回值：格式化后的时间
 */
string OfflineMessageTask::FormatTimeStamp(string &time) {
  ostringstream os;
  if (!time.empty()) {
    for (size_t i=0; i<time.size(); ++i) {
      if (time[i] == ' ')
        os << "T";
      else
        os << time[i];
    }
    os << "Z";
  }
  return os.str();
}

void Tester1Task::CreateResults() {
  vector<string> receivers;
  receivers.push_back("225521695");
  common_data_["to_id"] = ReceiversHandler::ReceiversAssembly(receivers);
  common_data_["message_type"] = boost::lexical_cast<string>(IQ_RESULT);
  common_data_["item_type"] = "1";
   
  common_data_["iq.id"] = "1231231";
  common_data_["iq.roomid"] = "1111";
  common_data_["iq.roomname"] = "qewr";
}
