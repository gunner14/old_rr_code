#include "MessageType.h"
#include <boost/lexical_cast.hpp>
#include "XmppTools.h"
#include "TalkFunStateAdapter.h"
#include "TalkProtoAdapter.h"
#include "TalkUtil/src/pugixml.hpp"
#include "TalkUtil/src/XmppTools.h"
#include "FeedReplyTransfer.h"

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

void DeliverFeedReplyTask::handle() {
  const string FuncName = "DeliverFeedReplyTalk::handle";
  TimeStatN ts;

  xml_document xml;
  if (!xml.load(reply_data_->xml.c_str())) {
    MCE_WARN(FuncName << "--> load xml error:  xml=n" << reply_data_->xml);
    return;
  }

  JidSeqMap jmap;
  try {
    jmap = TalkFunStateAdapter::instance().getUsersJids(target_, IM_ONLINE);
  } catch (Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkFunStateAdapter::getUsersJids-->" << reply_data_->actor << " " 
      << target_.size() << " " << e);
    return;
  } catch (...) {
    MCE_WARN("FeedReplyPipe::handle --> online center target size " << target_.size());
    return;
  }
  if (jmap.empty()) {
    MCE_INFO("FeedReplyPipe::handle --> no friend online");
    return;
  }
  
  /////////////////////////////////////////////
  pugi::xml_node node,node2;
  string replyString;
  if(reply_data_->stype == 502){
    replyString = xml.child("f").child("reply").child("im").child("body").first_child().value();
  } else {
    replyString = xml.child("f").child("reply").child("body").first_child().value();
    if (replyString.size()>=6) {//所有业务前后都有转义以后的双引号&quot;
      if (replyString.substr(0, 6)=="&quot;" && replyString.substr(replyString.size()-6, 6)=="&quot;") {
        replyString = replyString.substr(6,replyString.length()-12);
      } else {
        MCE_WARN("FeedReplyTask::handle-->error quot:" << replyString
                 << " stype:" <<reply_data_->stype << " data:" <<reply_data_->xml);
      }
    }
    node = xml.child("f").child("reply").child("im");
    string tmp = node.child("body").first_child().value();
    node.remove_child("body");
    node2 = node.append_child();
    node2.set_name("body");
    node2.append_child(node_pcdata).set_value( replyString.c_str() );
  }
  MCE_INFO("replyString:"<<replyString);
  node = xml.child("f").child("reply");
  node2 = node.append_child();
  node2.set_name("hcon");
  node2.append_child(node_pcdata).set_value( replyString.c_str() );
  ////////////////////////////////////////////////
  Str2StrMap msg;
  Xml2Map(FuncName, reply_data_->xml, msg);

  msg["config_type"] = FEED_REPLY_BASE_CONFIG_TYPE_STR;
  msg["mesage_type"] = boost::lexical_cast<std::string>(SYSTEM_MESSAGE);
  msg["item_type"] = "FeedReply";

  node = xml.child("f").child("reply");
  for(pugi::xml_node_iterator iter = node.begin(); iter!=node.end(); iter++) {
    // 为了保证不关心协议,这里会把协议f节点的整体和子节点分别发送至Proto,会增加网络通信量
    msg["feedreplyxml"] += iter->xml();
  }

  msg["id"] = boost::lexical_cast<std::string>(reply_data_->replyId);
  msg["actor"] = boost::lexical_cast<std::string>(reply_data_->actor);
  msg["time"] = boost::lexical_cast<std::string>(reply_data_->time);
  msg["type"] = boost::lexical_cast<std::string>(reply_data_->stype / 100 );
  msg["stype"] = boost::lexical_cast<std::string>(reply_data_->stype);
  msg["resource"] = boost::lexical_cast<std::string>(reply_data_->source);
  msg["replycount"] = boost::lexical_cast<std::string>(reply_data_->replyCount);

  vector<string> to_jid;
  for(JidSeqMap::const_iterator it = jmap.begin(); it != jmap.end(); ++it) {
    for(size_t j = 0; j < it->second.size(); ++j) {
      to_jid.push_back(jidToString(it->second.at(j)));
      if(to_jid.size() > 500) {
        Send2Proto(to_jid, msg);
      }
    }
  }
  Send2Proto(to_jid, msg);

  FunStatManager::instance().Stat(FuncName, ts.getTime(), false);
}
