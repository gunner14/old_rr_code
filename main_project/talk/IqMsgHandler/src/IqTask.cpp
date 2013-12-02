#include "IqTask.h"
#include "BuddyCoreAdapter.h"
#include "BuddyByIdReplicaAdapter.h"
#include "IceLogger.h"
#include "TalkRightAdapter.h"
#include "MessageType.h"
#include "XmppTools.h"
#include "PhotoUrlTools.h"
#include "TalkCacheAdapter.h"
#include "BuddyGroupAdapter.h"
#include "PresenceAdapter.h"
#include "OnlineCenterAdapter.h"
#include "TalkProxyAdapter.h"
#include <boost/lexical_cast.hpp>
#include "ConnectionQuery.h"
#include "TalkCacheLoaderAdapter.h"
#include "BuddyFriendsAdapter.h"
#include "TalkDeliverAdapter.h"
#include "QueryRunner.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "IqHandlerI.h"
#include "WTransitAdapter.h"
#include "WTransitForIqAdapter.h"
#include "UserPassportAdapter.h"
#include "XmppErrorCode.h"
#include "Utf8Format.h"
#include "UserCacheAdapter.h"
#include "NotifyIndexAdapter.h"
#include "NotifyIndexNAdapter.h"
#include "RestLogicAdapter.h"
#include "UserNetworkAdapter.h"
#include "UserConfigAdapter.h"
#include "IMWindowAdapter.h"
#include "TicketAdapter.h"
#include "XNTalkLoginFeedAdapter.h"
#include "BlockRelationAdapter.h"
#include "GetUbb.h"
#include "FeedFocusAdapter.h"
#include "FeedMemcProxy/client/talk_cache_client.h"
using namespace xce::feed;
using namespace com::xiaonei::talk::Ubb;
using namespace talk::window;
using namespace xce::talk;
using namespace xce::xntalk;
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::service::buddy;
using namespace com::xiaonei::talk::adapter;
using namespace com::xiaonei::talk::util;
using namespace mop::hi::svc::adapter;
using namespace com::xiaonei::xce;
using namespace MyUtil;
using namespace talk::adapter;
using namespace talk::online::adapter;
using namespace xce::buddy::adapter;
using namespace xce::usercache;
using namespace xce::notify;
using namespace xce::adapter;
using namespace talk::rest;
using namespace xce::adapter::userpassport;
using mop::hi::svc::adapter::UserNetworkAdapter;
using mop::hi::svc::model::NetworkSeq;
using namespace xce::blockrelation::adapter;

void IqGetTask::handle() {
  StatFunc statF("IqGetTask::handle");
  ostringstream os;
  os << "IqGetTask::";
  TimeStat ts;
  xml_document reply;
  xml_node iqNode = reply.append_child();
  iqNode.set_name("iq");
  iqNode.append_attribute("type").set_value("result");
  if (!_doc->child("iq").attribute("from").empty()) {
    iqNode.append_attribute("to") = jidToString(_jid).c_str();//_doc->child("iq").attribute("from").value().c_str();
  }
  if (!_doc->child("iq").attribute("to").empty()) {
    iqNode.append_attribute("from") = _doc->child("iq").attribute("to").value().c_str();
    //iqNode.append_attribute("from") = _doc->child("iq").attribute("to").value();
  }
  if (!_doc->child("iq").attribute("id").empty()) {
    iqNode.append_attribute("id") = _doc->child("iq").attribute("id").value().c_str();
    //iqNode.append_attribute("id") = _doc->child("iq").attribute("id").value();
  } else {
    iqNode.append_attribute("id") = (int)time(NULL);
  }
  for (xml_node n = _doc->child("iq").first_child(); n; n = n.next_sibling()) {
    if (strcmp(n.name(), "query") == 0) {
      string xmlns = n.attribute("xmlns").value();
      if (xmlns == "http://talk.renren.com/focuslist" || 
          xmlns == "http://talk.xiaonei.com/focuslist" ) {
        getfocuslist(reply);
        os << "getfocuslist"; 
      } else if (xmlns == "http://talk.renren.com/UpdataOnlineFace"||
          xmlns == "http://talk.xiaonei.com/UpdataOnlineFace") {
        getfaceubb(reply);
        os << "getfaceubb";
      } else if (xmlns == "http://jabber.org/protocol/disco#items") {
        _getItems(reply);
        os << "_getItems";
      } else if (xmlns == "http://jabber.org/protocol/disco#info") {
        _discoverService(reply);
        os << "_discoverService";
      } else if (xmlns == "jabber:iq:privacy") {
        getPrivacyHandle(reply);
        os << "getPrivacyHandle";
      } else if (xmlns == "jabber:iq:private") {
        getPrivateHandle(reply);
        os << "getPrivateHandle";
      } else if (xmlns == "jabber:iq:last") {
        try{
          TalkProxyAdapter::instance().express(_jid, reply.xml());
        }catch(Ice::Exception& e){
          MCE_WARN("IqGetTask::handle-->TalkProxyAdapter::express-->err" << e);
        }
      } else if (xmlns == "jabber:iq:roster") {
        getRosterHandle(reply);
        os << "getRosterHandle";
      } else if (xmlns == "http://talk.xiaonei.com/xfeed" || xmlns == "http://talk.renren.com/xfeed") {
        getXFeedHandle(reply);
        os << "getXFeedHandle";
      } else if (xmlns == "http://talk.xiaonei.com/popularized_applist" 
          || xmlns == "http://talk.renren.com/popularized_applist") {
        _getApplist(reply);
        os << "popularized_applist";
      } else if (xmlns == "http://talk.xiaonei.com/transfer" 
          || xmlns == "http://talk.renren.com/transfer") {
        getTransferHandle(*_doc);
        os << "getTransferHandle";
      } else if (xmlns == "http://talk.renren.com/xfeed/reply" 
          || xmlns == "http://talk.xiaonei.com/xfeed/reply"){
        _getFeedReply();
        os << "_getFeedReply";
      }else if (xmlns == "http://talk.renren.com/xfeed/xiaozureply" 
          || xmlns == "http://talk.xiaonei.com/xfeed/xiaozureply"){
        _getXiaozuReply();
        os << "_getXiaozuReply";
      } else if (xmlns == "http://talk.renren.com/xfeed/replycount" 
          || xmlns == "http://talk.xiaonei.com/xfeed/replycount"){
        _getFeedReplyCount();
        os << "_getFeedReplyCount";
      } else if(xmlns == "http://talk.renren.com/getsessionkey" 
          || xmlns == "http://talk.xiaonei.com/getsessionkey"){
        _getSessionKey();
        os << "_getSessionKey";
      } else if(xmlns == "http://talk.renren.com/getstrangerinfo" 
          || xmlns == "http://talk.xiaonei.com/getstrangerinfo"){
        _getStrangerInfo(reply);
        os << "_getStrangerInfo";
      } else if(xmlns == "http://talk.renren.com/getbuddyinfo" 
          || xmlns == "http://talk.xiaonei.com/getbuddyinfo"){
        _getBuddyInfo(reply);
        os << "_getBuddyInfo";
      } else if (xmlns == "http://talk.renren.com/webticket"  
          || xmlns == "http://talk.xiaonei.com/webticket") {
        _getWebTicket(reply);
        os << "_getWebTicket";
      } else if (xmlns == "http://talk.renren.com/permissiontoaddfriends" 
          || xmlns == "http://talk.xiaonei.com/permissiontoaddfriends") {
        _getPermissionToAddFriends(reply);
        os << "getPermissionToAddFriends";
      } else if(xmlns == "http://talk.renren.com/getxiaozucontent" 
          || xmlns == "http://talk.xiaonei.com/getxiaozucontent"){
        MCE_DEBUG("getxiaozucontent----------->before");
        _getXiaozuContent();//获取小组帖子内容;
        MCE_DEBUG("getxiaozucontent----------->end");
        os << "_getXiaozuContent";
      } else if(xmlns == "http://talk.renren.com/xfeed/pagereply" 
          || xmlns == "http://talk.xiaonei.com/xfeed/pagereply") {
        getPageReply();
        os << "getPageReply";
			}else if (xmlns == "http://talk.xiaonei.com/xfeed/getmorefeed"){
        if (_jid->userId==225521695 || _jid->userId==225521694 || _jid->userId==415201421|| _jid->userId==368028602|| _jid->userId==270614139||
         _jid->userId==481954149|| _jid->userId==415146513|| _jid->userId==415153420|| _jid->userId==415157150|| _jid->userId==415160480)
			  	_getMoreFeed(reply);
			}
    } else if (strcmp(n.name(), "vCard") == 0) {
      FunStatManager::instance().Stat(os.str(), ts.getTime(), (ts.getTime() >= 250.0));
      return ;
      getVcardHandle(reply);
      os << "getVcardHandle";
    } else if(strcmp(n.name(), "ping") == 0){
      string xmlns = n.attribute("xmlns").value();
      if("urn:xmpp:ping" == xmlns){
        FunStatManager::instance().Stat(os.str(), ts.getTime(), (ts.getTime() >= 250.0));
        return ;
        try{
          TalkProxyAdapter::instance().express(_jid, reply.xml());
        }catch(Ice::Exception& e){
          MCE_WARN("IqGetTask::handle-->TalkProxyAdapter::express-->err" << e);
        }
      }
    } 
    else {
      _default();
      return;
    }
  }
  FunStatManager::instance().Stat(os.str(), ts.getTime(), (ts.getTime() >= 250.0));
  //MCE_INFO(os.str());
}
void IqGetTask::_getMoreFeed(xml_document& reply){
	MCE_INFO("IqGetTask::_getMoreFeed  reply.xml" << _doc->xml());
	long source;
	int actor,shareType,notifyType,id;
	try{
		string ssource = _doc->child("iq").child("query").child("xfeed").attribute("source").value();
		source  = abs(boost::lexical_cast<long>(ssource));
		string sactor = _doc->child("iq").child("query").child("xfeed").attribute("actor").value();
		actor = boost::lexical_cast<int>(sactor);
		string sshareType = _doc->child("iq").child("query").child("xfeed").attribute("sharetype").value();
		shareType = boost::lexical_cast<int>(sshareType);
		string snotifyType = _doc->child("iq").child("query").child("xfeed").attribute("notifytype").value();
		notifyType = boost::lexical_cast<int>(snotifyType);
		string sid = _doc->child("iq").attribute("id").value();
		id = boost::lexical_cast<int>(sid); 
	}catch(...){
		MCE_INFO("IqGetTask::_getMoreFeed  reply.xml" << _doc->xml());
		return;
	}
	int feedId = getFeedId(shareType,notifyType);
	MCE_INFO("IqGetTask::_getMoreFeed    _jid" << _jid->userId << "source=" << source << "actor = " << actor << "shareType" << shareType << "notifyType" << notifyType << "feedId" << feedId);
  try {
  	WTransitAdapter::instance().deliverImoldFeed(_jid,id,source,actor,shareType,notifyType,feedId);
  } catch (Ice::Exception &e) {
	    MCE_WARN("IqGetTask::_getMoreFeed  WTransitAdapter::instance().deliverImoldFeed  _jid" << _jid->userId << "source=" << source << "actor = " << actor << "shareType" << shareType << "notifyType" << notifyType << "feedId" << feedId);

  }
}
int IqGetTask::getFeedId(int shareType , int notifyType){
	if(notifyType == -58 && shareType == 2)
		return 103;
	else if(notifyType == -168 && shareType ==4)
		return 101;
	else if((notifyType == -167 || notifyType == -59) && shareType == 8)
		return 104;
	else if((notifyType == -167 || notifyType == -58) && shareType == 10)
		return 110;
	else if((notifyType == -59  || notifyType == -168) && shareType == 6)
		return 107;
	else if((notifyType == -169 || notifyType == -95) && shareType == 1)
		return 102;
	else if((notifyType == -58 || notifyType == -167) && shareType == 2)
		return 103;

	else if(notifyType == -196 || notifyType == -16)
		return 502;
	else if(notifyType == -171 || notifyType == -19)
		return 709;
	else if(notifyType == -18 || notifyType == -197 )
		return 701;
	else if(notifyType == -17 || notifyType == -172 )
		return 601;
	else
		return -1;
}
void IqGetTask::_getSessionKey(){
  int id = 0;
  try{
    string tmp = _doc->child("iq").attribute("id").value();
    id = boost::lexical_cast<int>(tmp);
  }catch(...){
    MCE_WARN("IqGetTask::_getSessionKey --> cast parameter err");
    return;
  }
  int mark=0;
  string version = _doc->child("iq").first_child().attribute("src").value();
  if (version =="mac2") {
    mark=1;
  }

  //MCE_INFO("IqGetTask::_getSessionKey --> " << jidToString(_jid) << " id = " << id);
  try{
    RestLogicAdapter::instance().newgetSessionKeyByIqId(_jid, id,mark);
    //RestLogicAdapter::instance().getSessionKey(_jid, id);
  }catch(Ice::Exception& e){
    MCE_WARN("IqGetTask::_getSessionKey-->RestLogicAdapter::newgetSessionKey-->err" << e);
  }
}

