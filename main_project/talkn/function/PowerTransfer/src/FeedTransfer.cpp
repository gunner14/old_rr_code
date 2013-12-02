#include "FeedTransfer.h"
#include "PowerTransferI.h"
#include "MessageType.h"
#include <boost/lexical_cast.hpp>
#include "XmppTools.h"
#include "TalkFunStateAdapter.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "TalkProtoAdapter.h"
#include "TalkUtil/src/pugixml.hpp"
#include "TalkUtil/src/XmppTools.h"
#include "UrlCode.h"

using namespace com::xiaonei::talk::function;
using namespace com::xiaonei::talk::fun::state;
using namespace MyUtil;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::util;
using namespace ::xce::feed;
using namespace ::com::xiaonei::xce;
using namespace com::xiaonei::talk::function::transfer;
using namespace com::xiaonei::talk::proto;
using namespace pugi;
using namespace talk::http;
using namespace ::xce::talk;


void DeliverFeedTask::handle() {
  const string FuncName = "DeliverFeedTask::handle";
  TimeStatN ts;
  MCE_INFO("PowerTransferI::deliverFeedWithFocusFriend-> feed actor:" << feed_->actor);	

  if(!XmlParas())
    return;

  JidSeqMap jmap;
  try {
    jmap = TalkFunStateAdapter::instance().getUsersJids(target_,22);
  } catch (Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkFunStateAdapter::getUsersJids-->" << target_.size() << " " << e);
    return;
  } catch (...) {
    MCE_WARN(FuncName << "-->TalkFunStateAdapter::getUsersJids--> err: " << target_.size());
    return;
  }
  
  DeliverFeedInit();

  for(JidSeqMap::const_iterator it = jmap.begin(); it != jmap.end(); ++it) {
    for(size_t j = 0; j < it->second.size(); ++j) {
      if (it->second.at(j)->endpoint.empty())
        continue;
      if(it->second.at(j)->endpoint[0] == 'T') {
        client_jids_.push_back(jidToString(it->second.at(j)));
        if(client_jids_.size() > 500) 
          Send2Proto(client_jids_, msg2client_);
      } else if(it->second.at(j)->endpoint[0] == 'W') {
        if(focus_friends_.find(it->first) != focus_friends_.end()) {
          webpager_focus_jids_.push_back(jidToString(it->second.at(j)));
          if(webpager_focus_jids_.size() > 500) 
            Send2Proto(webpager_focus_jids_, msg2webpager_focus_);
        } else {
          webpager_unfocus_jids_.push_back(jidToString(it->second.at(j)));
          if(webpager_unfocus_jids_.size() > 500) 
            Send2Proto(webpager_unfocus_jids_, msg2webpager_unfocus_);
        }
      } else if(string::npos != it->second.at(j)->endpoint.find("PTalkProxy")) {
        if(PowerTransferI::instance().IsDeliver2PTalkproxy(feed_->type & 0xFFFF)) {
          phone_jids_.push_back(jidToString(it->second.at(j)));
          if(phone_jids_.size() > 500) 
          Send2Proto(phone_jids_, msg2phone_);
        }
      }
    }
  }
  Send2Proto(client_jids_, msg2client_);
  Send2Proto(phone_jids_, msg2phone_);
  Send2Proto(webpager_focus_jids_, msg2webpager_focus_);
  Send2Proto(webpager_unfocus_jids_, msg2webpager_unfocus_);

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 500.0));
}

bool DeliverFeedTask::XmlParas() {
  xml_document xml;
  if (!xml.load(feed_->xml.c_str())) {
    MCE_DEBUG("FeedToMessagConverter::_buildFNode --> load xml error:  xml=\n"<<feed_->xml.c_str());
    return false;
  }
  //////////////////////////////////////////////////////
  //兼容老版本
  if ( small_type_ == "102" || small_type_ == "103" || small_type_  == "104" || small_type_ == "107" || small_type_  == "110") {
    xml.child("f").child("share").remove_child("comment");
  }
  ///////////////////////////////////////////////////////
  Xml2Map("", feed_->xml, msg2client_);

  // 推送给PTalkProxy的不需要节点数据
  msg2phone_.insert(msg2client_.begin(), msg2client_.end());

  pugi::xml_node node = xml.child("f");
  for(pugi::xml_node_iterator iter = node.begin(); iter!=node.end(); iter++) {
    // 为了保证不关心协议,这里会把协议f节点的整体和子节点分别发送至Proto,会增加网络通信量
    msg2client_["feedxml"] += iter->xml();
  }
  ///////////////////////////////////////////////////////
  //兼容老版本
  if ( small_type_ == "102" || small_type_ == "103" || small_type_  == "104" || small_type_ == "107" || small_type_  == "110") {
    pugi::xml_node node2 = node.child("share");
    for (pugi::xml_node_iterator iter = node2.begin(); iter!=node2.end(); iter++) {
      msg2client_["feedshare"] += iter->xml();
    }
  }
  ///////////////////////////////////////////////////////
  return true;
}

void DeliverFeedTask::DeliverFeedInit() {
  const string FuncName = "DeliverFeed2Client";
  
  // 客户端
  msg2client_["config_type"] = FEED_BASE_CONFIG_TYPE_STR;
  msg2client_["item_type"] = small_type_;
  msg2client_["message_type"] = boost::lexical_cast<std::string>(SYSTEM_MESSAGE);
  msg2client_["id"] = boost::lexical_cast<std::string>(FEED_BASE_ID + feed_->feed);
  msg2client_["actor"] = actor_;
  msg2client_["time"] = time_;
  msg2client_["type"] = big_type_;
  msg2client_["stype"] = small_type_;
  msg2client_["resource"] = source_;

  // webpager
  msg2webpager_unfocus_["config_type"] = FEED_BASE_CONFIG_TYPE_STR;
  msg2webpager_unfocus_["item_type"] = FEED_WEBPAGER_ITEM;
  msg2webpager_unfocus_["message_type"] = boost::lexical_cast<std::string>(FEED_STATUS);
  msg2webpager_unfocus_["id"] = boost::lexical_cast<std::string>(feed_->feed);
  msg2webpager_unfocus_["focus"] = "0";
  msg2webpager_unfocus_["actor"] = actor_;
  msg2webpager_unfocus_["stype"] = small_type_;
  msg2webpager_unfocus_["resource"] = source_;
  msg2webpager_focus_["config_type"] = FEED_BASE_CONFIG_TYPE_STR;
  msg2webpager_focus_["item_type"] = FEED_WEBPAGER_ITEM;
  msg2webpager_focus_["message_type"] = boost::lexical_cast<std::string>(FEED_STATUS);
  msg2webpager_focus_["id"] = boost::lexical_cast<std::string>(feed_->feed);
  msg2webpager_focus_["focus"] = "1";
  msg2webpager_focus_["actor"] = actor_;
  msg2webpager_focus_["stype"] = small_type_;
  msg2webpager_focus_["resource"] = source_;

  //PTalkProxy
  msg2phone_["config_type"] = FEED_BASE_CONFIG_TYPE_STR;
  msg2phone_["item_type"] = boost::lexical_cast<std::string>(feed_->type);
  msg2phone_["message_type"] = boost::lexical_cast<std::string>(PHONE_NOTIFY);
  msg2phone_["stype"] = small_type_;
  msg2phone_["id"] = boost::lexical_cast<std::string>(feed_->feed);
  msg2phone_["time"] = time_;
}