void IqGetTask::_getApplist(xml_document& reply) {
	xml_node queryNode = reply.child("iq").append_child();
	queryNode.set_name("query");
	queryNode.append_attribute("xmlns") = "http://talk.renren.com/popularized_applist";
  queryNode.append_attribute("host") = _doc->child("iq").child("query").attribute("host").value().c_str();
  //queryNode.append_attribute("host") = _doc->child("iq").child("query").attribute("host").value();

  try{
  	IMWindowAdapter::instance().SendApplist(reply.xml(), _jid);
  }catch(Ice::Exception& e){
  	MCE_WARN("IqGetTask::_getApplist --> userid:" << _jid->userId << " error:" << e);
  }
}

void getNetworksName(NetworkDataPtr &network, MyUtil::StrSeq &seq) {
  if (!network)
    return ;
  int i = 0;
  for (i=0; i<(network->networks).size(); ++i) 
    seq.push_back((network->networks)[i]->networkName);
  for (i=0; i<(network->highschools).size(); ++i) 
    seq.push_back((network->highschools)[i]->HighSchoolName);
  for (i=0; i<(network->universities).size(); ++i) 
    seq.push_back((network->universities)[i]->universityName);
  for (i=0; i<(network->workspaces).size(); ++i) 
    seq.push_back((network->workspaces)[i]->workplaceName);
  for (i=0; i<(network->colleges).size(); ++i) 
    seq.push_back((network->colleges)[i]->collegeName);
  for (i=0; i<(network->elementaries).size(); ++i) 
    seq.push_back((network->elementaries)[i]->elementarySchoolName);
  for (i=0; i<(network->juniorhighschools).size(); ++i) 
    seq.push_back((network->juniorhighschools)[i]->juniorHighSchoolName);
  for (i=0; i<(network->regions).size(); ++i) 
    seq.push_back((network->regions)[i]->cityName);
}
void IqGetTask::_getPermissionToAddFriends(xml_document &reply) {
  MCE_DEBUG("IqGetTask::_getPermissionToAddFriends");
	xml_node queryNode = reply.child("iq").append_child();
	queryNode.set_name("query");
	queryNode.append_attribute("xmlns") = "http://talk.renren.com/permissiontoaddfriends";
  int from , to;
  from = to = 0;
  
	for (xml_node req_item = _doc->child("iq").child("query").first_child(); req_item; req_item = req_item.next_sibling()) {
    if (strcmp(req_item.name(), "item") == 0) {
      from = boost::lexical_cast<int>(req_item.attribute("from").value());
      to = boost::lexical_cast<int>(req_item.attribute("to").value());
    }
    int requestFriendsConfig = 0;
    try {
      requestFriendsConfig = UserConfigAdapter::instance().getUserConfig(to)->requestFriendConfig();
    } catch (Ice::Exception &e) {
      MCE_WARN("IqGetTask::_getPermissionToAddFriends -> UserConfigAdapter.getUserConfig.  error" << e);  
      continue;
    }
    MCE_DEBUG("IqGetTask::_getPermissionToAddFriends -> requestFriendsConfig:" <<requestFriendsConfig);  
    int ret = 1;
    MyUtil::IntSeq block_seq;
    try {
      block_seq = BlockRelationAdapter::instance().getBlockerList(from, 0 ,2000);
    } catch (Ice::Exception &e) {
      MCE_WARN("IqGetTask::_getPermissionToAddFriends -> BlockRelationAdapter.getBlockerList.  error" << e);  
      continue;
    }
    if (find(block_seq.begin(), block_seq.end(), to) == block_seq.end()) {
      if (requestFriendsConfig == 99) {
        ret = 0;
      } else if (requestFriendsConfig == 3) {
        NetworkDataPtr to_networks, from_networks;
        try {
          to_networks = UserNetworkAdapter::instance().getNetworkFullData(to);
          from_networks = UserNetworkAdapter::instance().getNetworkFullData(from);
        } catch  (Ice::Exception &e) {
          MCE_WARN("IqGetTask::_getPermissionToAddFriends -> UserConfigAdapter.getUserConfig.  error" << e);  
          continue;
        }
        if (!to_networks)
          MCE_WARN("IqGetTask::_getPermissionToAddFriends ->  to:" << to <<"    to_networks null");  
        if (!from_networks)
          MCE_WARN("IqGetTask::_getPermissionToAddFriends ->  from:" << from <<"    from_networks null");  
      
        MyUtil::StrSeq to_seq, from_seq;
        getNetworksName(to_networks, to_seq);
        getNetworksName(from_networks, from_seq);
        /*
        for (StrSeq::const_iterator to_iter1=to_seq.begin(); to_iter1!=to_seq.end(); ++to_iter1) {
          MCE_DEBUG("to.." << (*to_iter1));
        }
        for (StrSeq::const_iterator from_iter1=from_seq.begin(); from_iter1!=from_seq.end(); ++from_iter1) {
          MCE_DEBUG("from.." << (*from_iter1));
        }
        */
        for (StrSeq::const_iterator to_iter=to_seq.begin(); to_iter!=to_seq.end(); ++to_iter) {
          for (StrSeq::const_iterator from_iter=from_seq.begin(); from_iter!=from_seq.end(); ++from_iter) {
            if ((*to_iter) == (*from_iter)) {
              ret = 0;
              break;
            }
          }
          if (!ret)
            break;
        }
      }
    }
    xml_node res_item = queryNode.append_child();
    res_item.set_name("item");
	  res_item.append_attribute("from") = req_item.attribute("from").value().c_str();
	  //res_item.append_attribute("from") = req_item.attribute("from").value();
	  res_item.append_attribute("to") = req_item.attribute("to").value().c_str();
	  //res_item.append_attribute("to") = req_item.attribute("to").value();
    res_item.append_child(node_pcdata).set_value(boost::lexical_cast<string>(ret).c_str());      
  }
  MCE_DEBUG("IqGetTask::_getPermissionToAddFriends   reply:" << reply.xml());
  MessagePtr msg = new Message;
	msg -> from = _jid;
	msg -> to = _jid;
	msg -> type = IQ_RESULT;
	msg -> msg = reply.xml();
	try{
  	TalkProxyAdapter::instance().deliver(msg);
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::_getPermissionToAddFriends --->TalkProxyAdapter::deliver-->err" << e);
	}
  
}

void IqGetTask::_getBuddyInfo(xml_document& reply) {
	StatFunc statF("IqGetTask::getBuddyInfo");
	xml_node queryNode = reply.child("iq").append_child();
	queryNode.set_name("query");
	queryNode.append_attribute("xmlns") = "http://talk.renren.com/getbuddyinfo";
	for (xml_node get_item = _doc->child("iq").child("query").first_child(); get_item; get_item = get_item.next_sibling()) {
    if (strcmp(get_item.name(), "item") == 0) {
      xml_node result_item = queryNode.append_child();
      result_item.set_name("item");
	    result_item.append_attribute("to") = get_item.attribute("to").value().c_str();
	    //result_item.append_attribute("to") = get_item.attribute("to").value();
      int to = boost::lexical_cast<int>(get_item.attribute("to").value());
      NetworkSeq networks = UserNetworkAdapter::instance().getNetworkSeq(to);
      if (networks.size() > 0) {
        xml_node network = result_item.append_child();
        network.set_name("networks");
        network.append_child(node_pcdata).set_value((networks.at(networks.size()-1)->networkName).c_str());
      } 
    }
  }
  MCE_DEBUG("IqGetTask::_getBuddyInfo reply:" <<reply.xml());
  MessagePtr msg = new Message;
	msg -> from = _jid;
	msg -> to = _jid;
	msg -> type = IQ_RESULT;
	msg -> msg = reply.xml();
	try{
  	TalkProxyAdapter::instance().deliver(msg);
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::_getBuddyInfo --->TalkProxyAdapter::deliver-->err" << e);
	}

}

void IqGetTask::_getStrangerInfo(xml_document& reply) {
	StatFunc statF("IqGetTask::getStrangerInfo");
  MyUtil::Str2IntSeqMap countpairs;
  MyUtil::Int2IntMap idpairs;        
  int from , to;
	for (xml_node item = _doc->child("iq").child("query").first_child(); item; item = item.next_sibling()) {
    if (strcmp(item.name(), "item") == 0) {
      from = boost::lexical_cast<int>(item.attribute("from").value());
      to = boost::lexical_cast<int>(item.attribute("to").value());
      idpairs[to] = from;
    }
  }
  try {
    countpairs = BuddyFriendsCacheAdapter::instance().getCommonFriendsBatch(idpairs);
  } catch (Ice::Exception &e) {
    MCE_WARN("BuddyList::load -> get buddies error" << e);  
  }

  ostringstream os;
	xml_node queryNode = reply.child("iq").append_child();
	queryNode.set_name("query");
	queryNode.append_attribute("xmlns") = "http://talk.renren.com/getstrangerinfo";

  for (Int2IntMap::iterator iter=idpairs.begin(); iter!=idpairs.end(); ++iter) {
    os << iter->first << iter->second;

    xml_node send_item = queryNode.append_child();
    send_item.set_name("item");
	  send_item.append_attribute("from") = iter->second;
	  send_item.append_attribute("to") = iter->first;

    xml_node commonfriendscount = send_item.append_child();
    commonfriendscount.set_name("count");
    string count = "0";
    Str2IntSeqMap::iterator str_iter = countpairs.find(os.str()); 
    if (str_iter != countpairs.end()) {
      count = boost::lexical_cast<string>((str_iter->second).size());
    }
    commonfriendscount.append_child(node_pcdata).set_value(count.c_str());      
    os.str("");
  }
  MCE_DEBUG("IqGetTask::_getStrangerInfo  reply:" << reply.xml());
  MessagePtr msg = new Message;
	msg -> from = _jid;
	msg -> to = _jid;
	msg -> type = IQ_RESULT;
	msg -> msg = reply.xml();
	try{
  	TalkProxyAdapter::instance().deliver(msg);
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::_getStrangerInfo --->TalkProxyAdapter::deliver-->err" << e);
	}
}

void IqGetTask::_default(){
	xml_node iqNode = _doc->child("iq");
	iqNode.append_attribute("type") = "error";
	if(iqNode.attribute("from")){
		iqNode.remove_attribute("from");
	}
	iqNode.append_attribute("to") = jidToString(_jid).c_str();

	xml_node errNode = iqNode.append_child();
	errNode.set_name("error");
	errNode.append_attribute("code") = "503";
	errNode.append_attribute("type") = "cancel";

	xml_node unavailableNode = errNode.append_child();
	unavailableNode.set_name("service-unavailable");
	unavailableNode.append_attribute("xmlns") = "urn:ietf:params:xml:ns:xmpp-stanzas";
	try{
		TalkProxyAdapter::instance().express(_jid, _doc->xml());
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::_default-->TalkProxyAdapter::express-->" << e);
	}

}

void IqGetTask::getTransferHandle(xml_document& reply) {
	MCE_DEBUG("IqGetTask::getTransferHandle");
	if (!reply.child("iq").attribute("to")) {
		return;
	}

	JidPtr to = stringToJid(reply.child("iq").attribute("to").value());
	reply.child("iq").append_attribute("from") = jidToString(_jid).c_str();

	reply.child("iq").remove_attribute("to");

	MCE_DEBUG("IqGetTask::getTransferHandle --> to:"<< jidToString(to)
			<< ", msg:"<<reply.xml());
	try{
	TalkProxyAdapter::instance().express(to, reply.xml());
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::getTransferHandle-->TalkProxyAdapter::express-->" << e);
		}
}

void IqGetTask::_discoverService(xml_document& reply) {

	xml_node iqNode = reply.child("iq");
	//	iqNode.append_attribute("to") = jidToString(_jid).c_str();
	//	xml_node cn = iqNode.append_child();
	//	cn.set_name("query");
	//	cn.append_attribute("xmlns").set_value("http://jabber.org/protocol/disco#info");

	string domain = iqNode.attribute("from").value();

	ostringstream msg;
	if (domain == "talk.xiaonei.com" || domain == "talk.renren.com") {
		msg << "<iq from='"<< domain <<"' to='" << jidToString(_jid)
				<< "' id='"<< _doc->child("iq").attribute("id").value() <<"' type='result'>"
				<< "<query xmlns='http://jabber.org/protocol/disco#info'>"
				<< "</query>" << "</iq>";
	} else {
		msg << "<iq from='" << domain << "' to='" << jidToString(_jid)
				<< "' id='" << _doc->child("iq").attribute("id").value() << "' type='result'>"
				<< "<query xmlns='http://jabber.org/protocol/disco#info'>"
				<< "<identity category='conference' type='text' name='Chatrooms'/>" //display room list in another window
				<< "<feature var='http://jabber.org/protocol/muc'/>" //display join
				<< "</query>" << "</iq>";
	}
  try{
	TalkProxyAdapter::instance().express(_jid, msg.str());
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::_discoverService-->TalkProxyAdapter::express-->err" << e);
		}
}

void IqGetTask::_getItems(xml_document& reply) {

	xml_node iqNode = reply.child("iq");
	xml_node cn = iqNode.append_child();
	cn.set_name("query");
	cn.append_attribute("xmlns").set_value("http://jabber.org/protocol/disco#items");

	string domain = iqNode.attribute("from").value();
	if (domain == "talk.xiaonei.com" || domain == "talk.renren.com") {

		xml_node ccn = cn.append_child();
		ccn.set_name("item");
		ccn.append_attribute("jid").set_value("conference.talk.xiaonei.com");
		xml_node ccn2 = cn.append_child();
		ccn2.set_name("item");
		ccn2.append_attribute("jid").set_value("group.talk.xiaonei.com");

	} 
	else if (domain.find("group") == 0) {
	} else if (domain.find("conference") == 0) {
	}
	try{
	TalkProxyAdapter::instance().express(_jid, reply.xml());
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::_getItems-->TalkProxyAdapter::express-->err" << e);
		}
}

void IqGetTask::_getWebTicket(xml_document& reply) {
	StatFunc statF("IqGetTask::getWebTicket");

	xml_node queryNode = reply.child("iq").append_child();
	queryNode.set_name("query");
	queryNode.append_attribute("xmlns") = "http://talk.renren.com/webticket";
  xml_node tNode = queryNode.append_child();
  tNode.set_name("t");
  tNode.append_child(node_pcdata).set_value(passport::TicketAdapter::instance().createTicket(_jid->userId,"").c_str());

	try{
  	TalkProxyAdapter::instance().express(_jid, reply.xml());
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::_getWebTicket --->TalkProxyAdapter::express-->err" << e);
	}
}

void IqGetTask::getPrivacyHandle(xml_document& reply) {
	StatFunc statF("IqGetTask::getPrivacyHandle");

	xml_node qNode = _doc->child("iq").child("query");

	vector<xml_node> lNodes;
	for (xml_node n = qNode.child("list"); n; n = n.next_sibling("list")) {
		lNodes.push_back(n);
	}

	if (lNodes.empty()) {

		xml_node queryNode = reply.child("iq").append_child();
		queryNode.set_name("query");
		queryNode.append_attribute("xmlns") = "jabber:iq:privacy";

		xml_node activeNode = queryNode.append_child();
		activeNode.set_name("active");
		activeNode.append_attribute("name") = "blocked";

		xml_node defaultNode = queryNode.append_child();
		defaultNode.set_name("default");
		defaultNode.append_attribute("name") = "blocked";

		xml_node listNode = queryNode.append_child();
		listNode.set_name("list");
		listNode.append_attribute("name") = "blocked";

	} else if (lNodes.size()==1) {

		if (strcmp(lNodes.at(0).attribute("name").value().c_str(), "blocked")) {
		//if (strcmp(lNodes.at(0).attribute("name").value(), "blocked")) {
			xml_node iqNode = reply.child("iq");
			iqNode.append_attribute("type") = "error";

			xml_node errorNode = iqNode.append_child();
			errorNode.set_name("error");
			errorNode.append_attribute("type") = "cancel";

			xml_node notFindNode = errorNode.append_child();
			notFindNode.set_name("item-not-found");
			notFindNode.append_attribute("xmlns")
					= "urn:ietf:params:xml:ns:xmpp-stanzas";
		} else {
			MyUtil::IntSeq blockIds;
			try {
				blockIds = BuddyCoreAdapter::instance().getBlockSeq(_jid->userId, 0, -1,1000);
			} catch (Ice::Exception& e) {
				MCE_WARN("IqGetTask::getPrivacyHandle-->BuddyCoreAdapter::getBlockSeq-->userId:"<<_jid->userId<<" err:"<<e);
			} catch (...) {
				MCE_WARN("IqGetTask::getPrivacyHandle -> BuddyCoreAdapter::getBlockSeq err userId:"<<_jid->userId);
			}

			xml_node queryNode = reply.child("iq").append_child();
			queryNode.set_name("query");
			queryNode.append_attribute("xmlns") = "jabber:iq:privacy";

			xml_node listNode = queryNode.append_child();
			listNode.set_name("list");
			listNode.append_attribute("name") = "blocked";
			for (size_t i = 0; i < blockIds.size(); ++i) {
				xml_node itemNode = listNode.append_child();
				itemNode.set_name("item");
				itemNode.append_attribute("type") = "jid";
				itemNode.append_attribute("action") = "deny";
				itemNode.append_attribute("order") = blockIds.at(i);
				ostringstream jid;
				jid << blockIds.at(i)<<"@talk.xiaonei.com";
				itemNode.append_attribute("value") = jid.str().c_str();
			}
			MyUtil::StrSeq sysBlockSeq;
			try{
			sysBlockSeq = TalkRightAdapter::instance().getSystemMessageBlock(_jid->userId);
			}catch(Ice::Exception& e){
				MCE_WARN("IqGetTask::getPrivacyHandle-->TalkRightAdapter::getSystemMessageBlock-->err" << e);
				}
			for (size_t i = 0; i < sysBlockSeq.size(); ++i) {
				xml_node itemNode = listNode.append_child();
				itemNode.set_name("item");
				itemNode.append_attribute("type") = "jid";
				itemNode.append_attribute("action") = "deny";
				itemNode.append_attribute("order") = (int)time(NULL);
				ostringstream jid;
				jid << sysBlockSeq.at(i) <<"@news.talk.xiaonei.com";
				itemNode.append_attribute("value") = jid.str().c_str();
			}
			bool isHidden;
			try{
				isHidden = TalkRightAdapter::instance().isHiddenUser(_jid->userId);
			}catch(Ice::Exception& e){
				MCE_WARN("IqGetTask::getPrivacyHandle-->TalkRightAdapter::isHiddenUser-->err" << e);
				}
			if (isHidden) {
				xml_node itemNode = listNode.append_child();
				itemNode.set_name("item");
				itemNode.append_attribute("action") = "deny";
				itemNode.append_attribute("order") = (int)time(NULL);

				xml_node presenceOutNode = itemNode.append_child();
				presenceOutNode.set_name("presence-out");
			}
		}

	} else if (lNodes.size()> 1) {

		xml_node iqNode = reply.child("iq");
		iqNode.append_attribute("type") = "error";

		xml_node errorNode = iqNode.append_child();
		errorNode.set_name("error");
		errorNode.append_attribute("type") = "modify";

		xml_node badRequest = errorNode.append_child();
		badRequest.set_name("bad-request");
		badRequest.append_attribute("xmlns")
				= "urn:ietf:params:xml:ns:xmpp-stanzas";

	}

	MessagePtr msg = new Message();
	msg -> from = _jid;
	msg -> to = _jid;
	msg -> type = IQ_RESULT;
	msg -> msg = reply.xml();
	//TalkDeliverAdapter::instance().deliver(msg);
	try{
	TalkProxyAdapter::instance().deliver(msg);
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::getPrivacyHandle-->TalkProxyAdapter::deliver-->" << e);
		}
}

void IqGetTask::getPrivateHandle(xml_document& reply) {
	StatFunc statF("IqGetTask::getPrivateHandle");

	xml_node iqNode = _doc->child("iq");
	iqNode.append_attribute("type") = "result";
	iqNode.append_attribute("from") = jidToString(_jid, false).c_str();
	//iqNode.append_attribute("to") = jidToString(_jid).c_str();

	MessagePtr msg = new Message();
	msg -> from = _jid;
	msg -> to = _jid;
	msg -> type = IQ_RESULT;
	msg -> msg = _doc->xml();
	try{
	TalkProxyAdapter::instance().deliver(msg);
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::getPrivateHandle-->TalkProxyAdapter::deliver-->" << e);
		}
}

void IqGetTask::getfocuslist(xml_document& reply) {
  TimeStat ts;
  StatFunc statF("IqGetTask::getfocuslist");
  xml_node queryNode = reply.child("iq").append_child();
  queryNode.set_name("query");
  queryNode.append_attribute("xmlns") = "http://talk.xiaonei.com/focuslist";
  MyUtil::IntSeq ans;
  int res=0;
  try {
    ans = FeedFocusAdapter::instance().GetFocuses(_jid->userId);
    res=1;
  } catch (Ice::Exception& e) {
    MCE_WARN("IqGetTask::getfocuslist-->FeedFocusAdapter::GetFocuses error!"<<e.what());
    res=0;
  } catch(...) {
    MCE_WARN("IqGetTask::getfocuslist-->FeedFocusAdapter::GetFocuses error!");
    res=0;
  }
  if (res) {
    for (int i=0;i<ans.size();++i) {
      ostringstream os;
      os << ans[i];
      xml_node itemNode = queryNode.append_child();
      itemNode.set_name("uid");
      itemNode.append_child(node_pcdata).set_value(os.str().c_str());
    }  
  }
  reply.child("iq").append_attribute("res") = res>0?"1":"0"; 
  MCE_DEBUG(reply.xml());
  try {
    TalkProxyAdapter::instance().express(_jid, reply.xml());
  } catch(Ice::Exception& e) {
    MCE_WARN("IqGetTask::getfocuslist-->TalkProxyAdapter::express error!"<<e.what());
  }
  return ;

}


void IqGetTask::getfaceubb(xml_document& reply){ 
  TimeStat ts;
  StatFunc statF("IqGetTask::getfaceubb");
  xml_node queryNode = reply.child("iq").append_child();
  queryNode.set_name("query");
  queryNode.append_attribute("xmlns") = "http://talk.renren.com/UpdataOnlineFace";
//  MCE_DEBUG(reply.xml());
//  try {
//    TalkProxyAdapter::instance().express(_jid, reply.xml());
//  } catch (...) {
//    MCE_WARN("IqGetTask::getfaceubb-->TalkProxyAdapter::express-->error!");
//  }
//  return ;
//  Xoa2UbbListResult result;
//  ClientPool<UbbServiceClient> pool("status_ubb.ugc.xoa.renren.com", 200);
//  UbbServiceClient *client=NULL;
//  client = pool.Alloc(0);
//  if(client) {
//    bool success = true;
//    try {
//      client->getDoingUbbsBytypeNE(result,0);
//    } catch (...) {
//      MCE_WARN("IqGetTask::getfaceubb-->getDoingUbbsBytypeNE-->error!");
//      success = false;
//    }
//    pool.Release(client, success);
//    if(!success)return ;
//  }
//
  //MCE_INFO("flag:"<<GetUbb::instance().flag);
  if (!GetUbb::instance().judge()) {
    MCE_DEBUG("reply:"<<reply.xml()<<" flag:"<<GetUbb::instance().flag);
    MCE_WARN("IqGetTask::getfaceubb-->getDoingUbbsBytypeNE-->error!");
    try {
      TalkProxyAdapter::instance().express(_jid, reply.xml());
    } catch (...) {
      MCE_WARN("IqGetTask::getfaceubb-->TalkProxyAdapter::express-->error!");
    }
    return ;
  }
  if(GetUbb::instance().has_errorInfo()) {
    GetUbb::instance().getData(queryNode);
    MCE_DEBUG(reply.xml());
    try {
      TalkProxyAdapter::instance().express(_jid, reply.xml()); 
    } catch (...) {
      MCE_WARN("IqGetTask::getfaceubb-->TalkProxyAdapter::express-->error!");
    }
    return ;
  } else {
    MCE_DEBUG(reply.xml());
    MCE_WARN("IqGetTask::getfaceubb-->getDoingUbbsBytypeNE-->errorInfo is not null!");
    try {
      TalkProxyAdapter::instance().express(_jid, reply.xml());
    } catch (...) {
      MCE_WARN("IqGetTask::getfaceubb-->TalkProxyAdapter::express-->error!");
    }
    return ;
  }
}

void IqGetTask::getRosterHandle(xml_document& reply) {
	TimeStat ts;
	StatFunc statF("IqGetTask::getRosterHandle");
	xml_node queryNode = reply.child("iq").append_child();
	queryNode.set_name("query");
	queryNode.append_attribute("xmlns") = "jabber:iq:roster";

	MyUtil::IntSeq buddyIds;
	try {
		buddyIds = BuddyByIdCacheAdapter::instance().getFriendListAsc(_jid->userId, 2000);
	} catch(Ice::Exception& e) {
		MCE_WARN("IqGetTask::getRosterHandle-->BuddyByIdCacheAdapter::getFriendListAsc-->"<<_jid->userId<<":"<<e);
	} catch(...) {
		MCE_WARN("call IqGetTask::getRosterHandle -> getFrinedSeq err");
	}
	MCE_DEBUG("call IqGetTask::getRosterHandle -> finish getFriendSeq  userId="
			<<_jid->userId<< "  buddy.size="<<buddyIds.size());

	//MCE_INFO("IqGetTask::getRosterHandle --> usetime BuddyByIdCache --> " << ts.getTime());
	ts.reset();
	if (buddyIds.empty()) {
		//		msg -> msg = reply.xml();
		//		TalkMessenger::instance().deliver(msg);
		//TalkProxyAdapter::instance().express(_jid, reply.xml());
		return;
	}
  TalkUserMap usermaps;
  try{
		usermaps = TalkCacheClient::instance().GetUserBySeqWithLoad(_jid->userId, buddyIds);
		//for(TalkUserMap::iterator uit = usermaps.begin(); uit != usermaps.end(); ++uit){
		//	if()
		//}
		/*
		if(usermaps.size() != buddyIds.size()){
			MCE_INFO("IqGetTask::getRosterHandle --> may be warn get from TalkCacheClient size is not equal actor = " << _jid->userId << " ids.size = " << buddyIds.size() << " map.size = " << usermaps.size());
		}else{
			MCE_INFO("IqGetTask::getRosterHandle --> success get from TalkCacheClient size is equal actor = " << _jid->userId << " ids.size = " << buddyIds.size() << " map.size = " << usermaps.size());
    }
		*/
	}catch(Ice::Exception& e){
    MCE_WARN("IqGetTask::getRosterHandle-->TalkCacheClient::GetUserBySeqWithLoad-->" << e);
  }

	//MCE_INFO("IqGetTask::getRosterHandle --> usetime TalkCacheClient --> " << ts.getTime());
	ts.reset();
	BuddiedGroupsMap gMap;
	try {
		gMap = BuddyGroupAdapter::instance().getBuddiedGroupsMap(_jid->userId,500);
	} catch(Ice::Exception & e) {
		MCE_WARN("IqGetTask::getRosterHandle-->BuddyGroupAdapter::getBuddiedGroupsMap-->"<<e);
	}

	//MCE_INFO("IqGetTask::getRosterHandle --> usetime BuddyGroup --> " << ts.getTime());
	ts.reset();
	MyUtil::IntSeq ids;

	for (size_t i = 0; i < buddyIds.size(); ++i) {
    TalkUserMap::iterator it = usermaps.find(buddyIds.at(i));
		if (it != usermaps.end()) {
			//UserPtr u = UserPtr::dynamicCast(it->second);
      TalkUserPtr u = it->second;
			xml_node itemNode = queryNode.append_child();
			itemNode.set_name("item");
			ostringstream jid;
			jid << u->id << "@" << "talk.xiaonei.com";
			itemNode.append_attribute("jid") = jid.str().c_str();
			Utf8Formatting(const_cast<char*>(u->name.c_str()));
			itemNode.append_attribute("name") = u->name.c_str();
			itemNode.append_attribute("subscription") = "both";

			BuddiedGroupsMap::iterator bit = gMap.find(buddyIds.at(i));
			if (bit != gMap.end()) {
				for (size_t j = 0; j< bit->second.size(); ++j) {
					xml_node groupNode = itemNode.append_child();
					groupNode.set_name("group");
					groupNode.append_child(node_pcdata).set_value(bit->second.at(j).c_str());
				}
			}
		} else {
			ids.push_back(buddyIds.at(i));
		}
	}
	MCE_DEBUG("IqGetTask::getRosterHandle -> finish building item with buddyIds,  userId="
			<<_jid->userId<< "  buddy.size="<<buddyIds.size()
			<< ", missing.size=" << ids.size());
	//MCE_INFO("IqGetTask::getRosterHandle --> " << _jid->userId << " buddysize=" << buddyIds.size() << "miss=" << ids.size());
	//select from db
	if (!ids.empty()) {
		try {
			throw "";
		} catch (...) {
			for(size_t i = 0; i < ids.size(); ++i) {
				xml_node itemNode = queryNode.append_child();
				itemNode.set_name("item");
				ostringstream jid;
				jid << ids.at(i) << "@" << "talk.xiaonei.com";
				itemNode.append_attribute("jid") = jid.str().c_str();
				itemNode.append_attribute("name") = ids.at(i);
				itemNode.append_attribute("subscription") = "both";

				BuddiedGroupsMap::iterator bit = gMap.find(ids.at(i));
				if (bit != gMap.end()) {
					for (size_t j = 0; j< bit->second.size(); ++j) {
						xml_node groupNode = itemNode.append_child();
						groupNode.set_name("group");
						groupNode.append_child(node_pcdata).set_value(bit->second.at(j).c_str());
					}
				}
			}
		}
	}
	MCE_DEBUG("IqGetTask::getRosterHandle -> finish all item  userId="
			<<_jid->userId<< "  buddy.size="<<buddyIds.size()
			<< ", missing.size=" << ids.size());
	//msg -> msg = reply.xml();
	try{
		TalkProxyAdapter::instance().express(_jid, reply.xml());
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::getRosterHandle-->TalkProxyAdapter::express-->err" << e);
	}

	IqHandlerI::instance().GetBuddyApply(_jid);

  try{
  	IMWindowAdapter::instance().IMWindowReminder(_jid);
  }catch(Ice::Exception& e){
  	MCE_WARN("IqGetTask::getRosterHandle-->IMWindow --> userid:" << _jid->userId << " error:" << e);
  }
}

void IqGetTask::getVcardHandle(xml_document& reply) {
	TimeStat talkcacheclient_ts, usercache_ts;
	float vtalkcacheclient = 0.0, vusercache = 0.0;
	MessagePtr msg = new Message();
	msg -> from = _jid;
	msg -> to = _jid;
	msg -> type = IQ_RESULT;

	xml_attribute toAttr = _doc->child("iq").attribute("to");
	if (toAttr.empty()) {
		MCE_WARN("IqGetTask::getVcardHandle -> wrong, no to attribute: " << _doc->xml());
		msg -> msg = reply.xml();
		try{
			TalkProxyAdapter::instance().deliver(msg);
		}catch(Ice::Exception& e){
			MCE_WARN("IqGetTask::getVcardHandle-->TalkProxyAdapter::deliver-->" << e);
		}
		return;
	}

	JidPtr tojid = stringToJid(toAttr.value());
  
  TalkUserPtr ui;	
	try {
		talkcacheclient_ts.reset();
		ui = TalkCacheClient::instance().GetUserByIdWithLoad(tojid->userId);
		vtalkcacheclient = talkcacheclient_ts.getTime();
	} catch(Ice::Exception& e) {
		MCE_WARN("IqGetTask::getVcardHandle-->TalkCacheClient::GetUserByIdWithLoad-->"<<tojid->userId<< " err:"<<e);
	}
	if (!ui) {
		MCE_WARN("IqGetTask::getVcardHandle --> TalkCacheClient  UserNotExist:" << tojid->userId);
		msg -> msg = reply.xml();
		try{
			TalkProxyAdapter::instance().deliver(msg);
		}catch(Ice::Exception& e){
			MCE_WARN("IqGetTask::getVcardHandle-->TalkProxyAdapter::deliver-->err" << e);
		}
		return;
	}
  else{
		//MCE_INFO("IqGetTask::getVcardHandle _jid = " << jidToString(_jid) << " tojid = " << tojid << " id:" << ui->id << " name:" << ui->name << " statusOriginal:" << ui->statusOriginal << " statusShifted:" << ui->statusShifted << " headurl:" << ui->headurl << " tinyurl:" << ui->tinyurl);
		if(tojid->userId != ui->id){
			MCE_WARN("IqGetTask::getVcardHandle --> memcerror userid = " << tojid->userId << " error_id = " << ui->id << " after call loader okid = " << ui->id);
			try{
				ui = TalkCacheLoaderAdapter::instance().LoadById(tojid->userId);
			}catch(Ice::Exception& e){
				MCE_WARN("IqGetTask::getVcardHandle-->TalkCacheLoaderAdapter::LoadById-->" << tojid->userId << ":" << e);
			}
		}
  }

	int userId = tojid->userId;
	IntSeq ids;
	ids.push_back(userId);
	map<int, UserCachePtr> userVips;
	try{
		usercache_ts.reset();
		userVips = UserCacheAdapter::instance().GetUserCache(ids);
		vusercache = usercache_ts.getTime();	
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::getVcardHandle-->UserCacheAdapter::GetUserCache-->" << userId << ":" << e);
	}

	int vip = 0;
	UserCachePtr vipInfo;
	map<int, UserCachePtr>::iterator it = userVips.find(userId);
	if(it != userVips.end()){
		vipInfo = it->second;
		vip = vipInfo->level();
		if(vip == 0){
			vip = 1;
		}
		if(vipInfo->IsVipMember()){
		}else if(vipInfo->IsEverVipMember()){
			vip = -vip;
		}else{
			vip = 0;
		}
	}
	string strVip;
	try{
		strVip = boost::lexical_cast<string>(vip);
	}catch(...){
		MCE_WARN("PresenceHandlerI::presence --> lexical cast err");
	}

	xml_node vCardNode = reply.child("iq").append_child();
	vCardNode.set_name("vCard");
	vCardNode.append_attribute("xmlns") = "vcard-temp";
	vCardNode.append_attribute("xmlns:xn") = "xn:talk:vcard";

	xml_node photoNode = vCardNode.append_child();
	photoNode.set_name("PHOTO");
	xml_node extvalNode = photoNode.append_child();
	extvalNode.set_name("EXTVAL");
	//extvalNode.append_child(node_pcdata).set_value(buildHeadUrl(ui->headurl).c_str());
	string headurl = PhotoUrlHelper::instance().GetFullUrl(ui->headurl);
	if(headurl.empty()){
		MCE_WARN("IqGetTask::getVcardHandle--> get headurl from PhotoUrlHelper is empty ui->headurl=" << ui->headurl);
	}
	extvalNode.append_child(node_pcdata).set_value(headurl.c_str());
	//extvalNode.append_child(node_pcdata).set_value("");
	//-----------------
	//string newurl = PhotoUrlHelper::instance().GetFullUrl(ui->headurl);
	//MCE_INFO("VCARD ----------------> ui->headurl=" << ui->headurl << "  newurl" << newurl);
	//-----------------
	xml_node xnstatNode = vCardNode.append_child();
	xnstatNode.set_name("xn:STATUS");
	xnstatNode.append_child(node_pcdata).set_value(ui->statusOriginal.c_str());

	xml_node fnNode = vCardNode.append_child();
	fnNode.set_name("FN");
	Utf8Formatting(const_cast<char*>(ui->name.c_str()));
	fnNode.append_child(node_pcdata).set_value(ui->name.c_str());
	
	//string str = reply.xml();
	//size_t pos = str.rfind("</iq>");
	//string str1 = str.substr(0, pos);
	//string str2 = str.substr(pos);
	//str = str1 + ui->statusShifted + str2;
	
	xml_node embHtmlNode = reply.child("iq").append_child();
	embHtmlNode.set_name("richstatus");
	embHtmlNode.append_child(node_pcdata).set_value(ui->statusShifted.c_str());
	
	if(!strVip.empty()){
		xml_node vipNode = reply.child("iq").append_child();
		vipNode.set_name("vip");
		vipNode.append_child(node_pcdata).set_value(strVip.c_str());
	}

	msg -> msg = reply.xml();
	try{
		TalkProxyAdapter::instance().deliver(msg);
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::getVcardHandle-->TalkProxyAdapter::deliver-->" << e);
	}
	MONITOR("SignForTimeuse::IqGetTask::getVcardHandle-->TalkCacheClient::GetUserByIdWithLoad-->" << vtalkcacheclient << " UserCache::GetUserCache-->" << vusercache);
}
void IqGetTask::getPageReply() {
  return;
  /*
  string tmp ;
  int stype,actor,pageId,iqid,pagetype; 
  Ice::Long resource;
  try {  
    tmp= _doc->child("iq").child("query").child("feedreply").attribute("stype").value();
    stype = boost::lexical_cast<int>(tmp);
    tmp = _doc->child("iq").child("query").child("feedreply").attribute("resource").value();
    resource = boost::lexical_cast<Ice::Long>(tmp);
    tmp = _doc->child("iq").child("query").child("feedreply").attribute("actor").value();
    actor = boost::lexical_cast<int>(tmp);
    tmp = _doc->child("iq").child("query").child("feedreply").attribute("page").value();
    pageId = boost::lexical_cast<int>(tmp);
    tmp = _doc->child("iq").attribute("id").value();
    iqid = boost::lexical_cast<int>(tmp);
    tmp = _doc->child("iq").child("query").child("feedreply").attribute("pagetype").value();
    pagetype = boost::lexical_cast<int>(tmp);
  } catch (...) {
    MCE_WARN("IqGetTask::getPageReply-->cast parameter error!");
    return ;
  }
  try {
    WTransitForIqAdapter::instance().loadofflinePageReply(_jid,pageId,stype,actor,resource,iqid,pagetype);
  } catch (Ice::Exception &e) {
    MCE_WARN("IqGetTask::getPageReply-->WTransitForIq.loadofflinePageReply error:"<<e.what()
              <<"page:"<<pageId<<"stype:"<<stype<<"actor:"<<actor<<"resource:"<<resource);
  }
  */
}
void IqGetTask::_getXiaozuReply(){
  MCE_DEBUG("_getXiaozuReply------------------"<< _jid->userId);
  Ice::Long xiaozuid = 0;
  Ice::Long threadid = 0; //
  int page = 0;
  int type = 0;
  string iqid = "";
  try{
    string tmp = _doc->child("iq").child("query").child("feedreply").attribute("stype").value();
    type = boost::lexical_cast<int>(tmp);
    tmp = _doc->child("iq").child("query").child("feedreply").attribute("resource").value();
    threadid = boost::lexical_cast<Ice::Long>(tmp);
    tmp = _doc->child("iq").child("query").child("feedreply").attribute("actor").value();
    xiaozuid = boost::lexical_cast<Ice::Long>(tmp);
    tmp = _doc->child("iq").child("query").child("feedreply").attribute("page").value();
    page = boost::lexical_cast<int>(tmp);
    tmp = _doc->child("iq").attribute("id").value();
    iqid = tmp;
  }catch(...){
    MCE_WARN("IqGetTask::_getXiaozuReply---> cast parameter err");
    return ;
  }
  try{
    MCE_INFO("IqGetTask::_getXiaozuReply--> userid:" << _jid->userId << " xiaozuid:" << xiaozuid << " type:" << type << " threadid:" << threadid);
    WTransitForIqAdapter::instance().loadXiaozuOfflineFeedReply(_jid, type, xiaozuid, threadid, page, iqid);
  }catch(Ice::Exception& e){
    MCE_WARN("IqGetTask::_getXiaozuReply-->WTransitForIqAdapter::loadXiaozuReply-->id:" << _jid->userId << " xiaozuid:" << xiaozuid << " threadid:" << threadid << " err:" << e);
  }
}
void IqGetTask::_getXiaozuContent(){
        MCE_DEBUG("1111111111------------------");
        Ice::Long xiaozuid = 0;//小组id
        Ice::Long threadid = 0;//帖子id
        string iqid = "";
        int type = 0;
        try{
                string tmp = _doc->child("iq").child("query").child("xiaozucontent").attribute("stype").value();
                type = boost::lexical_cast<int>(tmp);
                tmp = _doc->child("iq").child("query").child("xiaozucontent").attribute("resource").value();
                threadid = boost::lexical_cast<Ice::Long>(tmp);
                tmp = _doc->child("iq").child("query").child("xiaozucontent").attribute("actor").value();
                xiaozuid = boost::lexical_cast<Ice::Long>(tmp);
                tmp = _doc->child("iq").attribute("id").value();
                iqid = tmp;
        }catch(...){
                MCE_WARN("IqGetTask::_getXiaozuContent ---> cast parameter err");
                return ;
        }
        try{
                MCE_INFO("IqGetTask::_getXiaozuContenty --> userid:" << _jid->userId << " xiaozuid:" << xiaozuid << " type:" << type << " threadid:" << threadid);
                WTransitForIqAdapter::instance().loadXiaozuContent(_jid, xiaozuid, threadid, iqid);
        }catch(Ice::Exception& e){
                MCE_WARN("IqGetTask::_getXiaozuContent-->WTransitForIqAdapter::loadXiaozuContent-->id:" << _jid->userId << " xiaozuid:" << xiaozuid << " threadid:" << threadid << " err:" << e);
        }

}

void IqGetTask::_getFeedReply() {
	int owner = 0;
	int type = 0;
	Ice::Long source = 0;
	try{
		string tmp = _doc->child("iq").child("query").child("feedreply").attribute("stype").value();
		type = boost::lexical_cast<int>(tmp);
		tmp = _doc->child("iq").child("query").child("feedreply").attribute("actor").value();
		owner= boost::lexical_cast<int>(tmp);
		tmp = _doc->child("iq").child("query").child("feedreply").attribute("resource").value();
		source = boost::lexical_cast<Ice::Long>(tmp);
	}catch(...){
		MCE_WARN("IqGetTask::_getFeedReply --> cast parameter err");
		return;
	}
	try{
		MCE_INFO("IqGetTask::_getFeedReply --> userid:" << _jid->userId << " owner:" << owner << " type:" << type << " source:" << source);
    if (!_doc)
      MCE_INFO("IqGetTask::_getFeedReply -->  _doc null");
		WTransitForIqAdapter::instance().loadOfflineFeedReply(_jid, type, owner, source, _doc->child("iq").attribute("id").value());
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::_getFeedReply-->WTransitForIqAdapter::loadOfflineFeedReply-->id:" << _jid->userId << " e:" << e);
	}

	/*
	//---------------------------------
	try{
		WTransitAdapter::instance().loadOfflineFeedReply(_jid, type, owner, source, _doc->child("iq").attribute("id").value());
	}catch(Ice::Exception& e){
	}
	//---------------------------------
	*/	
	/*ostringstream os;
	os << "<iq type='result' id='" << _doc->child("iq").attribute("id") << "'>"
	<< "<query xmlns='" << _doc->child("iq").child("query").attribute("xmlns").value() << "'>";
	os << "</query></iq>";

	MessagePtr msg = new Message;
	msg->to = _jid;
	msg->from = _jid;
	msg->type = IQ_RESULT;
	msg->msg = os.str();
	try{
		MCE_INFO("IqGetTask::_getFeedReply --> deliver " << jidToString(_jid) << " msg:" << os.str());
		TalkProxyAdapter::instance().express(_jid, os.str());
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::_getFeedReply --> call deliver err, " << e);
	}*/
}

void IqGetTask::_getFeedReplyCount() {
	vector<Ice::Long> feedids; 
	string id = _doc->child("iq").attribute("id").value();
  string xmlns = _doc->child("iq").child("query").attribute("xmlns").value();
	xml_node node = _doc->child("iq").child("query").child("feedreply");
	string strIds = node.child("feedid").first_child().value();
	vector<string> splitVec;
	boost::split(splitVec, strIds, boost::is_any_of(","), boost::token_compress_on);
	for(int i=0; i<(int)splitVec.size(); i++){
		Ice::Long feedid = 0;
		try{
			feedid = boost::lexical_cast<Ice::Long>(splitVec.at(i));
		}catch(...){
			continue;
		}
		feedids.push_back(feedid);
	}
  try{
		MCE_INFO("IqGetTask::_getFeedReplyCount-->id:" << _jid->userId);
		WTransitForIqAdapter::instance().deliverFeedReplyCount(_jid, feedids, id, xmlns);
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::_getFeedReplyCount-->WTransitForIqAdapter::deliverFeedReplyCount-->id:" << _jid->userId << " e:" << e);
	}


	//---------------------------------
	try{
		WTransitAdapter::instance().deliverFeedReplyCount(_jid, feedids, id, xmlns);
	}catch(Ice::Exception& e){
	}
}

void IqGetTask::getXFeedHandle(xml_document& reply) {

	MCE_DEBUG("IqGetTask::getXFeedHandle");
	xml_attribute iqIdAttr = _doc->child("iq").attribute("id");
	int iqId = time(NULL);
	if (iqIdAttr) {
		try {
			iqId = boost::lexical_cast<int>(iqIdAttr.value());
		} catch(...) {};
	}

	Ice::Long feedIndex = 0;
	Ice::Long replyIndex = 0;

	xml_attribute feedIndexAttr = _doc->child("iq").child("query").child("xfeed").attribute("begin");
	if (feedIndexAttr) {
		try {
			feedIndex = boost::lexical_cast<Ice::Long>(feedIndexAttr.value());
			MCE_DEBUG("IqGetTask::getXFeedHandle --> call WEventAdapter  jid:"<<jidToString(_jid)<< " begin:"<<feedIndex<<"   iqId:"<<iqId);

		} catch(Ice::Exception& e) {
			MCE_WARN("IqGetTask::getXFeedHandle --> call WEventAdapter err :"<<e);
		} catch(...) {
			MCE_WARN("IqGetTask::getXFeedHandle --> beginId is not a number");
		}
	}

	xml_attribute replyIndexAttr = _doc->child("iq").child("query").child("reply").attribute("begin");
	if (replyIndexAttr) {
		try {
			replyIndex = boost::lexical_cast<Ice::Long>(replyIndexAttr.value());
			MCE_DEBUG("IqGetTask::getXFeedHandle --> call WEventAdapter  jid:"<<jidToString(_jid)<< " begin:"<<replyIndexAttr<<"   iqId:"<<iqId);

		} catch(Ice::Exception& e) {
			MCE_WARN("IqGetTask::getXFeedHandle --> call WEventAdapter err :"<<e);
		} catch(...) {
			MCE_WARN("IqGetTask::getXFeedHandle --> beginId is not a number");
		}
	}
	//WEventAdapter::instance().loadOfflineXFeed(_jid, feedIndex, replyIndex, iqId);
	//if(232626782 != _jid->userId){
		try{
			MCE_INFO("IqGetTask::getXFeedHandle-->id:" << _jid->userId);
			WTransitForIqAdapter::instance().loadOfflineFeed(_jid, feedIndex, replyIndex, iqId);
		}catch(Ice::Exception& e){
			MCE_WARN("IqGetTask::getXFeedHandle-->WTransitForIqAdapter::loadOfflineFeed-->id:" << _jid->userId << " e:" << e);
		}
	//}


	//---------------------------------
	try{
		//WTransitAdapter::instance().CallFeedCacheAndContent(_jid, feedIndex, replyIndex, iqId);
	}catch(Ice::Exception& e){
		MCE_WARN("IqGetTask::getXFeedHandle-->WTransitAdapter::CallFeedCacheAndContent--> error :" << e );
	}
	//---------------------------------

}

//--------------------------------------

void IqSetTask::handle() {
  StatFunc statF("IqSetTask::handle");
  ostringstream os;
  os << "IqSetTask::";
  TimeStat ts;
  xml_document reply;
  xml_node iqNode = reply.append_child();
  iqNode.set_name("iq");
  iqNode.append_attribute("type").set_value("result");
  if (!_doc->child("iq").attribute("id").empty()) {
    iqNode.append_attribute("id") = _doc->child("iq").attribute("id").value().c_str();
    //iqNode.append_attribute("id") = _doc->child("iq").attribute("id").value();
  } else {
    iqNode.append_attribute("id") = (int)time(NULL);
  }
  string tag = _doc->child("iq").first_child().name();
  string xmlns = _doc->child("iq").first_child().attribute("xmlns").value();
  
  if (tag == "session") {
    setSessionHandle(reply);
    string version =  _doc->child("iq").first_child().attribute("src").value();
    int mark = 0;
    if(version == "mac2") {
      mark = 1;
    }

    try{
      RestLogicAdapter::instance().newgetSessionKey(_jid,mark);
      //RestLogicAdapter::instance().getSessionKey(_jid);
    }catch(Ice::Exception& e){
      MCE_WARN("IqSetTask::handle-->RestLogicAdapter::newgetSessionKey-->" << e);
    }
    os << "session";
  } else if (tag == "query") {
    if (xmlns == "jabber:iq:privacy") {
      setPrivacyHandle(reply);
      os << "privacy";
    } else if (xmlns == "jabber:iq:private") {
      setPrivateHandle(reply);
      os << "private";
    } else if (xmlns == "jabber:iq:roster") {
      setRosterHandle(reply);
      os << "roster";
    } else if(xmlns == "http://talk.renren.com/removenotify" 
        || xmlns == "http://talk.xiaonei.com/removenotify"){
      setNotifyHandle(reply);
      os << "setNotifyHandle";
    } else if(xmlns == "http://talk.xiaonei.com/xfeed/vernewfeed" 
        || xmlns == "http://talk.renren.com/xfeed/vernewfeed"){
      setVerNewFeedHandle(reply);
      os << "setNotifyHandle";
    } else if (xmlns == "http://talk.xiaonei.com/setfocuslist") {
      modifyfocuslist(reply);
      os << "modifyfocuslist";

    }
  } else {
    try{
      TalkProxyAdapter::instance().express(_jid, reply.xml());
    }catch(Ice::Exception& e){
      MCE_WARN("IqSetTask::handle-->TalkProxyAdapter::express-->" << e);
    }
  }
  FunStatManager::instance().Stat(os.str(), ts.getTime(), (ts.getTime() >= 250.0));
}
void IqSetTask::modifyfocuslist(xml_document& reply) {
  xml_node queryNode = _doc->child("iq").first_child();
  string type,uid;
  int target=-1,res=1,mark=0;

  for (xml_node item = queryNode.child("item"); item; item = item.next_sibling("item")) {
    type = item.attribute("type").value();
    uid = item.attribute("uid").value();
    stringstream ss;
    ss << uid;
    ss >> target;
    if (type == "add") {
      mark = FeedFocusAdapter::instance().AddFocusWithCheck(_jid->userId,target);
      MCE_DEBUG("add------>target:"<<target<<"result:"<<mark);
      res = mark > 0 ? 0:1;
    } else if (type == "del") {
      FeedFocusAdapter::instance().DelFocuses(_jid->userId,target);
      MCE_DEBUG("del------>DelFocuses");
    }
  }
  queryNode = reply.child("iq").append_child();
  queryNode.set_name("query");
  queryNode.append_attribute("xmlns") = "http://talk.xiaonei.com/setfocuslist";
  reply.child("iq").append_attribute("res") = res > 0 ? "1":"0";
  MCE_DEBUG(reply.xml());
  try{
    TalkProxyAdapter::instance().express(_jid, reply.xml());
  }catch(Ice::Exception& e){
    MCE_WARN("IqSetTask::handle-->TalkProxyAdapter::express-->" << e);
  }
}
void IqSetTask::setNotifyHandle(xml_document& reply){
	StatFunc statF("IqSetTask::setNotifyHandle");
	MCE_DEBUG("IqSetTask::setNotifyHandle --> xml :  " << _doc->xml());
	bool notify_other = false;
/*
	xml_document dispatch_doc;
  xml_node iqNode = dispatch_doc.append_child();
  iqNode.set_name("iq");
  iqNode.append_attribute("type") = "set";
  iqNode.append_attribute("id") = (int)time(NULL);
  xml_node queryNode = iqNode.append_child();
  queryNode.set_name("query");
  queryNode.append_attribute("xmlns") = "http://talk.renren.com/removenotify";
*/
	string removetype = _doc->child("iq").first_child().attribute("type").value();
	MCE_DEBUG("IqSetTask::setNotifyHandle --> removetype = " << removetype);
	if("removebyid" == removetype){
	  notify_other = true;
	  xml_node nid_node = _doc->child("iq").child("query").child("notifyid");
	  Ice::Long notifyid = boost::lexical_cast<Ice::Long>(nid_node.first_child().value());
	  if(notifyid < 0){
	    notifyid = -notifyid;
	  }
	  try{
      MCE_DEBUG("IqSetTask::setNotifyHandle --> NotifyIndexAdapter::instance().removeById(" << _jid->userId <<", " << notifyid<< ")");
      NotifyIndexAdapter::instance().removeById(_jid->userId, notifyid);
    }catch(Ice::Exception& e){
      MCE_WARN("IqSetTask::setNotifyHandle-->NotifyIndexAdapter::removeById-->"<<e);
    }

/*
    queryNode.append_attribute("type") = "removebyid";
    xml_node dispatch_nid_node = queryNode.append_child();
    dispatch_nid_node.set_name("notifyid");
    dispatch_nid_node.append_child(node_pcdata).set_value((boost::lexical_cast<string>(nid_node.first_child().value())).c_str());
*/
	}
	else if("removebysource" == removetype){
	  notify_other = true;
    xml_node type_node = _doc->child("iq").child("query").child("stype");
    int type = boost::lexical_cast<int>(type_node.first_child().value());
    xml_node resource_node = _doc->child("iq").child("query").child("resource");
    Ice::Long resource = boost::lexical_cast<Ice::Long>(resource_node.first_child().value());
    if(type < 0){
      type = -type;
    }
    if(resource < 0){
      resource = -resource;
    }
    try{
      MCE_DEBUG("IqSetTask::setNotifyHandle --> NotifyIndexAdapter::instance().removeBySource(" << _jid->userId <<", "<<type << ", " << resource << ")");
      NotifyIndexAdapter::instance().removeBySource(_jid->userId, type, resource);
      NotifyIndexNAdapter::instance().RemoveRMessageBySource(_jid->userId, type, resource, 1);
    }catch(Ice::Exception & e){
      MCE_WARN("IqSetTask::setNotifyHandle-->NotifyIndexAdapter::removeBySource-->"<<e);
    }

/*
    queryNode.append_attribute("type") = "removebysource";
    xml_node dispatch_stype_node = queryNode.append_child();
    xml_node dispatch_resource_node = queryNode.append_child();
    dispatch_stype_node.set_name("stype");
    dispatch_stype_node.append_child(node_pcdata).set_value((boost::lexical_cast<string>(type_node.first_child().value())).c_str());
    dispatch_resource_node.set_name("resource");
    dispatch_resource_node.append_child(node_pcdata).set_value((boost::lexical_cast<string>(resource_node.first_child().value())).c_str());
*/
	}
	//notify other avatars;
	else{
	  MCE_WARN("IqSetTask::setNotifyHandle --> unknown removetype = " << removetype);
	}
	if(notify_other){
	  MessageSeq mSeq;
		JidSeq self;
		try{
			self = OnlineCenterAdapter::instance().getUserJids(_jid->userId, 4);
		}catch(Ice::Exception& e){
			MCE_WARN("IqSetTask::setNotifyHandle-->OnlineCenterAdapter::getUserJids-->" << e);
		}
	  for(JidSeq::iterator it=self.begin(); it!=self.end(); ++it){
      //if((*it)->index != _jid->index ){
        MessagePtr mPtr = new Message();
        MCE_DEBUG("IqSetTask::setNotifyHandle --> notify self and other avatar from = " << jidToString(_jid) << " to = " << jidToString(*it) << " xml = " << _doc->xml());
        mPtr->to = *it;
        mPtr->type = IQ_SET;
        mPtr->msg = _doc->xml();
        //mPtr->msg = dispatch_doc.xml();
        mSeq.push_back(mPtr);
      //}
    }
	  if(!mSeq.empty()){
	    try{
	      MCE_DEBUG("IqSetTask::setNotifyHandle --> CALL TalkDeliverAdapter.deliver   size = " << mSeq.size());
	      TalkDeliverAdapter::instance().deliver(mSeq);
	    }
	    catch(Ice::Exception & e){
        MCE_WARN("IqSetTask::setNotifyHandle-->TalkDeliverAdapter::deliver-->"<<e);
      }
	  }
	}
}

void IqSetTask::setSessionHandle(xml_document& reply) {
	StatFunc statF("IqSetTask::setSessionHandle");
	xml_node iqNode = _doc->child("iq");
	xml_node sessionNode = iqNode.child("session");
	string xmlns = sessionNode.attribute("xmlns").value();
	MCE_DEBUG("@@@@@@IqSetTask::setSessionHandle-->jid=" << jidToString(_jid) << " reply:" << reply.xml() << " _doc.xml:" << _doc->xml());
	if (xmlns != "http://www.google.com/session") {
		//_session->deliverMsg(reply->xml());
		iqNode.attribute("type").set_value("result");

	MCE_DEBUG("!!!!!!!!IqSetTask::setSessionHandle-->jid=" << jidToString(_jid) << " reply:" << reply.xml() << " _doc.xml:" << _doc->xml());
		MessagePtr msg = new Message();
		msg -> from = _jid;
		msg -> to = _jid;
		msg -> type = IQ_RESULT;
		msg -> msg = _doc->xml();
		try{
		TalkProxyAdapter::instance().deliver(msg);
		}catch(Ice::Exception& e){
			MCE_WARN("IqSetTask::setSessionHandle-->TalkProxyAdapter::deliver-->" << e);
			}
	} else {
		xml_attribute to = iqNode.attribute("to");
		if (to) {
			iqNode.append_attribute("from") = jidToString(_jid).c_str();
			MessagePtr mPtr = new Message();
			mPtr->from = _jid;
			mPtr->to = stringToJid(string(to.value()));
			mPtr->type = IQ_SET;
			mPtr->msg = _doc->xml();

			MessageSeq seq;
			seq.push_back(mPtr);
			try{
			TalkProxyAdapter::instance().deliver(seq);
			}catch(Ice::Exception& e){
				MCE_WARN("IqSetTask::setSessionHandle-->TalkProxyAdapter::deliver-->"<< e);
				}
		}
	}
}

void IqSetTask::setPrivateHandle(xml_document& reply) {
	StatFunc statF("IqSetTask::setPrivateHandle");
	xml_node queryNode = reply.append_child();
	queryNode.set_name("query");
	queryNode.append_attribute("xmlns").set_value("jabber:iq:private");
	try{
	TalkProxyAdapter::instance().express(_jid, reply.xml());
	}catch(Ice::Exception& e){
		MCE_WARN("IqSetTask::setPrivateHandle-->TalkProxyAdapter::express-->" << e);
		}
}

void IqSetTask::setRosterHandle(xml_document& reply) {
	StatFunc statF("IqSetTask::setRosterHandle");
	string id = _doc->child("iq").attribute("id").value();
	xml_node itemN = _doc->child("iq").child("query").child("item");

	if (itemN) {
		string jid = itemN.attribute("jid").value();
		string name = itemN.attribute("name").value();
		string subscription = itemN.attribute("subscription").value();
		if (subscription == "remove") {
			//remove buddy
			//TalkLogicAdapter::instance().removeBuddy(_session->jid()->userId, stringToJid(jid)->userId);

			Friendship f;
			f.from = _jid->userId;
			f.to = stringToJid(jid)->userId;
			MCE_INFO("IqSetTask::setRosterHandle-->remove-->from:"<<f.from<<" to:" <<f.to << " doc:" << _doc->xml());
			try{
				BuddyCoreAdapter::instance().removeFriend(f);
			}catch(Ice::Exception& e){
				MCE_WARN("IqSetTask::setRosterHandle-->BuddyCoreAdapter::removeFriend-->" << e);
			}
		} else if (subscription == "" || subscription == "both") {
			MyUtil::StrSeq groups;
			for (xml_node n = itemN.child("group"); n; n
					= n.next_sibling("group")) {
				groups.push_back(n.first_child().value());
			}

			MCE_DEBUG("IqSetTask::setRosterHandle --> do setBuddyGroups");
			int desc;
			try{
				desc = BuddyGroupAdapter::instance().changeBuddyGroups(_jid->userId, stringToJid(jid)->userId, groups);
			}catch(Ice::Exception& e){
				MCE_WARN("IqSetTask::setRosterHandle-->BuddyGroupAdapter::changeBuddyGroups-->" << e);
			}
			MCE_DEBUG("desc:"<<desc<<"  firend:"<< com::xiaonei::service::buddy::Friend << "  host:"
					<<_jid->userId);
			if (desc == com::xiaonei::service::buddy::Friend) {
				MCE_DEBUG("host:"<<_jid->userId);
				//JidSeq jids = PresenceAdapter::instance().getOnlineUserJidSeq(_jid->userId);
				JidSeq jids;
				try{
					jids = OnlineCenterAdapter::instance().getUserJids(_jid->userId, 4);
				}catch(Ice::Exception& e){
					MCE_WARN("IqSetTask::setRosterHandle-->OnlineCenterAdapter::instance().getUserJids-->" << e)
				}
				MCE_DEBUG("IqSetTask::setRosterHandle --> host:"<<_jid->userId <<"   Jid size:" <<jids.size());

				//MessageSeq seq;
				for (size_t i = 0; i < jids.size(); ++i) {
					if (jids.at(i)->index <=1) {
						continue;
					}
					MessagePtr msg = new Message();
					msg->from = new Jid();
					msg->from->userId = jids.at(i)->userId;
					msg->from->index = -1;

					msg->to = jids.at(i);
					msg->type = IQ_SET;

					xml_document doc;
					xml_node iqNode = doc.append_child();
					iqNode.set_name("iq");
					iqNode.append_attribute("type") = "set";
					iqNode.append_attribute("id") = (int)time(NULL);
					iqNode.append_attribute("to") = jidToString(msg->to).c_str();

					xml_node queryNode = iqNode.append_child();
					queryNode.set_name("query");
					queryNode.append_attribute("xmlns") = "jabber:iq:roster";

					xml_node itemNode = queryNode.append_child();
					itemNode.set_name("item");
					itemNode.append_attribute("jid") = jid.c_str();
					itemNode.append_attribute("name") = name.c_str();
					itemNode.append_attribute("subscription") = "both";

					for (size_t j = 0; j < groups.size(); ++j) {
						xml_node groupNode = itemNode.append_child();
						groupNode.set_name("group");
						groupNode.append_child(node_pcdata).set_value(groups.at(j).c_str());
					}

					msg->msg = doc.xml();
					if (_jid->userId == 128487631) {
						MCE_DEBUG("msg:"<<msg->msg);
					}
					try{
						TalkDeliverAdapter::instance().deliver(msg);
					}catch(Ice::Exception& e){
						MCE_WARN("IqSetTask::setRosterHandle-->TalkDeliverAdapter::deliver-->" << e);
					}
				}

			}

		} else if (subscription == "from" || subscription == "to") {
			// ignore
		}
	}
	MessagePtr msg = new Message();
	msg -> from = _jid;
	msg -> to = _jid;
	msg -> type = IQ_RESULT;
	ostringstream iqMsg;
	iqMsg<<"<iq type='result' id='"<<id<<"'/>";
	msg -> msg = iqMsg.str();
	try{
		TalkProxyAdapter::instance().deliver(msg);
	}catch(Ice::Exception& e){
		MCE_WARN("IqSetTask::setRosterHandle-->TalkProxyAdapter::deliver-->" << e);
	}
}

void IqSetTask::setPrivacyHandle(xml_document& reply) {
	StatFunc statF("IqSetTask::setPrivacyHandle");
	xml_node queryNode = _doc->child("iq").child("query");
	vector<xml_node> listNodes;
	for (xml_node n = queryNode.child("list"); n; n = n.next_sibling("list")) {
		listNodes.push_back(n);
	}
	bool hasErr = false;
	bool removeHidden = true;

	if (listNodes.size() == 1) {
		if (listNodes.at(0).attribute("name").value()=="blocked") {
			MyUtil::IntSeq blockIds;
			MyUtil::StrSeq sysBlocks;
			for (xml_node n = listNodes.at(0).child("item"); n; n = n.next_sibling("item")) {
				if (n.attribute("action").value() != "deny") {
					hasErr = true;
					break;
				}

				if (n.attribute("type") && (n.attribute("type").value() == "jid")) {
					//block user
					string value = n.attribute("value").value();
					if (value[0]>='0' && value[0]<='9') {

						JidPtr jid = stringToJid(value);
						blockIds.push_back(jid->userId);

					} else {
						SysJidPtr sj = stringToSysJid(value);
						if (sj) {
							if (sj->category == "news") {
								sysBlocks.push_back(sj->node);
							}
						}
					}

					continue;
				}
				if ((!n.attribute("type")) && (!n.attribute("value"))
						&& n.child("presence-out")) {
					// hidden user
					removeHidden = false;
					try{
					TalkRightAdapter::instance().addHiddenUser(_jid->userId);
					}catch(Ice::Exception& e){
						MCE_WARN("IqSetTask::setPrivacyHandle-->TalkRightAdapter::instance().addHiddenUser-->" << e);
						}
					continue;
				}
				hasErr = true;
				break;

			}

			if (!hasErr) {
			try{
				BuddyCoreAdapter::instance().changeBlockList(_jid->userId, blockIds);
			}catch(Ice::Exception& e){
				MCE_WARN("IqSetTask::setPrivacyHandle-->BuddyCoreAdapter::changeBlockList-->" << e);
				}
				try{
				TalkRightAdapter::instance().modifySystemMessageBlock(_jid->userId, sysBlocks);
				}catch(Ice::Exception& e){
					MCE_WARN("IqSetTask::setPrivacyHandle-->TalkRightAdapter::modifySystemMessageBlock-->" << e);
					}
				if (removeHidden) {
					try{
					TalkRightAdapter::instance().removeHiddenUser(_jid->userId);
					}catch(Ice::Exception& e){
						MCE_WARN("IqSetTask::setPrivacyHandle-->TalkRightAdapter::removeHiddenUser-->" << e);
						}
				}

				return;
			}
		}
	}

	xml_node rIqNode = reply.child("iq");
	rIqNode.append_attribute("type") = "error";

	xml_node rErrorNode = rIqNode.append_child();
	rErrorNode.set_name("error");
	rErrorNode.append_attribute("type") = "cancel";

	xml_node rFeatureNotImpl = rErrorNode.append_child();
	rFeatureNotImpl.set_name("feature-not-implemented");
	rFeatureNotImpl.append_attribute("xmlns")
			= "urn:ietf:params:xml:ns:xmpp-stanzas";

	xml_node rTextNode = rErrorNode.append_child();
	rTextNode.set_name("text");
	rTextNode.append_attribute("xmlns") = "urn:ietf:params:xml:ns:xmpp-stanzas";
	rTextNode.append_child(node_pcdata).set_value("user can only set privacy into blocked list node , u want to block,in the list");

	MessagePtr msg = new Message();
	msg -> from = _jid;
	msg -> to = _jid;
	msg -> type = IQ_ERROR;
	msg -> msg = reply.xml();
	try{
	TalkProxyAdapter::instance().deliver(msg);
	}catch(Ice::Exception& e){
		MCE_WARN("IqSetTask::setPrivacyHandle-->TalkProxyAdapter::deliver-->" << e);
		}
}

void IqSetTask::setJingleHandle(xml_document& reply) {
	xml_node uniqueNode = reply.append_child();
	uniqueNode.set_name("unique");
	uniqueNode.append_attribute("xmlns").set_value("http://jabber.org/protocol/muc#unique");
	uniqueNode.append_child(node_pcdata).set_value(boost::lexical_cast<string>(_jid->index).c_str());
	try{
	TalkProxyAdapter::instance().express(_jid, reply.xml());
	}catch(Ice::Exception& e){
		MCE_WARN("IqSetTask::setJingleHandle-->TalkProxyAdapter::express-->" << e);
		}
}


void IqSetTask::setVerNewFeedHandle(xml_document& reply) {
  string ver = _doc->child("iq").child("query").child("ver").first_child().value();
	try{
	  LoginFeedAdapter::instance().SendInstallFeed(_jid->userId, ver);
	}catch(Ice::Exception& e){
		MCE_WARN("IqSetTask::setVerNewFeedHandle-->TalkProxyAdapter::express-->" << e);
	}
}

//--------------------------------------
void IqResultTask::handle() {
	if (_doc->child("iq").attribute("to")) {
		JidPtr to = stringToJid(_doc->child("iq").attribute("to").value());
		_doc->child("iq").append_attribute("from") = jidToString(_jid).c_str();

		_doc->child("iq").remove_attribute("to");
		try{
		TalkProxyAdapter::instance().express(to, _doc->xml());
		}catch(Ice::Exception& e){
			MCE_WARN("IqResultTask::handle-->TalkProxyAdapter::express-->" << e);
			}
	}
}
//--------------------------------------
void IqErrorTask::handle() {
	if (_doc->child("iq").attribute("to")) {
		JidPtr to = stringToJid(_doc->child("iq").attribute("to").value());
		_doc->child("iq").append_attribute("from") = jidToString(_jid).c_str();

		_doc->child("iq").remove_attribute("to");
		try{
		TalkProxyAdapter::instance().express(to, _doc->xml());
		}catch(Ice::Exception& e){
			MCE_WARN("IqErrorTask::handle-->TalkProxyAdapter::express-->" << e);
			}
	}
}
//-------------------------------------------memcach test


void TalkCacheLoaderIdTask::handle(){
	try{
		MCE_INFO("TalkCacheLoaderIdTask::handle --> call TalkCacheClient.GetUserByIqWithLoad  userid = " << _id);
		TalkUserPtr u = TalkCacheClient::instance().GetUserByIdWithLoad(_id);
		if(!u){
			MCE_INFO("TalkCacheLoaderIdTask::handle --> get from TalkCacheClient is null id = " << _id);
			return;
		}
		MCE_INFO("TalkCacheLoaderIdTask::handle --> check user : id = " << u->id << " name = " << u->name << " statusOriginal = " << u->statusOriginal << " statusShifted = " << u->statusShifted << " headurl = " << u->headurl << " tinyurl = " << u->tinyurl);
	}catch(Ice::Exception& e){
    MCE_WARN("TalkCacheLoaderIdTask::handle-->TalkCacheClient::GetUserByIdWithLoad-->" << e);
  }
}

void TalkCacheLoaderSeqTask::handle(){
	try{
		MCE_INFO("TalkCacheLoaderSeqTask::handle --> call TalkCacheClient.GetUserBySeqWithLoad  userids.size = " << _ids.size());
		TalkUserMap s = TalkCacheClient::instance().GetUserBySeqWithLoad(id, _ids);
		if(s.size() != _ids.size()){
			MCE_INFO("TalkCacheLoaderSeqTask::handle --> get from TalkCacheClient size is equal ids.size = " << _ids.size() << " map.size = " << s.size());
		}
	}catch(Ice::Exception& e){
    MCE_WARN("TalkCacheLoaderSeqTask::handle-->:TalkCacheClient::GetUserBySeqWithLoad-->" << e);
  }
}

